/*****************************************************************************

 @(#) svr4compat.c,v OpenSS7-0_9_2_D_rc2(0.9.2.29) 2006/07/07 20:49:55

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

 Last Modified 2006/07/07 20:49:55 by brian

 *****************************************************************************/

#ident "@(#) svr4compat.c,v OpenSS7-0_9_2_D_rc2(0.9.2.29) 2006/07/07 20:49:55"

static char const ident[] =
    "svr4compat.c,v OpenSS7-0_9_2_D_rc2(0.9.2.29) 2006/07/07 20:49:55";

/* 
 *  This is my solution for those who don't want to inline GPL'ed functions or
 *  who don't use optimizations when compiling or specifies
 *  -fnoinline-functions or something of the like.  This file implements all
 *  of the extern inlines from the header files by just including the header
 *  files with the functions declared 'inline' instead of 'extern inline'.
 *
 *  There are implemented here in a separate object, out of the way of the
 *  modules that don't use them.
 */

#define __SVR4_EXTERN_INLINE __inline__ streamscall

#define _SVR4_SOURCE

#include "sys/os7/compat.h"

#define SVR4COMP_DESCRIP	"UNIX SYSTEM V RELEASE 4.2 FAST STREAMS FOR LINUX"
#define SVR4COMP_COPYRIGHT	"Copyright (c) 1997-2005 OpenSS7 Corporation.  All Rights Reserved."
#define SVR4COMP_REVISION	"LfS svr4compat.c,v OpenSS7-0_9_2_D_rc2(0.9.2.29) 2006/07/07 20:49:55"
#define SVR4COMP_DEVICE		"UNIX(R) SVR 4.2 MP Compatibility"
#define SVR4COMP_CONTACT	"Brian Bidulock <bidulock@openss7.org>"
#define SVR4COMP_LICENSE	"GPL"
#define SVR4COMP_BANNER		SVR4COMP_DESCRIP	"\n" \
				SVR4COMP_COPYRIGHT	"\n" \
				SVR4COMP_REVISION	"\n" \
				SVR4COMP_DEVICE		"\n" \
				SVR4COMP_CONTACT	"\n"
#define SVR4COMP_SPLASH		SVR4COMP_DEVICE		" - " \
				SVR4COMP_REVISION	"\n"

#ifdef CONFIG_STREAMS_COMPAT_SVR4_MODULE
MODULE_AUTHOR(SVR4COMP_CONTACT);
MODULE_DESCRIPTION(SVR4COMP_DESCRIP);
MODULE_SUPPORTED_DEVICE(SVR4COMP_DEVICE);
MODULE_LICENSE(SVR4COMP_LICENSE);
#if defined MODULE_ALIAS
MODULE_ALIAS("streams-svr4compat");
#endif
#endif

#undef MPSTR_QLOCK
long
MPSTR_QLOCK(queue_t *q)
{
#ifdef LIS
	lis_flags_t flags;

	lis_rw_write_lock_irqsave(&q->q_isr_lock, &flags);
	return (flags);
#endif
#ifdef LFS
#if 0
	if (q->q_klock.kl_owner == current)
		q->q_klock.kl_nest++;
	else {
		unsigned long flags;

		local_irq_save(flags);
		write_lock(&q->q_klock.kl_lock);
		q->q_klock.kl_isrflags = flags;
		q->q_klock.kl_owner = current;
		q->q_klock.kl_nest = 0;
	}
	return (q->q_klock.kl_isrflags);
#else
	unsigned long flags;

	write_lock_irqsave(&q->q_lock, flags);
	return (flags);
#endif
#endif
}

EXPORT_SYMBOL_NOVERS(MPSTR_QLOCK);	/* svr4/ddi.h */

