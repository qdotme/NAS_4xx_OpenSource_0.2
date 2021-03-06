/* Generated by re2c 0.5 on Wed Jun  4 09:00:32 2003 */
#line 1 "ircg_scanner.re"
/*
   +----------------------------------------------------------------------+
   | PHP Version 4                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2006 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Sascha Schumann <sascha@schumann.cx>                         |
   +----------------------------------------------------------------------+
 */

/* $Id: ircg_scanner.c,v 1.1.1.1 2006/03/24 06:13:33 wiley Exp $ */

#include <ext/standard/php_smart_str.h>
#include <stdio.h>
#include <string.h>

static const char *color_list[] = {
    "white",
    "black",
    "blue",
    "green",
    "red",
    "brown",
    "purple",
    "orange",
    "yellow",
    "lightgreen",
    "teal",
    "lightcyan",
    "lightblue",
    "#ff00ff",
    "#bebebe",
    "lightgrey"
};


typedef struct {
	int bg_code;
	int fg_code;
	int font_tag_open;
	int bold_tag_open;
	int underline_tag_open;
	int italic_tag_open;
	char fg_color[6];
	char bg_color[6];
	
	smart_str scheme;
	smart_str *result;
} ircg_msg_scanner;

#line 75


#define YYFILL(n) do { } while (0)
#define YYCTYPE unsigned char
#define YYCURSOR xp
#define YYLIMIT end
#define YYMARKER q

#define STD_PARA ircg_msg_scanner *ctx, const char *start, const char *YYCURSOR
#define STD_ARGS ctx, start, YYCURSOR

#define passthru() do {									\
	size_t __len = xp - start;							\
	smart_str_appendl_ex(mctx.result, start, __len, 1); \
} while (0)

static inline void handle_scheme(STD_PARA)
{
	ctx->scheme.len = 0;
	smart_str_appendl_ex(&ctx->scheme, start, YYCURSOR - start, 1);
	smart_str_0(&ctx->scheme);
}

static inline void handle_url(STD_PARA)
{
	smart_str_appends_ex(ctx->result, "<a target=blank href=\"", 1);
	smart_str_append_ex(ctx->result, &ctx->scheme, 1);
	smart_str_appendl_ex(ctx->result, start, YYCURSOR - start, 1);
	smart_str_appends_ex(ctx->result, "\">", 1);
	smart_str_append_ex(ctx->result, &ctx->scheme, 1);
	smart_str_appendl_ex(ctx->result, start, YYCURSOR - start, 1);
	smart_str_appends_ex(ctx->result, "</a>", 1);
}

static void handle_color_digit(STD_PARA, int mode)
{
	int len;
	int nr;

	len = YYCURSOR - start;
	switch (len) {
		case 2:
			nr = (start[0] - '0') * 10 + (start[1] - '0');
			break;
		case 1:
			nr = start[0] - '0';
			break;
	}
	
	switch (mode) {
		case 0: ctx->fg_code = nr; break;
		case 1: ctx->bg_code = nr; break;
	}
}

static void handle_hex(STD_PARA, int mode)
{
	memcpy(mode == 0 ? ctx->fg_color : ctx->bg_color, start, 6);
}

#define IS_VALID_CODE(n) (n >= 0 && n <= 15)

static void finish_color_stuff(STD_PARA)
{
	if (ctx->font_tag_open) {
		smart_str_appends_ex(ctx->result, "</font>", 1);
		ctx->font_tag_open = 0;
	}
}

static void handle_bold(STD_PARA, int final)
{
	switch (ctx->bold_tag_open) {
	case 0:
		if (!final) smart_str_appends_ex(ctx->result, "<b>", 1);
		break;
	case 1:
		smart_str_appends_ex(ctx->result, "</b>", 1);
		break;
	}

	ctx->bold_tag_open = 1 - ctx->bold_tag_open;
}

static void handle_underline(STD_PARA, int final)
{
	switch (ctx->underline_tag_open) {
	case 0:
		if (!final) smart_str_appends_ex(ctx->result, "<u>", 1);
		break;
	case 1:
		smart_str_appends_ex(ctx->result, "</u>", 1);
		break;
	}

	ctx->underline_tag_open = 1 - ctx->underline_tag_open;
}

