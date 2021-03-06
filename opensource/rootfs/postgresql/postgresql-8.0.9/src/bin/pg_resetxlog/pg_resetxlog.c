/*-------------------------------------------------------------------------
 *
 * pg_resetxlog.c
 *	  A utility to "zero out" the xlog when it's corrupt beyond recovery.
 *	  Can also rebuild pg_control if needed.
 *
 * The theory of operation is fairly simple:
 *	  1. Read the existing pg_control (which will include the last
 *		 checkpoint record).  If it is an old format then update to
 *		 current format.
 *	  2. If pg_control is corrupt, attempt to intuit reasonable values,
 *		 by scanning the old xlog if necessary.
 *	  3. Modify pg_control to reflect a "shutdown" state with a checkpoint
 *		 record at the start of xlog.
 *	  4. Flush the existing xlog files and write a new segment with
 *		 just a checkpoint record in it.  The new segment is positioned
 *		 just past the end of the old xlog, so that existing LSNs in
 *		 data pages will appear to be "in the past".
 * This is all pretty straightforward except for the intuition part of
 * step 2 ...
 *
 *
 * Portions Copyright (c) 1996-2005, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/bin/pg_resetxlog/pg_resetxlog.c,v 1.28 2004/12/31 22:03:11 pgsql Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <dirent.h>
#include <fcntl.h>
#include <locale.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#include "access/xlog.h"
#include "access/xlog_internal.h"
#include "catalog/catversion.h"
#include "catalog/pg_control.h"

extern int	optind;
extern char *optarg;

#define _(x) gettext((x))


char		XLogDir[MAXPGPATH]; /* not static, see xlog_internal.h */
static char ControlFilePath[MAXPGPATH];

static ControlFileData ControlFile;		/* pg_control values */
static uint32 newXlogId,
			newXlogSeg;			/* ID/Segment of new XLOG segment */
static bool guessed = false;	/* T if we had to guess at any values */
static const char *progname;

static bool ReadControlFile(void);
static void GuessControlValues(void);
static void PrintControlValues(bool guessed);
static void RewriteControlFile(void);
static void KillExistingXLOG(void);
static void WriteEmptyXLOG(void);
static void usage(void);


