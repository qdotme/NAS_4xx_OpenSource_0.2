# $Id: MKkeyname.awk,v 1.1.1.1 2007/01/11 00:47:25 wiley Exp $
##############################################################################
# Copyright (c) 1999-2005,2006 Free Software Foundation, Inc.                #
#                                                                            #
# Permission is hereby granted, free of charge, to any person obtaining a    #
# copy of this software and associated documentation files (the "Software"), #
# to deal in the Software without restriction, including without limitation  #
# the rights to use, copy, modify, merge, publish, distribute, distribute    #
# with modifications, sublicense, and/or sell copies of the Software, and to #
# permit persons to whom the Software is furnished to do so, subject to the  #
# following conditions:                                                      #
#                                                                            #
# The above copyright notice and this permission notice shall be included in #
# all copies or substantial portions of the Software.                        #
#                                                                            #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR #
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    #
# THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      #
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    #
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        #
# DEALINGS IN THE SOFTWARE.                                                  #
#                                                                            #
# Except as contained in this notice, the name(s) of the above copyright     #
# holders shall not be used in advertising or otherwise to promote the sale, #
# use or other dealings in this Software without prior written               #
# authorization.                                                             #
##############################################################################
BEGIN {
	print "/* generated by MKkeyname.awk */"
	print ""
	print "#include <curses.priv.h>"
	print "#include <tic.h>"
	print "#include <term_entry.h>"
	print ""
	print "const struct kn _nc_key_names[] = {"
}

/^[^#]/ {
	printf "\t{ \"%s\", %s },\n", $1, $1;
	}

END {
	printf "\t{ 0, 0 }};\n"
	print ""
	print "#define SIZEOF_TABLE 256"
	print "static char **keyname_table;"
	print ""
	print "NCURSES_EXPORT(NCURSES_CONST char *) keyname (int c)"
	print "{"
	print "	int i;"
	print "	char name[20];"
	print "	char *p;"
	print "	NCURSES_CONST char *result = 0;"
	print ""
	print "	if (c == -1) {"
	print "		result = \"-1\";"
	print "	} else {"
	print "		for (i = 0; _nc_key_names[i].name != 0; i++) {"
	print "			if (_nc_key_names[i].code == c) {"
	print "				result = (NCURSES_CONST char *)_nc_key_names[i].name;"
	print "				break;"
	print "			}"
	print "		}"
	print ""
	print "		if (result == 0 && (c >= 0 && c < SIZEOF_TABLE)) {"
	print "			if (keyname_table == 0)"
	print "				keyname_table = typeCalloc(char *, SIZEOF_TABLE);"
	print "			if (keyname_table != 0) {"
	print "				if (keyname_table[c] == 0) {"
	print "					int cc = c;"
	print "					p = name;"
	print "					if (cc >= 128) {"
	print "						strcpy(p, \"M-\");"
	print "						p += 2;"
	print "						cc -= 128;"
	print "					}"
	print "					if (cc < 32)"
	print "						sprintf(p, \"^%c\", cc + '@');"
	print "					else if (cc == 127)"
	print "						strcpy(p, \"^?\");"
	print "					else"
	print "						sprintf(p, \"%c\", cc);"
	print "					keyname_table[c] = strdup(name);"
	print "				}"
	print "				result = keyname_table[c];"
	print "			}"
	print "#if NCURSES_EXT_FUNCS && NCURSES_XNAMES"
	print "		} else if (result == 0 && cur_term != 0) {"
	print "			int j, k;"
	print "			char * bound;"
	print "			TERMTYPE *tp = &(cur_term->type);"
	print "			int save_trace = _nc_tracing;"
	print ""
	print "			_nc_tracing = 0;	/* prevent recursion via keybound() */"
	print "			for (j = 0; (bound = keybound(c, j)) != 0; ++j) {"
	print "				for(k = STRCOUNT; k < NUM_STRINGS(tp);  k++) {"
	print "					if (tp->Strings[k] != 0 && !strcmp(bound, tp->Strings[k])) {"
	print "						result = ExtStrname(tp, k, strnames);"
	print "						break;"
	print "					}"
	print "				}"
	print "				free(bound);"
	print "				if (result != 0)"
	print "					break;"
	print "			}"
	print "			_nc_tracing = save_trace;"
	print "#endif"
	print "		}"
	print "	}"
	print "	return result;"
	print "}"
	print ""
	print "#if USE_WIDEC_SUPPORT"
	print "NCURSES_EXPORT(NCURSES_CONST char *) key_name (wchar_t c)"
	print "{"
	print "	NCURSES_CONST char *result = keyname((int)c);"
	print "	if (!strncmp(result, \"M-\", 2)) result = 0;"
	print "	return result;"
	print "}"
	print "#endif"
	print ""
	print "#if NO_LEAKS"
	print "void _nc_keyname_leaks(void)"
	print "{"
	print "	int j;"
	print "	if (keyname_table != 0) {"
	print "		for (j = 0; j < SIZEOF_TABLE; ++j) {"
	print "			FreeIfNeeded(keyname_table[j]);"
	print "		}"
	print "		FreeAndNull(keyname_table);"
	print "	}"
	print "}"
	print "#endif /* NO_LEAKS */"
}
