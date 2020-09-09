#ifndef _SCRIPT_H
#define _SCRIPT_H

extern void run_script(struct dhcpMessage *packet, const char *name);
int check_update_resolv_conf(char **envp, int count);

#endif