int
main(int argc, char *argv[])
{
	int			c;
	bool		force = false;
	bool		noupdate = false;
	TransactionId set_xid = 0;
	Oid			set_oid = 0;
	uint32		minXlogTli = 0,
				minXlogId = 0,
				minXlogSeg = 0;
	char	   *endptr;
	char	   *endptr2;
	char	   *endptr3;
	char	   *DataDir;
	int			fd;
	char		path[MAXPGPATH];

	set_pglocale_pgservice(argv[0], "pg_resetxlog");

	progname = get_progname(argv[0]);

	if (argc > 1)
	{
		if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-?") == 0)
		{
			usage();
			exit(0);
		}
		if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-V") == 0)
		{
			puts("pg_resetxlog (PostgreSQL) " PG_VERSION);
			exit(0);
		}
	}


	while ((c = getopt(argc, argv, "fl:no:x:")) != -1)
	{
		switch (c)
		{
			case 'f':
				force = true;
				break;

			case 'n':
				noupdate = true;
				break;

			case 'x':
				set_xid = strtoul(optarg, &endptr, 0);
				if (endptr == optarg || *endptr != '\0')
				{
					fprintf(stderr, _("%s: invalid argument for option -x\n"), progname);
					fprintf(stderr, _("Try \"%s --help\" for more information.\n"), progname);
					exit(1);
				}
				if (set_xid == 0)
				{
					fprintf(stderr, _("%s: transaction ID (-x) must not be 0\n"), progname);
					exit(1);
				}
				break;

			case 'o':
				set_oid = strtoul(optarg, &endptr, 0);
				if (endptr == optarg || *endptr != '\0')
				{
					fprintf(stderr, _("%s: invalid argument for option -o\n"), progname);
					fprintf(stderr, _("Try \"%s --help\" for more information.\n"), progname);
					exit(1);
				}
				if (set_oid == 0)
				{
					fprintf(stderr, _("%s: OID (-o) must not be 0\n"), progname);
					exit(1);
				}
				break;

			case 'l':
				minXlogTli = strtoul(optarg, &endptr, 0);
				if (endptr == optarg || *endptr != ',')
				{
					fprintf(stderr, _("%s: invalid argument for option -l\n"), progname);
					fprintf(stderr, _("Try \"%s --help\" for more information.\n"), progname);
					exit(1);
				}
				minXlogId = strtoul(endptr + 1, &endptr2, 0);
				if (endptr2 == endptr + 1 || *endptr2 != ',')
				{
					fprintf(stderr, _("%s: invalid argument for option -l\n"), progname);
					fprintf(stderr, _("Try \"%s --help\" for more information.\n"), progname);
					exit(1);
				}
				minXlogSeg = strtoul(endptr2 + 1, &endptr3, 0);
				if (endptr3 == endptr2 + 1 || *endptr3 != '\0')
				{
					fprintf(stderr, _("%s: invalid argument for option -l\n"), progname);
					fprintf(stderr, _("Try \"%s --help\" for more information.\n"), progname);
					exit(1);
				}
				break;

			default:
				fprintf(stderr, _("Try \"%s --help\" for more information.\n"), progname);
				exit(1);
		}
	}

	if (optind == argc)
	{
		fprintf(stderr, _("%s: no data directory specified\n"), progname);
		fprintf(stderr, _("Try \"%s --help\" for more information.\n"), progname);
		exit(1);
	}

 	/*
	 * Don't allow pg_resetxlog to be run as root, to avoid
	 * overwriting the ownership of files in the data directory. We
	 * need only check for root -- any other user won't have
	 * sufficient permissions to modify files in the data directory.
	 */
#ifndef WIN32
#ifndef __BEOS__				/* no root check on BeOS */
	if (geteuid() == 0)
	{
		fprintf(stderr, _("%s: cannot be executed by \"root\"\n"),
				progname);
		fprintf(stderr, _("You must run %s as the PostgreSQL superuser.\n"),
				progname);
		exit(1);
	}
#endif
#endif

	DataDir = argv[optind];
	snprintf(XLogDir, MAXPGPATH, "%s/pg_xlog", DataDir);
	snprintf(ControlFilePath, MAXPGPATH, "%s/global/pg_control", DataDir);

	/*
	 * Check for a postmaster lock file --- if there is one, refuse to
	 * proceed, on grounds we might be interfering with a live
	 * installation.
	 */
	snprintf(path, MAXPGPATH, "%s/postmaster.pid", DataDir);

	if ((fd = open(path, O_RDONLY)) < 0)
	{
		if (errno != ENOENT)
		{
			fprintf(stderr, _("%s: could not open file \"%s\" for reading: %s\n"), progname, path, strerror(errno));
			exit(1);
		}
	}
	else
	{
		fprintf(stderr, _("%s: lock file \"%s\" exists\n"
						  "Is a server running?  If not, delete the lock file and try again.\n"),
				progname, path);
		exit(1);
	}

	/*
	 * Attempt to read the existing pg_control file
	 */
	if (!ReadControlFile())
		GuessControlValues();

	/*
	 * Adjust fields if required by switches.  (Do this now so that
	 * printout, if any, includes these values.)
	 */
	if (set_xid != 0)
		ControlFile.checkPointCopy.nextXid = set_xid;

	if (set_oid != 0)
		ControlFile.checkPointCopy.nextOid = set_oid;

	if (minXlogTli > ControlFile.checkPointCopy.ThisTimeLineID)
		ControlFile.checkPointCopy.ThisTimeLineID = minXlogTli;

	if (minXlogId > ControlFile.logId ||
		(minXlogId == ControlFile.logId &&
		 minXlogSeg > ControlFile.logSeg))
	{
		ControlFile.logId = minXlogId;
		ControlFile.logSeg = minXlogSeg;
	}

	/*
	 * If we had to guess anything, and -f was not given, just print the
	 * guessed values and exit.  Also print if -n is given.
	 */
	if ((guessed && !force) || noupdate)
	{
		PrintControlValues(guessed);
		if (!noupdate)
		{
			printf(_("\nIf these values seem acceptable, use -f to force reset.\n"));
			exit(1);
		}
		else
			exit(0);
	}

	/*
	 * Don't reset from a dirty pg_control without -f, either.
	 */
	if (ControlFile.state != DB_SHUTDOWNED && !force)
	{
		printf(_("The database server was not shut down cleanly.\n"
			 "Resetting the transaction log may cause data to be lost.\n"
			 "If you want to proceed anyway, use -f to force reset.\n"));
		exit(1);
	}

	/*
	 * Else, do the dirty deed.
	 */
	RewriteControlFile();
	KillExistingXLOG();
	WriteEmptyXLOG();

	printf(_("Transaction log reset\n"));
	return 0;
}


