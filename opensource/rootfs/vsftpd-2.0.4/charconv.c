/*
 * Part of Very Secure FTPd
 * Licence: GPL v2
 * Author: Dmitriy Balashov
 * charconv.c
 */

#include "charconv.h"
#include "tunables.h"
#include "session.h"
#include "str.h"
#include "sysutil.h"

#ifdef VSFTP_CHARCONV_SUPPORT_CYRILLIC
#include "char_maps/cyrillic.map"
#endif
#ifdef VSFTP_CHARCONV_SUPPORT_WESTERN
#include "char_maps/western.map"
#endif
#ifdef VSFTP_CHARCONV_SUPPORT_CENTRAL
#include "char_maps/central.map"
#endif
#ifdef VSFTP_CHARCONV_SUPPORT_SOUTERN
#include "char_maps/soutern.map"
#endif

/* Tables mapping supported codepage names to runtime variables */
static struct available_charsets
{
  const char* p_charset_name;
  int p_variable;
}
available_charsets_array[] =
{
  { VSFTP_CP_NONE      , VSFTP_C_NONE      },
  // Cyrillic
#ifdef VSFTP_CHARCONV_SUPPORT_CYRILLIC
  { VSFTP_CP_UTF_8     , VSFTP_C_UTF8      },
  { VSFTP_CP_UTF8      , VSFTP_C_UTF8      },
  { VSFTP_CP_WIN_1251  , VSFTP_C_1251      },
  { VSFTP_CP_WIN1251   , VSFTP_C_1251      },
  { VSFTP_CP_1251      , VSFTP_C_1251      },
  { VSFTP_CP_KOI8_R    , VSFTP_C_878       },
  { VSFTP_CP_KOI8R     , VSFTP_C_878       },
  { VSFTP_CP_878       , VSFTP_C_878       },
  { VSFTP_CP_IBM866    , VSFTP_C_866       },
  { VSFTP_CP_866       , VSFTP_C_866       },
  { VSFTP_CP_ISO8859_5 , VSFTP_C_ISO5      },
  { VSFTP_CP_ISO5      , VSFTP_C_ISO5      },
#endif
  // Western European
#ifdef VSFTP_CHARCONV_SUPPORT_WESTERN
  { VSFTP_CP_ISO8859_1 , VSFTP_C_ISO1      },
  { VSFTP_CP_ISO1      , VSFTP_C_ISO1      },
  { VSFTP_CP_LATIN1    , VSFTP_C_ISO1      },
  { VSFTP_CP_ISO8859_15, VSFTP_C_ISO15     },
  { VSFTP_CP_ISO15     , VSFTP_C_ISO15     },
  { VSFTP_CP_LATIN9    , VSFTP_C_ISO15     },
  { VSFTP_CP_WIN_1252  , VSFTP_C_1252      },
  { VSFTP_CP_WIN1252   , VSFTP_C_1252      },
  { VSFTP_CP_1252      , VSFTP_C_1252      },
#endif
  // Central European
#ifdef VSFTP_CHARCONV_SUPPORT_CENTRAL
  { VSFTP_CP_ISO8859_2 , VSFTP_C_ISO2      },
  { VSFTP_CP_ISO2      , VSFTP_C_ISO2      },
  { VSFTP_CP_LATIN2    , VSFTP_C_ISO2      },
  { VSFTP_CP_ISO8859_16, VSFTP_C_ISO16     },
  { VSFTP_CP_ISO16     , VSFTP_C_ISO16     },
  { VSFTP_CP_WIN_1250  , VSFTP_C_1250      },
  { VSFTP_CP_WIN1250   , VSFTP_C_1250      },
  { VSFTP_CP_1250      , VSFTP_C_1250      },
#endif
  // Soutern European
#ifdef VSFTP_CHARCONV_SUPPORT_SOUTERN
  { VSFTP_CP_ISO8859_3 , VSFTP_C_ISO3      },
  { VSFTP_CP_ISO3      , VSFTP_C_ISO3      },
  { VSFTP_CP_LATIN3    , VSFTP_C_ISO3      },
#endif
  { 0, 0 }
};

