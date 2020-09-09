/*
 * container_list_sl.h
 * $Id: container_list_ssll.h,v 1.1.1.1 2006/04/04 02:19:00 wiley Exp $
 *
 */
#ifndef NETSNMP_CONTAINER_SSLL_H
#define NETSNMP_CONTAINER_SSLL_H


#include <net-snmp/library/container.h>

#ifdef  __cplusplus
extern "C" {
#endif

    netsnmp_container *netsnmp_container_get_sorted_singly_linked_list(void);

    void netsnmp_container_ssll_init(void);


#ifdef  __cplusplus
};
#endif

#endif /** NETSNMP_CONTAINER_SSLL_H */
