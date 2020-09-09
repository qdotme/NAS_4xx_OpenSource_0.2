#ifndef VSF_PASVRULES_H
#define VSF_PASVRULES_H

struct mystr;
struct vsf_session;


typedef struct pasv_rule
{
  unsigned int   local_ip        ;
  unsigned int   remote_mask1    ;
  unsigned int   remote_mask2    ;
  unsigned int   pasv_address    ;
  int            allow_anonymous ;  // default tunable_anonymous_enable
  int            anon_upl_enable ;  // default tunable_anon_upload_enable
  int            anon_md_enable  ;  // default tunable_anon_mkdir_write_enable
  int            anon_oth_enable ;  // default tunable_anon_other_write_enable
  void *         next            ;
} pasv_rule;

typedef struct pasv_rule * pasv_rules;

/* vsf_pasvrules_load()
 * PURPOSE
 * Load file with rules
 * PARAMETERS
 * p_filename   - file name with rules
 */
void vsf_pasvrules_load(const char* p_filename);

/* vsf_pasvrules_define()
 * PURPOSE
 * Find rule for IP in session
 * PARAMETERS
 * p_sess       - session
 */
void vsf_pasvrules_define(struct vsf_session* p_sess);

#endif /* VSF_PASVRULES_H */
