/*****************************************************************************

 @(#) npi_ip.h,v 0.9.2.7 2006/05/06 10:22:34 brian Exp

 -----------------------------------------------------------------------------

 Copyright (c) 2001-2006  OpenSS7 Corporation <http://www.openss7.com/>

 All Rights Reserved.

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation; version 2 of the License.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 details.

 You should have received a copy of the GNU General Public License along with
 this program; if not, write to the Free Software Foundation, Inc., 675 Mass
 Ave, Cambridge, MA 02139, USA.

 -----------------------------------------------------------------------------

 U.S. GOVERNMENT RESTRICTED RIGHTS.  If you are licensing this Software on
 behalf of the U.S. Government ("Government"), the following provisions apply
 to you.  If the Software is supplied by the Department of Defense ("DoD"), it
 is classified as "Commercial Computer Software" under paragraph 252.227-7014
 of the DoD Supplement to the Federal Acquisition Regulations ("DFARS") (or any
 successor regulations) and the Government is acquiring only the license rights
 granted herein (the license rights customarily provided to non-Government
 users).  If the Software is supplied to any unit or agency of the Government
 other than DoD, it is classified as "Restricted Computer Software" and the
 Government's rights in the Software are defined in paragraph 52.227-19 of the
 Federal Acquisition Regulations ("FAR") (or any successor regulations) or, in
 the cases of NASA, in paragraph 18.52.227-86 of the NASA Supplement to the FAR
 (or any successor regulations).

 -----------------------------------------------------------------------------

 Commercial licensing and support of this software is available from OpenSS7
 Corporation at a fee.  See http://www.openss7.com/

 -----------------------------------------------------------------------------

 Last Modified 2006/05/06 10:22:34 by brian

 -----------------------------------------------------------------------------

 npi_ip.h,v
 Revision 0.9.2.7  2006/05/06 10:22:34  brian
 - added test suite for NPI-IP driver

 Revision 0.9.2.6  2006/05/05 02:07:54  brian
 - working up NPI-IP driver

 Revision 0.9.2.5  2006/05/03 22:53:39  brian
 - working up NPI-IP driver

 Revision 0.9.2.4  2006/04/26 10:47:55  brian
 - sync

 Revision 0.9.2.3  2006/03/30 10:47:34  brian
 - working up NPI IP test and mux driver

 Revision 0.9.2.2  2006/03/27 01:25:56  brian
 - working up IP driver and SCTP testing

 Revision 0.9.2.1  2006/03/18 09:39:04  brian
 - added ip driver headers

 *****************************************************************************/

#ifndef SYS_NPI_IP_H
#define SYS_NPI_IP_H

#ident "@(#) npi_ip.h,v OpenSS7-0_9_2_D_rc2(0.9.2.7) Copyright (c) 2001-2006 OpenSS7 Corporation."

#include <sys/npi.h>

#define IP_FLAG_DEFAULT_RC_SEL	(1<<0)
#define IP_BINDPORT_LOCK	(1<<1)

#define N_QOS_SEL_INFO_IP	0x0201
#define N_QOS_RANGE_INFO_IP	0x0202
#define N_QOS_SEL_CONN_IP	0x0203
#define N_QOS_SEL_UD_IP		0x0204

typedef struct N_qos_sel_info_ip {
	np_ulong n_qos_type;		/* always N_QOS_SEL_INFO_IP */
	np_ulong protocol;		/* default protocol for data transmission */
	np_ulong priority;		/* default priority for data transmission */
	np_ulong ttl;			/* default time-to-live for outgoing packets */
	np_ulong tos;			/* default type-of-service for outgoing packets */
	np_ulong mtu;			/* default mtu for outgoing packets */
	np_ulong saddr;			/* default srce IP address for outgoing packets */
	np_ulong daddr;			/* default dest IP address for outgoing packets */
} N_qos_sel_info_ip_t;

typedef struct N_qos_sel_conn_ip {
	np_ulong n_qos_type;		/* always N_QOS_SEL_CONN_IP */
	np_ulong protocol;
	np_ulong priority;
	np_ulong ttl;			/* default time-to-live for connection */
	np_ulong tos;			/* default type-of-service for connection */
	np_ulong mtu;			/* default maximum-transfer-unit for connection */
	np_ulong saddr;			/* default srce IP address for outgoing packets */
	np_ulong daddr;			/* default dest IP address for outgoing packets */
} N_qos_sel_conn_ip_t;

typedef struct N_qos_sel_ud_ip {
	np_ulong n_qos_type;		/* always N_QOS_SEL_UD_IP */
	np_ulong protocol;		/* protocol for outgoing packet */
	np_ulong priority;		/* priority for outgoing packet */
	np_ulong ttl;			/* time to live for outgoing packet */
	np_ulong tos;			/* type of service for outgoing packet */
	np_ulong saddr;			/* source IP address for outgoing packet */
} N_qos_sel_ud_ip_t;

typedef struct {
	np_long priority_min_value;	/* */
	np_long priority_max_value;	/* */
} ip_priority_values_t;

typedef struct {
	np_long ttl_min_value;	/* */
	np_long ttl_max_value;	/* */
} ip_ttl_values_t;

typedef struct {
	np_long tos_min_value;	/* */
	np_long tos_max_value;	/* */
} ip_tos_values_t;

typedef struct {
	np_long mtu_min_value;	/* */
	np_long mtu_max_value;	/* */
} ip_mtu_values_t;

typedef struct N_qos_range_info_ip {
	np_ulong n_qos_type;		/* always N_QOS_RANGE_INFO_IP */
	ip_priority_values_t priority;	/* priority range */
	ip_ttl_values_t ttl;		/* ttl range */
	ip_tos_values_t tos;		/* tos range */
	ip_mtu_values_t mtu;		/* mtu range */
} N_qos_range_info_ip_t;

union N_qos_ip_types {
	np_ulong n_qos_type;
	struct N_qos_sel_info_ip n_qos_sel_info;
	struct N_qos_sel_conn_ip n_qos_sel_conn;
	struct N_qos_sel_ud_ip n_qos_sel_ud;
	struct N_qos_range_info_ip n_qos_range_info;
};

/* TOS values, logical OR of one from each group below */

/* TOS precedence */
#define N_ROUTINE		0x00
#define N_PRIORITY		0x20
#define N_IMMEDIATE		0x40
#define N_FLASH			0x60
#define N_OVERRIDEFLASH		0x80
#define N_CRITIC_ECP		0xa0
#define N_INETCONTROL		0xc0
#define N_NETCONTROL		0xe0

/* TOS type of service */
#define N_NOTOS			0
#define N_LDELAY		(1<<4)
#define N_HITHRPT		(1<<3)
#define N_HIREL			(1<<2)
#define N_LOCOST		(1<<1)

#endif				/* SYS_NPI_IP_H */