/* Available convertions */
static struct available_convertions
{
  int local;
  int remote;
  int localCharset;
  int remoteCharset;
}
available_convertions_array[] =
{ // Cyrillic
#ifdef VSFTP_CHARCONV_SUPPORT_CYRILLIC
  { VSFTP_C_UTF8      , VSFTP_C_1251      , VSFTP_CS_UTF8CYR   , VSFTP_CS_1251      },
  { VSFTP_C_UTF8      , VSFTP_C_878       , VSFTP_CS_UTF8CYR   , VSFTP_CS_878       },
  { VSFTP_C_UTF8      , VSFTP_C_866       , VSFTP_CS_UTF8CYR   , VSFTP_CS_866       },
  { VSFTP_C_UTF8      , VSFTP_C_ISO5      , VSFTP_CS_UTF8CYR   , VSFTP_CS_ISO5      },
  { VSFTP_C_1251      , VSFTP_C_UTF8      , VSFTP_CS_1251      , VSFTP_CS_UTF8CYR   },
  { VSFTP_C_1251      , VSFTP_C_878       , VSFTP_CS_1251      , VSFTP_CS_878       },
  { VSFTP_C_1251      , VSFTP_C_866       , VSFTP_CS_1251      , VSFTP_CS_866       },
  { VSFTP_C_1251      , VSFTP_C_ISO5      , VSFTP_CS_1251      , VSFTP_CS_ISO5      },
  { VSFTP_C_878       , VSFTP_C_UTF8      , VSFTP_CS_878       , VSFTP_CS_UTF8CYR   },
  { VSFTP_C_878       , VSFTP_C_1251      , VSFTP_CS_878       , VSFTP_CS_1251      },
  { VSFTP_C_878       , VSFTP_C_866       , VSFTP_CS_878       , VSFTP_CS_866       },
  { VSFTP_C_878       , VSFTP_C_ISO5      , VSFTP_CS_878       , VSFTP_CS_ISO5      },
  { VSFTP_C_866       , VSFTP_C_UTF8      , VSFTP_CS_866       , VSFTP_CS_UTF8CYR   },
  { VSFTP_C_866       , VSFTP_C_1251      , VSFTP_CS_866       , VSFTP_CS_1251      },
  { VSFTP_C_866       , VSFTP_C_878       , VSFTP_CS_866       , VSFTP_CS_878       },
  { VSFTP_C_866       , VSFTP_C_ISO5      , VSFTP_CS_866       , VSFTP_CS_ISO5      },
  { VSFTP_C_ISO5      , VSFTP_C_UTF8      , VSFTP_CS_ISO5      , VSFTP_CS_UTF8CYR   },
  { VSFTP_C_ISO5      , VSFTP_C_1251      , VSFTP_CS_ISO5      , VSFTP_CS_1251      },
  { VSFTP_C_ISO5      , VSFTP_C_878       , VSFTP_CS_ISO5      , VSFTP_CS_878       },
  { VSFTP_C_ISO5      , VSFTP_C_866       , VSFTP_CS_ISO5      , VSFTP_CS_866       },
#endif
  // Western European
#ifdef VSFTP_CHARCONV_SUPPORT_WESTERN
  { VSFTP_C_UTF8      , VSFTP_C_ISO1      , VSFTP_CS_UTF8WEST  , VSFTP_CS_ISO1      },
  { VSFTP_C_UTF8      , VSFTP_C_ISO15     , VSFTP_CS_UTF8WEST  , VSFTP_CS_ISO15     },
  { VSFTP_C_UTF8      , VSFTP_C_1252      , VSFTP_CS_UTF8WEST  , VSFTP_CS_1252      },
  { VSFTP_C_ISO1      , VSFTP_C_UTF8      , VSFTP_CS_ISO1      , VSFTP_CS_UTF8WEST  },
  { VSFTP_C_ISO1      , VSFTP_C_ISO15     , VSFTP_CS_ISO1      , VSFTP_CS_ISO15     },
  { VSFTP_C_ISO1      , VSFTP_C_1252      , VSFTP_CS_ISO1      , VSFTP_CS_1252      },
  { VSFTP_C_ISO15     , VSFTP_C_UTF8      , VSFTP_CS_ISO15     , VSFTP_CS_UTF8WEST  },
  { VSFTP_C_ISO15     , VSFTP_C_ISO1      , VSFTP_CS_ISO15     , VSFTP_CS_ISO1      },
  { VSFTP_C_ISO15     , VSFTP_C_1252      , VSFTP_CS_ISO15     , VSFTP_CS_1252      },
  { VSFTP_C_1252      , VSFTP_C_UTF8      , VSFTP_CS_1252      , VSFTP_CS_UTF8WEST  },
  { VSFTP_C_1252      , VSFTP_C_ISO1      , VSFTP_CS_1252      , VSFTP_CS_ISO1      },
  { VSFTP_C_1252      , VSFTP_C_ISO15     , VSFTP_CS_1252      , VSFTP_CS_ISO15     },
#endif
  // Central European
#ifdef VSFTP_CHARCONV_SUPPORT_CENTRAL
  { VSFTP_C_UTF8      , VSFTP_C_ISO2      , VSFTP_CS_UTF8CENT  , VSFTP_CS_ISO2      },
  { VSFTP_C_UTF8      , VSFTP_C_ISO16     , VSFTP_CS_UTF8CENT  , VSFTP_CS_ISO16     },
  { VSFTP_C_UTF8      , VSFTP_C_1250      , VSFTP_CS_UTF8CENT  , VSFTP_CS_1250      },
  { VSFTP_C_ISO2      , VSFTP_C_UTF8      , VSFTP_CS_ISO2      , VSFTP_CS_UTF8CENT  },
  { VSFTP_C_ISO2      , VSFTP_C_ISO16     , VSFTP_CS_ISO2      , VSFTP_CS_ISO16     },
  { VSFTP_C_ISO2      , VSFTP_C_1250      , VSFTP_CS_ISO2      , VSFTP_CS_1250      },
  { VSFTP_C_ISO16     , VSFTP_C_UTF8      , VSFTP_CS_ISO16     , VSFTP_CS_UTF8CENT  },
  { VSFTP_C_ISO16     , VSFTP_C_ISO2      , VSFTP_CS_ISO16     , VSFTP_CS_ISO2      },
  { VSFTP_C_ISO16     , VSFTP_C_1250      , VSFTP_CS_ISO16     , VSFTP_CS_1250      },
  { VSFTP_C_1250      , VSFTP_C_UTF8      , VSFTP_CS_1250      , VSFTP_CS_UTF8CENT  },
  { VSFTP_C_1250      , VSFTP_C_ISO2      , VSFTP_CS_1250      , VSFTP_CS_ISO2      },
  { VSFTP_C_1250      , VSFTP_C_ISO16     , VSFTP_CS_1250      , VSFTP_CS_ISO16     },
#endif
  // Soutern European
#ifdef VSFTP_CHARCONV_SUPPORT_SOUTERN
  { VSFTP_C_UTF8      , VSFTP_C_ISO3      , VSFTP_CS_UTF8SOUT  , VSFTP_CS_ISO3      },
  { VSFTP_C_ISO3      , VSFTP_C_UTF8      , VSFTP_CS_ISO3      , VSFTP_CS_UTF8SOUT  },
#endif

  { 0, 0, 0, 0 }
};

