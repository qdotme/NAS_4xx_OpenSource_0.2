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
/*

	History:
		February 2008 - path, file utility routines
  
*/

#define MODULE_TAG "PATH: "

#include <stdlib.h>

#include <stdio.h>
#include <ctype.h>
#include <memory.h>
#include <string.h>
#include <wctype.h>
#include "path.h"
#include "unicode_util.h"
#include "os.h"
#include "debug_if.h"
#include "safe_mem.h"

char *str_to_lwr(char *s)
{

	int		src_index=0;
	int		dest_index=0;
	int		bytes_copied;
	int		src_size;
	char	dest_char[7];


	if (!(s))

		return NULL;


	while(*(s+src_index)) {


		src_size=utf_8_encoded_size(s[src_index]);

		bytes_copied=utf_char_cpy(dest_char,towlower(utf_8_code_point((unsigned char *) (s+src_index))));


		memcpy((s+dest_index),dest_char,bytes_copied);

		src_index+=src_size;

		dest_index+=bytes_copied;
	}	

	*(s+dest_index)='\0';


	return s;
}

char *cross_platform_cwd(char **dir)
{

	#define	 MAX_CWD	4096

	unsigned int		buff_size=128;


#ifndef _WIN32

	*dir=safe_malloc(buff_size);


	while (!(getcwd(*dir,buff_size))) {

		buff_size+=128;

		free(*dir);

		if (buff_size>MAX_CWD) {

			*dir=NULL;

			return NULL;
		}

		*dir=safe_malloc(buff_size);
	}
#else

	if (!(isWinNT()))

  #ifndef UNICOWS

	{

		*dir=safe_malloc(buff_size);


		while (!(_getcwd(*dir,buff_size))) {

			buff_size+=128;

			free(*dir);

			if (buff_size>MAX_CWD) {

				*dir=NULL;

				return NULL;
			}

			*dir=safe_malloc(buff_size);
		}

		_strlwr(*dir);
	}

  #else

			;

  #endif

  #ifndef UNICOWS

	else

  #endif

		{

			wchar_t	*utf_16;

			utf_16=safe_malloc(buff_size);


			while (!(_wgetcwd(utf_16,buff_size))) {

				free(utf_16);

				buff_size+=128;

				if (buff_size>MAX_CWD) {

					*dir=NULL;

					return NULL;
				}

				utf_16=safe_malloc(buff_size);
			}	


			utf_16_to_8(utf_malloc_16_to_8(dir,utf_16),_wcslwr(utf_16));


			free(utf_16);
		}

#endif 


	return *dir;
}

#ifdef _WIN32

char *nt_console_name2(char **destName,char *srcName)
{

	int		buff_len;
	int		alloc_ret;


	buff_len=utf_8_len(srcName)+1; /*add 1 for appended dir separater*/


	if (!(isWinNT())) {

#ifndef UNICOWS

		while (1) {

			*destName=safe_malloc(buff_len+1);


			alloc_ret=GetShortPathName(srcName,*destName,buff_len+1);

			if (!(alloc_ret)) {

				strcpy(*destName,"");

				break;

			}
			else 

				if (alloc_ret<=buff_len)

					break;

				else {

					buff_len=alloc_ret;

					free(*destName);
				}
		}
	}

#else

		;
	}

#endif

#ifndef UNICOWS

	else

#endif

	{
		wchar_t *utf_16_src;
		wchar_t *utf_16_dest;
		char	*thisDest=NULL;


		utf_8_to_16(utf_malloc_8_to_16(&utf_16_src,srcName),srcName);

		SetLastError(0);


		while(1) {

			alloc_ret=buff_len*2+sizeof(wchar_t);

			*destName=safe_malloc(alloc_ret);

			utf_16_dest=safe_malloc(alloc_ret);

			memset(utf_16_dest,0,alloc_ret);


			alloc_ret=GetShortPathNameW(utf_16_src,utf_16_dest,buff_len+1);


			if (!(alloc_ret)) {

				strcpy(*destName,"");

				break;
			}
			else

				if (alloc_ret<=buff_len) {

					strcpy(*destName,utf_16_to_8(utf_malloc_16_to_8(&thisDest,utf_16_dest),utf_16_dest));

					free(thisDest);

					break;

				}
				else {

					buff_len=alloc_ret;

					free(*destName);
					free(utf_16_dest);
				}
		}

		free(utf_16_src);
		free(utf_16_dest);
	}
	

	return *destName;
}

char *nt_console_name(char **destName,char *path,char *srcName)
{

	char	*tmp_buff;



	tmp_buff=safe_malloc(strlen(path)+strlen(srcName)+2);


	nt_console_name2(destName,strcat(strcat(strcpy(tmp_buff,path),"\\"),srcName));



	free(tmp_buff);
	

	return *destName;
}

#endif

