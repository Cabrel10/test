/*****************************************************************************

 @(#) nullmod.c,v OpenSS7-0_9_2_D_rc2(1.1.2.5) 2005/12/20 15:11:41

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

 Last Modified 2005/12/20 15:11:41 by brian

 *****************************************************************************/

#ident "@(#) nullmod.c,v OpenSS7-0_9_2_D_rc2(1.1.2.5) 2005/12/20 15:11:41"

static char const ident[] =
    "nullmod.c,v OpenSS7-0_9_2_D_rc2(1.1.2.5) 2005/12/20 15:11:41";

/*
 *  This is NULLMOD a STREAMS null module that performs no actions other than acting as a STREAMS
 *  module.  Its purpose is primarily for testing and for serviing as an example of the skeleton of
 *  a STREAMS module.
 *
 *  This is an absurdly simple module.
 *
 *  In addition, this module provide a specialized set of input-output controls for testing and
 *  verifying various internal STREAMS functions.
 */

#include <linux/config.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>

#include <sys/kmem.h>
#include <sys/stream.h>
#include <sys/strconf.h>
#ifdef LFS
#include <sys/strsubr.h>
#endif
#include <sys/ddi.h>

#ifdef LFS
#include "sys/config.h"
#endif

#define NULLMOD_DESCRIP		"UNIX SYSTEM V RELEASE 4.2 FAST STREAMS FOR LINUX"
#define NULLMOD_COPYRIGHT	"Copyright (c) 1997-2005 OpenSS7 Corporation.  All Rights Reserved."
#define NULLMOD_REVISION	"LfS nullmod.c,v OpenSS7-0_9_2_D_rc2(1.1.2.5) 2005/12/20 15:11:41"
#define NULLMOD_DEVICE		"SVR 4.2 Null Module (NULLMOD) for STREAMS"
#define NULLMOD_CONTACT		"Brian Bidulock <bidulock@openss7.org>"
#define NULLMOD_LICENSE		"GPL"
#define NULLMOD_BANNER		NULLMOD_DESCRIP		"\n" \
				NULLMOD_COPYRIGHT	"\n" \
				NULLMOD_REVISION	"\n" \
				NULLMOD_DEVICE		"\n" \
				NULLMOD_CONTACT		"\n"
#define NULLMOD_SPLASH		NULLMOD_DEVICE		" - " \
				NULLMOD_REVISION	"\n"

#if defined LIS && defined MODULE
#define CONFIG_STREAMS_NULLMOD_MODULE MODULE
#endif

#ifdef CONFIG_STREAMS_NULLMOD_MODULE
MODULE_AUTHOR(NULLMOD_CONTACT);
MODULE_DESCRIPTION(NULLMOD_DESCRIP);
MODULE_SUPPORTED_DEVICE(NULLMOD_DEVICE);
MODULE_LICENSE(NULLMOD_LICENSE);
#if defined MODULE_ALIAS
MODULE_ALIAS("streams-nullmod");
#endif
#endif

#ifndef CONFIG_STREAMS_NULLMOD_NAME
#ifdef LIS
#define CONFIG_STREAMS_NULLMOD_NAME NULLMOD__MOD_NAME
#else
#error "CONFIG_STREAMS_NULLMOD_NAME must be defined."
#endif
#endif
#ifndef CONFIG_STREAMS_NULLMOD_MODID
#ifdef LIS
#define CONFIG_STREAMS_NULLMOD_MODID NULLMOD__ID
#else
#error "CONFIG_STREAMS_NULLMOD_MODID must be defined."
#endif
#endif

modID_t modid = CONFIG_STREAMS_NULLMOD_MODID;

#ifndef module_param
MODULE_PARM(modid, "h");
#else
module_param(modid, ushort, 0);
#endif
MODULE_PARM_DESC(modid, "Module ID for NULLMOD.");

#ifdef MODULE_ALIAS
#ifdef LFS
MODULE_ALIAS("streams-modid-" __stringify(CONFIG_STREAMS_NULLMOD_MODID));
MODULE_ALIAS("streams-module-nullmod");
#endif
#endif

#ifdef LIS
#define STRMINPSZ	0
#define STRMAXPSZ	4096
#define STRHIGH		5120
#define STRLOW		1024
#endif