#undef MPSTR_QRELE
void
MPSTR_QRELE(queue_t *q, long s)
{
#ifdef LIS
	lis_flags_t flags = s;

	lis_rw_write_unlock_irqrestore(&q->q_isr_lock, &flags);
	return;
#endif
#ifdef LFS
#if 0
	if (q->q_klock.kl_nest > 0)
		q->q_klock.kl_nest--;
	else {
		unsigned long flags = q->q_klock.kl_isrflags;

		q->q_klock.kl_owner = NULL;
		q->q_klock.kl_nest = 0;
		if (waitqueue_active(&q->q_klock.kl_waitq))
			wake_up(&q->q_klock.kl_waitq);
		write_unlock(&q->q_klock.kl_lock);
		local_irq_restore(flags);
	}
	return;
#else
	unsigned long flags = s;

	write_unlock_irqrestore(&q->q_lock, flags);
#endif
#endif
}

EXPORT_SYMBOL_NOVERS(MPSTR_QRELE);	/* svr4/ddi.h */

#undef MPSTR_STPLOCK
long
MPSTR_STPLOCK(struct stdata *sd)
{
#ifdef LIS
	lis_flags_t flags;

	lis_spin_lock_irqsave(&sd->sd_lock, &flags);
	return (flags);
#endif
#ifdef LFS
#if 0
	if (sd->sd_klock.kl_owner == current)
		sd->sd_klock.kl_nest++;
	else {
		unsigned long flags;

		local_irq_save(flags);
		write_lock(&sd->sd_klock.kl_lock);
		sd->sd_klock.kl_isrflags = flags;
		sd->sd_klock.kl_owner = current;
		sd->sd_klock.kl_nest = 0;
	}
	return (sd->sd_klock.kl_isrflags);
#else
	unsigned long flags;
	write_lock_irqsave(&sd->sd_lock, flags);
	return (flags);
#endif
#endif
}

EXPORT_SYMBOL_NOVERS(MPSTR_STPLOCK);	/* svr4/ddi.h */

#undef MPSTR_STPRELE
void
MPSTR_STPRELE(struct stdata *sd, long s)
{
#ifdef LIS
	lis_flags_t flags = s;

	lis_spin_unlock_irqrestore(&sd->sd_lock, &flags);
	return;
#endif
#ifdef LFS
#if 0
	if (sd->sd_klock.kl_nest > 0)
		sd->sd_klock.kl_nest--;
	else {
		unsigned long flags = sd->sd_klock.kl_isrflags;

		sd->sd_klock.kl_owner = NULL;
		sd->sd_klock.kl_nest = 0;
		if (waitqueue_active(&sd->sd_klock.kl_waitq))
			wake_up(&sd->sd_klock.kl_waitq);
		write_unlock(&sd->sd_klock.kl_lock);
		local_irq_restore(flags);
	}
#else
	unsigned long flags = s;
	write_unlock_irqrestore(&sd->sd_lock, flags);
#endif
#endif
}

EXPORT_SYMBOL_NOVERS(MPSTR_STPRELE);	/* svr4/ddi.h */

static pl_t current_spl[NR_CPUS] __cacheline_aligned;

pl_t
spl0(void)
{
	pl_t old_level = xchg(&current_spl[smp_processor_id()], 0);

	local_irq_enable();
	local_bh_enable();
	return (old_level);
}

#ifdef LFS
__SVR4_EXTERN_INLINE toid_t dtimeout(timo_fcn_t *timo_fcn, caddr_t arg, long ticks, pl_t pl,
				     processorid_t processor);
EXPORT_SYMBOL_NOVERS(dtimeout);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE toid_t itimeout(timo_fcn_t *timo_fcn, caddr_t arg, long ticks, pl_t pl);

EXPORT_SYMBOL_NOVERS(itimeout);	/* svr4/ddi.h */
#endif

EXPORT_SYMBOL_NOVERS(spl0);		/* svr4/ddi.h */

pl_t
spl1(void)
{
	pl_t old_level = xchg(&current_spl[smp_processor_id()], 1);

	local_irq_enable();
	local_bh_enable();
	return (old_level);
}

EXPORT_SYMBOL_NOVERS(spl1);		/* svr4/ddi.h */

pl_t
spl2(void)
{
	pl_t old_level = xchg(&current_spl[smp_processor_id()], 2);

	local_irq_enable();
	local_bh_enable();
	return (old_level);
}

EXPORT_SYMBOL_NOVERS(spl2);		/* svr4/ddi.h */

