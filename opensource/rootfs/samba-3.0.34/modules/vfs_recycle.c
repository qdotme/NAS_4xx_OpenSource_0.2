/* For samba-3.0.34
 * Recycle bin VFS module for Samba.
 *
 * Copyright (C) 2001, Brandon Stone, Amherst College, <bbstone@amherst.edu>.
 * Copyright (C) 2002, Jeremy Allison - modified to make a VFS module.
 * Copyright (C) 2002, Alexander Bokovoy - cascaded VFS adoption,
 * Copyright (C) 2002, Juergen Hasch - added some options.
 * Copyright (C) 2002, Simo Sorce
 * Copyright (C) 2002, Stefan (metze) Metzmacher
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "includes.h"

/*
Zild Database Library
The Zild Database Library implements a small, fast and easy to use
database API with the following features: C database library -
connect to multiple database systems - zero runtime configuration,
connect using URL scheme - Thread safe Connection Pooling and exceptions
handling.

This database library is part of the Zild Application Server, see
http://www.zild.org/ and is released as Open Source with the hope
that others may find it useful in their Open Source Work.
*/
#include <URL.h>
#include <ResultSet.h>
#include <PreparedStatement.h>
#include <Connection.h>
#include <ConnectionPool.h>
#include <SQLException.h>

#define ALLOC_CHECK( ptr, label ) do { if ( ( ptr ) == NULL ) { DEBUG( 0, ( "recycle.bin: out of memory!\n" ) ); errno = ENOMEM; goto label; } } while( 0 )

static int _vfs_recycle_debug_level = DBGC_VFS;

#undef DBGC_CLASS
#define DBGC_CLASS _vfs_recycle_debug_level

#define DBG_LOW  2
#define DBG_HIGH 2
 
static int recycle_connect(vfs_handle_struct *handle, const char *service, const char *user);
static void recycle_disconnect(vfs_handle_struct *handle);
static int recycle_unlink(vfs_handle_struct *handle, const char *name);

static vfs_op_tuple _recycle_ops[] = 
{
	/* Disk operations */
	{ SMB_VFS_OP( recycle_connect ),	SMB_VFS_OP_CONNECT,		SMB_VFS_LAYER_TRANSPARENT },
	{ SMB_VFS_OP( recycle_disconnect ), SMB_VFS_OP_DISCONNECT,	SMB_VFS_LAYER_TRANSPARENT },
	
	/* File operations */
	{ SMB_VFS_OP( recycle_unlink ),		SMB_VFS_OP_UNLINK,		SMB_VFS_LAYER_TRANSPARENT },
	{ SMB_VFS_OP( NULL ),				SMB_VFS_OP_NOOP,		SMB_VFS_LAYER_NOOP }
};