static void handle_italic(STD_PARA, int final)
{
	switch (ctx->italic_tag_open) {
	case 0:
		if (!final) smart_str_appends_ex(ctx->result, "<i>", 1);
		break;
	case 1:
		smart_str_appends_ex(ctx->result, "</i>", 1);
		break;
	}

	ctx->italic_tag_open = 1 - ctx->italic_tag_open;
}

static void commit_color_stuff(STD_PARA)
{
	finish_color_stuff(STD_ARGS);

	if (IS_VALID_CODE(ctx->fg_code)) {
		smart_str_appends_ex(ctx->result, "<font color=\"", 1);
		smart_str_appends_ex(ctx->result, color_list[ctx->fg_code], 1);
		smart_str_appends_ex(ctx->result, "\">", 1);
		ctx->font_tag_open = 1;
	}
}

static void commit_color_hex(STD_PARA)
{
	finish_color_stuff(STD_ARGS);

	if (ctx->fg_color[0] != 0) {
		smart_str_appends_ex(ctx->result, "<font color=\"", 1);
		smart_str_appendl_ex(ctx->result, ctx->fg_color, 6, 1);
		smart_str_appends_ex(ctx->result, "\">", 1);
		ctx->font_tag_open = 1;
	}
}

static void add_entity(STD_PARA, const char *entity)
{
	smart_str_appends_ex(ctx->result, entity, 1);
}

void ircg_mirc_color(const char *msg, smart_str *result, size_t msg_len, int auto_links, int gen_br) 
{
	const char *end, *xp, *q, *start;
	ircg_msg_scanner mctx, *ctx = &mctx;

	mctx.result = result;
	mctx.scheme.c = NULL;
	mctx.italic_tag_open = mctx.font_tag_open = mctx.bold_tag_open = mctx.underline_tag_open = 0;
	
	if (msg_len == -1)
		msg_len = strlen(msg);
	end = msg + msg_len;
	xp = msg;
	

state_plain:
	if (xp >= end) goto stop;
	start = YYCURSOR;
{
	YYCTYPE yych;
	unsigned int yyaccept;
	goto yy0;
yy1:	++YYCURSOR;
yy0:
	if((YYLIMIT - YYCURSOR) < 4) YYFILL(4);
	yych = *YYCURSOR;
	switch(yych){
	case '\000':	goto yy2;
	case '\002':	goto yy19;
	case '\003':	goto yy5;
	case '\004':	goto yy7;
	case '\035':	goto yy23;
	case '\036':	goto yy17;
	case '\037':	goto yy21;
	case '&':	goto yy13;
	case '<':	goto yy9;
	case '>':	goto yy11;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy3;
	case '\204':	case '\223':
	case '\224':	goto yy15;
	default:	goto yy25;
	}
yy2:	YYCURSOR = YYMARKER;
	switch(yyaccept){
	case 0:	goto yy4;
	}
yy3:	yyaccept = 0;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case ':':	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy27;
	default:	goto yy4;
	}
yy4:
#line 246
	{ passthru(); goto state_plain; }
yy5:	yych = *++YYCURSOR;
yy6:
#line 236
	{ mctx.fg_code = mctx.bg_code = -1; goto state_color_fg; }
yy7:	yych = *++YYCURSOR;
yy8:
#line 237
	{ mctx.fg_color[0] = mctx.bg_color[0] = 0; goto state_color_hex; }
yy9:	yych = *++YYCURSOR;
yy10:
#line 238
	{ add_entity(STD_ARGS, "&lt;"); goto state_plain; }
yy11:	yych = *++YYCURSOR;
yy12:
#line 239
	{ add_entity(STD_ARGS, "&gt;"); goto state_plain; }
yy13:	yych = *++YYCURSOR;
yy14:
#line 240
	{ add_entity(STD_ARGS, "&amp;"); goto state_plain; }
yy15:	yych = *++YYCURSOR;
yy16:
#line 241
	{ add_entity(STD_ARGS, "&quot;"); goto state_plain; }
yy17:	yych = *++YYCURSOR;
yy18:
#line 242
	{ if (gen_br) smart_str_appendl_ex(ctx->result, "<br>", 4, 1); goto state_plain; }
yy19:	yych = *++YYCURSOR;
yy20:
#line 243
	{ handle_bold(STD_ARGS, 0); goto state_plain; }
yy21:	yych = *++YYCURSOR;
yy22:
#line 244
	{ handle_underline(STD_ARGS, 0); goto state_plain; }
yy23:	yych = *++YYCURSOR;
yy24:
#line 245
	{ handle_italic(STD_ARGS, 0); goto state_plain; }
yy25:	yych = *++YYCURSOR;
	goto yy4;
yy26:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
yy27:	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy26;
	case ':':	goto yy28;
	default:	goto yy2;
	}