STATIC struct module_info nullmod_minfo = {
	.mi_idnum = CONFIG_STREAMS_NULLMOD_MODID,
	.mi_idname = CONFIG_STREAMS_NULLMOD_NAME,
	.mi_minpsz = STRMINPSZ,
	.mi_maxpsz = STRMAXPSZ,
	.mi_hiwat = STRHIGH,
	.mi_lowat = STRLOW,
};

/* 
 *  -------------------------------------------------------------------------
 *
 *  Test and Verification Cases
 *
 *  -------------------------------------------------------------------------
 */

#ifdef LIS
union ioctypes {
	struct iocblk iocblk;
	struct copyreq copyreq;
	struct copyresp copyresp;
};
#endif

/*
 *  Test Case 1: allocb()  Test allocation and freeing of message blocks.
 */
STATIC int
testcase_1(union ioctypes *ioc, mblk_t *dp)
{
	ioc->iocblk.ioc_rval = 0;
	ioc->iocblk.ioc_error = 0;
	ioc->iocblk.ioc_count = 0;
	return (0);
}

STATIC int
testcase_2(union ioctypes *ioc, mblk_t *dp)
{
	ioc->iocblk.ioc_rval = 0;
	ioc->iocblk.ioc_error = 0;
	ioc->iocblk.ioc_count = 0;
	return (0);
}

STATIC int
testcase_3(union ioctypes *ioc, mblk_t *dp)
{
	ioc->iocblk.ioc_rval = 0;
	ioc->iocblk.ioc_error = 0;
	ioc->iocblk.ioc_count = 0;
	return (0);
}

STATIC int
testcase_4(union ioctypes *ioc, mblk_t *dp)
{
	ioc->iocblk.ioc_rval = 0;
	ioc->iocblk.ioc_error = 0;
	ioc->iocblk.ioc_count = 0;
	return (0);
}

STATIC int
testcase_5(union ioctypes *ioc, mblk_t *dp)
{
	ioc->iocblk.ioc_rval = 0;
	ioc->iocblk.ioc_error = 0;
	ioc->iocblk.ioc_count = 0;
	return (0);
}

STATIC int
testcase_6(union ioctypes *ioc, mblk_t *dp)
{
	ioc->iocblk.ioc_rval = 0;
	ioc->iocblk.ioc_error = 0;
	ioc->iocblk.ioc_count = 0;
	return (0);
}

STATIC int
testcase_7(union ioctypes *ioc, mblk_t *dp)
{
	ioc->iocblk.ioc_rval = 0;
	ioc->iocblk.ioc_error = 0;
	ioc->iocblk.ioc_count = 0;
	return (0);
}

STATIC int
testcase_8(union ioctypes *ioc, mblk_t *dp)
{
	ioc->iocblk.ioc_rval = 0;
	ioc->iocblk.ioc_error = 0;
	ioc->iocblk.ioc_count = 0;
	return (0);
}

STATIC int
testcase_9(union ioctypes *ioc, mblk_t *dp)
{
	ioc->iocblk.ioc_rval = 0;
	ioc->iocblk.ioc_error = 0;
	ioc->iocblk.ioc_count = 0;
	return (0);
}

STATIC int
testcase_10(union ioctypes *ioc, mblk_t *dp)
{
	ioc->iocblk.ioc_rval = 0;
	ioc->iocblk.ioc_error = 0;
	ioc->iocblk.ioc_count = 0;
	return (0);
}

/* 
 *  -------------------------------------------------------------------------
 *
 *  PUT routines
 *
 *  -------------------------------------------------------------------------
 */

#ifdef LIS
#define streamscall _RP
#endif

STATIC streamscall int
nullmod_wput(queue_t *q, mblk_t *mp)
{
	switch (mp->b_datap->db_type) {
	case M_IOCTL:
	{
		int err = 0;
		union ioctypes *ioc;

		ioc = (typeof(ioc)) mp->b_rptr;
		if (_IOC_TYPE(ioc->iocblk.ioc_cmd) != 'V')
			break;
		switch (_IOC_NR(ioc->iocblk.ioc_cmd)) {
		case 1:
			err = testcase_1(ioc, mp->b_cont);
			break;
		case 2:
			err = testcase_2(ioc, mp->b_cont);
			break;
		case 3:
			err = testcase_3(ioc, mp->b_cont);
			break;
		case 4:
			err = testcase_4(ioc, mp->b_cont);
			break;
		case 5:
			err = testcase_5(ioc, mp->b_cont);
			break;
		case 6:
			err = testcase_6(ioc, mp->b_cont);
			break;
		case 7:
			err = testcase_7(ioc, mp->b_cont);
			break;
		case 8:
			err = testcase_8(ioc, mp->b_cont);
			break;
		case 9:
			err = testcase_9(ioc, mp->b_cont);
			break;
		case 10:
			err = testcase_10(ioc, mp->b_cont);
			break;
		default:
			err = -EINVAL;
			break;
		}
		if (err) {
			mp->b_datap->db_type = M_IOCNAK;
			ioc->iocblk.ioc_count = 0;
			ioc->iocblk.ioc_error = -err;
			ioc->iocblk.ioc_rval = -1;
		} else {
			mp->b_datap->db_type = M_IOCACK;
		}
		qreply(q, mp);
		return (0);
	}
	}
	putnext(q, mp);
	return (0);
}