pl_t
spl3(void)
{
	pl_t old_level = xchg(&current_spl[smp_processor_id()], 3);

	local_bh_disable();
	local_irq_enable();
	return (old_level);
}

EXPORT_SYMBOL_NOVERS(spl3);		/* svr4/ddi.h */

pl_t
spl4(void)
{
	pl_t old_level = xchg(&current_spl[smp_processor_id()], 4);

	local_bh_disable();
	local_irq_enable();
	return (old_level);
}

EXPORT_SYMBOL_NOVERS(spl4);		/* svr4/ddi.h */

pl_t
spl5(void)
{
	pl_t old_level = xchg(&current_spl[smp_processor_id()], 5);

	local_irq_disable();
	local_bh_disable();
	return (old_level);
}

EXPORT_SYMBOL_NOVERS(spl5);		/* svr4/ddi.h */

pl_t
spl6(void)
{
	pl_t old_level = xchg(&current_spl[smp_processor_id()], 6);

	local_irq_disable();
	local_bh_disable();
	return (old_level);
}

EXPORT_SYMBOL_NOVERS(spl6);		/* svr4/ddi.h */

pl_t
spl7(void)
{
	pl_t old_level = xchg(&current_spl[smp_processor_id()], 7);

	local_irq_disable();
	local_bh_disable();
	return (old_level);
}

EXPORT_SYMBOL_NOVERS(spl7);		/* svr4/ddi.h */

pl_t
spl(const pl_t level)
{
	switch (level) {
	case 0:
		return spl0();
	case 1:
		return spl1();
	case 2:
		return spl2();
	case 3:
		return spl3();
	case 4:
		return spl4();
	case 5:
		return spl5();
	case 6:
		return spl6();
	case 7:
		return spl7();
	}
	swerr();
	return (invpl);
}

EXPORT_SYMBOL_NOVERS(spl);		/* svr4/ddi.h */
void
splx(const pl_t level)
{
	return (void) spl(level);
}

EXPORT_SYMBOL_NOVERS(splx);		/* svr4/ddi.h */

__SVR4_EXTERN_INLINE major_t getemajor(dev_t dev);

EXPORT_SYMBOL_NOVERS(getemajor);	/* uw7/ddi.h */
__SVR4_EXTERN_INLINE minor_t geteminor(dev_t dev);

EXPORT_SYMBOL_NOVERS(geteminor);	/* uw7/ddi.h */

#ifndef NODEV
#define NODEV 0
#endif
int
etoimajor(major_t emajor)
{
	major_t major = NODEV;

#ifdef LFS
	struct cdevsw *cdev;

	if ((cdev = sdev_get(emajor))) {
		printd(("%s: %s: got device\n", __FUNCTION__, cdev->d_name));
		major = cdev->d_modid;
		printd(("%s: %s: putting device\n", __FUNCTION__, cdev->d_name));
		sdev_put(cdev);
	}
#endif
	return (major);
}

EXPORT_SYMBOL_NOVERS(etoimajor);	/* uw7/ddi.h */

int
itoemajor(major_t imajor, int prevemaj)
{
#ifdef LFS
	struct cdevsw *cdev;

	if ((cdev = cdrv_get(imajor)) && cdev->d_majors.next && !list_empty(&cdev->d_majors)) {
		struct list_head *pos;
		int found_previous = (prevemaj == NODEV) ? 1 : 0;

		printd(("%s: %s: got driver\n", __FUNCTION__, cdev->d_name));
		list_for_each(pos, &cdev->d_majors) {
			struct devnode *cmaj = list_entry(pos, struct devnode, n_list);

			if (found_previous)
				return (cmaj->n_major);
			if (prevemaj == cmaj->n_major)
				found_previous = 1;
		}
	}
#endif
	return (NODEV);
}

EXPORT_SYMBOL_NOVERS(itoemajor);	/* uw7/ddi.h */

//__SVR4_EXTERN_INLINE pl_t LOCK(lock_t * lockp, pl_t pl);
//EXPORT_SYMBOL_NOVERS(LOCK);          /* svr4/ddi.h */
__SVR4_EXTERN_INLINE lock_t *LOCK_ALLOC(unsigned char hierarchy, pl_t min_pl, lkinfo_t * lkinfop,
					int flag);
