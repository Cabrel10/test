/*****************************************************************************

 @(#) stream.h,v 0.9.2.6 2006/06/22 13:11:29 brian Exp

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

 Last Modified 2006/06/22 13:11:29 by brian

 -----------------------------------------------------------------------------

 stream.h,v
 Revision 0.9.2.6  2006/06/22 13:11:29  brian
 - more optmization tweaks and fixes

 Revision 0.9.2.5  2005/12/28 09:51:49  brian
 - remove warnings on FC4 compile

 Revision 0.9.2.4  2005/07/15 23:09:36  brian
 - checking in for sync

 Revision 0.9.2.3  2005/07/15 01:03:40  brian
 - i should reverse ddi and stream includes...

 Revision 0.9.2.2  2005/07/14 22:04:03  brian
 - updates for check pass and header splitting

 Revision 0.9.2.1  2005/07/12 13:54:44  brian
 - changes for os7 compatibility and check pass

 *****************************************************************************/

#ifndef __SYS_SVR3_STREAM_H__
#define __SYS_SVR3_STREAM_H__

#ident "@(#) stream.h,v OpenSS7-0_9_2_D_rc2(0.9.2.6) Copyright (c) 2001-2005 OpenSS7 Corporation."

#ifndef __SYS_STREAM_H__
#warning "Do not include sys/svr3/stream.h directly, include sys/stream.h instead."
#endif

#ifndef __KERNEL__
#error "Do not include kernel header files in user space programs."
#endif

#ifndef __EXTERN_INLINE
#define __EXTERN_INLINE extern __inline__
#endif

#ifndef __SVR3_EXTERN_INLINE
#define __SVR3_EXTERN_INLINE __EXTERN_INLINE
#endif

#ifndef _SVR3_SOURCE
#warning "_SVR3_SOURCE not defined but SVR3 stream.h included."
#endif

#include <sys/strcompat/config.h>

#if defined CONFIG_STREAMS_COMPAT_SVR3 || defined CONFIG_STREAMS_COMPAT_SVR3_MODULE

#ifndef dev_t
#define dev_t __streams_dev_t
#endif

#else
#ifdef _SVR3_SOURCE
#warning "_SVR3_SOURCE defined by not CONFIG_STREAMS_COMPAT_SVR3"
#endif
#endif

#endif				/* __SYS_SVR3_STREAM_H__ */