STATIC streamscall int
nullmod_rput(queue_t *q, mblk_t *mp)
{
	putnext(q, mp);
	return (0);
}

/*
 *  -------------------------------------------------------------------------
 *
 *  OPEN and CLOSE
 *
 *  -------------------------------------------------------------------------
 */
STATIC streamscall int
nullmod_open(queue_t *q, dev_t *devp, int oflag, int sflag, cred_t *crp)
{
	queue_t *wq;

	if (q->q_ptr != NULL) {
		return (0);	/* already open */
	}
	wq = WR(q);
	if (sflag == MODOPEN || wq->q_next != NULL) {
		/* inherit packet sizes of what we are pushed over */
		wq->q_minpsz = wq->q_next->q_minpsz;
		wq->q_maxpsz = wq->q_next->q_maxpsz;
		q->q_ptr = wq->q_ptr = q;	/* just set it to something */
		qprocson(q);
		return (0);
	}
	return (EIO);		/* can't be opened as driver */
}
STATIC streamscall int
nullmod_close(queue_t *q, int oflag, cred_t *crp)
{
	(void) oflag;
	(void) crp;
	if (!q->q_ptr)
		return (ENXIO);
	qprocsoff(q);
	q->q_ptr = WR(q)->q_ptr = NULL;
	return (0);
}

/* 
 *  -------------------------------------------------------------------------
 *
 *  Registration and initialization
 *
 *  -------------------------------------------------------------------------
 */
STATIC struct qinit nullmod_rinit = {
	.qi_putp = nullmod_rput,
	.qi_qopen = nullmod_open,
	.qi_qclose = nullmod_close,
	.qi_minfo = &nullmod_minfo,
};

STATIC struct qinit nullmod_winit = {
	.qi_putp = nullmod_wput,
	.qi_minfo = &nullmod_minfo,
};

STATIC struct streamtab nullmod_info = {
	.st_rdinit = &nullmod_rinit,
	.st_wrinit = &nullmod_winit,
};

#ifdef LFS
STATIC struct fmodsw nullmod_fmod = {
	.f_name = CONFIG_STREAMS_NULLMOD_NAME,
	.f_str = &nullmod_info,
	.f_flag = D_MP,
	.f_kmod = THIS_MODULE,
};
#endif

#ifdef CONFIG_STREAMS_NULLMOD_MODULE
STATIC
#endif
int __init
nullmod_init(void)
{
	int err;

#ifdef CONFIG_STREAMS_NULLMOD_MODULE
	printk(KERN_INFO NULLMOD_BANNER);
#else
	printk(KERN_INFO NULLMOD_SPLASH);
#endif
	nullmod_minfo.mi_idnum = modid;
#ifdef LFS
	if ((err = register_strmod(&nullmod_fmod)) < 0)
		return (err);
#endif
#ifdef LIS
	if ((err = lis_register_strmod(&nullmod_info, CONFIG_STREAMS_NULLMOD_NAME)) < 0)
		return (err);
#endif
	if (modid == 0 && err > 0)
		modid = err;
	return (0);
}

#ifdef CONFIG_STREAMS_NULLMOD_MODULE
STATIC
#endif
void __exit
nullmod_exit(void)
{
	int err;

#ifdef LFS
	if ((err = unregister_strmod(&nullmod_fmod)) < 0)
		return (void) (err);
#endif
#ifdef LIS
	if ((err = lis_unregister_strmod(&nullmod_info)) < 0)
		return (void) (err);
#endif
	return (void) (0);
}

#ifdef CONFIG_STREAMS_NULLMOD_MODULE
module_init(nullmod_init);
module_exit(nullmod_exit);
#endif
