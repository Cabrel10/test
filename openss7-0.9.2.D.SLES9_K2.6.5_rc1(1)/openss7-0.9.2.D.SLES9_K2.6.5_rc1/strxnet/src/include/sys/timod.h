/*****************************************************************************

 @(#) timod.h,v 0.9.2.5 2006/02/23 12:00:10 brian Exp

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

 Last Modified 2006/02/23 12:00:10 by brian

 -----------------------------------------------------------------------------

 timod.h,v
 Revision 0.9.2.5  2006/02/23 12:00:10  brian
 - corrections for 64bit and 32/64bit compatibility
 - updated headers

 Revision 0.9.2.4  2005/11/04 07:36:33  brian
 - all test cases pass on Linux Fast-STREAMS

 Revision 0.9.2.3  2005/05/14 08:28:29  brian
 - copyright header correction

 Revision 0.9.2.2  2004/05/16 04:12:33  brian
 - Updating strxnet release.

 Revision 1.4  2004/05/14 08:00:02  brian
 - Updated xns, tli, inet, xnet and documentation.

 *****************************************************************************/

#ifndef _SYS_TIMOD_H
#define _SYS_TIMOD_H

#ident "@(#) timod.h,v OpenSS7-0_9_2_D_rc2(0.9.2.5) Copyright (c) 2001-2006 OpenSS7 Corporation."

#define TIMOD			('T'<<8)
#define O_TI_GETINFO		(TIMOD|100)	/* OSF 1 */
#define O_TI_OPTMGMT		(TIMOD|101)	/* OSF 2 */
#define O_TI_BIND		(TIMOD|102)	/* OSF 3 */
#define O_TI_UNBIND		(TIMOD|103)	/* OSF 4 */

#define O_TI_USED		0x01	/* data structure in use */
#define O_TI_FATAL		0x02	/* fatal M_ERROR_occured */
#define O_TI_WAITIOCACK		0x04	/* waiting for info for ioctl act */
#define O_TI_MORE		0x08	/* more data */

struct _o_ti_user {
	ushort ti_flags;		/* flags */
	int ti_rcvsize;			/* receive buffer size */
	char *ti_rcvbuf;		/* receive buffer */
	int ti_ctlsize;			/* control buffer size */
	char *ti_ctlbuf;		/* control buffer */
	char *ti_lookdbuf;		/* look data buffer */
	char *ti_lookcbuf;		/* look ctrl buffer */
	int ti_lookdsize;		/* look data buffer size */
	int ti_lookcsize;		/* look ctrl buffer size */
	int ti_maxpsz;			/* TIDU size */
#ifdef __LP64__
	u_int32_t ti_servtype;		/* service type */
#else					/* __LP64__ */
	long ti_servtype;		/* service type */
#endif					/* __LP64__ */
	int ti_lookflg;			/* buffered look flag */
};

/* These clash with termios ioctls if 'T' is used. */
/* OSF uses 't' instead. */
#define _O_TI_GETINFO		(TIMOD|1)	/* OSF */
#define _O_TI_OPTMGMT		(TIMOD|2)	/* OSF */
#define _O_TI_BIND		(TIMOD|3)	/* OSF */
#define _O_TI_UNBIND		(TIMOD|4)	/* OSF */
#define _O_TI_GETMYNAME		(TIMOD|5)	/* OSF */
#define _O_TI_GETPEERNAME	(TIMOD|6)	/* OSF */
#define _O_TI_XTI_HELLO		(TIMOD|7)	/* OSF */
#define _O_TI_XTI_GET_STATE	(TIMOD|8)	/* OSF */
#define _O_TI_XTI_CLEAR_EVENT	(TIMOD|9)	/* OSF */
#define _O_TI_XTI_MODE		(TIMOD|10)	/* OSF */
#define _O_TI_TLI_MODE		(TIMOD|11)	/* OSF */

typedef struct xti_state {
	unsigned int xtis_qlen;		/* Saved qlen parameter from t_bind */
} XTIS, *XTISP;

/* Some of these clash with BSD termios ioctls if 'T' is used. */
/* Linux wronly uses 'T' instead of 't' for BSD ioctls. */
#define TI_GETINFO		(TIMOD|140)
#define TI_OPTMGMT		(TIMOD|141)
#define TI_BIND			(TIMOD|142)
#define TI_UNBIND		(TIMOD|143)
#define TI_GETMYNAME		(TIMOD|144)
#define TI_GETPEERNAME		(TIMOD|145)
#define TI_SETMYNAME		(TIMOD|146)
#define TI_SETPEERNAME		(TIMOD|147)
#define TI_SYNC			(TIMOD|148)
#define TI_GETADDRS		(TIMOD|149)
#define TI_CAPABILITY		(TIMOD|150)

struct ti_sync_req {
	u_int32_t tsr_flags;
};

#define TSRF_INFO_REQ		01
#define TSRF_IS_EXP_IN_RCVBUF	02
#define TSRF_QLEN_REQ		04

struct ti_sync_ack {
	t_scalar_t PRIM_type;
	t_scalar_t TSDU_size;
	t_scalar_t ETSDU_size;
	t_scalar_t CDATA_size;
	t_scalar_t DDATA_size;
	t_scalar_t ADDR_size;
	t_scalar_t OPT_size;
	t_scalar_t TIDU_size;
	t_scalar_t SERV_type;
	t_scalar_t CURRENT_state;
	t_scalar_t PROVIDER_flag;

	t_uscalar_t tsa_qlen;
	u_int32_t tsa_flags;
};

#define TSA_EXP_QUEUED		01

#endif				/* _SYS_TIMOD_H */
