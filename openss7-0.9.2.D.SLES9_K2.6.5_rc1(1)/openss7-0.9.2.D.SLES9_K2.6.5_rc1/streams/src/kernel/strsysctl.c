/*****************************************************************************

 @(#) strsysctl.c,v OpenSS7-0_9_2_D_rc2(0.9.2.37) 2006/02/20 10:59:22

 -----------------------------------------------------------------------------

 Copyright (c) 2001-2006  OpenSS7 Corporation <http://www.openss7.com/>
 Copyright (c) 1997-2000  Brian F. G. Bidulock <bidulock@openss7.org>

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

 Last Modified 2006/02/20 10:59:22 by brian

 -----------------------------------------------------------------------------

 strsysctl.c,v
 Revision 0.9.2.37  2006/02/20 10:59:22  brian
 - updated copyright headers on changed files

 *****************************************************************************/

#ident "@(#) strsysctl.c,v OpenSS7-0_9_2_D_rc2(0.9.2.37) 2006/02/20 10:59:22"

static char const ident[] =
    "strsysctl.c,v OpenSS7-0_9_2_D_rc2(0.9.2.37) 2006/02/20 10:59:22";

#include <linux/config.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>

#ifdef CONFIG_PROC_FS
#include <linux/proc_fs.h>
#endif
#include <linux/sysctl.h>

#include "sys/strdebug.h"

#include <sys/stream.h>
#include <sys/strsubr.h>

#include "sys/config.h"
#include "src/modules/sth.h"	/* for str_minfo */
#include "strsysctl.h"		/* extern verification */

BIG_STATIC ulong sysctl_str_maxpsz = STRMAXPSZ;	/* stream head default max packet size */
BIG_STATIC ulong sysctl_str_minpsz = STRMINPSZ;	/* stream head default min packet size */
BIG_STATIC ulong sysctl_str_hiwat = STRHIGH;	/* stream head default hi water mark */
BIG_STATIC ulong sysctl_str_lowat = STRLOW;	/* stream head default lo water mark */
BIG_STATIC ulong sysctl_str_cltime = 15 * HZ;	/* close wait time in msec (saved in ticks) */
BIG_STATIC ulong sysctl_str_rtime = (10 * HZ) / 1000;/* msec to wait to forward held msg (saved in ticks) */
BIG_STATIC ulong sysctl_str_ioctime = 15 * HZ;	/* msec to wait for ioctl() acknowledgement (saved in ticks) */
BIG_STATIC_STH ulong sysctl_str_nstrpush = 64;			/* maximum number of pushed modules */
BIG_STATIC ulong sysctl_str_strthresh = (1 << 20);/* memory limit */
BIG_STATIC ulong sysctl_str_strhold = 0;		/* active stream hold feature */
BIG_STATIC_STH ulong sysctl_str_strctlsz = (1 << 12);		/* maximum stream control size */
ulong sysctl_str_strmsgsz = (1 << 18);		/* maximum stream message size */
BIG_STATIC ulong sysctl_str_nstrmsgs = (1 << 18);	/* maximum number of streams messages */
BIG_STATIC ulong sysctl_str_nband = 256;		/* number of queue bands */
BIG_STATIC int sysctl_str_reuse_fmodsw = 0;	/* reuse fmodsw entries if true */
BIG_STATIC ulong sysctl_str_max_apush = MAX_APUSH;/* max no autopushed mods per stream */
BIG_STATIC ulong sysctl_str_max_stramod = 8;	/* max no autopushed modules */
BIG_STATIC ulong sysctl_str_max_strdev = MAX_STRDEV;/* max no streams devices */
BIG_STATIC ulong sysctl_str_max_strmod = MAX_STRMOD;/* max no streams modules */
BIG_STATIC ulong sysctl_str_max_mblk = 10;	/* max no of headers on free list */
BIG_STATIC int sysctl_str_msg_priority = 0;	/* use priority on allocation if true */

EXPORT_SYMBOL_NOVERS(sysctl_str_strmsgsz);	/* always needed for drv_getparm */
#if defined CONFIG_STREAMS_STH_MODULE || !defined CONFIG_STREAMS_STH
EXPORT_SYMBOL_NOVERS(sysctl_str_strctlsz);
EXPORT_SYMBOL_NOVERS(sysctl_str_nstrpush);
#endif

STATIC struct ctl_table_header *streams_sysctl_handle;

