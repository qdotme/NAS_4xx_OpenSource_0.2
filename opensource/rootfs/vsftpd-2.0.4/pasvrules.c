/*
 * Part of Very Secure FTPd
 * Licence: GPL v2
 * Author: Dmitriy Balashov
 * pasvrules.c
 */

#include "pasvrules.h"
#include "tunables.h"
#include "str.h"
#include "session.h"
#include "filestr.h"
#include "defs.h"
#include "sysutil.h"
#include "logging.h"
#include "ftpcodes.h"
#include "ftpcmdio.h"

static int parse_rules(struct mystr* p_rule_str, pasv_rules rule);
pasv_rules find_rule(struct vsf_session* p_sess);
void dispose_rules();

static pasv_rules rules_list = 0;

void vsf_pasvrules_load(const char* p_filename)
{
  struct mystr rules_file_str = INIT_MYSTR;
  struct mystr rule_setting_str = INIT_MYSTR;
  unsigned int str_pos = 0;
  int retval;
  pasv_rules rule = 0, owner = 0;

  retval = str_fileread(&rules_file_str, p_filename, VSFTP_CONF_FILE_MAX);
  if (vsf_sysutil_retval_is_error(retval))
  {
    return;
  }

  while (str_getline(&rules_file_str, &rule_setting_str, &str_pos))
  {
    if (str_isempty(&rule_setting_str) ||
        str_get_char_at(&rule_setting_str, 0) == '#')
    {
      continue;
    }
    if (!rules_list) {
      rule = vsf_sysutil_malloc(sizeof(pasv_rule));
      rules_list = rule;
    } else {
      owner = rule;
      rule = vsf_sysutil_malloc(sizeof(pasv_rule));
      owner->next = rule;
    }
    if (parse_rules(&rule_setting_str, rule) == 0) {
      vsf_sysutil_free(rule);
      if (owner) {
        owner->next = 0;
      } else {
        rules_list = 0;
      }
    }
  }
  str_free(&rules_file_str);
  if (!str_isempty(&rule_setting_str)) {
    str_free(&rule_setting_str);
  }
}

void
vsf_pasvrules_define(struct vsf_session* p_sess)
{
  static pasv_rules rules;
  rules = find_rule(p_sess);

  if (rules) {
    p_sess->allow_anonymous = rules->allow_anonymous;
    p_sess->anon_upl_enable = rules->anon_upl_enable;
    p_sess->anon_md_enable  = rules->anon_md_enable;
    p_sess->anon_oth_enable = rules->anon_oth_enable;
    p_sess->pasv_address    = rules->pasv_address;

    dispose_rules();
  } else {
    p_sess->allow_anonymous = tunable_anonymous_enable;
    p_sess->anon_upl_enable = tunable_anon_upload_enable;
    p_sess->anon_md_enable  = tunable_anon_mkdir_write_enable;
    p_sess->anon_oth_enable = tunable_anon_other_write_enable;
    p_sess->pasv_address    = 0;
  }
}

