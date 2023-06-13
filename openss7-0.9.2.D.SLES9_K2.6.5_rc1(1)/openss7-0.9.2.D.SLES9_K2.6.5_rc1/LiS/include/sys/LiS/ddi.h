/*****************************************************************************

 @(#) ddi.h,v 1.1.1.3.4.2 2005/07/13 01:37:13 brian Exp

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

 Last Modified 2005/07/13 01:37:13 by brian

 *****************************************************************************/

/*                               -*- Mode: C -*- 
 * ddi.h --- 
 * Author          : Francisco J. Ballesteros
 * Created On      : Tue May 31 21:40:37 1994
 * Last Modified By: 
 * Last Modified On: 
 * Update Count    : 0
 * RCS Id          : ddi.h,v 1.1.1.3.4.2 2005/07/13 01:37:13 brian Exp
 * Usage           : see below :)
 * Required        : see below :)
 * Status          : (Exp) Unknown, Use with caution!
 * Prefix(es)      : 
 * Requeriments    : 
 * Purpose         : 
 *                 : 
 *
 * Copyright (C) 1997  David Grothe, Gcom, Inc <dave@gcom.com>
 *
 */

#ifndef __SYS_LIS_DDI_H__
#define __SYS_LIS_DDI_H__ 1

#ident "@(#) ddi.h,v OpenSS7-0_9_2_D_rc2(1.1.1.3.4.2) 2005/07/13 01:37:13"

#include <sys/LiS/genconf.h>

/* This file has to be updated with the Linux counterpart of SVR4 <sys/ddi.h>
 * header file. 
 */
#ifdef makedevice
#undef makedevice
#endif
#ifdef getmajor
#undef getmajor
#endif
#ifdef getminor
#undef getminor
#endif

#define makedevice		lis_makedevice
#define	getmajor		lis_getmajor
#define	getminor		lis_getminor

#endif				/* __SYS_LIS_DDI_H__ */

/*----------------------------------------------------------------------
# Local Variables:      ***
# change-log-default-name: "~/src/prj/streams/src/NOTES" ***
# End: ***
  ----------------------------------------------------------------------*/
