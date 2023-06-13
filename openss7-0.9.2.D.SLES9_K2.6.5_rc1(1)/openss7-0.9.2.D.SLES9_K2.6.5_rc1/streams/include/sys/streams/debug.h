/*****************************************************************************

 @(#) debug.h,v 0.9.2.8 2005/08/30 03:37:09 brian Exp

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

 Last Modified 2005/08/30 03:37:09 by brian

 *****************************************************************************/

#ifndef __SYS_STREAMS_DEBUG_H__
#define __SYS_STREAMS_DEBUG_H__

#ident "@(#) debug.h,v OpenSS7-0_9_2_D_rc2(0.9.2.8) 2005/08/30 03:37:09"

#ifndef __SYS_DEBUG_H__
#warning "Do no include sys/streams/debug.h directly, include sys/debug.h instead."
#endif

#ifndef __KERNEL__
#error "Do not use kernel headers for user space programs"
#endif				/* __KERNEL__ */

#include <linux/kernel.h>

#ifdef ASSERT
#undef ASSERT
#endif				/* ASSERT */

#ifdef assert
#undef assert
#endif				/* assert */

#ifdef DEBUG

/* the greater ASSERT does a panic */
#define ASSERT(__exp) \
	do { if (!(__exp)) \
		panic("%s: ASSERT(" #__exp ") failed at " __FILE __ " +%d\n", __FUNCTION__, __LINE__); \
	} while (0)

/* the lesser assert does an oops */
#define assert(__exp) \
	do { if (!(__exp)) \
		printk(KERN_EMERG "%s: assert(" #__exp ") failed at " __FILE __ " +%d\n", __FUNCTION__, __LINE__); \
		*(int *)0 = 0; \
	} while (0)
#else				/* DEBUG */

#define ASSERT(__exp)
#define assert(__exp)

#endif				/* DEBUG */

#endif				/* __SYS_STREAMS_DEBUG_H__ */