yy28:	yych = *++YYCURSOR;
	switch(yych){
	case '/':	goto yy29;
	default:	goto yy2;
	}
yy29:	yych = *++YYCURSOR;
	switch(yych){
	case '/':	goto yy30;
	default:	goto yy2;
	}
yy30:	yych = *++YYCURSOR;
yy31:
#line 235
	{ if (auto_links) { handle_scheme(STD_ARGS); goto state_url; } else { passthru(); goto state_plain; } }
}
#line 247


state_color_hex:
	start = YYCURSOR;
{
	YYCTYPE yych;
	unsigned int yyaccept;
	goto yy32;
yy33:	++YYCURSOR;
yy32:
	if((YYLIMIT - YYCURSOR) < 6) YYFILL(6);
	yych = *YYCURSOR;
	switch(yych){
	case ',':	goto yy36;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy34;
	default:	goto yy38;
	}
yy34:	yyaccept = 0;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy39;
	default:	goto yy35;
	}
yy35:
#line 254
	{ finish_color_stuff(STD_ARGS); passthru(); goto state_plain; }
yy36:	yych = *++YYCURSOR;
yy37:
#line 253
	{ goto state_color_hex_bg; }
yy38:	yych = *++YYCURSOR;
	goto yy35;
yy39:	yych = *++YYCURSOR;
	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy41;
	default:	goto yy40;
	}
yy40:	YYCURSOR = YYMARKER;
	switch(yyaccept){
	case 0:	goto yy35;
	}
yy41:	yych = *++YYCURSOR;
	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy42;
	default:	goto yy40;
	}
yy42:	yych = *++YYCURSOR;
	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy43;
	default:	goto yy40;
	}
yy43:	yych = *++YYCURSOR;
	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy44;
	default:	goto yy40;
	}
yy44:	yych = *++YYCURSOR;
yy45:
#line 252
	{ handle_hex(STD_ARGS, 0); goto state_color_hex_bg; }
}
#line 255


	
state_color_hex_comma:	
	start = YYCURSOR;
{
	YYCTYPE yych;
	unsigned int yyaccept;
	goto yy46;
yy47:	++YYCURSOR;
yy46:
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych){
	case ',':	goto yy48;
	default:	goto yy50;
	}
yy48:	yych = *++YYCURSOR;
yy49:
#line 261
	{ goto state_color_hex_bg; }
yy50:	yych = *++YYCURSOR;
yy51:
#line 262
	{ YYCURSOR--; commit_color_hex(STD_ARGS); goto state_plain; }
}
#line 263



state_color_hex_bg:
	start = YYCURSOR;
{
	YYCTYPE yych;
	unsigned int yyaccept;
	goto yy52;
yy53:	++YYCURSOR;
yy52:
	if((YYLIMIT - YYCURSOR) < 6) YYFILL(6);
	yych = *YYCURSOR;
	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy54;
	default:	goto yy56;
	}
yy54:	yyaccept = 0;
	yych = *(YYMARKER = ++YYCURSOR);
	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy57;
	default:	goto yy55;
	}
yy55:
#line 270
	{ commit_color_hex(STD_ARGS); passthru(); goto state_plain; }
yy56:	yych = *++YYCURSOR;
	goto yy55;
yy57:	yych = *++YYCURSOR;
	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy59;
	default:	goto yy58;
	}
yy58:	YYCURSOR = YYMARKER;
	switch(yyaccept){
	case 0:	goto yy55;
	}
yy59:	yych = *++YYCURSOR;
	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy60;
	default:	goto yy58;
	}