/* --- */
inline int ENCODE_REQUEST( char** p_req, 
						   int * p_req_size, 
						   char* cmd_mode, 
						   char* db_path, 
						   char* t_id, 
						   char* del_fname, 
						   char* del_fpath, 
						   double del_fsize, 
						   double param,
						   int force_insertion )
{ 
	int cmd_mode_size;
	int db_path_size;
	int t_id_size;
	int del_fname_size;
	int del_fpath_size;
    int req_size; 
	
	char* cur, *req;

	cmd_mode_size = strlen( cmd_mode ) + 1;
	db_path_size = strlen( db_path ) + 1; 
	t_id_size = strlen( t_id ) + 1; 
	del_fname_size = strlen( del_fname ) + 1; 
	del_fpath_size = strlen( del_fpath ) + 1; 
	req_size = cmd_mode_size + db_path_size + t_id_size + del_fname_size + del_fpath_size + sizeof ( double ) + sizeof ( double ) + sizeof ( int ) + sizeof( int ) * 5;
	
	// 4 is to store req_size
	req = malloc( req_size + 4 ); 
	//syslog( LOG_INFO, "request size %d\n", req_size + 4 );
	
	if ( req == NULL ) 
		return -1;
	
	*( ( int* )req ) = req_size; 
	cur = req + sizeof ( int ); 
	//syslog( LOG_INFO, "cur1  %x\n", cur );

	*( ( int* ) cur ) = cmd_mode_size; 
	cur += sizeof( int ); 
	strlcpy( cur, cmd_mode, cmd_mode_size + 1 );
	cur += cmd_mode_size; 
	//syslog( LOG_INFO, "cur2  %x\n", cur );
	
	*( ( int* ) cur ) = db_path_size; 
	cur += sizeof( int ); 
	strlcpy( cur, db_path, db_path_size + 1 );
	cur += db_path_size; 
	//syslog( LOG_INFO, "cur3  %x\n", cur );
	
	*( ( int* ) cur ) = t_id_size; 
	cur += sizeof( int ); 
	strlcpy( cur, t_id, t_id_size + 1 );
	cur += t_id_size; 	
	//syslog( LOG_INFO, "cur4  %x\n", cur );
	
	*( ( int* ) cur ) = del_fname_size; 
	cur += sizeof( int ); 
	strlcpy( cur, del_fname, del_fname_size + 1 );
	cur += del_fname_size; 
	//syslog( LOG_INFO, "cur5  %x\n", cur );
	
	*( ( int* ) cur ) = del_fpath_size; 
	cur += sizeof( int ); 
	strlcpy( cur, del_fpath, del_fpath_size + 1 );
	cur += del_fpath_size; 		
	//syslog( LOG_INFO, "cur6  %x\n", cur );
	
	*( ( double*) cur ) = del_fsize; 
	cur += sizeof( double ); 
	//syslog( LOG_INFO, "cur7  %x\n", cur );

	*( ( double* ) cur ) = param; 
	cur += sizeof( double ); 
	//syslog( LOG_INFO, "cur8  %x\n", cur );
	
	*( ( int* )cur ) = force_insertion; 

	*p_req = req;
	*p_req_size = req_size + 4;
	
	return 0;
} 

static BOOL request_insert_data_to_db( char *db_path, 
									   char *t_id, 
									   char *del_fname, 
									   char *del_fpath, 
									   double del_fsize,
									   double param,
									   BOOL force_insertion )
{
	int fd;
	int size;
	char* req = NULL; 
	char* cmd_mode = "insert";

	ENCODE_REQUEST( &req, &size, cmd_mode, db_path, t_id, del_fname, del_fpath, del_fsize, param, force_insertion );

	setuid(0);

	if( ( fd = open( "/dev/myfifo", O_WRONLY ) ) >= 0 &&
		write( fd, req, size ) == size ) 
	{
		close(fd);
		free(req);
		return True;
	}

	free(req);
	return False;
}

static int recycle_connect( vfs_handle_struct *handle, const char *service, const char *user )
{
	DEBUG( DBG_HIGH,( "recycle_connect() connect to service[%s] as user[%s].\n", service,user ) );

	return SMB_VFS_NEXT_CONNECT( handle, service, user );
}

static void recycle_disconnect( vfs_handle_struct *handle )
{
	DEBUG( DBG_HIGH,( "recycle_disconnect() connect to service[%s].\n", lp_servicename( SNUM( handle->conn ) ) ) );

	SMB_VFS_NEXT_DISCONNECT(handle);
}

/* --- */
static const char *recycle_volume( vfs_handle_struct *handle )
{
	const char *tmp_str = NULL;	

	tmp_str = lp_parm_const_string( SNUM( handle->conn ), "recycle", "volume",".recycle" );

	DEBUG( DBG_HIGH, ( "recycle: volume = %s\n", tmp_str ) );
	
	return tmp_str;
}

static BOOL recycle_db_sqlite( vfs_handle_struct *handle )
{
	BOOL ret;

	ret = lp_parm_bool( SNUM( handle->conn ), "recycle", "db_sqlite", False );

	DEBUG( DBG_HIGH, ( "recycle: db_sqlite = %s\n", ret ? "True" : "False" ) );
	
	return ret;
}

static const char *recycle_bin_directory( vfs_handle_struct *handle )
{
	const char *tmp_str = NULL;	

	tmp_str = lp_parm_const_string( SNUM( handle->conn ), "recycle", "bin_directory",".recycle" );

	DEBUG( DBG_HIGH, ( "recycle: bin_directory = %s\n", tmp_str ) );
	
	return tmp_str;
}