static int
parse_rules(struct mystr* p_rule_str, pasv_rules rule)
{
  static struct mystr s_lhs_chunk_str;
  static struct mystr s_rhs_chunk_str;
  static struct vsf_sysutil_sockaddr *sockaddr;
  int mask;

  str_replace_char(p_rule_str, '\t', ' ');
  str_trim_char(p_rule_str, ' ');

  str_copy(&s_lhs_chunk_str, p_rule_str);
  str_free(p_rule_str);

  vsf_sysutil_sockaddr_alloc_ipv4(&sockaddr);

  str_split_char(&s_lhs_chunk_str, &s_rhs_chunk_str, ' '); // Get local_ip
  str_append_char(&s_lhs_chunk_str, '\0');
  if (vsf_sysutil_inet_aton(str_getbuf(&s_lhs_chunk_str), sockaddr) == 0) {
    return 0;
  }
  rule->local_ip = *(int*)vsf_sysutil_sockaddr_get_raw_addr(sockaddr);

  str_copy(&s_lhs_chunk_str, &s_rhs_chunk_str); // Get remote_mask1
  str_split_char(&s_lhs_chunk_str, &s_rhs_chunk_str, '/');
  str_append_char(&s_lhs_chunk_str, '\0');
  if (vsf_sysutil_inet_aton(str_getbuf(&s_lhs_chunk_str), sockaddr) == 0) {
    return 0;
  }
  rule->remote_mask1 = *(unsigned int*)vsf_sysutil_sockaddr_get_raw_addr(sockaddr);

  str_copy(&s_lhs_chunk_str, &s_rhs_chunk_str); // Get remote_mask2
  str_split_char(&s_lhs_chunk_str, &s_rhs_chunk_str, ' ');
  mask = str_atoi(&s_lhs_chunk_str);
  if (mask <= 0) {
    rule->remote_mask2 = 0;
  } else
  if (mask >= 32) {
    rule->remote_mask2 = 0xFFFFFFFF;
  } else {
    rule->remote_mask2 = 1;
    rule->remote_mask2 <<= mask;
    rule->remote_mask2--;
  }
  rule->remote_mask1 &= rule->remote_mask2;

  str_copy(&s_lhs_chunk_str, &s_rhs_chunk_str); // Get pasv_address
  str_split_char(&s_lhs_chunk_str, &s_rhs_chunk_str, ' ');
  str_append_char(&s_lhs_chunk_str, '\0');
  if (vsf_sysutil_inet_aton(str_getbuf(&s_lhs_chunk_str), sockaddr) == 0) {
    return 0;
  }
  rule->pasv_address = *(int*)vsf_sysutil_sockaddr_get_raw_addr(sockaddr);

  rule->allow_anonymous = tunable_anonymous_enable;
  rule->anon_upl_enable = tunable_anon_upload_enable;
  rule->anon_md_enable  = tunable_anon_mkdir_write_enable;
  rule->anon_oth_enable = tunable_anon_other_write_enable;

  if (!str_isempty(&s_rhs_chunk_str)) {  // Get allow_anonymous
    str_copy(&s_lhs_chunk_str, &s_rhs_chunk_str);
    str_split_char(&s_lhs_chunk_str, &s_rhs_chunk_str, ' ');
    str_upper(&s_lhs_chunk_str);
    if (str_equal_text(&s_lhs_chunk_str, "YES") ||
        str_equal_text(&s_lhs_chunk_str, "TRUE") ||
        str_equal_text(&s_lhs_chunk_str, "1"))
    {
      rule->allow_anonymous = 1;
    } else
    if (str_equal_text(&s_lhs_chunk_str, "NO") ||
        str_equal_text(&s_lhs_chunk_str, "FALSE") ||
        str_equal_text(&s_lhs_chunk_str, "0"))
    {
      rule->allow_anonymous = 0;
    }
  }

  if (!str_isempty(&s_rhs_chunk_str)) {  // Get anon_upl_enable
    str_copy(&s_lhs_chunk_str, &s_rhs_chunk_str);
    str_split_char(&s_lhs_chunk_str, &s_rhs_chunk_str, ' ');
    str_upper(&s_lhs_chunk_str);
    if (str_equal_text(&s_lhs_chunk_str, "YES") ||
        str_equal_text(&s_lhs_chunk_str, "TRUE") ||
        str_equal_text(&s_lhs_chunk_str, "1"))
    {
      rule->anon_upl_enable = 1;
    } else
    if (str_equal_text(&s_lhs_chunk_str, "NO") ||
        str_equal_text(&s_lhs_chunk_str, "FALSE") ||
        str_equal_text(&s_lhs_chunk_str, "0"))
    {
      rule->anon_upl_enable = 0;
    }
  }

  if (!str_isempty(&s_rhs_chunk_str)) {  // Get anon_md_enable
    str_copy(&s_lhs_chunk_str, &s_rhs_chunk_str);
    str_split_char(&s_lhs_chunk_str, &s_rhs_chunk_str, ' ');
    str_upper(&s_lhs_chunk_str);
    if (str_equal_text(&s_lhs_chunk_str, "YES") ||
        str_equal_text(&s_lhs_chunk_str, "TRUE") ||
        str_equal_text(&s_lhs_chunk_str, "1"))
    {
      rule->anon_md_enable = 1;
    } else
    if (str_equal_text(&s_lhs_chunk_str, "NO") ||
        str_equal_text(&s_lhs_chunk_str, "FALSE") ||
        str_equal_text(&s_lhs_chunk_str, "0"))
    {
      rule->anon_md_enable = 0;
    }
  }

  if (!str_isempty(&s_rhs_chunk_str)) {  // Get anon_oth_enable
    str_upper(&s_rhs_chunk_str);
    if (str_equal_text(&s_rhs_chunk_str, "YES") ||
        str_equal_text(&s_rhs_chunk_str, "TRUE") ||
        str_equal_text(&s_rhs_chunk_str, "1"))
    {
      rule->anon_oth_enable = 1;
    } else
    if (str_equal_text(&s_rhs_chunk_str, "NO") ||
        str_equal_text(&s_rhs_chunk_str, "FALSE") ||
        str_equal_text(&s_rhs_chunk_str, "0"))
    {
      rule->anon_oth_enable = 0;
    }
  }

  vsf_sysutil_sockaddr_clear(&sockaddr);
  rule->next = 0;
  return 1;
}

void
dispose_rules()
{
  static pasv_rules rules;

  while (rules_list) {
    rules = rules_list->next;
    vsf_sysutil_free(rules_list);
    rules_list = rules;
  }
}

pasv_rules
find_rule(struct vsf_session* p_sess)
{
  static pasv_rules rules;
  rules = rules_list;

  while (rules) {
    if (rules->local_ip == *(unsigned int*)vsf_sysutil_sockaddr_get_raw_addr(p_sess->p_local_addr) &&
        rules->remote_mask1 == (*(unsigned int*)vsf_sysutil_sockaddr_get_raw_addr(p_sess->p_remote_addr) & rules->remote_mask2)) {
      break;
    }
    rules = rules->next;
  }

  return rules;
}
