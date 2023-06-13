/*****************************************************************************

 @(#) ddi.h,v 0.9.2.5 2006/06/22 13:11:26 brian Exp

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

 Last Modified 2006/06/22 13:11:26 by brian

 *****************************************************************************/

#ifndef __SYS_OS7_DDI_H__
#define __SYS_OS7_DDI_H__

#ident "@(#) ddi.h,v OpenSS7-0_9_2_D_rc2(0.9.2.5) Copyright (c) 2001-2006 OpenSS7 Corporation."

#ifndef __KERNEL__
#error "Do not use kernel headers for user space programs"
#endif				/* __KERNEL__ */

#ifndef __EXTERN_INLINE
#define __EXTERN_INLINE extern __inline__
#endif

#ifndef __OS7_EXTERN_INLINE
#define __OS7_EXTERN_INLINE __EXTERN_INLINE
#endif				/* __OS7_EXTERN_INLINE */

#ifndef _OS7_SOURCE
#warning "_OS7_SOURCE not defined but OS7 ddi.h included"
#endif

#if defined(CONFIG_STREAMS_COMPAT_OS7) || defined(CONFIG_STREAMS_COMPAT_OS7_MODULE)

#else
#ifdef _OS7_SOURCE
#warning "_OS7_SOURCE defined but not CONFIG_STREAMS_COMPAT_OS7"
#endif
#endif				/* CONFIG_STREAMS_COMPAT_OS7 */

#endif				/* __SYS_OS7_DDI_H__ */
