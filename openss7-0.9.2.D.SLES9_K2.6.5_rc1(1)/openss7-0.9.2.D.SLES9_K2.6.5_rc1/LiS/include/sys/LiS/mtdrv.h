/*****************************************************************************

 @(#) mtdrv.h,v 1.1.1.1.12.1 2005/04/12 22:45:25 brian Exp

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

 Last Modified 2005/04/12 22:45:25 by brian

 *****************************************************************************/

#ifndef __SYS_LIS_MTDRV_H__
#define __SYS_LIS_MTDRV_H__

/*
 * IOCTL definitions for mtdrv
 */

#ident "@(#) mtdrv.h,v OpenSS7-0_9_2_D_rc2(1.1.1.1.12.1) 2005/04/12 22:45:25"

#define	MTDRV_IOCTL(n)	(('M' << 8) | (n))
#define MTDRV_SET_CLONE_DEV	MTDRV_IOCTL(1)
#define MTDRV_SET_OPEN_SLEEP	MTDRV_IOCTL(2)

#endif				/* __SYS_LIS_MTDRV_H__ */