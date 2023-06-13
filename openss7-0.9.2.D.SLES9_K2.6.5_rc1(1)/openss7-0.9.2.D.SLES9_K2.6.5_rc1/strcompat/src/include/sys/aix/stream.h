/*****************************************************************************

 @(#) stream.h,v 0.9.2.7 2006/06/22 13:11:20 brian Exp

 -----------------------------------------------------------------------------

 Copyright (c) 2001-2005  OpenSS7 Corporation <http://www.openss7.com/>

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

 Last Modified 2006/06/22 13:11:20 by brian

 -----------------------------------------------------------------------------

 stream.h,v
 Revision 0.9.2.7  2006/06/22 13:11:20  brian
 - more optmization tweaks and fixes

 Revision 0.9.2.6  2006/05/23 10:43:59  brian
 - mark normal inline functions for unlikely text section

 Revision 0.9.2.5  2005/12/28 09:51:47  brian
 - remove warnings on FC4 compile

 Revision 0.9.2.4  2005/12/19 12:44:13  brian
 - locking down for release

 Revision 0.9.2.3  2005/07/18 12:25:39  brian
 - standard indentation

 Revision 0.9.2.2  2005/07/14 22:03:43  brian
 - updates for check pass and header splitting

 Revision 0.9.2.1  2005/07/12 13:54:41  brian
 - changes for os7 compatibility and check pass

 *****************************************************************************/

#ifndef __SYS_AIX_STREAM_H__
#define __SYS_AIX_STREAM_H__

#ident "@(#) stream.h,v OpenSS7-0_9_2_D_rc2(0.9.2.7) Copyright (c) 2001-2005 OpenSS7 Corporation."

#ifndef __SYS_STREAM_H__
#warning "Do not include sys/aix/stream.h directly, include sys/stream.h instead."
#endif

#ifndef __KERNEL__
#error "Do not include kernel header files in user space programs."
#endif

#ifndef __EXTERN_INLINE
#define __EXTERN_INLINE extern __inline__
#endif

#ifndef __AIX_EXTERN_INLINE
#define __AIX_EXTERN_INLINE __EXTERN_INLINE
#endif

#ifndef _AIX_SOURCE
#warning "_AIX_SOURCE not defined but AIX stream.h included."
#endif

#include <sys/strcompat/config.h>

#if defined CONFIG_STREAMS_COMPAT_AIX || defined CONFIG_STREAMS_COMPAT_AIX_MODULE

#ifndef dev_t
#define dev_t __streams_dev_t
#endif

/* These are MPS definitions exposed by AIX, but implemented in mpscompat.c */
extern int mi_open_comm(caddr_t *mi_list, uint size, queue_t *q, dev_t *dev, int flag, int sflag,
			cred_t *credp);
extern int mi_close_comm(caddr_t *mi_list, queue_t *q);
extern caddr_t mi_next_ptr(caddr_t strptr);
extern caddr_t mi_prev_ptr(caddr_t strptr);
extern void mi_bufcall(queue_t *q, int size, int priority);

#ifdef LFS
extern int wantio(queue_t *q, struct wantio *w);

__AIX_EXTERN_INLINE __unlikely int
wantmsg(queue_t *q, int streamscall (*func) (mblk_t *))
{
	if (!q->q_qinfo->qi_srvp) {
		q->q_ftmsg = func;
		return (1);
	}
	return (0);
}
#endif

#else
#ifdef _AIX_SOURCE
#warning "_AIX_SOURCE defined by not CONFIG_STREAMS_COMPAT_AIX"
#endif
#endif

#endif				/* __SYS_AIX_STREAM_H__ */