EXPORT_SYMBOL_NOVERS(LOCK_ALLOC);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE void LOCK_DEALLOC(lock_t * lockp);

EXPORT_SYMBOL_NOVERS(LOCK_DEALLOC);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE pl_t TRYLOCK(lock_t * lockp, pl_t pl);

EXPORT_SYMBOL_NOVERS(TRYLOCK);		/* svr4/ddi.h */
__SVR4_EXTERN_INLINE void UNLOCK(lock_t * lockp, pl_t pl);

EXPORT_SYMBOL_NOVERS(UNLOCK);		/* svr4/ddi.h */
__SVR4_EXTERN_INLINE int LOCK_OWNED(lock_t * lockp);

EXPORT_SYMBOL_NOVERS(LOCK_OWNED);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE rwlock_t *RW_ALLOC(unsigned char hierarchy, pl_t min_pl, lkinfo_t * lkinfop,
					int flag);
EXPORT_SYMBOL_NOVERS(RW_ALLOC);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE void RW_DEALLOC(rwlock_t *lockp);

EXPORT_SYMBOL_NOVERS(RW_DEALLOC);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE pl_t RW_RDLOCK(rwlock_t *lockp, pl_t pl);

EXPORT_SYMBOL_NOVERS(RW_RDLOCK);	/* svr4/ddi.h */
pl_t
RW_TRYRDLOCK(rwlock_t *lockp, pl_t pl)
{
	pl_t old_pl = spl(pl);

#if defined CONFIG_SMP && (defined HAVE_KFUNC_READ_TRYLOCK || defined HAVE_KMACRO_READ_TRYLOCK)
	if (read_trylock(lockp))
		return (old_pl);
#else
#if defined CONFIG_SMP && (defined HAVE_KFUNC_WRITE_TRYLOCK || defined HAVE_KMACRO_WRITE_TRYLOCK)
	if (write_trylock(lockp))
		return (old_pl);
#else
#if defined CONFIG_SMP
	/* this will jam up sometimes */
	if (!spin_is_locked(lockp)) {
#endif
		read_lock(lockp);
		return (old_pl);
#if defined CONFIG_SMP
	}
#endif
#endif
#endif
	splx(old_pl);
	return (invpl);
}

EXPORT_SYMBOL_NOVERS(RW_TRYRDLOCK);	/* svr4/ddi.h */
pl_t
RW_TRYWRLOCK(rwlock_t *lockp, pl_t pl)
{
	pl_t old_pl = spl(pl);

#if defined CONFIG_SMP && (defined HAVE_KFUNC_WRITE_TRYLOCK || defined HAVE_KMACRO_WRITE_TRYLOCK)
	if (write_trylock(lockp))
		return (old_pl);
#else
#if defined CONFIG_SMP
	/* this will jam up sometimes */
	if (!spin_is_locked(lockp)) {
#endif
		write_lock(lockp);
		return (old_pl);
#if defined CONFIG_SMP
	}
#endif
#endif
	splx(old_pl);
	return (invpl);
}

EXPORT_SYMBOL_NOVERS(RW_TRYWRLOCK);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE void RW_UNLOCK(rwlock_t *lockp, pl_t pl);

EXPORT_SYMBOL_NOVERS(RW_UNLOCK);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE pl_t RW_WRLOCK(rwlock_t *, pl_t pl);

EXPORT_SYMBOL_NOVERS(RW_WRLOCK);	/* svr4/ddi.h */

__SVR4_EXTERN_INLINE sleep_t *SLEEP_ALLOC(int arg, lkinfo_t * lkinfop, int flag);

EXPORT_SYMBOL_NOVERS(SLEEP_ALLOC);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE void SLEEP_DEALLOC(sleep_t * lockp);

EXPORT_SYMBOL_NOVERS(SLEEP_DEALLOC);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE int SLEEP_LOCKAVAIL(sleep_t * lockp);

EXPORT_SYMBOL_NOVERS(SLEEP_LOCKAVAIL);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE void SLEEP_LOCK(sleep_t * lockp, int priority);

