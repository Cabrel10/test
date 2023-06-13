/*****************************************************************************

 @(#) cmn_err.h,v 0.9.2.12 2005/12/19 12:44:53 brian Exp

 -----------------------------------------------------------------------------

 Copyright (C) 2001-2005  OpenSS7 Corporation <http://www.openss7.com>

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

 Last Modified 2005/12/19 12:44:53 by brian

 *****************************************************************************/

#ifndef __SYS_STREAMS_CMN_ERR_H__
#define __SYS_STREAMS_CMN_ERR_H__ 1

#ident "@(#) cmn_err.h,v OpenSS7-0_9_2_D_rc2(0.9.2.12) 2005/12/19 12:44:53"

#ifndef __SYS_CMN_ERR_H__
#warning "Do no include sys/streams/cmn_err.h directly, include sys/cmn_err.h instead."
#endif

#ifndef __KERNEL__
#error "Do not include kernel header files in user space programs."
#endif				/* __KERNEL__ */

#ifndef __EXTERN
#define __EXTERN extern
#endif
#ifndef __STREAMS_EXTERN
#define __STREAMS_EXTERN __EXTERN streams_fastcall
#endif

#include <stdarg.h>

#define CE_CONT	    0
#define CE_NOTE	    1
#define CE_WARN	    2
#define CE_PANIC    3
#define CE_DEBUG    4		/* IRIX 6.5 */
#define CE_ALERT    5		/* IRIX 6.5 */

__STREAMS_EXTERN void vcmn_err(int err_lvl, const char *fmt, va_list args);
__STREAMS_EXTERN void cmn_err(int err_lvl, const char *fmt, ...)
    __attribute__ ((__format__(__printf__, 2, 3)));

#endif				/* __SYS_STREAMS_CMN_ERR_H__ */