STATIC ctl_table streams_table[] = {
	/* stream head default maximum packet size */
	{STREAMS_MAXPSZ, "maxpsz",
	 &sysctl_str_maxpsz, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_minmax, NULL, NULL, &sysctl_str_minpsz, NULL},
	/* stream head default minimum packet size */
	{STREAMS_MINPSZ, "minpsz",
	 &sysctl_str_minpsz, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_minmax, NULL, NULL, NULL, &sysctl_str_maxpsz},
	/* stream head default high water mark */
	{STREAMS_HIWAT, "hiwat",
	 &sysctl_str_hiwat, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_minmax, NULL, NULL, &sysctl_str_lowat, NULL},
	/* stream head default low water mark */
	{STREAMS_LOWAT, "lowat",
	 &sysctl_str_lowat, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_minmax, NULL, NULL, NULL, &sysctl_str_hiwat},
	/* default close time */
	{STREAMS_CLTIME, "cltime",
	 &sysctl_str_cltime, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_ms_jiffies_minmax, NULL, NULL, NULL, NULL},
	/* default hold time maximum */
	{STREAMS_RTIME, "rtime",
	 &sysctl_str_rtime, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_ms_jiffies_minmax, NULL, NULL, NULL, NULL},
	/* default ioctl() acknowledgement time */
	{STREAMS_IOCTIME, "ioctime",
	 &sysctl_str_ioctime, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_ms_jiffies_minmax, NULL, NULL, NULL, NULL},
	/* maximum number of pushed modules - system wide */
	{STREAMS_NSTRPUSH, "nstrpush",
	 &sysctl_str_nstrpush, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_minmax, NULL, NULL, NULL, NULL},
	/* maximum memory threshold */
	{STREAMS_STRTHRESH, "strthresh",
	 &sysctl_str_strthresh, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_minmax, NULL, NULL, NULL, NULL},
	/* hold time for STRHOLD feature in milliseconds, zero disables */
	{STREAMS_STRHOLD, "strhold",
	 &sysctl_str_strhold, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_ms_jiffies_minmax, NULL, NULL, NULL, NULL},
	/* maximum ctrl part size */
	{STREAMS_STRCTLSZ, "strctlsz",
	 &sysctl_str_strctlsz, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_minmax, NULL, NULL, NULL, NULL},
	/* maximum data part size */
	{STREAMS_STRMSGSZ, "strmsgsz",
	 &sysctl_str_strmsgsz, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_minmax, NULL, NULL, NULL, NULL},
	{STREAMS_NSTRMSGS, "nstrmsgs",
	 &sysctl_str_nstrmsgs, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_minmax, NULL, NULL, NULL, NULL},
	/* number of queue bands, 256 is too many already */
	{STREAMS_NBAND, "nband",
	 &sysctl_str_nband, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_minmax, NULL, NULL, NULL, NULL},
	{STREAMS_REUSE_FMODSW, "reuse_fmodsw",
	 &sysctl_str_reuse_fmodsw, sizeof(int), 0644, NULL,
	 &proc_dointvec, NULL, NULL, NULL, NULL},
	/* maximum number of autopushed modules - per device */
	{STREAMS_MAX_APUSH, "max_apush",
	 &sysctl_str_max_apush, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_minmax, NULL, NULL, NULL, NULL},
	{STREAMS_MAX_STRAMOD, "max_stramod",
	 &sysctl_str_max_stramod, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_minmax, NULL, NULL, NULL, NULL},
	/* maximum number of STREAMS cdevsw[] entries */
	{STREAMS_MAX_STRDEV, "max_strdev",
	 &sysctl_str_max_strdev, sizeof(ulong), 0444, NULL,
	 &proc_doulongvec_minmax, NULL, NULL, NULL, NULL},
	/* maximum number of STREAMS fmodsw[] entries */
	{STREAMS_MAX_STRMOD, "max_strmod",
	 &sysctl_str_max_strmod, sizeof(ulong), 0444, NULL,
	 &proc_doulongvec_minmax, NULL, NULL, NULL, NULL},
	/* maximum number of allocated mdbblocks */
	{STREAMS_MAX_MBLK, "max_mblk",
	 &sysctl_str_max_mblk, sizeof(ulong), 0644, NULL,
	 &proc_doulongvec_minmax, NULL, NULL, NULL, NULL},
	{STREAMS_MSG_PRIORITY, "msg_priority",
	 &sysctl_str_msg_priority, sizeof(int), 0644, NULL,
	 &proc_dointvec, NULL, NULL, NULL, NULL},
	{0}
};

STATIC ctl_table streams_root_table[] = {
	{CTL_STREAMS, "streams", NULL, 0, 0555, streams_table},
	{0}
};

BIG_STATIC int
strsysctl_init(void)
{
	sysctl_str_maxpsz = STRMAXPSZ;	/* stream head default max packet size */
	sysctl_str_minpsz = STRMINPSZ;	/* stream head default min packet size */
	sysctl_str_hiwat = STRHIGH;	/* stream head default hi water mark */
	sysctl_str_lowat = STRLOW;	/* stream head default lo water mark */
	if ((streams_sysctl_handle = register_sysctl_table(streams_root_table, 1)))
		return (0);
	return (-EIO);
}

BIG_STATIC void
strsysctl_exit(void)
{
	unregister_sysctl_table(streams_sysctl_handle);
	streams_sysctl_handle = NULL;
}
