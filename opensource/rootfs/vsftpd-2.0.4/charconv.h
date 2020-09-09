#ifndef VSF_CHARCONV_H
#define VSF_CHARCONV_H

struct mystr;
struct vsf_session;

#define VSFTP_CHARCONV_SUPPORT_CYRILLIC
#define VSFTP_CHARCONV_SUPPORT_WESTERN
#define VSFTP_CHARCONV_SUPPORT_CENTRAL
#define VSFTP_CHARCONV_SUPPORT_SOUTERN

#define VSFTP_CONVDIRECT_FORWARD      1
#define VSFTP_CONVDIRECT_UNPRINTABLE  0
#define VSFTP_CONVDIRECT_BACKWARD    -1

/* Supported charset for convertion */
#define VSFTP_CP_NONE       "NONE"
#define VSFTP_CP_UTF_8      "UTF-8"
#define VSFTP_CP_UTF8       "UTF8"
#ifdef VSFTP_CHARCONV_SUPPORT_CYRILLIC
#define VSFTP_CP_WIN_1251   "Win-1251"
#define VSFTP_CP_WIN1251    "WIN1251"
#define VSFTP_CP_1251       "1251"
#define VSFTP_CP_KOI8_R     "Koi8-R"
#define VSFTP_CP_KOI8R      "KOI8R"
#define VSFTP_CP_878        "878"
#define VSFTP_CP_IBM866     "IBM866"
#define VSFTP_CP_866        "866"
#define VSFTP_CP_ISO8859_5  "ISO-8859-5"
#define VSFTP_CP_ISO5       "ISO5"
#endif
#ifdef VSFTP_CHARCONV_SUPPORT_WESTERN
#define VSFTP_CP_ISO8859_1  "ISO-8859-1"
#define VSFTP_CP_ISO1       "ISO1"
#define VSFTP_CP_LATIN1     "LATIN1"
#define VSFTP_CP_ISO8859_15 "ISO-8859-15"
#define VSFTP_CP_ISO15      "ISO15"
#define VSFTP_CP_LATIN9     "LATIN9"
#define VSFTP_CP_WIN_1252   "Win-1252"
#define VSFTP_CP_WIN1252    "WIN1252"
#define VSFTP_CP_1252       "1252"
#endif
#ifdef VSFTP_CHARCONV_SUPPORT_CENTRAL
#define VSFTP_CP_ISO8859_2  "ISO-8859-2"
#define VSFTP_CP_ISO2       "ISO2"
#define VSFTP_CP_LATIN2     "LATIN2"
#define VSFTP_CP_ISO8859_16 "ISO-8859-16"
#define VSFTP_CP_ISO16      "ISO16"
#define VSFTP_CP_WIN_1250   "Win-1250"
#define VSFTP_CP_WIN1250    "WIN1250"
#define VSFTP_CP_1250       "1250"
#endif
#ifdef VSFTP_CHARCONV_SUPPORT_SOUTERN
#define VSFTP_CP_ISO8859_3  "ISO-8859-3"
#define VSFTP_CP_ISO3       "ISO3"
#define VSFTP_CP_LATIN3     "LATIN3"
#endif

#define VSFTP_C_NONE       0
#define VSFTP_C_UTF8       1
#ifdef VSFTP_CHARCONV_SUPPORT_CYRILLIC
#define VSFTP_C_1251       2
#define VSFTP_C_878        3
#define VSFTP_C_866        4
#define VSFTP_C_ISO5       5
#endif
#ifdef VSFTP_CHARCONV_SUPPORT_WESTERN
#define VSFTP_C_ISO1       6
#define VSFTP_C_ISO15      7
#define VSFTP_C_1252       8
#endif
#ifdef VSFTP_CHARCONV_SUPPORT_CENTRAL
#define VSFTP_C_ISO2       9
#define VSFTP_C_ISO16     10
#define VSFTP_C_1250      11
#endif
#ifdef VSFTP_CHARCONV_SUPPORT_SOUTERN
#define VSFTP_C_ISO3      12
#endif

#define VSFTP_CS_NONE      0
#ifdef VSFTP_CHARCONV_SUPPORT_CYRILLIC
#define VSFTP_CS_UTF8CYR   1
#define VSFTP_CS_1251      2
#define VSFTP_CS_878       3
#define VSFTP_CS_866       4
#define VSFTP_CS_ISO5      5
#endif
#ifdef VSFTP_CHARCONV_SUPPORT_WESTERN
#define VSFTP_CS_UTF8WEST  6
#define VSFTP_CS_ISO1      7
#define VSFTP_CS_ISO15     8
#define VSFTP_CS_1252      9
#endif
#ifdef VSFTP_CHARCONV_SUPPORT_CENTRAL
#define VSFTP_CS_UTF8CENT 10
#define VSFTP_CS_ISO2     11
#define VSFTP_CS_ISO16    12
#define VSFTP_CS_1250     13
#endif
#ifdef VSFTP_CHARCONV_SUPPORT_SOUTERN
#define VSFTP_CS_UTF8SOUT 14
#define VSFTP_CS_ISO3     15
#endif

struct codepage_map
{
  unsigned int char_code; // The first element is count bytes of char.
  unsigned int order;
} _codepage_map;

typedef struct codepage_map * map_ptr;

extern map_ptr localMap;

/* vsf_charconv_charset_name()
 * PURPOSE
 * Get charset name by code;
 * PARAMETERS
 * code         - Internal charset code
 * RETURNS
 * Charset name
 */
const char* vsf_charconv_charset_name(int code);

/* vsf_charconv_codepage()
 * PURPOSE
 * Get internal charset code
 * PARAMETERS
 * p_str        - String value with code page
 * RETURNS
 * Internal charset code
 */
int vsf_charconv_codepage(const char* p_str);

/* vsf_charconv_avail_convertion()
 * PURPOS
 * Checking for available convertion characters
 * PARAMETERS
 * localCode    - source internal code page
 * remoteCode   - destination internal code page
 * RETURNS
 * Available ot not converion
 */
int vsf_charconv_avail_convertion(int localCode, int remoteCode);

/* vsf_charconv_convert()
 * PURPOSE
 * Converting string via charsets
 * PARAMETERS
 * p_sess       - the current session object
 * p_str        - string for convertin
 * direction    - converting from host to remoute charsetr or otherwise
 */
void vsf_charconv_convert(struct vsf_session* p_sess, struct mystr* p_str, int direction);

/* vsf_charconv_replace_unprintable
 */
void vsf_charconv_replace_unprintable(struct mystr* p_str, char new_char);

#endif
