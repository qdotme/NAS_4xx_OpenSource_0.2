/*
Copyright (C) 2008 Bryan Hoover (bhoover@wecs.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifdef _WIN32

#define dir_delim_str "\\"
#define dir_delim '\\'

#define ROOT_LEN 3

#include <direct.h>

#else

#define dir_delim_str "/"
#define dir_delim '/'

#define ROOT_LEN 1

#endif

#include <stdio.h>

char *str_to_lwr(char *s);
char *cross_platform_cwd(char **dir);
char *nt_console_name(char **destName,char *path,char *srcName);
char *nt_console_name2(char **destName,char *srcName);
int is_file_name(char *s);
unsigned int subStr(char *s,char *subS);
char *cat_root(char *root,char *ancestor);
char *path_ancestor(char *a,char *path,int numAnc);
char *modulePath(FILE *pLOG_FILE,char **dir);
char *ancestor_path(char **a_path,char *ancestor,char *path);
int is_file(char *name);