EXPORT_SYMBOL_NOVERS(SLEEP_LOCK);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE int SLEEP_LOCKOWNED(sleep_t * lockp);

EXPORT_SYMBOL_NOVERS(SLEEP_LOCKOWNED);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE int SLEEP_LOCK_SIG(sleep_t * lockp, int priority);

EXPORT_SYMBOL_NOVERS(SLEEP_LOCK_SIG);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE int SLEEP_TRYLOCK(sleep_t * lockp);

EXPORT_SYMBOL_NOVERS(SLEEP_TRYLOCK);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE void SLEEP_UNLOCK(sleep_t * lockp);

EXPORT_SYMBOL_NOVERS(SLEEP_UNLOCK);	/* svr4/ddi.h */

__SVR4_EXTERN_INLINE sv_t *SV_ALLOC(int flag);

EXPORT_SYMBOL_NOVERS(SV_ALLOC);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE void SV_BROADCAST(sv_t * svp, int flags);

EXPORT_SYMBOL_NOVERS(SV_BROADCAST);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE void SV_DEALLOC(sv_t * svp);

EXPORT_SYMBOL_NOVERS(SV_DEALLOC);	/* svr4/ddi.h */
#if ! ( defined HAVE___WAKE_UP_SYNC_ADDR || defined HAVE___WAKE_UP_SYNC_EXPORT )
#undef	__wake_up_sync
#define __wake_up_sync __wake_up
#endif
void
SV_SIGNAL(sv_t * svp)
{
#ifdef HAVE___WAKE_UP_SYNC_ADDR
#undef	__wake_up_sync
	typeof(&__wake_up_sync) ___wake_up_sync =
	    (typeof(___wake_up_sync)) HAVE___WAKE_UP_SYNC_ADDR;
#define	__wake_up_sync ___wake_up_sync
#endif
	svp->sv_condv = 1;
	wake_up_interruptible_sync(&svp->sv_waitq);
}

EXPORT_SYMBOL_NOVERS(SV_SIGNAL);	/* svr4/ddi.h */
__SVR4_EXTERN_INLINE void SV_WAIT(sv_t * svp, int priority, lock_t * lkp);

EXPORT_SYMBOL_NOVERS(SV_WAIT);		/* svr4/ddi.h */
__SVR4_EXTERN_INLINE int SV_WAIT_SIG(sv_t * svp, int priority, lock_t * lkp);

EXPORT_SYMBOL_NOVERS(SV_WAIT_SIG);	/* svr4/ddi.h */

int ts_kmdpris[] = {
	60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
	70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
	80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
	90, 91, 92, 93, 94, 95, 96, 97, 98, 99
};

int ts_maxkmdpri = 39;

#define PSWP	     0
#define PMEM	     0
#define PINOD	    10
#define PRIBIO	    20
#define PZERO	    25
#define PPIPE	    26
#define PVFS	    27
#define TTIPRI	    28
#define TTOPRIO	    29
#define PWAIT	    30
#define PSLEP	    39

#define PCATCH	    0x8000
#define PNOSTOP	    0x4000

int
sleep(caddr_t event, pl_t pl)
{
	return 1;
}

EXPORT_SYMBOL_NOVERS(sleep);		/* svr4/ddi.h */

void
wakeup(caddr_t event)
{
	return;
}

EXPORT_SYMBOL_NOVERS(wakeup);		/* svr4/ddi.h */

#ifdef CONFIG_STREAMS_COMPAT_SVR4_MODULE
static
#endif
int __init
svr4comp_init(void)
{
#ifdef CONFIG_STREAMS_COMPAT_SVR4_MODULE
	printk(KERN_INFO SVR4COMP_BANNER);
#else
	printk(KERN_INFO SVR4COMP_SPLASH);
#endif
	return (0);
}

#ifdef CONFIG_STREAMS_COMPAT_SVR4_MODULE
static
#endif
void __exit
svr4comp_exit(void)
{
	return;
}

#ifdef CONFIG_STREAMS_COMPAT_SVR4_MODULE
module_init(svr4comp_init);
module_exit(svr4comp_exit);
#endif
