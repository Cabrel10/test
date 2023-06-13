/*****************************************************************************

 @(#) debug.h,v OpenSS7-0_9_2_D_rc2(0.9.2.3) 2005/05/14 08:26:11

 -----------------------------------------------------------------------------

 Copyright (c) 2001-2004  OpenSS7 Corporation <http://www.openss7.com>
 Copyright (c) 1997-2000  Brian F. G. Bidulock <bidulock@dallas.net>

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

 Last Modified 2005/05/14 08:26:11 by brian

 *****************************************************************************/

#ifndef __DEBUG_H__
#define __DEBUG_H__

#ident "@(#) debug.h,v OpenSS7-0_9_2_D_rc2(0.9.2.3) 2005/05/14 08:26:11"

#ifdef LFS
#include <sys/strdebug.h>
#else				/* LFS */

#if 1
#define DDTRACE          do { printk("%s [%s %d] trace\n",__FUNCTION__,__FILE__, __LINE__); } while(0)
#define DDPRINT(n,args)  do { printk args; } while(0)

#ifdef  _DEBUG
#define DTRACE           do { if ( DEBUG_LEVEL ) { printk("%s [%s %d] trace\n",__FUNCTION__,__FILE__, __LINE__); } } while(0)
#define DPRINT(n,args)   do { if (DEBUG_LEVEL>n) { printk args; } } while(0)
#define DBLOCK(n,block)     { if (DEBUG_LEVEL>n) { block } }
#else
#define DTRACE
#define DPRINT(n,args)
#define DBLOCK(n,block)
#endif
#endif

#undef  __never
#undef  __rare
#undef  __seldom
#undef  __usual
#undef  __normal
#undef  __assert
#undef  __assure
#undef  __ensure
#undef  __trace
#undef  __ptrace
#undef  __fixme
#undef  __todo
#undef  __ctrace
#undef  __printd
#undef  __swerr
#undef  __pswerr

#define __never() \
do { printk(KERN_EMERG "%s: never() at "__FILE__ " +%d\n", __FUNCTION__, __LINE__); *(int *)0 = 0; } while(0)

#define __rare() \
do { printk(KERN_NOTICE "%s: rare() at "__FILE__ " +%d\n", __FUNCTION__, __LINE__); } while(0)

#define __seldom() \
do { printk(KERN_NOTICE "%s: seldom() at "__FILE__ " +%d\n", __FUNCTION__, __LINE__); } while(0)