/*
 * Try to read the existing pg_control file.
 *
 * This routine is also responsible for updating old pg_control versions
 * to the current format.  (Currently we don't do anything of the sort.)
 */
static bool
ReadControlFile(void)
{
	int			fd;
	int			len;
	char	   *buffer;
	crc64		crc;

	if ((fd = open(ControlFilePath, O_RDONLY)) < 0)
	{
		/*
		 * If pg_control is not there at all, or we can't read it, the
		 * odds are we've been handed a bad DataDir path, so give up. User
		 * can do "touch pg_control" to force us to proceed.
		 */
		fprintf(stderr, _("%s: could not open file \"%s\" for reading: %s\n"),
				progname, ControlFilePath, strerror(errno));
		if (errno == ENOENT)
			fprintf(stderr, _("If you are sure the data directory path is correct, execute\n"
							  "  touch %s\n"
							  "and try again.\n"),
					ControlFilePath);
		exit(1);
	}

	/* Use malloc to ensure we have a maxaligned buffer */
	buffer = (char *) malloc(BLCKSZ);

	len = read(fd, buffer, BLCKSZ);
	if (len < 0)
	{
		fprintf(stderr, _("%s: could not read file \"%s\": %s\n"),
				progname, ControlFilePath, strerror(errno));
		exit(1);
	}
	close(fd);

	if (len >= sizeof(ControlFileData) &&
		((ControlFileData *) buffer)->pg_control_version == PG_CONTROL_VERSION)
	{
		/* Check the CRC. */
		INIT_CRC64(crc);
		COMP_CRC64(crc,
				   buffer + sizeof(crc64),
				   sizeof(ControlFileData) - sizeof(crc64));
		FIN_CRC64(crc);

		if (EQ_CRC64(crc, ((ControlFileData *) buffer)->crc))
		{
			/* Valid data... */
			memcpy(&ControlFile, buffer, sizeof(ControlFile));
			return true;
		}

		fprintf(stderr, _("%s: pg_control exists but has invalid CRC; proceed with caution\n"),
				progname);
		/* We will use the data anyway, but treat it as guessed. */
		memcpy(&ControlFile, buffer, sizeof(ControlFile));
		guessed = true;
		return true;
	}

	/* Looks like it's a mess. */
	fprintf(stderr, _("%s: pg_control exists but is broken or unknown version; ignoring it\n"),
			progname);
	return false;
}


/*
 * Guess at pg_control values when we can't read the old ones.
 */
static void
GuessControlValues(void)
{
	uint64		sysidentifier;
	struct timeval tv;
	char	   *localeptr;

	/*
	 * Set up a completely default set of pg_control values.
	 */
	guessed = true;
	memset(&ControlFile, 0, sizeof(ControlFile));

	ControlFile.pg_control_version = PG_CONTROL_VERSION;
	ControlFile.catalog_version_no = CATALOG_VERSION_NO;

	/*
	 * Create a new unique installation identifier, since we can no longer
	 * use any old XLOG records.  See notes in xlog.c about the algorithm.
	 */
	gettimeofday(&tv, NULL);
	sysidentifier = ((uint64) tv.tv_sec) << 32;
	sysidentifier |= (uint32) (tv.tv_sec | tv.tv_usec);

	ControlFile.system_identifier = sysidentifier;

	ControlFile.checkPointCopy.redo.xlogid = 0;
	ControlFile.checkPointCopy.redo.xrecoff = SizeOfXLogLongPHD;
	ControlFile.checkPointCopy.undo = ControlFile.checkPointCopy.redo;
	ControlFile.checkPointCopy.ThisTimeLineID = 1;
	ControlFile.checkPointCopy.nextXid = (TransactionId) 514;	/* XXX */
	ControlFile.checkPointCopy.nextOid = BootstrapObjectIdData;
	ControlFile.checkPointCopy.time = time(NULL);

	ControlFile.state = DB_SHUTDOWNED;
	ControlFile.time = time(NULL);
	ControlFile.logId = 0;
	ControlFile.logSeg = 1;
	ControlFile.checkPoint = ControlFile.checkPointCopy.redo;

	ControlFile.blcksz = BLCKSZ;
	ControlFile.relseg_size = RELSEG_SIZE;
	ControlFile.xlog_seg_size = XLOG_SEG_SIZE;
	ControlFile.nameDataLen = NAMEDATALEN;
	ControlFile.funcMaxArgs = FUNC_MAX_ARGS;
#ifdef HAVE_INT64_TIMESTAMP
	ControlFile.enableIntTimes = TRUE;
#else
	ControlFile.enableIntTimes = FALSE;
#endif
	ControlFile.localeBuflen = LOCALE_NAME_BUFLEN;

	localeptr = setlocale(LC_COLLATE, "");
	if (!localeptr)
	{
		fprintf(stderr, _("%s: invalid LC_COLLATE setting\n"), progname);
		exit(1);
	}
	StrNCpy(ControlFile.lc_collate, localeptr, LOCALE_NAME_BUFLEN);
	localeptr = setlocale(LC_CTYPE, "");
	if (!localeptr)
	{
		fprintf(stderr, _("%s: invalid LC_CTYPE setting\n"), progname);
		exit(1);
	}
	StrNCpy(ControlFile.lc_ctype, localeptr, LOCALE_NAME_BUFLEN);

	/*
	 * XXX eventually, should try to grovel through old XLOG to develop
	 * more accurate values for TimeLineID, nextXID, and nextOID.
	 */
}


