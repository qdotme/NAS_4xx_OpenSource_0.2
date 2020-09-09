/*
 * $Id: ad_attr.c,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <string.h>
#include <atalk/adouble.h>

#define FILEIOFF_ATTR 14
#define AFPFILEIOFF_ATTR 2

int ad_getattr(const struct adouble *ad, u_int16_t *attr)
{
   *attr = 0;
        
   if (ad->ad_version == AD_VERSION1) {
       if (ad_getentryoff(ad, ADEID_FILEI)) {
           memcpy(attr, ad_entry(ad, ADEID_FILEI) + FILEIOFF_ATTR,
	          sizeof(u_int16_t));
       }
   }
#if AD_VERSION == AD_VERSION2
   else if (ad->ad_version == AD_VERSION2) {
       if (ad_getentryoff(ad, ADEID_AFPFILEI)) {
           memcpy(attr, ad_entry(ad, ADEID_AFPFILEI) + AFPFILEIOFF_ATTR,
	          sizeof(u_int16_t));
       }
   }	   
#endif
   else 
      return -1;

   return 0;
}

/* ----------------- */
int ad_setattr(const struct adouble *ad, const u_int16_t attr)
{
   if (ad->ad_version == AD_VERSION1) {
       if (ad_getentryoff(ad, ADEID_FILEI)) {
           memcpy(ad_entry(ad, ADEID_FILEI) + FILEIOFF_ATTR, &attr,
	           sizeof(attr));
       }
   }	   
#if AD_VERSION == AD_VERSION2
   else if (ad->ad_version == AD_VERSION2) {
       if (ad_getentryoff(ad, ADEID_AFPFILEI)) {
            memcpy(ad_entry(ad, ADEID_AFPFILEI) + AFPFILEIOFF_ATTR, &attr,
  	            sizeof(attr));
       }
   }	   
#endif
   else 
      return -1;

   return 0;
}

/* -------------- 
 * save file/folder ID in AppleDoubleV2 netatalk private parameters
 * return 1 if resource fork has been modified
*/
#if AD_VERSION == AD_VERSION2
int ad_setid (struct adouble *adp, const dev_t dev, const ino_t ino , const u_int32_t id, const cnid_t did, const void *stamp)
{
    if (adp->ad_flags == AD_VERSION2  && ( adp->ad_options & ADVOL_CACHE) &&
                ad_getentryoff(adp, ADEID_PRIVDEV) &&
                sizeof(dev_t) == ADEDLEN_PRIVDEV && sizeof(ino_t) == ADEDLEN_PRIVINO) 
    {
    
        ad_setentrylen( adp, ADEID_PRIVDEV, sizeof(dev_t));
	if ((adp->ad_options & ADVOL_NODEV)) {
            memset(ad_entry( adp, ADEID_PRIVDEV ), 0, sizeof(dev_t));
        }
	else {
            memcpy(ad_entry( adp, ADEID_PRIVDEV ), &dev, sizeof(dev_t));
        }

        ad_setentrylen( adp, ADEID_PRIVINO, sizeof(ino_t));
        memcpy(ad_entry( adp, ADEID_PRIVINO ), &ino, sizeof(ino_t));

	ad_setentrylen( adp, ADEID_PRIVID, sizeof(id));
        memcpy(ad_entry( adp, ADEID_PRIVID ), &id, sizeof(id));

        ad_setentrylen( adp, ADEID_DID, sizeof(did));
        memcpy(ad_entry( adp, ADEID_DID ), &did, sizeof(did));

        ad_setentrylen( adp, ADEID_PRIVSYN, ADEDLEN_PRIVSYN);
        memcpy(ad_entry( adp, ADEID_PRIVSYN ), stamp, ADEDLEN_PRIVSYN);
        return 1;
    }
    return 0;
}

/* ----------------------------- */
u_int32_t ad_getid (struct adouble *adp, const dev_t st_dev, const ino_t st_ino , const cnid_t did, const void *stamp)
{
u_int32_t aint = 0;
dev_t  dev;
ino_t  ino;
cnid_t a_did;
char   temp[ADEDLEN_PRIVSYN];

    /* look in AD v2 header 
     * note inode and device are opaques and not in network order
     * only use the ID if adouble is writable for us.
    */
    if (adp && ( adp->ad_options & ADVOL_CACHE) && ( adp->ad_hf.adf_flags & O_RDWR )
            && sizeof(dev_t) == ad_getentrylen(adp, ADEID_PRIVDEV)
            && sizeof(ino_t) == ad_getentrylen(adp,ADEID_PRIVINO)
            && sizeof(temp) == ad_getentrylen(adp,ADEID_PRIVSYN)
            && sizeof(cnid_t) == ad_getentrylen(adp, ADEID_DID)
            && sizeof(cnid_t) == ad_getentrylen(adp, ADEID_PRIVID)
    ) {
        memcpy(&dev, ad_entry(adp, ADEID_PRIVDEV), sizeof(dev_t));
        memcpy(&ino, ad_entry(adp, ADEID_PRIVINO), sizeof(ino_t));
        memcpy(temp, ad_entry(adp, ADEID_PRIVSYN), sizeof(temp));
        memcpy(&a_did, ad_entry(adp, ADEID_DID), sizeof(cnid_t));

        if (  ((adp->ad_options & ADVOL_NODEV) || dev == st_dev)
              && ino == st_ino && a_did == did 
              && !memcmp(stamp, temp, sizeof(temp))) { 
            memcpy(&aint, ad_entry(adp, ADEID_PRIVID), sizeof(aint));
            return aint;
        }
    }
    return 0; 
}

#endif

/* ----------------- 
 * set resource fork filename attribute.
*/
int ad_setname(struct adouble *ad, const char *path)
{
    if (ad_getentryoff(ad, ADEID_NAME)) {
        ad_setentrylen( ad, ADEID_NAME, strlen( path ));
        memcpy(ad_entry( ad, ADEID_NAME ), path, ad_getentrylen( ad, ADEID_NAME ));
        return 1;
    }
    return 0;
}
