/*****************************************************************************

 @(#) stream.c,v OpenSS7-0_9_2_D_rc2(1.1.1.4.4.5) 2006/07/07 21:21:47

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

 Last Modified 2006/07/07 21:21:47 by brian

 *****************************************************************************/

#ident "@(#) stream.c,v OpenSS7-0_9_2_D_rc2(1.1.1.4.4.5) 2006/07/07 21:21:47"

/*                               -*- Mode: C -*- 
 * stream.c --- STREAMS entry points and main routines 
 * Author          : Graham Wheeler, Francisco J. Ballesteros
 * Created On      : Tue May 31 22:25:19 1994
 * Last Modified By: David Grothe
 * RCS Id          : stream.c,v 1.1.1.4.4.5 2006/07/07 21:21:47 brian Exp
 * ----------------______________________________________________
 *
 *   Copyright (C) 1995  Francisco J. Ballesteros, Graham Wheeler,
 *                       Denis Froschauer
 *   Copyright (C) 1997  David Grothe, Gcom, Inc <dave@gcom.com>
 *
 *    You can reach us by email to any of
 *    nemo@ordago.uc3m.es, gram@aztec.co.za, 100741.1151@compuserve.com
 */

/*  -------------------------------------------------------------------  */
/*				 Dependencies                            */

#include <sys/strport.h>
#include <sys/stream.h>		/* this will include the whole stuff */
#include <sys/osif.h>

/*  -------------------------------------------------------------------  */
/*				    Types                                */

/*  -------------------------------------------------------------------  */
/*				  Glob. Vars                             */

volatile queue_t *lis_currq[LIS_NR_CPUS];
extern lis_atomic_t lis_queues_running;
extern lis_atomic_t lis_runq_req_cnt;	/* # req's to run queue runners */
extern lis_atomic_t lis_runq_active_flags[];
extern volatile unsigned long lis_runq_cnts[];
extern volatile unsigned long lis_queuerun_cnts[];
extern lis_spin_lock_t lis_qhead_lock;
extern lis_spin_lock_t lis_bc_lock;	/* buffcall.c */
extern int lis_down_nintr(lis_semaphore_t *sem);
extern void lis_wakeup_close_wt(void *q_str);	/* wait.c */

#if defined(CONFIG_DEV)
extern void lis_cpfl(void *p, long a, const char *fcn, const char *f, int l);

#define CP(p,a)		lis_cpfl((p),(a),__FUNCTION__,__LIS_FILE__,__LINE__)
#else
#define lis_cpfl(a,b,c,d,e)
#define CP(p,a)		do {} while (0)
#endif

/*  -------------------------------------------------------------------  */
/*			   Local functions & macros                      */
#if !defined(__GNUC__) && !defined(HAS__FUNCTION__)
#define	__FUNCTION__	""
#endif

/*  -------------------------------------------------------------------  */

/*  -------------------------------------------------------------------  */
/* service scheduler
 *
 * Note concerning the QRUNNING flag:  There is a race between this routine and
 * the lis_qdetach routine.  lis_qdetach may be trying to close this stream on
 * another CPU while we are trying to run its service procedure on this CPU.
 * We watch the QPROCSOFF flag in the queue as a signal that this is happening
 * and do not perform queue processing when this flag is set.  Likewise,
 * lis_qdetach may be waiting for us to finish with the queue before proceeding
 * to deallocate the queue structure.  It is watching the QENAB, QSCAN and
 * QRUNNING flag.  We are careful to set QRUNNING as we are clearing either
 * QENAB or QSCAN so that lis_qdetach won't think that the queue is not in use
 * when we are still using it.  However, when we clear QRUNNING, under lock
 * protection, we must consider two cases.  If the QWAITING flag is set then
 * qdetach is trying to close the queue and is waiting on a semaphore for us to
 * finish using the queue.  In that case we need to do a wakeup.  If QWAITING
 * is not set then we just release the queue isr lock.  In that case we need to
 * be sure that this is the LAST thing we do to the queue because qdetach could
 * be executing simultaneously on another CPU, see that it does not have to
 * wait, and proceed to deallocate the queue as soon as it can get the isr
 * lock.
 *
 * Locking order:  When getting the lis_qhead_lock and the queue lock for a
 * given queue simultaneously, we must get the queue lock first.  This 
 * ordering is dictated by lis_qenable which is called from user service 
 * procedures with the queue locked, and which needs to get lis_qhead_lock to 
 * protect its insertion into the global queue scheduling lists.  However,
 * that lock order doesn't work here.  We don't know what the queue is (to be
 * able to lock it) until we get it off of the scan list which is protected by
 * lis_qhead_lock.  See the note below on how a resulting race condition is
 * avoided.
 */
