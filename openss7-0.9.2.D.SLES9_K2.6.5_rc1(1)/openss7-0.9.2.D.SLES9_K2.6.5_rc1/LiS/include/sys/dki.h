/*****************************************************************************

 @(#) dki.h,v 1.1.1.3.4.6 2005/12/19 03:22:21 brian Exp

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

 Last Modified 2005/12/19 03:22:21 by brian

 *****************************************************************************/

/************************************************************************
*                     SVR4 Driver-Kernel Interface			*
*************************************************************************
*									*
* These are the external definitions for SVR4 compatible DKI functions.	*
*									*
* Author:	David Grothe <dave@gcom.com>				*
*									*
* Copyright (C) 1997  David Grothe, Gcom, Inc <dave@gcom.com>		*
*									*
************************************************************************/

#ifndef	DKI_H
#define	DKI_H		1

#ident "@(#) dki.h,v OpenSS7-0_9_2_D_rc2(1.1.1.3.4.6) 2005/12/19 03:22:21"

#ifndef _SYS_TYPES_H
#include <sys/types.h>
#endif

#ifndef NULL
#define	NULL		( (void *) 0 )
#endif

/************************************************************************
*                           Timeouts                                    *
*************************************************************************
*									*
* The function timeout is called with a pointer to a function to	*
* invoke at a later time, an argument to be passed to the function,	*
* and the number of ticks to wait.  'timeout' returns an integer	*
* that serves as a "handle" for the timer.  The untimeout function	*
* takes a timer handle as an argument and stops the timer.		*
*									*
* These two functions are defined in the dki.c file.  Their		*
* implementation depends upon lis_tmout() and lis_untimout() which	*
* are defined in either linux-mdep.c, which uses Linux timers to	*
* implement the functions, or in the port-mdep.c file, which implements	*
* a portable timer algorithm based on the Linux timer mechanism.	*
*									*
************************************************************************/

#ifdef __KERNEL__

typedef void _RP timo_fcn_t (caddr_t arg);
typedef unsigned long toid_t;		/* SVR4 */
typedef unsigned long timeout_id_t;	/* Solaris */

#define	timeout(fcn,arg,ticks)	lis_timeout_fcn(fcn,arg,ticks,__FILE__, __LINE__)
#define	untimeout		lis_untimeout

extern toid_t
_RP lis_timeout_fcn(timo_fcn_t *timo_fcn, caddr_t arg, long ticks, char *file_name, int line_nr);
extern toid_t _RP lis_untimeout(toid_t id);

/*
 * The following are internal routines not exported
 */
#ifdef __LIS_INTERNAL__
void lis_initialize_dki(void);		/* dki.c */
void lis_terminate_dki(void);		/* dki.c */

void *lis_alloc_timer(char *file, int line);	/* mdep rouitine */
void *lis_free_timer(void *timerp);	/* mdep rouitine */
void lis_init_timers(int size);		/* mdep routine */
void lis_terminate_timers(void);	/* mdep rouitine */
#endif

#endif				/* __KERNEL__ */

#ifndef HZ
#define	HZ	100		/* ticks per second */
#endif

/*
 * Convert micro-seconds to proper ticks value.  The return value of
 * this routine is suitable for use with the "timeout" function.
 *
 * The routine is located in osif.c.
 */
unsigned _RP lis_usectohz(unsigned usec);

/************************************************************************
*                        Creating Nodes                                 *
*************************************************************************
*									*
* The following two routines can be used to create and remove nodes	*
* in the "/dev" directory.  These routines are used by drivers that	*
* wish to create these nodes dynamically at driver initialization time.	*
* They are especially useful for dynamically loaded drivers which obtain*
* their major device number at module load time.			*
*									*
* These routines have the same calling sequence as their user level	*
* versions (do "man mknod" for example).  These all return 0 upon	*
* success and a negative error number upon failure.			*
*									*
* When using mknod, remember to or in S_IFCHR with the mode (<stat.h>).	*
*									*
************************************************************************/

extern int _RP lis_mknod(char *name, int mode, dev_t dev);
extern int _RP lis_unlink(char *name);

#ifdef __LIS_INTERNAL__
extern int lis_mount(char *dev_name, char *dir_name, char *fstype, unsigned long rwflag,
		     void *data);
extern int lis_umount2(char *path, int flags);
#endif

#endif				/* from top of file */
