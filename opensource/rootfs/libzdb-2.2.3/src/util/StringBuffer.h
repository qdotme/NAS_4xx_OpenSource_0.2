/*
 * Copyright (C) 2008 Tildeslash Ltd. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef STRINGBUFFER_INCLUDED
#define STRINGBUFFER_INCLUDED
#include <stdarg.h>


/** 
 * A <b>String Buffer</b> implements a mutable sequence of characters.
 *
 * @version \$Id: StringBuffer.h,v 1.1 2008/11/12 13:25:56 wiley Exp $
 * @file
 */


#define T StringBuffer_T
typedef struct T *T;


/**
 * Constructs a string buffer so that it represents the same sequence of 
 * characters as the string argument; in other  words, the initial contents 
 * of the string buffer is a copy of the argument string. 
 * @param s the initial contents of the buffer
 * @return A new StringBuffer object
 */
T StringBuffer_new(const char *s);


/**
 * Destroy a StringBuffer object and free allocated resources
 * @param S a StringBuffer object reference
 */
void StringBuffer_free(T *S);


/**
 * The characters of the String argument are appended, in order, to the 
 * contents of this string buffer, increasing the length of this string 
 * buffer by the length of the arguments. 
 * @param S StringBuffer object
 * @param s A string with optional var args
 * @return a reference to this StringBuffer
 */
T StringBuffer_append(T S, const char *s, ...);


/**
 * The characters of the String argument are appended, in order, to the 
 * contents of this string buffer, increasing the length of this string 
 * buffer by the length of the arguments. 
 * @param S StringBuffer object
 * @param s A string with optional var args
 * @param ap A variable argument list
 * @return a reference to this StringBuffer
 */
T StringBuffer_vappend(T S, const char *s, va_list ap);


/**
 * Returns the length (character count) of this string buffer.
 * @param S StringBuffer object
 * @return the length of the sequence of characters currently represented 
 * by this string buffer
 */
int StringBuffer_length(T S);


/**
 * Clear the contents of the string buffer. I.e. set buffer length to 0.
 * @param S StringBuffer object
 */
void StringBuffer_clear(T S);


/**
 * Converts to a string representing the data in this string buffer.
 * @param S StringBuffer object
 * @return a string representation of the string buffer 
 */
const char *StringBuffer_toString(T S);


#undef T
#endif
