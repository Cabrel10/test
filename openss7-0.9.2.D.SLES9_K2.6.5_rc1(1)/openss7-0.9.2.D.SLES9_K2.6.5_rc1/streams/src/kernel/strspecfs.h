/*****************************************************************************

 @(#) strspecfs.h,v OpenSS7-0_9_2_D_rc2(0.9.2.19) 2005/12/19 12:45:20

 -----------------------------------------------------------------------------

 Copyright (c) 2001-2005  OpenSS7 Corporation <http://www.openss7.com>
 Copyright (c) 1997-2000  Brian F. G. Bidulock <bidulock@openss7.org>

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

 Last Modified 2005/12/19 12:45:20 by brian

 *****************************************************************************/

#ifndef __LOCCAL_STRSPECFS_H__
#define __LOCCAL_STRSPECFS_H__

#ifndef __EXTERN
#define __EXTERN extern
#endif

#ifndef __STREAMS_EXTERN
#define __STREAMS_EXTERN __EXTERN streams_fastcall
#endif

#define SPEC_SBI_MAGIC 0XFEEDDEAF
struct spec_sb_info {
	u32 sbi_magic;
	int sbi_setuid;
	int sbi_setgid;
	int sbi_setmod;
	uid_t sbi_uid;
	gid_t sbi_gid;
	umode_t sbi_mode;
};

// extern struct file_operations spec_dev_f_ops;

__STREAMS_EXTERN struct vfsmount *specfs_mount(void);
__STREAMS_EXTERN void specfs_umount(void);

__STREAMS_EXTERN int spec_reparent(struct file *file, struct cdevsw *cdev, dev_t dev);
__STREAMS_EXTERN int spec_open(struct file *file, struct cdevsw *cdev, dev_t dev, int sflag);
#ifndef BIG_COMPILE
__STREAMS_EXTERN struct inode *spec_snode(dev_t dev, struct cdevsw *cdev);
#endif

#if 0
/* initialization for main */
__STREAMS_EXTERN int strspecfs_init(void);
__STREAMS_EXTERN void strspecfs_exit(void);
#endif

#endif				/* __LOCCAL_STRSPECFS_H__ */