/*
 * Print the guessed pg_control values when we had to guess.
 *
 * NB: this display should be just those fields that will not be
 * reset by RewriteControlFile().
 */
static void
PrintControlValues(bool guessed)
{
	char		sysident_str[32];

	if (guessed)
		printf(_("Guessed pg_control values:\n\n"));
	else
		printf(_("pg_control values:\n\n"));

	/*
	 * Format system_identifier separately to keep platform-dependent
	 * format code out of the translatable message string.
	 */
	snprintf(sysident_str, sizeof(sysident_str), UINT64_FORMAT,
			 ControlFile.system_identifier);

	printf(_("pg_control version number:            %u\n"), ControlFile.pg_control_version);
	printf(_("Catalog version number:               %u\n"), ControlFile.catalog_version_no);
	printf(_("Database system identifier:           %s\n"), sysident_str);
	printf(_("Current log file ID:                  %u\n"), ControlFile.logId);
	printf(_("Next log file segment:                %u\n"), ControlFile.logSeg);
	printf(_("Latest checkpoint's TimeLineID:       %u\n"), ControlFile.checkPointCopy.ThisTimeLineID);
	printf(_("Latest checkpoint's NextXID:          %u\n"), ControlFile.checkPointCopy.nextXid);
	printf(_("Latest checkpoint's NextOID:          %u\n"), ControlFile.checkPointCopy.nextOid);
	printf(_("Database block size:                  %u\n"), ControlFile.blcksz);
	printf(_("Blocks per segment of large relation: %u\n"), ControlFile.relseg_size);
	printf(_("Maximum length of identifiers:        %u\n"), ControlFile.nameDataLen);
	printf(_("Maximum number of function arguments: %u\n"), ControlFile.funcMaxArgs);
	printf(_("Date/time type storage:               %s\n"),
		   (ControlFile.enableIntTimes ? _("64-bit integers") : _("floating-point numbers")));
	printf(_("Maximum length of locale name:        %u\n"), ControlFile.localeBuflen);
	printf(_("LC_COLLATE:                           %s\n"), ControlFile.lc_collate);
	printf(_("LC_CTYPE:                             %s\n"), ControlFile.lc_ctype);
}


/*
 * Write out the new pg_control file.
 */