map_ptr map_array[] =
{
  0,
#ifdef VSFTP_CHARCONV_SUPPORT_CYRILLIC
  codepage_utf8cyr_array, codepage_win1251_array, codepage_koi8r_array,
  codepage_ibm866_array, codepage_iso5_array,
#else
  0, 0, 0, 0, 0,
#endif

#ifdef VSFTP_CHARCONV_SUPPORT_WESTERN
  codepage_utf8west_array, codepage_iso1_array, codepage_iso15_array,
  codepage_win1252_array,
#else
  0, 0, 0, 0, 
#endif

#ifdef VSFTP_CHARCONV_SUPPORT_CENTRAL
  codepage_utf8cent_array, codepage_iso2_array, codepage_iso16_array,
  codepage_win1250_array,
#else
  0, 0, 0, 0,
#endif

#ifdef VSFTP_CHARCONV_SUPPORT_SOUTERN
  codepage_utf8sout_array, codepage_iso3_array,
#else
  0, 0,
#endif

  0
};

map_ptr localMap = 0, remoteMap = 0;
map_ptr localTbl = 0, remoteTbl = 0;

void char_convertion(struct mystr* p_str, int direction, char unprintable);
void InitTables(map_ptr* map, map_ptr* table, int index);
static int char_len(unsigned int s);
static unsigned int bsearch_index(map_ptr map, unsigned int low, unsigned int high, unsigned int char_code);

const char* vsf_charconv_charset_name(int code)
{
  int i = 0;
  while (available_charsets_array [i].p_charset_name && available_charsets_array [i].p_variable != code) i++;
  return available_charsets_array [i].p_charset_name;
}

int vsf_charconv_codepage(const char* p_str)
{
  const struct available_charsets* charsets = available_charsets_array;
  
  while (charsets->p_charset_name != 0)
  {
    if (str_equal_str (charsets->p_charset_name, p_str))
    {
      return charsets->p_variable;
    }
    charsets++;
  }
  
  return 0;
}

