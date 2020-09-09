#ifndef VSF_POSTLOGIN_H
#define VSF_POSTLOGIN_H

struct vsf_session;

struct UPNPDev {
	struct UPNPDev* pNext;
	char* descURL;
	char* st;
	char buffer[2];
};

/* structure used to get fast access to urls
 * controlURL: controlURL of the WANIPConnection
 * ipcondescURL: url of the description of the WANIPConnection
 * controlURL_CIF: controlURL of the WANCOmmonInterfaceConfig
 */
struct UPNPUrls {
	char* controlURL;
	char* ipcondescURL;
	char* controlURL_CIF;
};

/* process_post_login()
 * PURPOSE
 * Called to begin FTP protocol parsing for a logged in session.
 * PARAMETERS
 * p_sess       - the current session object
 */
void process_post_login(struct vsf_session* p_sess);

#endif /* VSF_POSTLOGIN_H */