yy60:	yych = *++YYCURSOR;
	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy61;
	default:	goto yy58;
	}
yy61:	yych = *++YYCURSOR;
	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy62;
	default:	goto yy58;
	}
yy62:	yych = *++YYCURSOR;
yy63:
#line 269
	{ handle_hex(STD_ARGS, 1); commit_color_hex(STD_ARGS); goto state_plain; }
}
#line 271


state_url:
	start = YYCURSOR;
{
	YYCTYPE yych;
	unsigned int yyaccept;
	goto yy64;
yy65:	++YYCURSOR;
yy64:
	if((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
	switch(yych){
	case '!':	case '#':
	case '$':
	case '%':
	case '&':
	case '\'':
	case '(':
	case ')':
	case '*':
	case '+':
	case ',':
	case '-':
	case '.':
	case '/':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case ':':
	case ';':	case '=':	case '?':
	case '@':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':	case '_':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	case '~':	goto yy66;
	default:	goto yy68;
	}
yy66:	yych = *++YYCURSOR;
	goto yy71;
yy67:
#line 276
	{ handle_url(STD_ARGS); goto state_plain; }
yy68:	yych = *++YYCURSOR;
yy69:
#line 277
	{ passthru(); goto state_plain; }
yy70:	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
yy71:	switch(yych){
	case '!':	case '#':
	case '$':
	case '%':
	case '&':
	case '\'':
	case '(':
	case ')':
	case '*':
	case '+':
	case ',':
	case '-':
	case '.':
	case '/':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case ':':
	case ';':	case '=':	case '?':
	case '@':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':	case '_':	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	case '~':	goto yy70;
	default:	goto yy67;
	}
}
#line 278



state_color_fg:		
	start = YYCURSOR;
{
	YYCTYPE yych;
	unsigned int yyaccept;
	goto yy72;
yy73:	++YYCURSOR;
yy72:
	if((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
	switch(yych){
	case ',':	goto yy76;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy74;
	default:	goto yy78;
	}
yy74:	yych = *++YYCURSOR;
	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy80;
	default:	goto yy75;
	}
yy75:
#line 284
	{ handle_color_digit(STD_ARGS, 0); goto state_color_comma; }
yy76:	yych = *++YYCURSOR;
yy77:
#line 285
	{ goto state_color_bg; }
yy78:	yych = *++YYCURSOR;
yy79:
#line 286
	{ finish_color_stuff(STD_ARGS); passthru(); goto state_plain; }
yy80:	yych = *++YYCURSOR;
	goto yy75;
}
#line 287


	
state_color_comma:	
	start = YYCURSOR;
{
	YYCTYPE yych;
	unsigned int yyaccept;
	goto yy81;
yy82:	++YYCURSOR;
yy81:
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch(yych){
	case ',':	goto yy83;
	default:	goto yy85;
	}
yy83:	yych = *++YYCURSOR;
yy84:
#line 293
	{ goto state_color_bg; }
yy85:	yych = *++YYCURSOR;
yy86:
#line 294
	{ YYCURSOR--; commit_color_stuff(STD_ARGS); goto state_plain; }
}
#line 295

	

state_color_bg:
	start = YYCURSOR;
{
	YYCTYPE yych;
	unsigned int yyaccept;
	goto yy87;
yy88:	++YYCURSOR;
yy87:
	if((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy89;
	default:	goto yy91;
	}
yy89:	yych = *++YYCURSOR;
	switch(yych){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy93;
	default:	goto yy90;
	}
yy90:
#line 301
	{ handle_color_digit(STD_ARGS, 1); commit_color_stuff(STD_ARGS); goto state_plain; }
yy91:	yych = *++YYCURSOR;
yy92:
#line 302
	{ commit_color_stuff(STD_ARGS); passthru(); goto state_plain; }
yy93:	yych = *++YYCURSOR;
	goto yy90;
}
#line 303


stop:
	smart_str_free_ex(&ctx->scheme, 1);

	finish_color_stuff(STD_ARGS);
	handle_bold(STD_ARGS, 1);
	handle_underline(STD_ARGS, 1);
	handle_italic(STD_ARGS, 1);
}