int vsf_charconv_avail_convertion(int localCode, int remoteCode)
{
  const struct available_convertions* aconv = available_convertions_array;

  while (aconv->local != 0)
  {
    if (localCode == aconv->local && remoteCode == aconv->remote)
    {
      InitTables(&localMap, &localTbl, aconv->localCharset);
      InitTables(&remoteMap, &remoteTbl, aconv->remoteCharset);
      return 1;
    }
    aconv++; 
  }

  return 0;
}

void vsf_charconv_convert(struct vsf_session* p_sess, struct mystr* p_str, int direction)
{
  if (!p_sess->enable_convertion || !p_sess->remote_charset || !tunable_local_codepage) return;

  char_convertion(p_str, direction, '?');
}

void vsf_charconv_replace_unprintable(struct mystr* p_str, char new_char)
{
  if (localMap) char_convertion(p_str, VSFTP_CONVDIRECT_UNPRINTABLE, new_char);
}

void InitTables(map_ptr* map, map_ptr* table, int index)
{
  *table = map_array [index];

  if (*map) vsf_sysutil_free(*map);

  *map = vsf_sysutil_malloc(((*table) [0].order + 1) * sizeof(_codepage_map));
  (*map) [0].char_code = (*table) [0].char_code;
  (*map) [0].order     = (*table) [0].order;

  index = 1;
  while ((*table) [index].char_code)
  {
    if ((*table) [index].order)
    {
      (*map) [(*table) [index].order].char_code = (*table) [index].char_code;
      (*map) [(*table) [index].order].order     = index;
    }
    index++;
  }

  return;
}

static int char_len(unsigned int s)
{
  int len = 1;
       if ((s & 0x80) == 0x00) len = 1;
  else if ((s & 0xe0) == 0xc0) len = 2;
  else if ((s & 0xf0) == 0xe0) len = 3;
  else if ((s & 0xf8) == 0xf0) len = 4;
//  else if ((s & 0xfc) == 0xf8) len = 5;
//  else if ((s & 0xfe) == 0xce) len = 6;
  return (len);
}

static unsigned int bsearch_index(map_ptr map, unsigned int low, unsigned int high, unsigned int char_code)
{
  unsigned int m, l = low, r = high;

  m = (l + r) >> 1;
  while ((m != 0) && (map [m].char_code != char_code))
  {
    if (map [m].char_code < char_code) l = m + 1;
    else
    if (map [m].char_code > char_code) r = m - 1;
    if (l > r)
      return 0;
    else
      m = (l + r) >> 1;
  }

  if (m) m = map [m].order;
  return m;
}

void char_convertion(struct mystr* p_str, int direction, char unprintable)
{
  const char* srcbuf;
  unsigned int srclen;
  char* dstbuf;
  map_ptr src, dst;
  unsigned int sl;
  unsigned int srcpos = 0, dstpos = 0;
  unsigned int char_code = 0;

  srclen = str_getlen(p_str);     // Len of source string
  srcbuf = str_getbuf(p_str);

  if (direction == VSFTP_CONVDIRECT_FORWARD)
  {
    src = localMap;
    dst = remoteTbl;
  }
  else
  if (direction == VSFTP_CONVDIRECT_BACKWARD)
  {
    src = remoteMap;
    dst = localTbl;
  }
  else
  {
    src = localMap;
    dst = localTbl;
  }

  dstbuf = vsf_sysutil_malloc(srclen * dst [0].char_code + dst [0].char_code);

  while (srcpos < srclen)
  {
    char_code = (unsigned char)srcbuf [srcpos++];
    if (src [0].char_code > 1)
    {
      sl = char_len (char_code);
      while (sl-- > 1)
        char_code = (char_code << 8) | (unsigned char)srcbuf [srcpos++];
    }

    if (char_code > 127)
    {
      sl = bsearch_index (src, 1, src [0].order, char_code);
      char_code = 0;
      if (sl) char_code = dst [sl].char_code;
    }

    if (char_code == 0) char_code = (unsigned int)unprintable;

    if (char_code >= 32 || direction != VSFTP_CONVDIRECT_UNPRINTABLE) {
      if (char_code & 0xff000000) dstbuf [dstpos++] = (char)((char_code >> 24) & 0xff);
      if (char_code & 0x00ff0000) dstbuf [dstpos++] = (char)((char_code >> 16) & 0xff);
      if (char_code & 0x0000ff00) dstbuf [dstpos++] = (char)((char_code >>  8) & 0xff);
      if (char_code & 0x000000ff) dstbuf [dstpos++] = (char)((char_code      ) & 0xff);
    }
  }

  dstbuf [dstpos] = '\0';

  str_empty(p_str);
  str_append_text(p_str, dstbuf);

  vsf_sysutil_free(dstbuf);
}