static void
queuerun(int cpu_id)
{
	queue_t *q;
	volatile queue_t *vq;
	stdata_t *strmhd;
	lis_flags_t psw, pswq;
	int procsoff;

	lis_queuerun_cnts[cpu_id]++;
	if (LIS_DEBUG_MONITOR_MEM)
		lis_check_mem();

	/* Process scan queue head list Process these one at a time.  Take the first entry off of
	   the front of the queue.  Other queue runners can take others off the queue
	   simultaneously, subject to spin lock protection. */
	lis_spin_lock_irqsave(&lis_qhead_lock, &psw);
	for (vq = lis_scanqhead, q = (queue_t *) vq; q != NULL;
	     vq = lis_scanqhead, q = (queue_t *) vq) {

		lis_scanqhead = q->q_scnxt;
		q->q_scnxt = NULL;
		if (q == lis_scanqtail)	/* entry at end of list */
			lis_scanqtail = NULL;

		K_ATOMIC_DEC(&lis_runq_req_cnt);	/* one less thing to do */
		/* 
		 * The normal rule is to obtain the queue lock and then lis_qhead_lock.
		 * But we can't do that in this case.  We don't know what the queue is
		 * until we get it off of lis_scanqhead which is protected by
		 * lis_qhead_lock.
		 *
		 * The following race condition therefore exists: This routine gets
		 * the queue off of lis_scanqhead.  Before we lock the queue, 
		 * lis_qdetach() runs on another CPU and frees the queue (including
		 * destroying the lock).  This routine then tries to lock and use the
		 * queue which is no longer valid.
		 *
		 * This race condition is avoided by setting the QRUNNING flag now.
		 * lis_qdetach() takes the queue off of lis_scanqhead before looking
		 * for the QRUNNING flag.  Since we are now holding the lock for
		 * lis_scanqhead, we will prevent it from getting past that point.
		 * It will then hold off freeing the queue until we clear it after
		 * running the queue.  If lis_qdetach() gets to the lock for
		 * lis_scanqhead first, we won't have found the q in the scan list
		 * (lis_qdetach() takes the queue off the scan list before giving up
		 * that lock).
		 */
		q->q_flag |= QRUNNING;	/* protects from lis_qdetach */

		lis_spin_unlock_irqrestore(&lis_qhead_lock, &psw);

		lis_currq[cpu_id] = q;	/* for debugging */
		if (!LIS_CHECK_Q_MAGIC(q)) {
#if 0
			printk("queuerun: scan queue is broken\n");
			lis_scanqhead = NULL;
			lis_scanqtail = NULL;
			K_ATOMIC_SET(&lis_runq_req_cnt, 0);
#else
			printk("queuerun: queue flags are 0x%lx\n", q->q_flag);
#endif
			goto scan_loop_bottom;
		}

		LIS_QISRLOCK(q, &pswq);
#if 0
		q->q_flag |= QRUNNING;	/* protects from lis_qdetach */
#endif
		q->q_flag &= ~QSCAN;
		procsoff = (q->q_flag & (QCLOSING | QPROCSOFF));
		strmhd = (stdata_t *) q->q_str;
		LIS_QISRUNLOCK(q, &pswq);

		if (lis_lockq(q) < 0) {	/* see "locking order" above */
			LIS_QISRLOCK(q, &pswq);
			q->q_flag &= ~QRUNNING;
			LIS_QISRUNLOCK(q, &pswq);
			goto scan_loop_bottom;
		}

		if (!procsoff && strmhd != NULL && strmhd->sd_wmsg != NULL) {	/* held msg waiting 
										 */
			lis_runq_cnts[cpu_id]++;
			lis_putnext(LIS_WR(q), strmhd->sd_wmsg);	/* send it */
			strmhd->sd_wmsg = NULL;	/* don't reuse msg */
		}

		lis_unlockq(q);

		LIS_QISRLOCK(q, &pswq);
		q->q_flag &= ~QRUNNING;	/* see note at head of func */
		if (F_ISSET(q->q_flag, QWAITING) && q->q_wakeup_sem != NULL) {
			LIS_QISRUNLOCK(q, &pswq);
			lis_up(q->q_wakeup_sem);
		} else
			LIS_QISRUNLOCK(q, &pswq);

		lis_head_put(strmhd);	/* balance ins scanq */
	      scan_loop_bottom:
		lis_spin_lock_irqsave(&lis_qhead_lock, &psw);	/* for loop iteration */
	}

	lis_spin_unlock_irqrestore(&lis_qhead_lock, &psw);

	lis_spin_lock_irqsave(&lis_bc_lock, &psw);
	if (lis_strbcflag) {
		lis_runq_cnts[cpu_id]++;
		lis_spin_unlock_irqrestore(&lis_bc_lock, &psw);
		K_ATOMIC_DEC(&lis_runq_req_cnt);	/* one less thing to do */
		lis_dobufcall(cpu_id);
	} else
		lis_spin_unlock_irqrestore(&lis_bc_lock, &psw);

	/* 
	 * This is the main queue list.  Process one at a time so that other
	 * threads can do the same simultaneously.
	 */
	lis_spin_lock_irqsave(&lis_qhead_lock, &psw);
	for (vq = lis_qhead, q = (queue_t *) vq; q != NULL; vq = lis_qhead, q = (queue_t *) vq) {
#if 0				/* minimize time holding qhead_lock */
		lis_cpfl((void *) lis_qhead, (long) lis_qtail, __FUNCTION__, __FILE__, __LINE__);
		if ((lis_qhead != NULL && lis_qtail == NULL)
		    || (lis_qhead == NULL && lis_qtail != NULL)
		    )
			printk("LiS: queuerun before: Qhead error: " "lis_qhead=%p lis_qtail=%p\n",
			       lis_qhead, lis_qtail);
#endif
		lis_qhead = q->q_link;	/* remove it from the list */
		q->q_link = NULL;
		q->q_flag |= QRUNNING;	/* protects from lis_qdetach */

		if (q == lis_qtail)	/* entry at end of list */
			lis_qtail = NULL;

#if 0				/* minimize time holding qhead_lock */
		lis_cpfl((void *) lis_qhead, (long) lis_qtail, __FUNCTION__, __FILE__, __LINE__);
		if ((lis_qhead != NULL && lis_qtail == NULL)
		    || (lis_qhead == NULL && lis_qtail != NULL)
		    )
			printk("LiS: queuerun after: Qhead error: " "lis_qhead=%p lis_qtail=%p\n",
			       lis_qhead, lis_qtail);

#endif
		K_ATOMIC_DEC(&lis_runq_req_cnt);	/* one less thing to do */
		lis_spin_unlock_irqrestore(&lis_qhead_lock, &psw);

		lis_currq[cpu_id] = q;	/* for debugging */
		if (!LIS_CHECK_Q_MAGIC(q)) {
#if 0
			printk("queuerun: service queue is broken\n");
			lis_qhead = NULL;
			lis_qtail = NULL;
			K_ATOMIC_SET(&lis_runq_req_cnt, 0);
#else
			printk("queuerun(2): queue flags are 0x%lx\n", q->q_flag);
#endif
			goto qsched_loop_bottom;
		}

		LIS_QISRLOCK(q, &pswq);
		CP(q->q_str, q->q_flag);
#if 0
		q->q_flag |= QRUNNING;	/* protects from lis_qdetach */
#endif
		q->q_flag &= ~QENAB;	/* allow qenable again */
		procsoff = (q->q_flag & QPROCSOFF);	/* queue procs off */
		strmhd = (stdata_t *) q->q_str;
		LIS_QISRUNLOCK(q, &pswq);

		if (lis_lockq(q) < 0) {	/* see "locking order" above */
			LIS_QISRLOCK(q, &pswq);
			q->q_flag &= ~QRUNNING;
			LIS_QISRUNLOCK(q, &pswq);
			goto qsched_loop_bottom;
		}

		LisDownCount(QSCHEDS);

		/* call its service procedure, if any */
		lis_runq_cnts[cpu_id]++;	/* count qrun for this cpu */
		if (!procsoff && q->q_qinfo != NULL && q->q_qinfo->qi_srvp	/* has svc proc */
		    ) {
			CP(strmhd, (long) q);
			(*q->q_qinfo->qi_srvp) (q);	/* enter svc proc w/q locked */

			if (lis_own_spl()) {	/* driver did not do splx */
				printk("queuerun: STREAMS driver %s returned from service "
				       "procedure with splstr locked\n", lis_queue_name(q));
				do
					lis_splx(psw);	/* attempt to undo */
				while (lis_own_spl());
			}
		}

		lis_unlockq(q);

		LIS_QISRLOCK(q, &pswq);
		CP(q->q_str, q->q_flag);
		q->q_flag &= ~QRUNNING;	/* see note at head of func */
		if (F_ISSET(q->q_flag, QWAITING) && q->q_wakeup_sem != NULL) {
			CP(strmhd, (long) q);
			LIS_QISRUNLOCK(q, &pswq);	/* release b4 wakeup */
			lis_up(q->q_wakeup_sem);	/* safe because qdetach is waiting */
		} else if (F_ISSET(q->q_flag, QWANTENAB)) {	/* enabled while running */
			CP(strmhd, (long) q);
			LIS_QISRUNLOCK(q, &pswq);
			lis_retry_qenable(q);	/* reschedule the queue */
		} else {
			CP(q->q_str, q->q_flag);
			LIS_QISRUNLOCK(q, &pswq);	/* last touch of q */
		}

		/* 
		 * Balance the head_get done by qenable if this is a stream
		 * head queue.
		 */
		if (strmhd->magic == STDATA_MAGIC && (strmhd->sd_wq == q || strmhd->sd_rq == q)
		    )
			lis_head_put(strmhd);

	      qsched_loop_bottom:
		lis_spin_lock_irqsave(&lis_qhead_lock, &psw);	/* for loop */
	}

	/* 
	 * Just in case lis_runq_req_cnt gets over-incremented, check to see if all
	 * queuerun conditions are now false.  If so set lis_runq_req_cnt back
	 * to zero so that we don't loop indefinitely through here.  We will also
	 * correct any decrements through zero here.  We do this while holding the
	 * lis_qhead_lock.  All incr/decrs of lis_runq_req_cnt are protected by
	 * this lock so we know that it won't change while we evaluate conditions.
	 */
	if (K_ATOMIC_READ(&lis_runq_req_cnt) != 0 && lis_scanqhead == NULL && lis_qhead == NULL
	    && lis_strbcflag == 0)
		K_ATOMIC_SET(&lis_runq_req_cnt, 0);

	lis_spin_unlock_irqrestore(&lis_qhead_lock, &psw);

}				/* queuerun */

extern int lis_nthreads;		/* module parameter from linux-mdep.c */

/*  -------------------------------------------------------------------  */
/*			Exported functions & macros                      */

/*  -------------------------------------------------------------------  */
/* lis_run_queues - entry point to the STREAMS scheduler. This is called
 *		    whenever we want the STREAMS queues to actually
 *		    be run.
 *
 * We are passed the cpu id of the processor that we are running on so we can
 * maintain the per-cpu running flags.
 */
void
lis_run_queues(int cpu)
{
	extern int lis_runq_sched;	/* linux-mdep.c BH code */

	while (K_ATOMIC_READ(&lis_runq_req_cnt) > 0) {
#if !defined(__SMP__)		/* only for single-threaded */
		if (K_ATOMIC_READ(&lis_queues_running))	/* recursion protection */
			return;
#endif
		K_ATOMIC_INC(&lis_queues_running);
		K_ATOMIC_INC(&lis_runq_active_flags[cpu]);
		queuerun(cpu);	/* really run the queues */
		K_ATOMIC_DEC(&lis_runq_active_flags[cpu]);
		K_ATOMIC_DEC(&lis_queues_running);
	}

	lis_runq_sched = 0;	/* OK to V semaphore now */

}				/* lis_run_queues */