static const char *recycle_db_id( vfs_handle_struct *handle )
{
	const char *tmp_str = NULL;	

	tmp_str = lp_parm_const_string( SNUM( handle->conn ), "recycle", "t_id",".recycle" );

	DEBUG( DBG_HIGH, ( "recycle: t_id = %s\n", tmp_str ) );
	
	return tmp_str;
}
/* --- */

static const char *recycle_repository( vfs_handle_struct *handle )
{
	const char *tmp_str = NULL;
	
	tmp_str = lp_parm_const_string( SNUM( handle->conn ), "recycle", "repository",".recycle" );

	DEBUG( DBG_HIGH, ( "recycle: repository = %s\n", tmp_str ) );
	
	return tmp_str;
}

static BOOL recycle_keep_dir_tree( vfs_handle_struct *handle )
{
	BOOL ret;
	
	ret = lp_parm_bool( SNUM( handle->conn ), "recycle", "keeptree", False );

	DEBUG( DBG_HIGH, ( "recycle_bin: keeptree = %s\n", ret?"True":"False" ) );
	
	return ret;
}

static BOOL recycle_versions( vfs_handle_struct *handle )
{
	BOOL ret;

	ret = lp_parm_bool( SNUM( handle->conn ), "recycle", "versions", False );

	DEBUG( DBG_HIGH, ( "recycle: versions = %s\n", ret?"True":"False" ) );
	
	return ret;
}

static BOOL recycle_touch( vfs_handle_struct *handle )
{
	BOOL ret;

	ret = lp_parm_bool( SNUM( handle->conn ), "recycle", "touch", False );

	DEBUG( DBG_HIGH, ( "recycle: touch = %s\n", ret?"True":"False" ) );
	
	return ret;
}

static BOOL recycle_touch_mtime( vfs_handle_struct *handle )
{
	BOOL ret;

	ret = lp_parm_bool( SNUM( handle->conn ), "recycle", "touch_mtime", False );

	DEBUG( DBG_HIGH, ( "recycle: touch_mtime = %s\n", ret?"True":"False" ) );
	
	return ret;
}

static const char **recycle_exclude( vfs_handle_struct *handle )
{
	const char **tmp_lp;
	
	tmp_lp = lp_parm_string_list( SNUM( handle->conn ), "recycle", "exclude", NULL );

	DEBUG( DBG_HIGH, ( "recycle: exclude = %s ...\n", tmp_lp?*tmp_lp:"" ) );
	
	return tmp_lp;
}

static const char **recycle_exclude_dir( vfs_handle_struct *handle )
{
	const char **tmp_lp;
	
	tmp_lp = lp_parm_string_list( SNUM( handle->conn ), "recycle", "exclude_dir", NULL );

	DEBUG( DBG_HIGH, ( "recycle: exclude_dir = %s ...\n", tmp_lp?*tmp_lp:"" ) );
	
	return tmp_lp;
}

static const char **recycle_noversions( vfs_handle_struct *handle )
{
	const char **tmp_lp;
	
	tmp_lp = lp_parm_string_list( SNUM( handle->conn ), "recycle", "noversions", NULL );

	DEBUG( DBG_HIGH, ( "recycle: noversions = %s\n", tmp_lp?*tmp_lp:"" ) );
	
	return tmp_lp;
}

static SMB_OFF_T recycle_maxsize( vfs_handle_struct *handle )
{
	SMB_OFF_T maxsize;
	
	maxsize = conv_str_size( lp_parm_const_string( SNUM( handle->conn ), "recycle", "maxsize", NULL));

	DEBUG(10, ("recycle: maxsize = %lu\n", ( long unsigned int ) maxsize ) );
	
	return maxsize;
}

static SMB_OFF_T recycle_minsize( vfs_handle_struct *handle )
{
	SMB_OFF_T minsize;
	
	minsize = conv_str_size( lp_parm_const_string( SNUM( handle->conn ), "recycle", "minsize", NULL ) );

	DEBUG( DBG_HIGH, ( "recycle: minsize = %lu\n", ( long unsigned int ) minsize ) );
	
	return minsize;
}

