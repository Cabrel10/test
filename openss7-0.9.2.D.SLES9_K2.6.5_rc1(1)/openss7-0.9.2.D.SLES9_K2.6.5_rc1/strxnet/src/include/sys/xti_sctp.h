/*****************************************************************************

 @(#) xti_sctp.h,v 0.9.2.3 2005/07/18 12:45:04 brian Exp

 -----------------------------------------------------------------------------

 Copyright (C) 2001-2004  OpenSS7 Corporation <http://www.openss7.com>

 All Rights Reserved.

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation; either version 2 of the License, or (at your option) any later
 version.

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

 Last Modified 2005/07/18 12:45:04 by brian

 xti_sctp.h,v
 Revision 0.9.2.3  2005/07/18 12:45:04  brian
 - standard indentation

 Revision 0.9.2.2  2005/05/14 08:28:29  brian
 - copyright header correction

 Revision 0.9.2.1  2004/05/16 04:12:33  brian
 - Updating strxnet release.

 Revision 0.9  2004/05/14 08:00:03  brian
 - Updated xns, tli, inet, xnet and documentation.

 Revision 0.9.2.1  2004/04/13 12:12:52  brian
 - Rearranged header files.

 *****************************************************************************/

#ifndef _SYS_XTI_SCTP_H
#define _SYS_XTI_SCTP_H

#ident "@(#) xti_sctp.h,v OpenSS7-0_9_2_D_rc2(0.9.2.3) Copyright (c) 1997-2004 OpenSS7 Corporation."

#define T_INET_SCTP	132	/* SCTP level (same as protocol number) */

/*
 *  SCTP Transport Provider Options
 */
#define T_SCTP_NODELAY			 1
#define T_SCTP_CORK			 2
#define T_SCTP_PPI			 3
#define T_SCTP_SID			 4
#define T_SCTP_SSN			 5
#define T_SCTP_TSN			 6
#define T_SCTP_RECVOPT			 7
#define T_SCTP_COOKIE_LIFE		 8
#define T_SCTP_SACK_DELAY		 9
#define T_SCTP_PATH_MAX_RETRANS		10
#define T_SCTP_ASSOC_MAX_RETRANS	11
#define T_SCTP_MAX_INIT_RETRIES		12
#define T_SCTP_HEARTBEAT_ITVL		13
#define T_SCTP_RTO_INITIAL		14
#define T_SCTP_RTO_MIN			15
#define T_SCTP_RTO_MAX			16
#define T_SCTP_OSTREAMS			17
#define T_SCTP_ISTREAMS			18
#define T_SCTP_COOKIE_INC		19
#define T_SCTP_THROTTLE_ITVL		20
#define T_SCTP_MAC_TYPE			21

#define T_SCTP_HMAC_NONE	0
#define T_SCTP_HMAC_SHA1	1
#define T_SCTP_HMAC_MD5		2

#define T_SCTP_CKSUM_TYPE		22

#define T_SCTP_CSUM_ADLER32	0
#define T_SCTP_CSUM_CRC32C	1

#define T_SCTP_ECN			23
#define T_SCTP_ALI			24
#define T_SCTP_ADD			25
#define T_SCTP_SET			26
#define T_SCTP_ADD_IP			27
#define T_SCTP_DEL_IP			28
#define T_SCTP_SET_IP			29
#define T_SCTP_PR			30
#define T_SCTP_LIFETIME			31

#define T_SCTP_DISPOSITION		32

#define T_SCTP_DISPOSITION_NONE		0
#define T_SCTP_DISPOSITION_UNSENT	1
#define T_SCTP_DISPOSITION_SENT		2
#define T_SCTP_DISPOSITION_GAP_ACKED	3
#define T_SCTP_DISPOSITION_ACKED	4

#define T_SCTP_MAX_BURST		33

#define T_SCTP_HB			34
typedef struct t_sctp_hb {
	t_uscalar_t hb_dest;		/* destination address */
	t_uscalar_t hb_onoff;		/* activation flag */
	t_uscalar_t hb_itvl;		/* interval in milliseconds */
} t_sctp_hb_t;

#define T_SCTP_RTO			35
typedef struct t_sctp_rto {
	t_uscalar_t rto_dest;		/* destination address */
	t_uscalar_t rto_initial;	/* RTO.Initial (milliseconds) */
	t_uscalar_t rto_min;		/* RTO.Min (milliseconds) */
	t_uscalar_t rto_max;		/* RTO.Max (milliseconds) */
	t_uscalar_t max_retrans;	/* Path.Max.Retrans (retries) */
} t_sctp_rto_t;

/*
 *  Read-only options...
 */
#define T_SCTP_MAXSEG			36
#define T_SCTP_STATUS			37
typedef struct t_sctp_dest_status {
	t_uscalar_t dest_addr;		/* dest address */
	t_uscalar_t dest_cwnd;		/* dest congestion window */
	t_uscalar_t dest_unack;		/* dest unacknowledged chunks */
	t_uscalar_t dest_srtt;		/* dest smooth round trip time */
	t_uscalar_t dest_rvar;		/* dest rtt variance */
	t_uscalar_t dest_rto;		/* dest current rto */
	t_uscalar_t dest_sst;		/* dest slow start threshold */
} t_sctp_dest_status_t;

typedef struct t_sctp_status {
	t_uscalar_t curr_rwnd;		/* current receive window */
	t_uscalar_t curr_rbuf;		/* current receive buffer */
	t_uscalar_t curr_nrep;		/* current dests reported */
	t_sctp_dest_status_t curr_dest[0];	/* current primary dest */
} t_sctp_status_t;

#define T_SCTP_DEBUG			38

#ifndef SCTP_OPTION_DROPPING
#define SCTP_OPTION_DROPPING	0x01	/* stream will drop packets */
#define SCTP_OPTION_BREAK	0x02	/* stream will break dest #1 */
#define SCTP_OPTION_DBREAK	0x04	/* stream will break dest both ways */
#define SCTP_OPTION_RANDOM	0x08	/* stream will drop packets at random */
#endif

#endif				/* _SYS_XTI_SCTP_H */
