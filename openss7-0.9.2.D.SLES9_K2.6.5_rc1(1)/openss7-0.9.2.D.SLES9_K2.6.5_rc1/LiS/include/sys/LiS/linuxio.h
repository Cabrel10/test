/*****************************************************************************

 @(#) linuxio.h,v 1.1.1.1.12.2 2005/07/13 12:01:20 brian Exp

 -----------------------------------------------------------------------------

 Copyright (c) 2001-2005  OpenSS7 Corporation <http://www.openss7.com>

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

 Last Modified 2005/07/13 12:01:20 by brian

 *****************************************************************************/

#ifndef _USRIO_H_
#define _USRIO_H_
/************************************************************************
*                        Linux I/O Functions				*
*************************************************************************
*									*
* This file is included by strtst.c if -DLINUX is set.  The strtst	*
* program is built for use in a user-level debugging context in which	*
* the test program (strtst.o) is linked in with the streams package	*
* itself plus a "user" I/O library.  When -DLINUX is set then the	*
* test program is a user level program running against the streams	*
* package in the kernel.  We have to change some names and the like	*
* to make this all work out.						*
*									*
************************************************************************/

#ident "@(#) linuxio.h,v OpenSS7-0_9_2_D_rc2(1.1.1.1.12.2) 2005/07/13 12:01:20"

#ifndef _SYS_POLL_H
#include <sys/poll.h>
#endif

#ifndef NULL
#define	NULL		( (void *) 0 )
#endif

#ifndef _IOW
#define _IOW(x,y,t)     (((((int)sizeof(t))&0xFFFF)<<16)|(x<<8)|y)
#endif

#define user_stat	stat
#define user_fstat	fstat
#define user_mknod	mknod
#define user_open(p,f,m)	open(p,f)
#define user_close	close
#define user_read	read
#define user_write	write
#define user_ioctl	ioctl
#define user_fcntl	fcntl
#define user_putpmsg	putpmsg
#define user_getpmsg	getpmsg
#define user_poll	poll
#define user_set_ptr	set_ptr
#define user_get_ptr	get_ptr
#define user_print_dir	print_dir
#define user_opendir	opendir
#define user_readdir	readdir
#define user_closedir	closedir
#define user_rewinddir	rewinddir
#define user_pipe       pipe
#define user_isastream  isastream
#define user_fattach    fattach
#define user_fdetach    fdetach

/************************************************************************
*                         fcntl Types                                   *
*************************************************************************
*									*
* The following codes will be defined here for calls to fcntl if they	*
* have not been defined elsewhere first.  These are SVR4 command codes	*
* that are not included in every environment's fcntl.h.			*
*									*
************************************************************************/

#ifndef F_ALLOCSP
#define F_ALLOCSP	10
#endif
#ifndef F_FREESP
#define F_FREESP	11
#endif
#ifndef F_RSETLK
#define F_RSETLK	20
#endif
#ifndef F_RGETLK
#define F_RGETLK	21
#endif
#ifndef F_RSETLKW
#define F_RSETLKW	22
#endif
#ifndef F_GETOWN
#define F_GETOWN	23
#endif
#ifndef F_SETOWN
#define F_SETOWN	24
#endif
#ifndef F_CHKFL
#define F_CHKFL		99	/* completely bogus value */
#endif

#endif				/* _USRIO_H_ */