static mode_t recycle_directory_mode( vfs_handle_struct *handle )
{
	int dirmode;
	const char *buff;

	buff = lp_parm_const_string( SNUM( handle->conn ), "recycle", "directory_mode", NULL );

	if( buff != NULL ) 
	{
		sscanf( buff, "%o", &dirmode );
	} 
	else 
	{
		dirmode = S_IRUSR | S_IWUSR | S_IXUSR;
	}

	DEBUG( DBG_HIGH, ( "recycle: directory_mode = %o\n", dirmode ) );
	return ( mode_t ) dirmode;
}

static mode_t recycle_subdir_mode( vfs_handle_struct *handle )
{
	int dirmode;
	const char *buff;

	buff = lp_parm_const_string( SNUM( handle->conn ), "recycle", "subdir_mode", NULL );

	if( buff != NULL ) 
	{
		sscanf( buff, "%o", &dirmode );
	} 
	else 
	{
		dirmode = recycle_directory_mode( handle );
	}

	DEBUG( DBG_HIGH, ( "recycle: subdir_mode = %o\n", dirmode ) );
	return ( mode_t ) dirmode;
}

static BOOL recycle_directory_exist( vfs_handle_struct *handle, const char *dname )
{
	SMB_STRUCT_STAT st;

	if( SMB_VFS_NEXT_STAT( handle, dname, &st ) == 0 ) 
	{
		if( S_ISDIR( st.st_mode ) ) 
		{
			return True;
		}
	}

	return False;
}

static BOOL recycle_file_exist( vfs_handle_struct *handle, const char *fname )
{
	SMB_STRUCT_STAT st;

	if( SMB_VFS_NEXT_STAT( handle, fname, &st ) == 0 ) 
	{
		if( S_ISREG( st.st_mode ) ) 
		{
			return True;
		}
	}

	return False;
}

/**
 * Return file size
 * @param conn connection
 * @param fname file name
 * @return size in bytes
 **/
static SMB_OFF_T recycle_get_file_size( vfs_handle_struct *handle, const char *fname )
{
	SMB_STRUCT_STAT st;

	if( SMB_VFS_NEXT_STAT( handle, fname, &st ) != 0 ) 
	{
		DEBUG( 0, ( "recycle: stat for %s returned %s\n", fname, strerror( errno ) ) );
		return ( SMB_OFF_T ) 0;
	}

	//syslog( LOG_INFO, "recycle_get_file_size - %.0f\n", ( double ) st.st_size );
	return ( st.st_size );
}

static SMB_OFF_T recycle_get_file_disk_size( vfs_handle_struct *handle, const char *fname )
{
	SMB_STRUCT_STAT st;

	if( SMB_VFS_NEXT_LSTAT( handle, fname, &st ) != 0 ) 
	{
		DEBUG( 0, ( "recycle: stat for %s returned %s\n", fname, strerror( errno ) ) );
		return ( SMB_OFF_T ) 0;
	}

	//syslog( LOG_INFO, "recycle_get_file_disk_size - %.0f\n", ( double ) 512 * st.st_blocks );
	return ( 512 * st.st_blocks );
}

/**
 * Create directory tree
 * @param conn connection
 * @param dname Directory tree to be created
 * @return Returns True for success
 **/
static BOOL recycle_create_dir( vfs_handle_struct *handle, const char *dname )
{
	size_t len;
	mode_t mode;
	char *new_dir = NULL;
	char *tmp_str = NULL;
	char *token;
	char *tok_str;
	BOOL ret = False;

	mode = recycle_directory_mode( handle );

	tmp_str = SMB_STRDUP( dname );
	ALLOC_CHECK( tmp_str, done );
	tok_str = tmp_str;

	len = strlen( dname ) + 1;
	new_dir = ( char * ) SMB_MALLOC( len + 1 );
	ALLOC_CHECK( new_dir, done );
	*new_dir = '\0';
	if( dname[0] == '/' ) 
	{
		/* Absolute path. */
		safe_strcat( new_dir,"/",len );
	}

	/* Create directory tree if neccessary */
	for( token = strtok( tok_str, "/" ); token; token = strtok( NULL, "/" ) ) 
	{
		safe_strcat( new_dir, token, len );
		if( recycle_directory_exist( handle, new_dir ) )
			DEBUG( DBG_HIGH, ( "recycle: dir %s already exists\n", new_dir ) );
		else 
		{
			DEBUG( 5, ( "recycle: creating new dir %s\n", new_dir ) );
			if( SMB_VFS_NEXT_MKDIR( handle, new_dir, mode ) != 0 ) 
			{
				DEBUG( 1, ( "recycle: mkdir failed for %s with error: %s\n", new_dir, strerror( errno ) ) );
				ret = False;
				goto done;
			}
		}
		
		safe_strcat( new_dir, "/", len );
		mode = recycle_subdir_mode( handle );
	}

	ret = True;
done:
	SAFE_FREE( tmp_str );
	SAFE_FREE( new_dir );
	return ret;
}