#define __usual(__exp) \
do { if (!(__exp)) printk(KERN_WARNING "%s: usual(" #__exp ") failed at " __FILE__ " +%d\n",__FUNCTION__, __LINE__); } while(0)

#define __normal(__exp) \
do { if (!(__exp)) printk(KERN_WARNING "%s: normal(" #__exp ") failed at " __FILE__ " +%d\n",__FUNCTION__, __LINE__); } while(0)

#define __assert(__exp) \
do { if (!(__exp)) { printk(KERN_EMERG "%s: assert(" #__exp ") failed at " __FILE__ " +%d\n",__FUNCTION__, __LINE__); *(int *)0 = 0; } } while(0)

#define __assure(__exp) \
do { if (!(__exp)) printk(KERN_WARNING "%s: assure(" #__exp ") failed at " __FILE__ " +%d\n",__FUNCTION__, __LINE__); } while(0)

#define __ensure(__exp,__sta) \
do { if (!(__exp)) { printk(KERN_WARNING "%s: ensure(" #__exp ") failed at " __FILE__ " +%d\n",__FUNCTION__, __LINE__); __sta; } } while(0)

#define __trace() \
do { printk(KERN_INFO "%s: trace() at " __FILE__ " +%d\n", __FUNCTION__, __LINE__); } while(0)

#define __ptrace(__pkspec) \
do { printk(KERN_INFO "%s: ptrace() at " __FILE__ " +%d\n", __FUNCTION__, __LINE__); printk __pkspec; } while(0)

#define __fixme(__pkspec) \
do { printk(KERN_INFO "%s: fixme() at " __FILE__ " +%d\n", __FUNCTION__, __LINE__); printk __pkspec; } while(0)

#define __todo(__pkspec) \
do { printk(KERN_INFO "%s: todo() at " __FILE__ " +%d\n", __FUNCTION__, __LINE__); printk __pkspec; } while(0)

#define __ctrace(__fnc) \
({ printk(KERN_INFO "%s: calling " #__fnc " at " __FILE__ "+%d\n", __FUNCTION__, __LINE__); __fnc; })

#define __printd(__pkspec) \
do { printk __pkspec; } while(0)

#define __swerr() \
do { printk(KERN_WARNING "%s: swerr() at " __FILE__ " +%d\n", __FUNCTION__, __LINE__); } while(0)

#define __pswerr(__pkspec) \
do { printk(KERN_WARNING "%s: pswerr() at " __FILE__ " +%d\n", __FUNCTION__, __LINE__); printk __pkspec; } while(0)

#undef     never
#undef     rare
#undef   seldom
#undef    usual
#undef  unusual
#undef   normal
#undef  abnormal
#undef   assert
#undef   assure
#undef   ensure
#undef   unless
#undef    trace
#undef   ptrace
#undef   ctrace
#undef    fixme
#undef     todo
#undef   printd
#undef    swerr
#undef   pswerr

#undef  STATIC
#undef  INLINE

#ifdef _DEBUG

#define   never()		__never()
#define    rare()		__rare()
#define  seldom()		__seldom()
#define   usual(__exp)		__usual(__exp)
#define unusual(__exp)		__usual(!(__exp))
#define  normal(__exp)		__normal(__exp)
#define abnormal(__exp)		__normal(!(__exp))
#define  assert(__exp)		__assert(__exp)
#define  assure(__exp)		__assure(__exp)
#define  ensure(__exp,__sta)	__ensure(__exp,__sta)
#define  unless(__exp,__sta)	__ensure(!(__exp),__sta)
#define   trace()		__trace()
#define  ptrace(__pks)		__ptrace(__pks)
#define  ctrace(__fnc)		__ctrace(__fnc)
#define   fixme(__pks)		__fixme(__pks)
#define    todo(__pks)		__todo(__pks)
#define  printd(__pks)		__printd(__pks)
#define   swerr()		__swerr()
#define  pswerr(__pks)		__pswerr(__pks)

#define STATIC
#define INLINE

#else				/* _DEBUG */
#ifdef _SAFE

#define   never()		do { *(int *)0 = 0; } while(0)
#define    rare()		do { } while(0)
#define  seldom()		do { } while(0)
#define   usual(__exp)		do { } while(0)
#define unusual(__exp)		do { } while(0)
#define  normal(__exp)		do { } while(0)
#define abnormal(__exp)		do { } while(0)
#define  assert(__exp)		{ if (!(__exp)) *(int *)0 = 0; } while(0)
#define  assure(__exp)		__assure(__exp)
#define  ensure(__exp,__sta)	__ensure(__exp,__sta)
#define  unless(__exp,__sta)	__ensure(!(__exp),__sta)
#define   trace()		do { } while(0)
#define  ptrace(__pks)		do { } while(0)
#define  ctrace(__fnc)		(__fnc)
#define   fixme(__pks)		__fixme(__pks)
#define    todo(__pks)		__todo(__pks)
#define  printd(__pks)		do { } while(0)
#define   swerr()		__swerr()
#define  pswerr(__pks)		__pswerr(__pks)

#define STATIC static
#define INLINE inline

#else				/* _SAFE */

#define   never()		do { } while(0)
#define	   rare()		do { } while(0)
#define	 seldom()		do { } while(0)
#define   usual(__exp)		do { } while(0)
#define unusual(__exp)		do { } while(0)
#define  normal(__exp)		do { } while(0)
#define abnormal(__exp)		do { } while(0)
#define  assert(__exp)		do { } while(0)
#define  assure(__exp)		do { } while(0)
#define  ensure(__exp,__sta)	do { } while(0)
#define  unless(__exp,__sta)	do { } while(0)
#define  trace()		do { } while(0)
#define  ptrace(__pks)		do { } while(0)
#define  ctrace(__fnc)		(__fnc)
#define   fixme(__pks)		__fixme(__pks)
#define    todo(__pks)		do { } while(0)
#define  printd(__pks)		do { } while(0)
#define   swerr()		__swerr()
#define  pswerr(__pks)		__pswerr(__pks)

#define STATIC static
#define INLINE inline

#endif				/* _SAFE */

#endif				/* _DEBUG */

#endif				/* LFS */

#endif				/* __DEBUG_H__ */
