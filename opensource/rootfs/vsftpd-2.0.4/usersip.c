/*
 * Part of Very Secure FTPd
 * Licence: GPL v2
 * Author: Dmitriy Balashov
 * usersip.c
 */

#include "usersip.h"
#include "tunables.h"
#include "str.h"
#include "filestr.h"
#include "sysutil.h"
#include "session.h"
#include "defs.h"
#include "logging.h"
#include "utility.h"

static int parse_rules(struct mystr* p_rule_str);

static ip_rules rules_list = 0;

void
vsf_userip_load(const char* p_filename)
{
  struct mystr rules_file_str = INIT_MYSTR;
  struct mystr rule_setting_str = INIT_MYSTR;
  int retval;
  unsigned int str_pos = 0;

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
    parse_rules(&rule_setting_str);
  }

  str_free(&rules_file_str);
  if (!str_isempty(&rule_setting_str)) {
    str_free(&rule_setting_str);
  }

  if (rules_list) {
    while (rules_list->prev)
    {
      rules_list = rules_list->prev;
    }
  }
}

int
vsf_userip_check(const struct mystr* p_user,
                 const struct vsf_session* p_sess)
{
  unsigned int remote_ip = 0;
  int accept_login = 1;
  static ip_rules rules;

  if (!rules_list) {
    return(1);
  }

  rules = rules_list;

  remote_ip  = *(unsigned int*)vsf_sysutil_sockaddr_get_raw_addr(p_sess->p_remote_addr);

  do
  {
    if (str_equal(p_user, &rules->p_user)) {
      accept_login = 0;
      if ((remote_ip & rules->remote_mask) == rules->remote_ip) {
        return (1);
      }
    }
    rules = rules->next;
  }
  while (rules);

  return (accept_login);
}

void
vsf_userip_free()
{
  if (rules_list)
  {
    do
    {
      if (rules_list->next) {
        rules_list = rules_list->next;
        vsf_sysutil_free (rules_list->prev);
      } else {
        vsf_sysutil_free (rules_list);
        rules_list = 0;
      }
    }
    while (rules_list);
  }
}

static int
parse_rules(struct mystr* p_rule_str)
{
  static struct mystr s_lhs_chunk_str;
  static struct mystr s_rhs_chunk_str;
  static struct mystr login;
  static struct str_locate_result locate;
  static struct mystr s_ip;
  static struct vsf_sysutil_sockaddr *sockaddr;
  unsigned int ui_ip = 0;
  unsigned int ui_mask = 0;
  unsigned int mask = 0;

  vsf_sysutil_sockaddr_alloc_ipv4(&sockaddr);

  str_replace_char(p_rule_str, '\t', ' ');
  str_trim_char(p_rule_str, ' ');

  str_copy(&s_lhs_chunk_str, p_rule_str);
  str_split_char(&s_lhs_chunk_str, &s_rhs_chunk_str, ' '); // Get user login
  str_copy(&login, &s_lhs_chunk_str);

  do
  {
    str_copy(&s_lhs_chunk_str, &s_rhs_chunk_str);
    str_split_char(&s_lhs_chunk_str, &s_rhs_chunk_str, ' ');

    locate = str_locate_char(&s_lhs_chunk_str, '/');
    if (locate.found == 1) {
      str_copy(&s_ip, &s_lhs_chunk_str);
      str_split_char(&s_ip, &s_lhs_chunk_str, '/');
      str_append_char(&s_ip, '\0');
      if (vsf_sysutil_inet_aton(str_getbuf(&s_ip), sockaddr) != 0)
      {
        ui_ip = *(unsigned int*)vsf_sysutil_sockaddr_get_raw_addr(sockaddr);
        mask = str_atoi(&s_lhs_chunk_str);
        if (mask <= 0) {
          ui_mask = 0;
        } else
        if (mask >= 32) {
          ui_mask = 0xFFFFFFFF;
        } else {
          ui_mask = 1;
          ui_mask <<= mask;
          ui_mask--;
        }
        ui_ip &= ui_mask;
      } else {
        continue;
      }
    } else {
      str_append_char(&s_lhs_chunk_str, '\0');
      if (vsf_sysutil_inet_aton(str_getbuf(&s_lhs_chunk_str), sockaddr) != 0) {
        ui_ip = *(unsigned int*)vsf_sysutil_sockaddr_get_raw_addr(sockaddr);
      }
      ui_mask = 0xFFFFFFFF;
    }

    if (!rules_list)
    {
      rules_list = vsf_sysutil_malloc(sizeof(ip_rule));
      vsf_sysutil_fillbuff(rules_list, sizeof(ip_rule), 0);
    } else {
      rules_list->next = vsf_sysutil_malloc(sizeof(ip_rule));
      vsf_sysutil_fillbuff(rules_list->next, sizeof(ip_rule), 0);
      rules_list->next->prev = rules_list;
      rules_list = rules_list->next;
    }
    rules_list->remote_ip = ui_ip;
    rules_list->remote_mask = ui_mask;
    str_copy (&rules_list->p_user, &login);

  }
  while (!str_isempty(&s_rhs_chunk_str));

  str_free(&s_lhs_chunk_str);
  str_free(&s_rhs_chunk_str);
  str_free(&login);
  str_free(&s_ip);
}

