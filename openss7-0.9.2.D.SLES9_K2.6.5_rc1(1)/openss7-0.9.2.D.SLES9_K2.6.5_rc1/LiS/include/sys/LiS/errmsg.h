/*****************************************************************************

 @(#) errmsg.h,v 1.1.1.2.12.2 2005/07/13 12:01:19 brian Exp

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

 Last Modified 2005/07/13 12:01:19 by brian

 *****************************************************************************/

/*                               -*- Mode: C -*- 
 * errmsg.h --- streams error messages
 * Author          : Francisco J. Ballesteros
 * Created On      : Tue May 31 22:25:19 1994
 * Last Modified By: Francisco J. Ballesteros
 * Last Modified On: Tue Sep 26 15:19:24 1995
 * Update Count    : 2
 * RCS Id          : errmsg.h,v 1.1.1.2.12.2 2005/07/13 12:01:19 brian Exp
 * Usage           : see below :)
 * Required        : see below :)
 * Status          : (Exp) complete, untested, compiled
 * Prefix(es)      : lis
 * Requeriments    : 
 * Purpose         : provide streams errmsg
 *                 : 
 * ----------------______________________________________________
 *
 *    Copyright (C) 1995  Francisco J. Ballesteros
 *    Copyright (C) 1997  David Grothe, Gcom, Inc <dave@gcom.com>
 *
 *    You can reach me by email to 
 *    nemo@ordago.uc3m.es
 */

#ifndef _ERRMSG_H
#define _ERRMSG_H 1

#ident "@(#) errmsg.h,v OpenSS7-0_9_2_D_rc2(1.1.1.2.12.2) 2005/07/13 12:01:19"

#ifdef __KERNEL__

/*  -------------------------------------------------------------------  */
/*				   Symbols                               */

/*  -------------------------------------------------------------------  */
/*			Exported functions & macros                      */

/*
 * lis_error
 *
 * Simple error print
 */
#define lis_error(a,b,msg,arg)	printk("LiS:" msg, arg)

/*  -------------------------------------------------------------------  */
#endif				/* __KERNEL__ */
#endif				/* !_ERRMSG_H */

/*----------------------------------------------------------------------
# Local Variables:      ***
# change-log-default-name: "~/src/prj/streams/src/NOTES" ***
# End: ***
  ----------------------------------------------------------------------*/