static void
RewriteControlFile(void)
{
	int			fd;
	char		buffer[BLCKSZ]; /* need not be aligned */

	/*
	 * Adjust fields as needed to force an empty XLOG starting at the next
	 * available segment.
	 */
	newXlogId = ControlFile.logId;
	newXlogSeg = ControlFile.logSeg;

	/* adjust in case we are changing segment size */
	newXlogSeg *= ControlFile.xlog_seg_size;
	newXlogSeg = (newXlogSeg + XLogSegSize - 1) / XLogSegSize;

	/* be sure we wrap around correctly at end of a logfile */
	NextLogSeg(newXlogId, newXlogSeg);

	/* Now we can force the recorded xlog seg size to the right thing. */
	ControlFile.xlog_seg_size = XLogSegSize;

	ControlFile.checkPointCopy.redo.xlogid = newXlogId;
	ControlFile.checkPointCopy.redo.xrecoff =
		newXlogSeg * XLogSegSize + SizeOfXLogLongPHD;
	ControlFile.checkPointCopy.undo = ControlFile.checkPointCopy.redo;
	ControlFile.checkPointCopy.time = time(NULL);

	ControlFile.state = DB_SHUTDOWNED;
	ControlFile.time = time(NULL);
	ControlFile.logId = newXlogId;
	ControlFile.logSeg = newXlogSeg + 1;
	ControlFile.checkPoint = ControlFile.checkPointCopy.redo;
	ControlFile.prevCheckPoint.xlogid = 0;
	ControlFile.prevCheckPoint.xrecoff = 0;

	/* Contents are protected with a CRC */
	INIT_CRC64(ControlFile.crc);
	COMP_CRC64(ControlFile.crc,
			   (char *) &ControlFile + sizeof(crc64),
			   sizeof(ControlFileData) - sizeof(crc64));
	FIN_CRC64(ControlFile.crc);

	/*
	 * We write out BLCKSZ bytes into pg_control, zero-padding the excess
	 * over sizeof(ControlFileData).  This reduces the odds of
	 * premature-EOF errors when reading pg_control.  We'll still fail
	 * when we check the contents of the file, but hopefully with a more
	 * specific error than "couldn't read pg_control".
	 */
	if (sizeof(ControlFileData) > BLCKSZ)
	{
		fprintf(stderr,
				_("%s: internal error -- sizeof(ControlFileData) is too large ... fix xlog.c\n"),
				progname);
		exit(1);
	}

	memset(buffer, 0, BLCKSZ);
	memcpy(buffer, &ControlFile, sizeof(ControlFileData));

	unlink(ControlFilePath);

	fd = open(ControlFilePath, O_RDWR | O_CREAT | O_EXCL | PG_BINARY, S_IRUSR | S_IWUSR);
	if (fd < 0)
	{
		fprintf(stderr, _("%s: could not create pg_control file: %s\n"),
				progname, strerror(errno));
		exit(1);
	}

	errno = 0;
	if (write(fd, buffer, BLCKSZ) != BLCKSZ)
	{
		/* if write didn't set errno, assume problem is no disk space */
		if (errno == 0)
			errno = ENOSPC;
		fprintf(stderr, _("%s: could not write pg_control file: %s\n"),
				progname, strerror(errno));
		exit(1);
	}

	if (fsync(fd) != 0)
	{
		fprintf(stderr, _("%s: fsync error: %s\n"), progname, strerror(errno));
		exit(1);
	}

	close(fd);
}


/*
 * Remove existing XLOG files
 */
static void
KillExistingXLOG(void)
{
	DIR		   *xldir;
	struct dirent *xlde;
	char		path[MAXPGPATH];

	xldir = opendir(XLogDir);
	if (xldir == NULL)
	{
		fprintf(stderr, _("%s: could not open directory \"%s\": %s\n"),
				progname, XLogDir, strerror(errno));
		exit(1);
	}

	errno = 0;
	while ((xlde = readdir(xldir)) != NULL)
	{
		if (strlen(xlde->d_name) == 24 &&
			strspn(xlde->d_name, "0123456789ABCDEF") == 24)
		{
			snprintf(path, MAXPGPATH, "%s/%s", XLogDir, xlde->d_name);
			if (unlink(path) < 0)
			{
				fprintf(stderr, _("%s: could not delete file \"%s\": %s\n"),
						progname, path, strerror(errno));
				exit(1);
			}
		}
		errno = 0;
	}
#ifdef WIN32

	/*
	 * This fix is in mingw cvs (runtime/mingwex/dirent.c rev 1.4), but
	 * not in released version
	 */
	if (GetLastError() == ERROR_NO_MORE_FILES)
		errno = 0;
#endif

	if (errno)
	{
		fprintf(stderr, _("%s: could not read from directory \"%s\": %s\n"),
				progname, XLogDir, strerror(errno));
		exit(1);
	}
	closedir(xldir);
}


/*
 * Write an empty XLOG file, containing only the checkpoint record
 * already set up in ControlFile.
 */
