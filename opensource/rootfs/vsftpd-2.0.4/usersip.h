#ifndef VSF_USERSIP_H
#define VSF_USERSIP_H

#ifndef VSFTP_STR_H
#include "str.h"
#endif
struct vsf_session;

typedef struct ip_rule * ip_rules;

typedef struct ip_rule
{
  struct mystr   p_user     ;
  unsigned int   remote_ip  ;
  unsigned int   remote_mask;
  ip_rules       prev       ;
  ip_rules       next       ;
} ip_rule;

void vsf_userip_load(const char* p_filename);

int vsf_userip_check(const struct mystr* p_user,
                     const struct vsf_session* p_sess);

void vsf_userip_free();

#endif /* VSF_USERSIP_H */
