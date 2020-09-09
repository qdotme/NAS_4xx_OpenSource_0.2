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
	integer to string conversion

	Author: bhoover@wecs.com
	Date: Jan 2008

	History:
		- first implemetnation
*/
#define MODULE_TAG "NUMBERS: "

#include <stdio.h>


void doNumConvert(long num,char *rStr,int *strLen,long *x)
{
  if((*x=num/10))

    doNumConvert(*x,rStr,strLen,x);

  *(rStr+(*(strLen))++)=num%10+'0';
}

int convertToString(double num,char *rStr)
{
  int    strLen=0;
  long   x;


  doNumConvert((long) num,rStr,&strLen,&x);   

  *(rStr+strLen)=0;


  return strLen;
}

char *numStr(double num,char *dest)
{


  convertToString(num,dest);


  return dest;
}