static void
WriteEmptyXLOG(void)
{
	char	   *buffer;
	XLogPageHeader page;
	XLogLongPageHeader longpage;
	XLogRecord *record;
	crc64		crc;
	char		path[MAXPGPATH];
	int			fd;
	int			nbytes;

	/* Use malloc() to ensure buffer is MAXALIGNED */
	buffer = (char *) malloc(BLCKSZ);
	page = (XLogPageHeader) buffer;
	memset(buffer, 0, BLCKSZ);

	/* Set up the XLOG page header */
	page->xlp_magic = XLOG_PAGE_MAGIC;
	page->xlp_info = XLP_LONG_HEADER;
	page->xlp_tli = ControlFile.checkPointCopy.ThisTimeLineID;
	page->xlp_pageaddr.xlogid =
		ControlFile.checkPointCopy.redo.xlogid;
	page->xlp_pageaddr.xrecoff =
		ControlFile.checkPointCopy.redo.xrecoff - SizeOfXLogLongPHD;
	longpage = (XLogLongPageHeader) page;
	longpage->xlp_sysid = ControlFile.system_identifier;
	longpage->xlp_seg_size = XLogSegSize;

	/* Insert the initial checkpoint record */
	record = (XLogRecord *) ((char *) page + SizeOfXLogLongPHD);
	record->xl_prev.xlogid = 0;
	record->xl_prev.xrecoff = 0;
	record->xl_xid = InvalidTransactionId;
	record->xl_len = sizeof(CheckPoint);
	record->xl_info = XLOG_CHECKPOINT_SHUTDOWN;
	record->xl_rmid = RM_XLOG_ID;
	memcpy(XLogRecGetData(record), &ControlFile.checkPointCopy,
		   sizeof(CheckPoint));

	INIT_CRC64(crc);
	COMP_CRC64(crc, &ControlFile.checkPointCopy, sizeof(CheckPoint));
	COMP_CRC64(crc, (char *) record + sizeof(crc64),
			   SizeOfXLogRecord - sizeof(crc64));
	FIN_CRC64(crc);
	record->xl_crc = crc;

	/* Write the first page */
	XLogFilePath(path, ControlFile.checkPointCopy.ThisTimeLineID,
				 newXlogId, newXlogSeg);

	unlink(path);

	fd = open(path, O_RDWR | O_CREAT | O_EXCL | PG_BINARY,
			  S_IRUSR | S_IWUSR);
	if (fd < 0)
	{
		fprintf(stderr, _("%s: could not open file \"%s\": %s\n"),
				progname, path, strerror(errno));
		exit(1);
	}

	errno = 0;
	if (write(fd, buffer, BLCKSZ) != BLCKSZ)
	{
		/* if write didn't set errno, assume problem is no disk space */
		if (errno == 0)
			errno = ENOSPC;
		fprintf(stderr, _("%s: could not write file \"%s\": %s\n"),
				progname, path, strerror(errno));
		exit(1);
	}

	/* Fill the rest of the file with zeroes */
	memset(buffer, 0, BLCKSZ);
	for (nbytes = BLCKSZ; nbytes < XLogSegSize; nbytes += BLCKSZ)
	{
		errno = 0;
		if (write(fd, buffer, BLCKSZ) != BLCKSZ)
		{
			if (errno == 0)
				errno = ENOSPC;
			fprintf(stderr, _("%s: could not write file \"%s\": %s\n"),
					progname, path, strerror(errno));
			exit(1);
		}
	}

	if (fsync(fd) != 0)
	{
		fprintf(stderr, _("%s: fsync error: %s\n"), progname, strerror(errno));
		exit(1);
	}

	close(fd);
}


static void
usage(void)
{
	printf(_("%s resets the PostgreSQL transaction log.\n\n"), progname);
	printf(_("Usage:\n  %s [OPTION]... DATADIR\n\n"), progname);
	printf(_("Options:\n"));
	printf(_("  -f              force update to be done\n"));
	printf(_("  -l TLI,FILE,SEG force minimum WAL starting location for new transaction log\n"));
	printf(_("  -n              no update, just show extracted control values (for testing)\n"));
	printf(_("  -o OID          set next OID\n"));
	printf(_("  -x XID          set next transaction ID\n"));
	printf(_("  --help          show this help, then exit\n"));
	printf(_("  --version       output version information, then exit\n"));
	printf(_("\nReport bugs to <pgsql-bugs@postgresql.org>.\n"));
}