/*
returns start position of subS, within s, or NULL if subS is not within s
*/
unsigned int subStr(char *s,char *subS)
{

	char			*p_s=s;
	unsigned int	len_subS;


	if (!(s && subS))

		return 0;

	len_subS=strlen(subS);

	while(1) {

		if ((strlen(p_s)<len_subS))

			return 0;

		else

			if (strncmp(p_s,subS,len_subS))

				p_s++;

			else

				return p_s-s+1;
				
	}
}

char *cat_root(char *root,char *ancestor)
{

	if (!(strcmp(root+strlen(root)-1,dir_delim_str))) 

		strcat(root,ancestor);

	else

		strcat(strcat(root,dir_delim_str),ancestor);


	return root;
}

char *path_ancestor(char *a,char *path,int numAnc)
{

	int		i;
	char	*path_buff;
	char	*p_match;


	if (strlen(path)<=ROOT_LEN)

		return NULL;


	path_buff=safe_malloc(strlen(path)+1);

	strcpy(path_buff,path);

	
	for (i=0;i<numAnc;i++) {  /*back up numAnc number of parents*/

		p_match=strrchr(path_buff,dir_delim);


		if (p_match)

			path_buff[(p_match-path_buff)]=0;
	}

	if (p_match)

		strcpy(a,path_buff);

	else

		a=NULL;


	free(path_buff);


	return a;
}

#ifdef _WIN32

int is_file_name(char *s)
{

	WIN32_FIND_DATA	FindFileData;
	char			*dest;
	int				retVal;


	dest=safe_malloc(strlen(s)+1);



	retVal=(!(FindFirstFile(nt_console_name2(&dest,s),&FindFileData)==INVALID_HANDLE_VALUE));


	
	free(dest);

	
	if (retVal)

		return (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));


	return retVal;
}

char *modulePath(FILE *pLOG_FILE,char **dir)
{	

	unsigned int	path_len=128;


	if (!(isWinNT()))

#ifndef UNICOWS

		while (1) {

			*dir=safe_malloc(path_len);


			if (!(GetModuleFileName(NULL,*dir,path_len))) {

				free(*dir);

				return NULL;
			}

			if (is_file_name(*dir))

				break;


			free(*dir);

			path_len+=128;
		}
#else

		;
#endif

#ifndef UNICOWS

	else

#endif
	
	{
		char *utf_8;

		path_len=256;


		while (1) {

			*dir=safe_malloc(path_len);

			memset(*dir,0,path_len);


			if (!(GetModuleFileNameW(NULL,(wchar_t *) *dir,path_len/2))) {

				free(*dir);

				return NULL;
			}


			utf_16_to_8(utf_malloc_16_to_8(&utf_8,(wchar_t *) *dir),(wchar_t *) *dir);

			free(*dir);

			*dir=safe_malloc(strlen(utf_8)+1);

			strcpy(*dir,utf_8);

			free(utf_8);


			if (is_file_name(*dir))

				break;


			free(*dir);

			path_len+=256;
		}
	}


	path_ancestor(*dir,*dir,1);


	return *dir;
}

#endif

int is_file(char *name)
{

	FILE		*p_f=NULL;


#ifdef _WIN32

	return is_file_name(name);

#else

	if (!(p_f=fopen(name,"r")))

		return 0;


	fclose(p_f);


	return 1;

#endif

}

char *ancestor_path(char **a_path,char *ancestor,char *path)
{

	char		*a_path_buff;
	char		*p_a_path_buff;	

#ifdef _WIN32

	char		*nt_cons_name;
#endif

	int			anc_len=strlen(ancestor);


	if (!(path))

		return NULL;


	/*check for root*/

	if (anc_len==ROOT_LEN) {

#ifndef _WIN32

		if (!(strstr(path,ancestor)-path))

#else

		if (!(strstr(str_to_lwr(path),str_to_lwr(ancestor))-path))		

#endif

			return ancestor;

		else

			return NULL;
	}

	a_path_buff=safe_malloc(strlen(path)+1);

	strcpy(a_path_buff,path);

	p_a_path_buff=a_path_buff;


	while (1) {

#ifdef _WIN32


		if (strlen(nt_console_name(&nt_cons_name,a_path_buff,ancestor))) {

			strcpy(a_path_buff,nt_cons_name);

			break;
		}

#else

		{
			char	*delim_match;

		
			delim_match=strrchr(a_path_buff,dir_delim);

			if (subStr(delim_match,ancestor))

				break;
		}

#endif

		p_a_path_buff=path_ancestor(a_path_buff,a_path_buff,1);


		if (!(p_a_path_buff))

			break;
	}

	if (!(p_a_path_buff))

		*a_path=NULL;

	else {

		*a_path=safe_malloc(strlen(p_a_path_buff)+1);

		strcpy(*a_path,p_a_path_buff);
	}


	free(a_path_buff);


#ifdef _WIN32

	free(nt_cons_name);

#endif

	return *a_path;
}