/**
 * Check if any of the components of "exclude_list" are contained in path.
 * Return True if found
 **/

static BOOL matchdirparam( const char **dir_exclude_list, char *path )
{
	char *startp = NULL, *endp = NULL;

	if( dir_exclude_list == NULL || 
		dir_exclude_list[0] == NULL ||
		*dir_exclude_list[0] == '\0' || 
		path == NULL || 
		*path == '\0') 
	{
		return False;
	}

	/* 
	 * Walk the components of path, looking for matches with the
	 * exclude list on each component. 
	 */

	for( startp = path; startp; startp = endp ) 
	{
		int i;

		while( *startp == '/' ) 
		{
			startp++;
		}
		
		endp = strchr( startp, '/' );

		if( endp ) 
		{
			*endp = '\0';
		}

		for( i=0; dir_exclude_list[i] ; i++ ) 
		{
			if( unix_wild_match( dir_exclude_list[i], startp ) ) 
			{
				/* Repair path. */
				if( endp ) 
				{
					*endp = '/';
				}

				return True;
			}
		}

		/* Repair path. */
		if( endp ) 
		{
			*endp = '/';
		}
	}

	return False;
}

/**
 * Check if needle is contained in haystack, * and ? patterns are resolved
 * @param haystack list of parameters separated by delimimiter character
 * @param needle string to be matched exectly to haystack including pattern matching
 * @return True if found
 **/
static BOOL matchparam( const char **haystack_list, const char *needle )
{
	int i;

	if( haystack_list == NULL || 
		haystack_list[0] == NULL ||
		*haystack_list[0] == '\0' || 
		needle == NULL || 
		*needle == '\0') 
	{
		return False;
	}

	for( i=0; haystack_list[i]; i++ ) 
	{
		if( unix_wild_match( haystack_list[i], needle ) ) 
		{
			return True;
		}
	}

	return False;
}

/**
 * Touch access or modify date
 **/
static void recycle_do_touch( vfs_handle_struct *handle, const char *fname, BOOL touch_mtime )
{
	SMB_STRUCT_STAT st;
	struct timespec ts[2];
	
	if( SMB_VFS_NEXT_STAT( handle, fname, &st ) != 0 ) 
	{
		DEBUG( 0,( "recycle: stat for %s returned %s\n", fname, strerror( errno ) ) );
		return;
	}

	ts[0] = timespec_current();						  /* atime */
	ts[1] = touch_mtime ? ts[0] : get_mtimespec(&st); /* mtime */

	// SMB_VFS_NEXT_NTIMES - can not work on SHASTA?
	if ( SMB_VFS_NEXT_NTIMES( handle, fname, ts ) == -1 ) 
	{
        DEBUG( 0, ( "recycle: touching %s failed, reason = %s\n", fname, strerror( errno ) ) ); 
		char *s = NULL;
		asprintf( &s, "touch %s", fname );
		system( s );		
		DEBUG( 0, ( "recycle: ststem command -> %s\n", s ) );
		SAFE_FREE( s );
	}

	/* can not work	
	struct stat buf;
	struct utimbuf utb;
	int fd = -1;

	fd = open( fname, O_WRONLY | O_CREAT | O_NONBLOCK | O_NOCTTY,
					  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );

	DEBUG( 0, ( "recycle: open file %d\n", fd ) ); 
	fstat( fd, &buf );
	utb.actime = buf.st_atime;
	DEBUG( 0, ( "recycle: touching %d ; %d\n", utb.actime, buf.st_atime ) ); 

	if( utime( fd, &utb ) )
	{
		 DEBUG( 0, ( "recycle: touching %s failed(fstat)\n", fname ) ); 
	}*/
}

