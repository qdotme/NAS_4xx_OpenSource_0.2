# Makefile for Microsoft Visual C++ 5.0 (or compat)

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
PERL=perl.exe
FLEX=flex.exe

!IFDEF DEBUG
OPT=/Od /Zi /MDd
LOPT=/DEBUG
DEBUGDEF=/D _DEBUG
OUTDIR=.\Debug
INTDIR=.\Debug
!ELSE
OPT=/O2 /MD
LOPT=
DEBUGDEF=/D NDEBUG
OUTDIR=.\Release
INTDIR=.\Release
!ENDIF

REFDOCDIR= ../../../doc/src/sgml/ref

ALL : sql_help.h psqlscan.c "..\..\port\pg_config_paths.h" "$(OUTDIR)\psql.exe"

CLEAN :
	-@erase "$(INTDIR)\command.obj"
	-@erase "$(INTDIR)\common.obj"
	-@erase "$(INTDIR)\copy.obj"
	-@erase "$(INTDIR)\describe.obj"
	-@erase "$(INTDIR)\help.obj"
	-@erase "$(INTDIR)\input.obj"
	-@erase "$(INTDIR)\large_obj.obj"
	-@erase "$(INTDIR)\mainloop.obj"
	-@erase "$(INTDIR)\mbprint.obj"
	-@erase "$(INTDIR)\print.obj"
	-@erase "$(INTDIR)\prompt.obj"
	-@erase "$(INTDIR)\psqlscan.obj"
	-@erase "$(INTDIR)\startup.obj"
	-@erase "$(INTDIR)\stringutils.obj"
	-@erase "$(INTDIR)\tab-complete.obj"
	-@erase "$(INTDIR)\variables.obj"
	-@erase "$(INTDIR)\exec.obj"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\getopt_long.obj"
	-@erase "$(INTDIR)\path.obj"
	-@erase "$(INTDIR)\pgstrcasecmp.obj"
	-@erase "$(INTDIR)\sprompt.obj"
	-@erase "$(INTDIR)\*psql.pch"
	-@erase "$(OUTDIR)\psql.exe"
	-@erase "$(INTDIR)\..\..\port\pg_config_paths.h"

"..\..\port\pg_config_paths.h": win32.mak
	echo #define PGBINDIR "" >$@
	echo #define PGSHAREDIR "" >>$@
	echo #define SYSCONFDIR "" >>$@
	echo #define INCLUDEDIR "" >>$@
	echo #define PKGINCLUDEDIR "" >>$@
	echo #define INCLUDEDIRSERVER "" >>$@
	echo #define LIBDIR "" >>$@
	echo #define PKGLIBDIR "" >>$@
	echo #define LOCALEDIR "" >>$@

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo $(OPT) /W3 /GX /D "WIN32" $(DEBUGDEF) /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR)\psql.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c \
 /I ..\..\include /I ..\..\interfaces\libpq /I ..\..\include\port\win32 \
 /D "HAVE_STRDUP" /D "FRONTEND"

CPP_OBJS=$(INTDIR)/
CPP_SBRS=.

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shfolder.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib wsock32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)\psql.pdb" /machine:I386 $(LOPT) /out:"$(OUTDIR)\psql.exe" 
LINK32_OBJS= \
	"$(INTDIR)\command.obj" \
	"$(INTDIR)\common.obj" \
	"$(INTDIR)\copy.obj" \
	"$(INTDIR)\describe.obj" \
	"$(INTDIR)\help.obj" \
	"$(INTDIR)\input.obj" \
	"$(INTDIR)\large_obj.obj" \
	"$(INTDIR)\mainloop.obj" \
	"$(INTDIR)\mbprint.obj" \
	"$(INTDIR)\print.obj" \
	"$(INTDIR)\prompt.obj" \
	"$(INTDIR)\psqlscan.obj" \
	"$(INTDIR)\startup.obj" \
	"$(INTDIR)\stringutils.obj" \
	"$(INTDIR)\tab-complete.obj" \
	"$(INTDIR)\variables.obj" \
	"$(INTDIR)\exec.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\getopt_long.obj" \
	"$(INTDIR)\path.obj" \
	"$(INTDIR)\pgstrcasecmp.obj" \
	"$(INTDIR)\sprompt.obj" \
!IFDEF DEBUG
	"..\..\interfaces\libpq\Debug\libpqddll.lib"
!ELSE
	"..\..\interfaces\libpq\Release\libpqdll.lib"
!ENDIF

"$(OUTDIR)\psql.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

"$(INTDIR)\exec.obj" : ..\..\port\exec.c
    $(CPP) @<<
    $(CPP_PROJ) ..\..\port\exec.c
<<

"$(OUTDIR)\getopt.obj" : "$(OUTDIR)" ..\..\port\getopt.c
    $(CPP) @<<
    $(CPP_PROJ) ..\..\port\getopt.c
<<

"$(OUTDIR)\getopt_long.obj" : "$(OUTDIR)" ..\..\port\getopt_long.c
    $(CPP) @<<
    $(CPP_PROJ) ..\..\port\getopt_long.c
<<

"$(OUTDIR)\path.obj" : "$(OUTDIR)" ..\..\port\path.c
    $(CPP) @<<
    $(CPP_PROJ) ..\..\port\path.c
<<

"$(INTDIR)\pgstrcasecmp.obj" : ..\..\port\pgstrcasecmp.c
    $(CPP) @<<
    $(CPP_PROJ) ..\..\port\pgstrcasecmp.c
<<

"$(OUTDIR)\sprompt.obj" : "$(OUTDIR)" ..\..\port\sprompt.c
    $(CPP) @<<
    $(CPP_PROJ) ..\..\port\sprompt.c
<<

sql_help.h : create_help.pl
        $(PERL) create_help.pl $(REFDOCDIR) $@
	
psqlscan.c: psqlscan.l
	$(FLEX) -Cfe -opsqlscan.c psqlscan.l

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