extern userdom_struct current_user_info;

/**
 * Check if file should be recycled
 **/
static int recycle_unlink( vfs_handle_struct *handle, const char *file_name )
{
	connection_struct *conn = handle->conn;
	char *path_name = NULL;
    char *temp_name = NULL;
	char *final_name = NULL;
	char *repository = NULL;
	char *base = NULL;
	char *volume = NULL;
	char *bin_directory = NULL;
	char *t_id = NULL;
	int i = 1;
	SMB_OFF_T maxsize;
	SMB_OFF_T minsize;
	SMB_OFF_T file_size; /* space_avail; */
	SMB_OFF_T disk_size; /* space_avail; */
	BOOL exist;
	int rc = -1;

	/* for db */
	double db_column_fsize = 0;
	char *db_path = NULL;
	char *db_column_fname = NULL;
	char *db_column_fpath = NULL;
	 
	if( recycle_db_sqlite( handle ) == True ) 
	{
		volume = ( char* ) recycle_volume( handle );
		ALLOC_CHECK( volume, done );
		DEBUG( DBG_HIGH, ( "recycle: volume = %s\n", volume ) );

		bin_directory = ( char* ) recycle_bin_directory( handle );
		ALLOC_CHECK( bin_directory, done );
		DEBUG( DBG_HIGH, ( "recycle: recycle bin directory = %s\n", bin_directory ) );

		t_id = ( char* ) recycle_db_id( handle );
		ALLOC_CHECK( t_id, done );
		DEBUG( DBG_HIGH, ( "recycle: table ID = %s\n", t_id ) );

		asprintf( &db_path, "/%s/%s", volume, bin_directory );
		ALLOC_CHECK( db_path, done );		
		DEBUG( DBG_HIGH, ( "recycle: absolute path for DB = %s\n", db_path ) );
	}

	repository = talloc_sub_advanced( NULL, 
									  lp_servicename( SNUM( conn ) ),
									  conn->user,
									  conn->connectpath, 
									  conn->gid,
									  get_current_username(),
									  current_user_info.domain,
									  recycle_repository( handle ) );

	ALLOC_CHECK( repository, done );
	DEBUG( DBG_HIGH, ( "recycle: repository = %s\n", repository ) );
	
	/* shouldn't we allow absolute path names here? --metze */
	/* Yes :-). JRA. */
	trim_char( repository, '\0', '/' );
	
	if( !repository || *( repository ) == '\0' ) 
	{
		DEBUG( DBG_HIGH, ( "recycle: repository path not set, purging %s...\n", file_name ) );
		rc = SMB_VFS_NEXT_UNLINK( handle, file_name );
		goto done;
	}

	/* we don't recycle the recycle bin... */
	if( strncmp( file_name, repository, strlen( repository ) ) == 0 ) 
	{
		DEBUG( DBG_HIGH, ( "recycle: File is within recycling bin, unlinking ...\n" ) );
		rc = SMB_VFS_NEXT_UNLINK( handle, file_name );
		goto done;
	}

	db_column_fsize = file_size = recycle_get_file_size( handle, file_name );
	DEBUG( DBG_HIGH, ( "recycle: file size = %.0f\n", ( double ) file_size ) );

	disk_size = recycle_get_file_disk_size( handle, file_name );
	DEBUG( DBG_HIGH, ( "recycle: file size = %.0f\n", ( double ) disk_size ) );

	if( disk_size < db_column_fsize )
	{
		DEBUG( DBG_HIGH, ( "recycle: File %s is not completed, purging...\n", file_name ) );
		rc = SMB_VFS_NEXT_UNLINK( handle, file_name );
		goto done;
	}

	/* it is wrong to purge filenames only because they are empty imho
	 *   --- simo
	 *
	if( fsize == 0 ) 
	{
		DEBUG( 3, ( "recycle: File %s is empty, purging...\n", file_name ) );
		rc = SMB_VFS_NEXT_UNLINK( handle, file_name );
		goto done;
	}
	 */

	/* FIXME: this is wrong, we should check the whole size of the recycle bin is
	 * not greater then maxsize, not the size of the single file, also it is better
	 * to remove older files
	 */
	maxsize = recycle_maxsize( handle );

	if( maxsize > 0 && file_size > maxsize )
	{
		DEBUG( DBG_HIGH, ( "recycle: File %s exceeds maximum recycle size, purging... \n", file_name ) );
		rc = SMB_VFS_NEXT_UNLINK( handle, file_name );
		goto done;
	}

	minsize = recycle_minsize( handle );

	if( minsize > 0 && file_size < minsize ) 
	{
		DEBUG( DBG_HIGH, ( "recycle: File %s lowers minimum recycle size, purging... \n", file_name ) );
		rc = SMB_VFS_NEXT_UNLINK( handle, file_name );
		goto done;
	}

	/* FIXME: this is wrong: moving files with rename does not change the disk space
	 * allocation
	 *
	space_avail = SMB_VFS_NEXT_DISK_FREE( handle, ".", True, &bsize, &dfree, &dsize ) * 1024L;
	DEBUG( 5, ( "space_avail = %Lu, file_size = %Lu\n", space_avail, file_size ) );
	if( space_avail < file_size ) 
	{
		DEBUG( 3, ( "recycle: Not enough diskspace, purging file %s\n", file_name ) );
		rc = SMB_VFS_NEXT_UNLINK( handle, file_name );
		goto done;
	}
	 */

	/* extract filename and path */
	base = strrchr( file_name, '/' );

	if( base == NULL )  
	{
		base = ( char* ) file_name;
		path_name = SMB_STRDUP( "/" );
		ALLOC_CHECK( path_name, done );
	}
	else
	{
		path_name = SMB_STRDUP( file_name );
		ALLOC_CHECK( path_name, done );
		path_name[ base - file_name ] = '\0';
		base++;
	}

	DEBUG( DBG_LOW, ( "recycle: fname = %s\n", file_name ) );	/* original filename with path */
	DEBUG( DBG_LOW, ( "recycle: fpath = %s\n", path_name ) );	/* original path */
	DEBUG( DBG_LOW, ( "recycle: base = %s\n", base ) );			/* filename without path */

	db_column_fname = SMB_STRDUP( base );
	db_column_fpath = SMB_STRDUP( path_name );

	if( matchparam( recycle_exclude( handle ), base ) ) 
	{
		DEBUG( DBG_HIGH, ( "recycle: file %s is excluded \n", base ) );
		rc = SMB_VFS_NEXT_UNLINK( handle, file_name );
		goto done;
	}

	if( matchdirparam( recycle_exclude_dir( handle ), path_name ) ) 
	{
		DEBUG( DBG_HIGH, ( "recycle: directory %s is excluded \n", path_name ) );
		rc = SMB_VFS_NEXT_UNLINK( handle, file_name );
		goto done;
	}

	if( recycle_keep_dir_tree( handle ) == True ) 
	{
		asprintf( &temp_name, "%s/%s", repository, path_name );
		DEBUG( DBG_HIGH, ( "recycle: recycled temp name: %s\n", temp_name ) );
	} 
	else 
	{
		temp_name = SMB_STRDUP( repository );
	}

	ALLOC_CHECK( temp_name, done );

	exist = recycle_directory_exist( handle, temp_name );
	
	if( exist ) 
	{
		DEBUG( DBG_HIGH, ( "recycle: Directory already exists\n" ) );
	} 
	else 
	{
		DEBUG( DBG_HIGH, ( "recycle: Creating directory %s\n", temp_name ) );
		
		if( recycle_create_dir( handle, temp_name ) == False ) 
		{
			DEBUG( DBG_HIGH, ( "recycle: Could not create directory, purging %s...\n", file_name ) );
			rc = SMB_VFS_NEXT_UNLINK( handle, file_name );
			goto done;
		}
	}

	asprintf( &final_name, "%s/%s", temp_name, base );
	ALLOC_CHECK( final_name, done );
	DEBUG( DBG_HIGH, ( "recycle: recycled file name: %s\n", final_name ) );		/* new filename with path */

	/* check if we should delete file from recycle bin */
	if( recycle_file_exist( handle, final_name ) ) 
	{
		if( recycle_versions( handle ) == False || 
			matchparam( recycle_noversions( handle ), base ) == True ) 
		{
			DEBUG( DBG_HIGH, ( "recycle: Removing old file %s from recycle bin\n", final_name ) );
			if( SMB_VFS_NEXT_UNLINK( handle, final_name ) != 0 ) 
			{
				DEBUG( DBG_HIGH, ( "recycle: Error deleting old file: %s\n", strerror( errno ) ) );
			}
		}
		else
		{
			/* rename file we move to recycle bin */
			i = 1;

			while( recycle_file_exist( handle, final_name ) )
			{
				SAFE_FREE( final_name );
				asprintf( &final_name, "%s/Copy(%d) of %s", temp_name, i, base );
				SAFE_FREE( db_column_fname );
				asprintf( &db_column_fname, "Copy(%d) of %s",  i++, base );
			}
		}
	}

	

	DEBUG( DBG_HIGH, ( "recycle: Moving %s to %s\n", file_name, final_name ) );
	rc = SMB_VFS_NEXT_RENAME( handle, file_name, final_name );

	if( rc != 0 ) 
	{
		DEBUG( DBG_HIGH, ( "recycle: Move error %d (%s), purging file %s (%s)\n", errno, strerror( errno ), file_name, final_name ) );
		rc = SMB_VFS_NEXT_UNLINK( handle, file_name );
		goto done;
	}

	/* touch access date of moved file */
	if( recycle_touch( handle ) == True || 
		recycle_touch_mtime( handle ) == True )
	{
		recycle_do_touch( handle, final_name, recycle_touch_mtime( handle ) );
	}

	/* insert item into database - (sqlite) */
	if( recycle_db_sqlite( handle ) == True ) 
	{
		DEBUG( DBG_LOW, ( "DB column (filename) = %s\n", db_column_fname ) );
		DEBUG( DBG_LOW, ( "DB column (filesize) = %.0f\n", ( double ) db_column_fsize ) );
		DEBUG( DBG_LOW, ( "DB column (original_path) = %s\n", db_column_fpath ) );
		request_insert_data_to_db( db_path, t_id, db_column_fname, db_column_fpath,( double ) db_column_fsize, disk_size, False );
		DEBUG( DBG_LOW, ( "1\n" ) );
	}

done:
	TALLOC_FREE( repository );
	DEBUG( DBG_LOW, ( "2\n" ) );
	
	SAFE_FREE( path_name );
	DEBUG( DBG_LOW, ( "3\n" ) );
	SAFE_FREE( temp_name );
	DEBUG( DBG_LOW, ( "4\n" ) );
	SAFE_FREE( final_name );
	DEBUG( DBG_LOW, ( "5\n" ) );

	SAFE_FREE( db_path );
	DEBUG( DBG_LOW, ( "6\n" ) );
	SAFE_FREE( db_column_fname );
	DEBUG( DBG_LOW, ( "7\n" ) );
	SAFE_FREE( db_column_fpath );
	DEBUG( DBG_LOW, ( "8\n" ) );

	return rc;
}

NTSTATUS vfs_recycle_init( void );
NTSTATUS vfs_recycle_init( void )
{
	NTSTATUS ret = smb_register_vfs( SMB_VFS_INTERFACE_VERSION, "recycle", _recycle_ops );

	if( !NT_STATUS_IS_OK( ret ) )
		return ret;
	
	_vfs_recycle_debug_level = debug_add_class( "recycle" );
	
	if( _vfs_recycle_debug_level == -1 ) 
	{
		_vfs_recycle_debug_level = DBGC_VFS;
		DEBUG( 0, ( "vfs_recycle: Couldn't register custom debugging class!\n" ) );
	} 
	else 
	{
		DEBUG( DBG_HIGH, ( "vfs_recycle: Debug class number of 'recycle': %d\n", _vfs_recycle_debug_level ) );
	}
	
	return ret;
}
