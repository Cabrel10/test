/*****************************************************************************

 @(#) test-xnet.c,v OpenSS7-0_9_2_D_rc2(0.9.2.26) 2006/07/08 07:26:39

 -----------------------------------------------------------------------------

 Copyright (c) 2001-2006  OpenSS7 Corporation <http://www.openss7.com/>
 Copyright (c) 1997-2000  Brian F. G. Bidulock <bidulock@openss7.org>

 All Rights Reserved.

 Unauthorized distribution or duplication is prohibited.

 This software and related documentation is protected by copyright and
 distributed under licenses restricting its use, copying, distribution and
 decompilation.  No part of this software or related documentation may be
 reproduced in any form by any means without the prior written authorization
 of the copyright holder, and licensors, if any.

 The recipient of this document, by its retention and use, warrants that the
 recipient will protect this information and keep it confidential, and will
 not disclose the information contained in this document without the written
 permission of its owner.

 The author reserves the right to revise this software and documentation for
 any reason, including but not limited to, conformity with standards
 promulgated by various agencies, utilization of advances in the state of the
 technical arts, or the reflection of changes in the design of any techniques,
 or procedures embodied, described, or referred to herein.  The author is
 under no obligation to provide any feature listed herein.

 -----------------------------------------------------------------------------

 As an exception to the above, this software may be distributed under the GNU
 General Public License (GPL) Version 2, so long as the software is distributed
 with, and only used for the testing of, OpenSS7 modules, drivers, and libraries.

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

 Last Modified 2006/07/08 07:26:39 by brian

 -----------------------------------------------------------------------------

 test-xnet.c,v
 Revision 0.9.2.26  2006/07/08 07:26:39  brian
 - removed trigraphs

 Revision 0.9.2.25  2006/02/23 11:51:09  brian
 - updated headers

 Revision 0.9.2.24  2006/02/23 11:43:19  brian
 - updates for 64 bit
 - disabled lockf because it doesn't work too well on SMP

 Revision 0.9.2.23  2005/12/28 10:01:41  brian
 - remove warnings on FC4 compile

 Revision 0.9.2.22  2005/07/18 12:45:12  brian
 - standard indentation

 Revision 0.9.2.21  2005/06/30 07:10:16  brian
 - corrected some more timing

 Revision 0.9.2.20  2005/06/29 23:20:41  brian
 - a couple more timing adjustments

 Revision 0.9.2.19  2005/06/29 23:17:47  brian
 - converted to expect operation

 Revision 0.9.2.18  2005/06/29 20:01:57  brian
 - conversion to expect syntax

 Revision 0.9.2.17  2005/06/29 04:21:22  brian
 - further upgrades of test suites

 Revision 0.9.2.16  2005/06/28 03:18:56  brian
 - upgrading test suites

 Revision 0.9.2.15  2005/06/23 22:07:13  brian
 - changes to pass _FORTIFY_SOURCE=2 on gcc 4 testing on FC4

 Revision 0.9.2.14  2005/06/07 00:52:26  brian
 - upgrading test suites

 Revision 0.9.2.13  2005/06/06 11:32:40  brian
 - more upgrades to test suites

 Revision 0.9.2.12  2005/06/05 05:15:15  brian
 - provide some additiona info and adjust timing

 Revision 0.9.2.11  2005/06/04 13:38:32  brian
 - final workup of test suites

 Revision 0.9.2.10  2005/06/03 23:57:36  brian
 - minor correction

 Revision 0.9.2.9  2005/06/03 12:19:17  brian
 - more upgrading of test suites

 Revision 0.9.2.8  2005/06/02 09:57:46  brian
 - a few corrections to upgraded test suites

 Revision 0.9.2.7  2005/05/19 20:49:29  brian
 - upgrading test suites

 Revision 0.9.2.6  2005/05/14 08:28:32  brian
 - copyright header correction

 *****************************************************************************/

#ident "@(#) test-xnet.c,v OpenSS7-0_9_2_D_rc2(0.9.2.26) 2006/07/08 07:26:39"

static char const ident[] = "test-xnet.c,v OpenSS7-0_9_2_D_rc2(0.9.2.26) 2006/07/08 07:26:39";

/*
 *  This is a ferry-clip XTI/TLI conformance test program for testing the
 *  OpenSS7 XTI/TLI Library.
 *
 *  GENERAL OPERATION:
 *
 *  The test program opens STREAMS pipe and pushes the "timod" module onto one
 *  side of the pipe.
 *
 *  The test program exchanges TPI primitives with one side of the open pipe
 *  and performs XTI/TLI Library Calls on a file descriptor at the other end
 *  of the pipe.  The side with the XTI/TLI Library represents the XTI user;
 *  the other side, the test harness, as shown below:
 *
 *                               USER SPACE
 *                              TEST PROGRAM
 *   ___________________________________  _________________________________
 *  |                                   |            \   /  |              
 *  |                                   |             \ /   |              
 *  |                                   |              |    |              
 *  |                                   |              |    |              
 *  |             XTI/TLI               |              |    |              
 *  |             Library               |              |    |              
 *  |                                   |              |    |              
 *  |                                   |              |    |              
 *  |                                   |              |    |              
 *  |___________________________________|              |    |              
 *               \   /  |                              |    |              
 *                \ /   |                              |    |              
 *                 |   / \                             |    |              
 *     ____________|__/___\__________                  |    |              
 *    |                              |                 |    |              
 *    |                              |                 |    |              
 *    |            TIMOD             |                 |    |              
 *    |                              |                 |    |              
 *    |        STREAMS MODULE        |                 |    |              
 *    |                              |                 |    |              
 *    |                              |                 |    |              
 *    |______________________________|                 |    |              
 *               \   /  |                              |    |              
 *                \ /   |                              |    |              
 *                 |    |                              |    |              
 *                 |   / \                             |    |              
 *     ____________|__/___\____________________________|____|_____________ 
 *    |                                                                   |
 *    |                                                                   |
 *    |                               PIPE                                |
 *    |                                                                   |
 *    |___________________________________________________________________|
 *
 *
 *  This test arrangement results in a a ferry-clip around the XTI/TLI Library
 *  and TIMOD module, where TPI primitives are injected and removed beneath
 *  the module as well XTI/TLI Library calls peformed above the module.
 *
 *  To preserve the environment and ensure that the tests are repeatable in
 *  any order, the entire test harness (pipe) is assembled and disassembled
 *  for each test.  A test preamble is used to place the module in the correct
 *  state for a test case to begin and then a postable is used to ensure that
 *  the module can be removed correctly.
 */

#include <sys/types.h>
#include <stropts.h>
#include <stdlib.h>

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#else
# ifdef HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif

#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/uio.h>
#include <time.h>

#ifdef HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif

#ifdef _GNU_SOURCE
#include <getopt.h>
#endif

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define NEED_T_USCALAR_T

#include <xti.h>
#include <tihdr.h>
#include <timod.h>
#include <xti_inet.h>

/*
 *  -------------------------------------------------------------------------
 *
 *  Configuration
 *
 *  -------------------------------------------------------------------------
 */

static const char *lpkgname = "OpenSS7 XTI/TLI Library";

/* static const char *spkgname = "XNET"; */
static const char *lstdname = "XNS 5.2";
static const char *sstdname = "XNS";
static const char *shortname = "XNET";
static char devname[256] = "/dev/echo";

static int exit_on_failure = 0;

static int verbose = 1;

static int show_msg = 0;
static int show_acks = 0;
static int show_timeout = 0;
static int show_data = 1;

static int last_prim = 0;
static int last_event = 0;
static int last_errno = 0;
static int last_retval = 0;
static int last_t_errno = 0;
static int last_tevent = 0;
static int last_qlen = 2;
static int last_sequence = 1;
static int last_servtype = T_COTS_ORD;
static int last_provflag = T_SENDZERO | T_ORDRELDATA | T_XPG4_1;
static int last_tstate = TS_UNBND;
struct T_info_ack last_info = { 0, };
static int last_prio = 0;

static int MORE_flag = 0;
static int DATA_flag = T_ODF_EX | T_ODF_MORE;

int test_fd[3] = { 0, 0, 0 };

#define BUFSIZE 5*4096

#define FFLUSH(stream)

#define SHORT_WAIT	  20	// 100 // 10
#define NORMAL_WAIT	 100	// 500 // 100
#define LONG_WAIT	 500	// 5000 // 500
#define LONGER_WAIT	1000	// 10000 // 5000
#define INFINITE_WAIT	-1
#define TEST_DURATION	20000

char cbuf[BUFSIZE];
char dbuf[BUFSIZE];

struct strbuf ctrl = { BUFSIZE, -1, cbuf };
struct strbuf data = { BUFSIZE, -1, dbuf };

static int test_sndflags = 0;		/* T_MORE | T_EXPEDITED | T_PUSH */
static int test_rcvflags = 0;		/* T_MORE | T_EXPEDITED */
static int test_pflags = MSG_BAND;	/* MSG_BAND | MSG_HIPRI */
static int test_pband = 0;
static int test_gflags = 0;		/* MSG_BAND | MSG_HIPRI */
static int test_gband = 0;
static int test_bufsize = 256;
static int test_tidu = 256;
static int test_mgmtflags = T_NEGOTIATE;
static struct sockaddr_in *test_addr = NULL;
static socklen_t test_alen = sizeof(*test_addr);
static const char *test_data = NULL;
static int test_resfd = -1;
static int test_timout = 200;
static void *test_opts = NULL;
static int test_olen = 0;
static int test_prio = 1;

struct strfdinsert fdi = {
	{BUFSIZE, 0, cbuf},
	{BUFSIZE, 0, dbuf},
	0,
	0,
	0
};
int flags = 0;

int dummy = 0;

struct timeval when;

char test_opt[256], test_udata[256], test_locadd[256], test_remadd[256];
struct t_unitdata test_sndudata = {
	{256, 256, test_remadd},
	{256, 256, test_opt},
	{256, 256, test_udata}
};
struct t_unitdata test_rcvudata = {
	{256, 0, test_remadd},
	{256, 0, test_opt},
	{256, 0, test_udata}
};
struct t_uderr test_uderr = {
	{256, 0, test_remadd},
	{256, 0, test_opt},
	0
};

/* listen call used in t_listen */
struct t_call test_liscall = {
	{256, 0, test_remadd},
	{256, 0, test_opt},
	{256, 0, test_udata},
	1
};

/* accepted call used in t_accept */
struct t_call test_acccall = {
	{256, 256, test_locadd},
	{256, 256, test_opt},
	{256, 256, test_udata},
	1
};

/* send to call used in t_connect */
struct t_call test_sndcall = {
	{256, 256, test_remadd},
	{256, 256, test_opt},
	{256, 256, test_udata},
	0
};

/* receive call used in t_connect an t_rcvconnect */
struct t_call test_rcvcall = {
	{256, 0, test_remadd},
	{256, 0, test_opt},
	{256, 0, test_udata},
	0
};
struct t_call test_discall = {
	{256, 256, test_locadd},
	{256, 256, test_opt},
	{256, 256, test_udata},
	1
};
struct t_optmgmt test_req = {
	{256, 256, test_opt},
	T_NEGOTIATE
};
struct t_optmgmt test_ret = {
	{256, 0, test_opt},
	0
};
struct t_discon test_rcvdis = {
	{256, 0, test_udata},
	0
};
struct t_discon test_snddis = {
	{256, 256, test_udata},
	0, 1
};
struct t_bind test_bindreq = {
	{256, 256, test_locadd},
	2
};
struct t_bind test_bindret = {
	{256, 0, test_locadd},
	2
};
struct t_bind test_addrloc = {
	{256, 0, test_locadd},
	0
};
struct t_bind test_addrrem = {
	{256, 0, test_remadd},
	0
};
struct t_info test_info;
struct iovec test_iov[4] = {
	[0] = {test_udata, 64},
	[1] = {test_udata + 64, 64},
	[2] = {test_udata + 64 + 64, 64},
	[3] = {test_udata + 64 + 64 + 64, 64},
};
struct t_iovec test_t_iov[4] = {
	[0] = {test_udata, 64},
	[1] = {test_udata + 64, 64},
	[2] = {test_udata + 64 + 64, 64},
	[3] = {test_udata + 64 + 64 + 64, 64},
};
unsigned int test_iovcount = 4;

/*
 *  -------------------------------------------------------------------------
 *
 *  Events and Actions
 *
 *  -------------------------------------------------------------------------
 */
enum {
	__EVENT_EOF = -7, __EVENT_NO_MSG = -6, __EVENT_TIMEOUT = -5, __EVENT_UNKNOWN = -4,
	__RESULT_DECODE_ERROR = -3, __RESULT_SCRIPT_ERROR = -2,
	__RESULT_INCONCLUSIVE = -1, __RESULT_SUCCESS = 0, __RESULT_FAILURE = 1,
	__RESULT_NOTAPPL = 3, __RESULT_SKIPPED = 77,
};

/*
 *  -------------------------------------------------------------------------
 */

int show = 1;

enum {
	__TEST_CONN_REQ = 2, __TEST_CONN_RES, __TEST_DISCON_REQ,
	__TEST_DATA_REQ, __TEST_EXDATA_REQ, __TEST_INFO_REQ, __TEST_BIND_REQ,
	__TEST_UNBIND_REQ, __TEST_UNITDATA_REQ, __TEST_OPTMGMT_REQ,
	__TEST_ORDREL_REQ, __TEST_OPTDATA_REQ, __TEST_ADDR_REQ,
	__TEST_CAPABILITY_REQ, __TEST_CONN_IND, __TEST_CONN_CON,
	__TEST_DISCON_IND, __TEST_DATA_IND, __TEST_EXDATA_IND,
	__TEST_INFO_ACK, __TEST_BIND_ACK, __TEST_ERROR_ACK, __TEST_OK_ACK,
	__TEST_UNITDATA_IND, __TEST_UDERROR_IND, __TEST_OPTMGMT_ACK,
	__TEST_ORDREL_IND, __TEST_NRM_OPTDATA_IND, __TEST_EXP_OPTDATA_IND,
	__TEST_ADDR_ACK, __TEST_CAPABILITY_ACK, __TEST_WRITE, __TEST_WRITEV,
	__TEST_PUTMSG_DATA, __TEST_PUTPMSG_DATA, __TEST_PUSH, __TEST_POP,
	__TEST_READ, __TEST_READV, __TEST_GETMSG, __TEST_GETPMSG,
	__TEST_DATA,
	__TEST_DATACK_REQ, __TEST_DATACK_IND, __TEST_RESET_REQ,
	__TEST_RESET_IND, __TEST_RESET_RES, __TEST_RESET_CON,
	__TEST_O_TI_GETINFO, __TEST_O_TI_OPTMGMT, __TEST_O_TI_BIND,
	__TEST_O_TI_UNBIND,
	__TEST__O_TI_GETINFO, __TEST__O_TI_OPTMGMT, __TEST__O_TI_BIND,
	__TEST__O_TI_UNBIND, __TEST__O_TI_GETMYNAME, __TEST__O_TI_GETPEERNAME,
	__TEST__O_TI_XTI_HELLO, __TEST__O_TI_XTI_GET_STATE,
	__TEST__O_TI_XTI_CLEAR_EVENT, __TEST__O_TI_XTI_MODE,
	__TEST__O_TI_TLI_MODE,
	__TEST_TI_GETINFO, __TEST_TI_OPTMGMT, __TEST_TI_BIND,
	__TEST_TI_UNBIND, __TEST_TI_GETMYNAME, __TEST_TI_GETPEERNAME,
	__TEST_TI_SETMYNAME, __TEST_TI_SETPEERNAME, __TEST_TI_SYNC,
	__TEST_TI_GETADDRS, __TEST_TI_CAPABILITY,
	__TEST_TI_SETMYNAME_DATA, __TEST_TI_SETPEERNAME_DATA,
	__TEST_TI_SETMYNAME_DISC, __TEST_TI_SETPEERNAME_DISC,
	__TEST_TI_SETMYNAME_DISC_DATA, __TEST_TI_SETPEERNAME_DISC_DATA,
	__TEST_O_NONBLOCK, __TEST_O_BLOCK,
	__TEST_T_ACCEPT, __TEST_T_BIND, __TEST_T_CLOSE, __TEST_T_CONNECT,
	__TEST_T_GETINFO, __TEST_T_GETPROTADDR, __TEST_T_GETSTATE,
	__TEST_T_LISTEN, __TEST_T_LOOK, __TEST_T_OPTMGMT, __TEST_T_RCV,
	__TEST_T_RCVCONNECT, __TEST_T_RCVDIS, __TEST_T_RCVREL,
	__TEST_T_RCVRELDATA, __TEST_T_RCVUDATA, __TEST_T_RCVUDERR,
	__TEST_T_RCVV, __TEST_T_RCVVUDATA, __TEST_T_SND, __TEST_T_SNDDIS,
	__TEST_T_SNDREL, __TEST_T_SNDRELDATA, __TEST_T_SNDUDATA,
	__TEST_T_SNDV, __TEST_T_SNDVUDATA, __TEST_T_SYNC, __TEST_T_UNBIND,
};

/*
 *  -------------------------------------------------------------------------
 *
 *  Timer Functions
 *
 *  -------------------------------------------------------------------------
 */

/*
 *  Timer values for tests: each timer has a low range (minus error margin)
 *  and a high range (plus error margin).
 */

static long timer_scale = 1;

#define TEST_TIMEOUT 5000

typedef struct timer_range {
	long lo;
	long hi;
} timer_range_t;

enum {
	t1 = 0, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15,
	t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29,
	t30, t31, t32, t33, t34, t35, t36, t37, t38, tmax
};

long test_start = 0;

static int state;

#if 1
#undef lockf
#define lockf(x,y,z) 0
#endif

#if 0
/*
 *  Return the current time in milliseconds.
 */
static long
now(void)
{
	long ret;
	struct timeval now;

	if (gettimeofday(&now, NULL)) {
		last_errno = errno;
		dummy = lockf(fileno(stdout), F_LOCK, 0);
		fprintf(stdout, "***************ERROR! couldn't get time!            !  !                    \n");
		fprintf(stdout, "%20s! %-54s\n", __FUNCTION__, strerror(last_errno));
		fflush(stdout);
		dummy = lockf(fileno(stdout), F_ULOCK, 0);
		return (0);
	}
	if (!test_start)	/* avoid blowing over precision */
		test_start = now.tv_sec;
	ret = (now.tv_sec - test_start) * 1000L;
	ret += (now.tv_usec + 999L) / 1000L;
	return ret;
}
static long
milliseconds(char *t)
{
	if (verbose > 0) {
		dummy = lockf(fileno(stdout), F_LOCK, 0);
		fprintf(stdout, "                    .               :               .  .                    \n");
		fprintf(stdout, "                    .             %6s            .  .                    <%d>\n", t, state);
		fprintf(stdout, "                    .               :               .  .                    \n");
		fflush(stdout);
		dummy = lockf(fileno(stdout), F_ULOCK, 0);
	}
	return now();
}
static long
milliseconds_2nd(char *t)
{
	if (verbose > 0) {
		dummy = lockf(fileno(stdout), F_LOCK, 0);
		fprintf(stdout, "                    .               :   :           .  .                    \n");
		fprintf(stdout, "                    .               : %6s        .  .                    <%d>\n", t, state);
		fprintf(stdout, "                    .               :   :           .  .                    \n");
		fflush(stdout);
		dummy = lockf(fileno(stdout), F_ULOCK, 0);
	}
	return now();
}

/*
 *  Check the current time against the beginning time provided as an argnument
 *  and see if the time inverval falls between the low and high values for the
 *  timer as specified by arguments.  Return SUCCESS if the interval is within
 *  the allowable range and FAILURE otherwise.
 */
static int
check_time(const char *t, long i, long lo, long hi)
{
	float tol, dlo, dhi, itv;

	itv = i * timer_scale;
	dlo = lo;
	dhi = hi;
	tol = 100 * timer_scale;
	itv = itv / 1000;
	dlo = dlo / 1000;
	dhi = dhi / 1000;
	tol = tol / 1000;
	if (verbose > 0) {
		dummy = lockf(fileno(stdout), F_LOCK, 0);
		fprintf(stdout, "                    |(%7.3g <= %7.3g <= %7.3g)|  | %6s             <%d>\n", dlo - tol, itv, dhi + tol, t, state);
		fflush(stdout);
		dummy = lockf(fileno(stdout), F_ULOCK, 0);
	}
	if (dlo - tol <= itv && itv <= dhi + tol)
		return __RESULT_SUCCESS;
	else
		return __RESULT_FAILURE;
}
#endif

static int
time_event(int child, int event)
{
	if (verbose > 4) {
		float t, m;
		struct timeval now;

		gettimeofday(&now, NULL);
		if (!test_start)
			test_start = now.tv_sec;
		t = (now.tv_sec - test_start);
		m = now.tv_usec;
		m = m / 1000000;
		t += m;
		dummy = lockf(fileno(stdout), F_LOCK, 0);
		fprintf(stdout, "                    |  | %11.6g                    |                    <%d:%03d>\n", t, child, state);
		fflush(stdout);
		dummy = lockf(fileno(stdout), F_ULOCK, 0);
	}
	return (event);
}

static int timer_timeout = 0;

static void
timer_handler(int signum)
{
	if (signum == SIGALRM)
		timer_timeout = 1;
	return;
}

static int
timer_sethandler(void)
{
	sigset_t mask;
	struct sigaction act;

	act.sa_handler = timer_handler;
	act.sa_flags = SA_RESTART | SA_ONESHOT;
	sigemptyset(&act.sa_mask);
	if (sigaction(SIGALRM, &act, NULL))
		return __RESULT_FAILURE;
	sigemptyset(&mask);
	sigaddset(&mask, SIGALRM);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);
	siginterrupt(SIGALRM, 1);
	return __RESULT_SUCCESS;
}

/*
 *  Start an interval timer as the overall test timer.
 */
static int
start_tt(long duration)
{
	struct itimerval setting = {
		{0, 0},
		{duration / 1000, (duration % 1000) * 1000}
	};

	if (timer_sethandler())
		return __RESULT_FAILURE;
	if (setitimer(ITIMER_REAL, &setting, NULL))
		return __RESULT_FAILURE;
	timer_timeout = 0;
	return __RESULT_SUCCESS;
}

#if 0
static int
start_st(long duration)
{
	long sdur = (duration + timer_scale - 1) / timer_scale;

	return start_tt(sdur);
}
#endif

static int
stop_tt(void)
{
	struct itimerval setting = { {0, 0}, {0, 0} };
	sigset_t mask;
	struct sigaction act;

	if (setitimer(ITIMER_REAL, &setting, NULL))
		return __RESULT_FAILURE;
	act.sa_handler = SIG_DFL;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	if (sigaction(SIGALRM, &act, NULL))
		return __RESULT_FAILURE;
	timer_timeout = 0;
	sigemptyset(&mask);
	sigaddset(&mask, SIGALRM);
	sigprocmask(SIG_BLOCK, &mask, NULL);
	return __RESULT_SUCCESS;
}

/*
 *  Addresses
 */

/*
 *  Options
 */

/*
 * data options
 */
struct {
	struct t_opthdr xti_hdr __attribute__ ((packed));
	t_scalar_t xti_val __attribute__ ((packed));
} opt_data = {
	{
	sizeof(struct t_opthdr) + sizeof(t_scalar_t), XTI_GENERIC, XTI_SNDBUF, T_SUCCESS}
, 32767};

/*
 * connect options
 */
struct {
	struct t_opthdr xti_hdr __attribute__ ((packed));
	t_scalar_t xti_val __attribute__ ((packed));
} opt_conn = {
	{
	sizeof(struct t_opthdr) + sizeof(t_scalar_t), XTI_GENERIC, XTI_SNDBUF, T_SUCCESS}
, 32767};

/*
 * management options
 */
struct {
	struct t_opthdr xti_hdr __attribute__ ((packed));
	t_scalar_t xti_val __attribute__ ((packed));
} opt_optm = {
	{
	sizeof(struct t_opthdr) + sizeof(t_scalar_t), XTI_GENERIC, XTI_SNDBUF, T_SUCCESS}
, 32767};

struct t_opthdr *
find_option(int level, int name, const char *cmd_buf, size_t opt_ofs, size_t opt_len)
{
	const char *opt_ptr = cmd_buf + opt_ofs;
	struct t_opthdr *oh = NULL;

	for (oh = _T_OPT_FIRSTHDR_OFS(opt_ptr, opt_len, 0); oh; oh = _T_OPT_NEXTHDR_OFS(opt_ptr, opt_len, oh, 0)) {
		int len = oh->len - sizeof(*oh);

		if (len < 0) {
			oh = NULL;
			break;
		}
		if (oh->level != level)
			continue;
		if (oh->name != name)
			continue;
		break;
	}
	return (oh);
}

/*
 *  -------------------------------------------------------------------------
 *
 *  Printing things
 *
 *  -------------------------------------------------------------------------
 */

char *
errno_string(long err)
{
	switch (err) {
	case 0:
		return ("ok");
	case EPERM:
		return ("[EPERM]");
	case ENOENT:
		return ("[ENOENT]");
	case ESRCH:
		return ("[ESRCH]");
	case EINTR:
		return ("[EINTR]");
	case EIO:
		return ("[EIO]");
	case ENXIO:
		return ("[ENXIO]");
	case E2BIG:
		return ("[E2BIG]");
	case ENOEXEC:
		return ("[ENOEXEC]");
	case EBADF:
		return ("[EBADF]");
	case ECHILD:
		return ("[ECHILD]");
	case EAGAIN:
		return ("[EAGAIN]");
	case ENOMEM:
		return ("[ENOMEM]");
	case EACCES:
		return ("[EACCES]");
	case EFAULT:
		return ("[EFAULT]");
	case ENOTBLK:
		return ("[ENOTBLK]");
	case EBUSY:
		return ("[EBUSY]");
	case EEXIST:
		return ("[EEXIST]");
	case EXDEV:
		return ("[EXDEV]");
	case ENODEV:
		return ("[ENODEV]");
	case ENOTDIR:
		return ("[ENOTDIR]");
	case EISDIR:
		return ("[EISDIR]");
	case EINVAL:
		return ("[EINVAL]");
	case ENFILE:
		return ("[ENFILE]");
	case EMFILE:
		return ("[EMFILE]");
	case ENOTTY:
		return ("[ENOTTY]");
	case ETXTBSY:
		return ("[ETXTBSY]");
	case EFBIG:
		return ("[EFBIG]");
	case ENOSPC:
		return ("[ENOSPC]");
	case ESPIPE:
		return ("[ESPIPE]");
	case EROFS:
		return ("[EROFS]");
	case EMLINK:
		return ("[EMLINK]");
	case EPIPE:
		return ("[EPIPE]");
	case EDOM:
		return ("[EDOM]");
	case ERANGE:
		return ("[ERANGE]");
	case EDEADLK:
		return ("[EDEADLK]");
	case ENAMETOOLONG:
		return ("[ENAMETOOLONG]");
	case ENOLCK:
		return ("[ENOLCK]");
	case ENOSYS:
		return ("[ENOSYS]");
	case ENOTEMPTY:
		return ("[ENOTEMPTY]");
	case ELOOP:
		return ("[ELOOP]");
	case ENOMSG:
		return ("[ENOMSG]");
	case EIDRM:
		return ("[EIDRM]");
	case ECHRNG:
		return ("[ECHRNG]");
	case EL2NSYNC:
		return ("[EL2NSYNC]");
	case EL3HLT:
		return ("[EL3HLT]");
	case EL3RST:
		return ("[EL3RST]");
	case ELNRNG:
		return ("[ELNRNG]");
	case EUNATCH:
		return ("[EUNATCH]");
	case ENOCSI:
		return ("[ENOCSI]");
	case EL2HLT:
		return ("[EL2HLT]");
	case EBADE:
		return ("[EBADE]");
	case EBADR:
		return ("[EBADR]");
	case EXFULL:
		return ("[EXFULL]");
	case ENOANO:
		return ("[ENOANO]");
	case EBADRQC:
		return ("[EBADRQC]");
	case EBADSLT:
		return ("[EBADSLT]");
	case EBFONT:
		return ("[EBFONT]");
	case ENOSTR:
		return ("[ENOSTR]");
	case ENODATA:
		return ("[ENODATA]");
	case ETIME:
		return ("[ETIME]");
	case ENOSR:
		return ("[ENOSR]");
	case ENONET:
		return ("[ENONET]");
	case ENOPKG:
		return ("[ENOPKG]");
	case EREMOTE:
		return ("[EREMOTE]");
	case ENOLINK:
		return ("[ENOLINK]");
	case EADV:
		return ("[EADV]");
	case ESRMNT:
		return ("[ESRMNT]");
	case ECOMM:
		return ("[ECOMM]");
	case EPROTO:
		return ("[EPROTO]");
	case EMULTIHOP:
		return ("[EMULTIHOP]");
	case EDOTDOT:
		return ("[EDOTDOT]");
	case EBADMSG:
		return ("[EBADMSG]");
	case EOVERFLOW:
		return ("[EOVERFLOW]");
	case ENOTUNIQ:
		return ("[ENOTUNIQ]");
	case EBADFD:
		return ("[EBADFD]");
	case EREMCHG:
		return ("[EREMCHG]");
	case ELIBACC:
		return ("[ELIBACC]");
	case ELIBBAD:
		return ("[ELIBBAD]");
	case ELIBSCN:
		return ("[ELIBSCN]");
	case ELIBMAX:
		return ("[ELIBMAX]");
	case ELIBEXEC:
		return ("[ELIBEXEC]");
	case EILSEQ:
		return ("[EILSEQ]");
	case ERESTART:
		return ("[ERESTART]");
	case ESTRPIPE:
		return ("[ESTRPIPE]");
	case EUSERS:
		return ("[EUSERS]");
	case ENOTSOCK:
		return ("[ENOTSOCK]");
	case EDESTADDRREQ:
		return ("[EDESTADDRREQ]");
	case EMSGSIZE:
		return ("[EMSGSIZE]");
	case EPROTOTYPE:
		return ("[EPROTOTYPE]");
	case ENOPROTOOPT:
		return ("[ENOPROTOOPT]");
	case EPROTONOSUPPORT:
		return ("[EPROTONOSUPPORT]");
	case ESOCKTNOSUPPORT:
		return ("[ESOCKTNOSUPPORT]");
	case EOPNOTSUPP:
		return ("[EOPNOTSUPP]");
	case EPFNOSUPPORT:
		return ("[EPFNOSUPPORT]");
	case EAFNOSUPPORT:
		return ("[EAFNOSUPPORT]");
	case EADDRINUSE:
		return ("[EADDRINUSE]");
	case EADDRNOTAVAIL:
		return ("[EADDRNOTAVAIL]");
	case ENETDOWN:
		return ("[ENETDOWN]");
	case ENETUNREACH:
		return ("[ENETUNREACH]");
	case ENETRESET:
		return ("[ENETRESET]");
	case ECONNABORTED:
		return ("[ECONNABORTED]");
	case ECONNRESET:
		return ("[ECONNRESET]");
	case ENOBUFS:
		return ("[ENOBUFS]");
	case EISCONN:
		return ("[EISCONN]");
	case ENOTCONN:
		return ("[ENOTCONN]");
	case ESHUTDOWN:
		return ("[ESHUTDOWN]");
	case ETOOMANYREFS:
		return ("[ETOOMANYREFS]");
	case ETIMEDOUT:
		return ("[ETIMEDOUT]");
	case ECONNREFUSED:
		return ("[ECONNREFUSED]");
	case EHOSTDOWN:
		return ("[EHOSTDOWN]");
	case EHOSTUNREACH:
		return ("[EHOSTUNREACH]");
	case EALREADY:
		return ("[EALREADY]");
	case EINPROGRESS:
		return ("[EINPROGRESS]");
	case ESTALE:
		return ("[ESTALE]");
	case EUCLEAN:
		return ("[EUCLEAN]");
	case ENOTNAM:
		return ("[ENOTNAM]");
	case ENAVAIL:
		return ("[ENAVAIL]");
	case EISNAM:
		return ("[EISNAM]");
	case EREMOTEIO:
		return ("[EREMOTEIO]");
	case EDQUOT:
		return ("[EDQUOT]");
	case ENOMEDIUM:
		return ("[ENOMEDIUM]");
	case EMEDIUMTYPE:
		return ("[EMEDIUMTYPE]");
	default:
	{
		static char buf[32];

		snprintf(buf, sizeof(buf), "[%ld]", err);
		return buf;
	}
	}
}

char *
terrno_string(ulong terr, long uerr)
{
	switch (terr) {
	case TBADADDR:
		return ("[TBADADDR]");
	case TBADOPT:
		return ("[TBADOPT]");
	case TACCES:
		return ("[TACCES]");
	case TBADF:
		return ("[TBADF]");
	case TNOADDR:
		return ("[TNOADDR]");
	case TOUTSTATE:
		return ("[TOUTSTATE]");
	case TBADSEQ:
		return ("[TBADSEQ]");
	case TSYSERR:
		return errno_string(uerr);
	case TLOOK:
		return ("[TLOOK]");
	case TBADDATA:
		return ("[TBADDATA]");
	case TBUFOVFLW:
		return ("[TBUFOVFLW]");
	case TFLOW:
		return ("[TFLOW]");
	case TNODATA:
		return ("[TNODATA]");
	case TNODIS:
		return ("[TNODIS]");
	case TNOUDERR:
		return ("[TNOUDERR]");
	case TBADFLAG:
		return ("[TBADFLAG]");
	case TNOREL:
		return ("[TNOREL]");
	case TNOTSUPPORT:
		return ("[TNOTSUPPORT]");
	case TSTATECHNG:
		return ("[TSTATECHNG]");
	case TNOSTRUCTYPE:
		return ("[TNOSTRUCTYPE]");
	case TBADNAME:
		return ("[TBADNAME]");
	case TBADQLEN:
		return ("[TBADQLEN]");
	case TADDRBUSY:
		return ("[TADDRBUSY]");
	case TINDOUT:
		return ("[TINDOUT]");
	case TPROVMISMATCH:
		return ("[TPROVMISMATCH]");
	case TRESQLEN:
		return ("[TRESQLEN]");
	case TRESADDR:
		return ("[TRESADDR]");
	case TQFULL:
		return ("[TQFULL]");
	case TPROTO:
		return ("[TPROTO]");
	default:
	{
		static char buf[32];

		snprintf(buf, sizeof(buf), "[%lu]", terr);
		return buf;
	}
	}
}

const char *
event_string(int event)
{
	switch (event) {
	case __EVENT_EOF:
		return ("END OF FILE");
	case __EVENT_NO_MSG:
		return ("NO MESSAGE");
	case __EVENT_TIMEOUT:
		return ("TIMEOUT");
	case __EVENT_UNKNOWN:
		return ("UNKNOWN");
	case __RESULT_DECODE_ERROR:
		return ("DECODE ERROR");
	case __RESULT_SCRIPT_ERROR:
		return ("SCRIPT ERROR");
	case __RESULT_INCONCLUSIVE:
		return ("INCONCLUSIVE");
	case __RESULT_SUCCESS:
		return ("SUCCESS");
	case __RESULT_FAILURE:
		return ("FAILURE");
	case __TEST_CONN_REQ:
		return ("T_CONN_REQ");
	case __TEST_CONN_RES:
		return ("T_CONN_RES");
	case __TEST_DISCON_REQ:
		return ("T_DISCON_REQ");
	case __TEST_DATA_REQ:
		return ("T_DATA_REQ");
	case __TEST_EXDATA_REQ:
		return ("T_EXDATA_REQ");
	case __TEST_OPTDATA_REQ:
		return ("T_OPTDATA_REQ");
	case __TEST_INFO_REQ:
		return ("T_INFO_REQ");
	case __TEST_BIND_REQ:
		return ("T_BIND_REQ");
	case __TEST_UNBIND_REQ:
		return ("T_UNBIND_REQ");
	case __TEST_UNITDATA_REQ:
		return ("T_UNITDATA_REQ");
	case __TEST_OPTMGMT_REQ:
		return ("T_OPTMGMT_REQ");
	case __TEST_ORDREL_REQ:
		return ("T_ORDREL_REQ");
	case __TEST_CONN_IND:
		return ("T_CONN_IND");
	case __TEST_CONN_CON:
		return ("T_CONN_CON");
	case __TEST_DISCON_IND:
		return ("T_DISCON_IND");
	case __TEST_DATA_IND:
		return ("T_DATA_IND");
	case __TEST_EXDATA_IND:
		return ("T_EXDATA_IND");
	case __TEST_NRM_OPTDATA_IND:
		return ("T_OPTDATA_IND");
	case __TEST_EXP_OPTDATA_IND:
		return ("T_OPTDATA_IND");
	case __TEST_INFO_ACK:
		return ("T_INFO_ACK");
	case __TEST_BIND_ACK:
		return ("T_BIND_ACK");
	case __TEST_ERROR_ACK:
		return ("T_ERROR_ACK");
	case __TEST_OK_ACK:
		return ("T_OK_ACK");
	case __TEST_UNITDATA_IND:
		return ("T_UNITDATA_IND");
	case __TEST_UDERROR_IND:
		return ("T_UDERROR_IND");
	case __TEST_OPTMGMT_ACK:
		return ("T_OPTMGMT_ACK");
	case __TEST_ORDREL_IND:
		return ("T_ORDREL_IND");
	case __TEST_ADDR_REQ:
		return ("T_ADDR_REQ");
	case __TEST_ADDR_ACK:
		return ("T_ADDR_ACK");
	case __TEST_CAPABILITY_REQ:
		return ("T_CAPABILITY_REQ");
	case __TEST_CAPABILITY_ACK:
		return ("T_CAPABILITY_ACK");
	default:
		return ("(unexpected");
	}
}

const char *
ioctl_string(int cmd, intptr_t arg)
{
	switch (cmd) {
	case I_NREAD:
		return ("I_NREAD");
	case I_PUSH:
		return ("I_PUSH");
	case I_POP:
		return ("I_POP");
	case I_LOOK:
		return ("I_LOOK");
	case I_FLUSH:
		return ("I_FLUSH");
	case I_SRDOPT:
		return ("I_SRDOPT");
	case I_GRDOPT:
		return ("I_GRDOPT");
	case I_STR:
		if (arg) {
			struct strioctl *icp = (struct strioctl *) arg;

			switch (icp->ic_cmd) {
			case _O_TI_BIND:
				return ("_O_TI_BIND");
			case O_TI_BIND:
				return ("O_TI_BIND");
			case _O_TI_GETINFO:
				return ("_O_TI_GETINFO");
			case O_TI_GETINFO:
				return ("O_TI_GETINFO");
			case _O_TI_GETMYNAME:
				return ("_O_TI_GETMYNAME");
			case _O_TI_GETPEERNAME:
				return ("_O_TI_GETPEERNAME");
			case _O_TI_OPTMGMT:
				return ("_O_TI_OPTMGMT");
			case O_TI_OPTMGMT:
				return ("O_TI_OPTMGMT");
			case _O_TI_TLI_MODE:
				return ("_O_TI_TLI_MODE");
			case _O_TI_UNBIND:
				return ("_O_TI_UNBIND");
			case O_TI_UNBIND:
				return ("O_TI_UNBIND");
			case _O_TI_XTI_CLEAR_EVENT:
				return ("_O_TI_XTI_CLEAR_EVENT");
			case _O_TI_XTI_GET_STATE:
				return ("_O_TI_XTI_GET_STATE");
			case _O_TI_XTI_HELLO:
				return ("_O_TI_XTI_HELLO");
			case _O_TI_XTI_MODE:
				return ("_O_TI_XTI_MODE");
			case TI_BIND:
				return ("TI_BIND");
			case TI_CAPABILITY:
				return ("TI_CAPABILITY");
			case TI_GETADDRS:
				return ("TI_GETADDRS");
			case TI_GETINFO:
				return ("TI_GETINFO");
			case TI_GETMYNAME:
				return ("TI_GETMYNAME");
			case TI_GETPEERNAME:
				return ("TI_GETPEERNAME");
			case TI_OPTMGMT:
				return ("TI_OPTMGMT");
			case TI_SETMYNAME:
				return ("TI_SETMYNAME");
			case TI_SETPEERNAME:
				return ("TI_SETPEERNAME");
			case TI_SYNC:
				return ("TI_SYNC");
			case TI_UNBIND:
				return ("TI_UNBIND");
			}
		}
		return ("I_STR");
	case I_SETSIG:
		return ("I_SETSIG");
	case I_GETSIG:
		return ("I_GETSIG");
	case I_FIND:
		return ("I_FIND");
	case I_LINK:
		return ("I_LINK");
	case I_UNLINK:
		return ("I_UNLINK");
	case I_RECVFD:
		return ("I_RECVFD");
	case I_PEEK:
		return ("I_PEEK");
	case I_FDINSERT:
		return ("I_FDINSERT");
	case I_SENDFD:
		return ("I_SENDFD");
#if 0
	case I_E_RECVFD:
		return ("I_E_RECVFD");
#endif
	case I_SWROPT:
		return ("I_SWROPT");
	case I_GWROPT:
		return ("I_GWROPT");
	case I_LIST:
		return ("I_LIST");
	case I_PLINK:
		return ("I_PLINK");
	case I_PUNLINK:
		return ("I_PUNLINK");
	case I_FLUSHBAND:
		return ("I_FLUSHBAND");
	case I_CKBAND:
		return ("I_CKBAND");
	case I_GETBAND:
		return ("I_GETBAND");
	case I_ATMARK:
		return ("I_ATMARK");
	case I_SETCLTIME:
		return ("I_SETCLTIME");
	case I_GETCLTIME:
		return ("I_GETCLTIME");
	case I_CANPUT:
		return ("I_CANPUT");
#if 0
	case I_SERROPT:
		return ("I_SERROPT");
	case I_GERROPT:
		return ("I_GERROPT");
	case I_ANCHOR:
		return ("I_ANCHOR");
#endif
#if 0
	case I_S_RECVFD:
		return ("I_S_RECVFD");
	case I_STATS:
		return ("I_STATS");
	case I_BIGPIPE:
		return ("I_BIGPIPE");
#endif
#if 0
	case I_GETTP:
		return ("I_GETTP");
	case I_AUTOPUSH:
		return ("I_AUTOPUSH");
	case I_HEAP_REPORT:
		return ("I_HEAP_REPORT");
	case I_FIFO:
		return ("I_FIFO");
	case I_PUTPMSG:
		return ("I_PUTPMSG");
	case I_GETPMSG:
		return ("I_GETPMSG");
	case I_FATTACH:
		return ("I_FATTACH");
	case I_FDETACH:
		return ("I_FDETACH");
	case I_PIPE:
		return ("I_PIPE");
#endif
	default:
		return ("(unexpected)");
	}
}

const char *
service_type(ulong type)
{
	switch (type) {
	case T_CLTS:
		return ("T_CLTS");
	case T_COTS:
		return ("T_COTS");
	case T_COTS_ORD:
		return ("T_COTS_ORD");
	default:
		return ("(unknown)");
	}
}

const char *
state_string(ulong state)
{
	switch (state) {
	case TS_UNBND:
		return ("TS_UNBND");
	case TS_WACK_BREQ:
		return ("TS_WACK_BREQ");
	case TS_WACK_UREQ:
		return ("TS_WACK_UREQ");
	case TS_IDLE:
		return ("TS_IDLE");
	case TS_WACK_OPTREQ:
		return ("TS_WACK_OPTREQ");
	case TS_WACK_CREQ:
		return ("TS_WACK_CREQ");
	case TS_WCON_CREQ:
		return ("TS_WCON_CREQ");
	case TS_WRES_CIND:
		return ("TS_WRES_CIND");
	case TS_WACK_CRES:
		return ("TS_WACK_CRES");
	case TS_DATA_XFER:
		return ("TS_DATA_XFER");
	case TS_WIND_ORDREL:
		return ("TS_WIND_ORDREL");
	case TS_WREQ_ORDREL:
		return ("TS_WRES_ORDREL");
	case TS_WACK_DREQ6:
		return ("TS_WACK_DREQ6");
	case TS_WACK_DREQ7:
		return ("TS_WACK_DREQ7");
	case TS_WACK_DREQ9:
		return ("TS_WACK_DREQ9");
	case TS_WACK_DREQ10:
		return ("TS_WACK_DREQ10");
	case TS_WACK_DREQ11:
		return ("TS_WACK_DREQ11");
	default:
		return ("(unknown)");
	}
}

#if 0
void
print_addr(char *add_ptr, size_t add_len)
{
	sctp_addr_t *a = (sctp_addr_t *) add_ptr;
	size_t anum = add_len >= sizeof(a->port) ? (add_len - sizeof(a->port)) / sizeof(a->addr[0]) : 0;

	dummy = lockf(fileno(stdout), F_LOCK, 0);
	if (add_len) {
		int i;

		if (add_len != sizeof(a->port) + anum * sizeof(a->addr[0]))
			fprintf(stdout, "Aaarrg! add_len = %d, anum = %d, ", add_len, anum);
		fprintf(stdout, "[%d]", ntohs(a->port));
		for (i = 0; i < anum; i++) {
			fprintf(stdout, "%s%d.%d.%d.%d", i ? "," : "", (a->addr[i] >> 0) & 0xff, (a->addr[i] >> 8) & 0xff, (a->addr[i] >> 16) & 0xff, (a->addr[i] >> 24) & 0xff);
		}
	} else
		fprintf(stdout, "(no address)");
	fprintf(stdout, "\n");
	fflush(stdout);
	dummy = lockf(fileno(stdout), F_ULOCK, 0);
}

char *
addr_string(char *add_ptr, size_t add_len)
{
	static char buf[128];
	size_t len = 0;
	sctp_addr_t *a = (sctp_addr_t *) add_ptr;
	size_t anum = add_len >= sizeof(a->port) ? (add_len - sizeof(a->port)) / sizeof(a->addr[0]) : 0;

	if (add_len) {
		int i;

		if (add_len != sizeof(a->port) + anum * sizeof(a->addr[0]))
			len += snprintf(buf + len, sizeof(buf) - len, "Aaarrg! add_len = %d, anum = %d, ", add_len, anum);
		len += snprintf(buf + len, sizeof(buf) - len, "[%d]", ntohs(a->port));
		for (i = 0; i < anum; i++) {
			len += snprintf(buf + len, sizeof(buf) - len, "%s%d.%d.%d.%d", i ? "," : "", (a->addr[i] >> 0) & 0xff, (a->addr[i] >> 8) & 0xff, (a->addr[i] >> 16) & 0xff, (a->addr[i] >> 24) & 0xff);
		}
	} else
		len += snprintf(buf + len, sizeof(buf) - len, "(no address)");
	/* len += snprintf(buf + len, sizeof(buf) - len, "\0"); */
	return buf;
}

void
print_addrs(int fd, char *add_ptr, size_t add_len)
{
	fprintf(stdout, "Stupid!\n");
}
#else
void
print_addr(char *add_ptr, size_t add_len)
{
	struct sockaddr_in *a = (struct sockaddr_in *) add_ptr;

	dummy = lockf(fileno(stdout), F_LOCK, 0);
	if (add_len) {
		if (add_len != sizeof(*a))
			fprintf(stdout, "Aaarrg! add_len = %lu, ", (ulong) add_len);
		fprintf(stdout, "%d.%d.%d.%d:%d", (a->sin_addr.s_addr >> 0) & 0xff, (a->sin_addr.s_addr >> 8) & 0xff, (a->sin_addr.s_addr >> 16) & 0xff, (a->sin_addr.s_addr >> 24) & 0xff, ntohs(a->sin_port));
	} else
		fprintf(stdout, "(no address)");
	fprintf(stdout, "\n");
	fflush(stdout);
	dummy = lockf(fileno(stdout), F_ULOCK, 0);
}

char *
addr_string(char *add_ptr, size_t add_len)
{
	static char buf[128];
	size_t len = 0;
	struct sockaddr_in *a = (struct sockaddr_in *) add_ptr;

	if (add_len) {
		if (add_len != sizeof(*a))
			len += snprintf(buf + len, sizeof(buf) - len, "Aaarrg! add_len = %lu, ", (ulong) add_len);
		len += snprintf(buf + len, sizeof(buf) - len, "%d.%d.%d.%d:%d", (a->sin_addr.s_addr >> 0) & 0xff, (a->sin_addr.s_addr >> 8) & 0xff, (a->sin_addr.s_addr >> 16) & 0xff, (a->sin_addr.s_addr >> 24) & 0xff, ntohs(a->sin_port));
	} else
		len += snprintf(buf + len, sizeof(buf) - len, "(no address)");
	/* len += snprintf(buf + len, sizeof(buf) - len, "\0"); */
	return buf;
}

void
print_addrs(int fd, char *add_ptr, size_t add_len)
{
	struct sockaddr_in *sin;

	if (verbose < 3)
		return;
	for (sin = (typeof(sin)) add_ptr; add_len >= sizeof(*sin); sin++, add_len -= sizeof(*sin)) {
		char buf[128];

		snprintf(buf, sizeof(buf), "%d.%d.%d.%d:%d", (sin->sin_addr.s_addr >> 0) & 0xff, (sin->sin_addr.s_addr >> 8) & 0xff, (sin->sin_addr.s_addr >> 16) & 0xff, (sin->sin_addr.s_addr >> 24) & 0xff, ntohs(sin->sin_port));
		if (fd == test_fd[0]) {
			fprintf(stdout, "%-20s|  |                               |                    \n", buf);
		}
		if (fd == test_fd[1]) {
			fprintf(stdout, "                    |  |                               |     %-15s\n", buf);
		}
		if (fd == test_fd[2]) {
			fprintf(stdout, "                    |  |                               |     %-15s\n", buf);
		}
	}
}
#endif

char *
status_string(struct t_opthdr *oh)
{
	switch (oh->status) {
	case 0:
		return (NULL);
	case T_SUCCESS:
		return ("T_SUCCESS");
	case T_FAILURE:
		return ("T_FAILURE");
	case T_PARTSUCCESS:
		return ("T_PARTSUCCESS");
	case T_READONLY:
		return ("T_READONLY");
	case T_NOTSUPPORT:
		return ("T_NOTSUPPORT");
	default:
	{
		static char buf[32];

		snprintf(buf, sizeof(buf), "(unknown status %ld)", (long) oh->status);
		return buf;
	}
	}
}

#ifndef T_ALLLEVELS
#define T_ALLLEVELS -1
#endif

char *
level_string(struct t_opthdr *oh)
{
	switch (oh->level) {
	case T_ALLLEVELS:
		return ("T_ALLLEVELS");
	case XTI_GENERIC:
		return ("XTI_GENERIC");
#if 0
	case T_INET_IP:
		return ("T_INET_IP");
	case T_INET_UDP:
		return ("T_INET_UDP");
	case T_INET_TCP:
		return ("T_INET_TCP");
	case T_INET_SCTP:
		return ("T_INET_SCTP");
#endif
	default:
	{
		static char buf[32];

		snprintf(buf, sizeof(buf), "(unknown level %ld)", (long) oh->level);
		return buf;
	}
	}
}

char *
name_string(struct t_opthdr *oh)
{
	if (oh->name == T_ALLOPT)
		return ("T_ALLOPT");
	switch (oh->level) {
	case XTI_GENERIC:
		switch (oh->name) {
		case XTI_DEBUG:
			return ("XTI_DEBUG");
		case XTI_LINGER:
			return ("XTI_LINGER");
		case XTI_RCVBUF:
			return ("XTI_RCVBUF");
		case XTI_RCVLOWAT:
			return ("XTI_RCVLOWAT");
		case XTI_SNDBUF:
			return ("XTI_SNDBUF");
		case XTI_SNDLOWAT:
			return ("XTI_SNDLOWAT");
		}
		break;
#if 0
	case T_INET_IP:
		switch (oh->name) {
		case T_IP_OPTIONS:
			return ("T_IP_OPTIONS");
		case T_IP_TOS:
			return ("T_IP_TOS");
		case T_IP_TTL:
			return ("T_IP_TTL");
		case T_IP_REUSEADDR:
			return ("T_IP_REUSEADDR");
		case T_IP_DONTROUTE:
			return ("T_IP_DONTROUTE");
		case T_IP_BROADCAST:
			return ("T_IP_BROADCAST");
		case T_IP_ADDR:
			return ("T_IP_ADDR");
		}
		break;
	case T_INET_UDP:
		switch (oh->name) {
		case T_UDP_CHECKSUM:
			return ("T_UDP_CHECKSUM");
		}
		break;
	case T_INET_TCP:
		switch (oh->name) {
		case T_TCP_NODELAY:
			return ("T_TCP_NODELAY");
		case T_TCP_MAXSEG:
			return ("T_TCP_MAXSEG");
		case T_TCP_KEEPALIVE:
			return ("T_TCP_KEEPALIVE");
		case T_TCP_CORK:
			return ("T_TCP_CORK");
		case T_TCP_KEEPIDLE:
			return ("T_TCP_KEEPIDLE");
		case T_TCP_KEEPINTVL:
			return ("T_TCP_KEEPINTVL");
		case T_TCP_KEEPCNT:
			return ("T_TCP_KEEPCNT");
		case T_TCP_SYNCNT:
			return ("T_TCP_SYNCNT");
		case T_TCP_LINGER2:
			return ("T_TCP_LINGER2");
		case T_TCP_DEFER_ACCEPT:
			return ("T_TCP_DEFER_ACCEPT");
		case T_TCP_WINDOW_CLAMP:
			return ("T_TCP_WINDOW_CLAMP");
		case T_TCP_INFO:
			return ("T_TCP_INFO");
		case T_TCP_QUICKACK:
			return ("T_TCP_QUICKACK");
		}
		break;
	case T_INET_SCTP:
		switch (oh->name) {
		case T_SCTP_NODELAY:
			return ("T_SCTP_NODELAY");
		case T_SCTP_CORK:
			return ("T_SCTP_CORK");
		case T_SCTP_PPI:
			return ("T_SCTP_PPI");
		case T_SCTP_SID:
			return ("T_SCTP_SID");
		case T_SCTP_SSN:
			return ("T_SCTP_SSN");
		case T_SCTP_TSN:
			return ("T_SCTP_TSN");
		case T_SCTP_RECVOPT:
			return ("T_SCTP_RECVOPT");
		case T_SCTP_COOKIE_LIFE:
			return ("T_SCTP_COOKIE_LIFE");
		case T_SCTP_SACK_DELAY:
			return ("T_SCTP_SACK_DELAY");
		case T_SCTP_PATH_MAX_RETRANS:
			return ("T_SCTP_PATH_MAX_RETRANS");
		case T_SCTP_ASSOC_MAX_RETRANS:
			return ("T_SCTP_ASSOC_MAX_RETRANS");
		case T_SCTP_MAX_INIT_RETRIES:
			return ("T_SCTP_MAX_INIT_RETRIES");
		case T_SCTP_HEARTBEAT_ITVL:
			return ("T_SCTP_HEARTBEAT_ITVL");
		case T_SCTP_RTO_INITIAL:
			return ("T_SCTP_RTO_INITIAL");
		case T_SCTP_RTO_MIN:
			return ("T_SCTP_RTO_MIN");
		case T_SCTP_RTO_MAX:
			return ("T_SCTP_RTO_MAX");
		case T_SCTP_OSTREAMS:
			return ("T_SCTP_OSTREAMS");
		case T_SCTP_ISTREAMS:
			return ("T_SCTP_ISTREAMS");
		case T_SCTP_COOKIE_INC:
			return ("T_SCTP_COOKIE_INC");
		case T_SCTP_THROTTLE_ITVL:
			return ("T_SCTP_THROTTLE_ITVL");
		case T_SCTP_MAC_TYPE:
			return ("T_SCTP_MAC_TYPE");
		case T_SCTP_CKSUM_TYPE:
			return ("T_SCTP_CKSUM_TYPE");
		case T_SCTP_ECN:
			return ("T_SCTP_ECN");
		case T_SCTP_ALI:
			return ("T_SCTP_ALI");
		case T_SCTP_ADD:
			return ("T_SCTP_ADD");
		case T_SCTP_SET:
			return ("T_SCTP_SET");
		case T_SCTP_ADD_IP:
			return ("T_SCTP_ADD_IP");
		case T_SCTP_DEL_IP:
			return ("T_SCTP_DEL_IP");
		case T_SCTP_SET_IP:
			return ("T_SCTP_SET_IP");
		case T_SCTP_PR:
			return ("T_SCTP_PR");
		case T_SCTP_LIFETIME:
			return ("T_SCTP_LIFETIME");
		case T_SCTP_DISPOSITION:
			return ("T_SCTP_DISPOSITION");
		case T_SCTP_MAX_BURST:
			return ("T_SCTP_MAX_BURST");
		case T_SCTP_HB:
			return ("T_SCTP_HB");
		case T_SCTP_RTO:
			return ("T_SCTP_RTO");
		case T_SCTP_MAXSEG:
			return ("T_SCTP_MAXSEG");
		case T_SCTP_STATUS:
			return ("T_SCTP_STATUS");
		case T_SCTP_DEBUG:
			return ("T_SCTP_DEBUG");
		}
		break;
#endif
	}
	{
		static char buf[32];

		snprintf(buf, sizeof(buf), "(unknown name %ld)", (long) oh->name);
		return buf;
	}
}

char *
yesno_string(struct t_opthdr *oh)
{
	switch (*((t_uscalar_t *) T_OPT_DATA(oh))) {
	case T_YES:
		return ("T_YES");
	case T_NO:
		return ("T_NO");
	default:
		return ("(invalid)");
	}
}

char *
number_string(struct t_opthdr *oh)
{
	static char buf[32];

	snprintf(buf, 32, "%d", *((t_scalar_t *) T_OPT_DATA(oh)));
	return (buf);
}

char *
value_string(int child, struct t_opthdr *oh)
{
#if 0
	static char buf[64] = "(invalid)";
#endif
	if (oh->len == sizeof(*oh))
		return (NULL);
	switch (oh->level) {
	case XTI_GENERIC:
		switch (oh->name) {
		case XTI_DEBUG:
			break;
		case XTI_LINGER:
			break;
		case XTI_RCVBUF:
			break;
		case XTI_RCVLOWAT:
			break;
		case XTI_SNDBUF:
			break;
		case XTI_SNDLOWAT:
			break;
		}
		break;
#if 0
	case T_INET_IP:
		switch (oh->name) {
		case T_IP_OPTIONS:
			break;
		case T_IP_TOS:
			if (oh->len == sizeof(*oh) + sizeof(unsigned char))
				snprintf(buf, sizeof(buf), "0x%02x", *((unsigned char *) T_OPT_DATA(oh)));
			return buf;
		case T_IP_TTL:
			if (oh->len == sizeof(*oh) + sizeof(unsigned char))
				snprintf(buf, sizeof(buf), "0x%02x", *((unsigned char *) T_OPT_DATA(oh)));
			return buf;
		case T_IP_REUSEADDR:
			return yesno_string(oh);
		case T_IP_DONTROUTE:
			return yesno_string(oh);
		case T_IP_BROADCAST:
			return yesno_string(oh);
		case T_IP_ADDR:
			if (oh->len == sizeof(*oh) + sizeof(uint32_t)) {
				uint32_t addr = *((uint32_t *) T_OPT_DATA(oh));

				snprintf(buf, sizeof(buf), "%d.%d.%d.%d", (addr >> 0) & 0x00ff, (addr >> 8) & 0x00ff, (addr >> 16) & 0x00ff, (addr >> 24) & 0x00ff);
			}
			return buf;
		}
		break;
	case T_INET_UDP:
		switch (oh->name) {
		case T_UDP_CHECKSUM:
			return yesno_string(oh);
		}
		break;
	case T_INET_TCP:
		switch (oh->name) {
		case T_TCP_NODELAY:
			return yesno_string(oh);
		case T_TCP_MAXSEG:
			if (oh->len == sizeof(*oh) + sizeof(t_uscalar_t))
				snprintf(buf, sizeof(buf), "%lu", (ulong) *((t_uscalar_t *) T_OPT_DATA(oh)));
			return buf;
		case T_TCP_KEEPALIVE:
			break;
		case T_TCP_CORK:
			return yesno_string(oh);
		case T_TCP_KEEPIDLE:
			break;
		case T_TCP_KEEPINTVL:
			break;
		case T_TCP_KEEPCNT:
			break;
		case T_TCP_SYNCNT:
			break;
		case T_TCP_LINGER2:
			break;
		case T_TCP_DEFER_ACCEPT:
			break;
		case T_TCP_WINDOW_CLAMP:
			break;
		case T_TCP_INFO:
			break;
		case T_TCP_QUICKACK:
			break;
		}
		break;
	case T_INET_SCTP:
		switch (oh->name) {
		case T_SCTP_NODELAY:
			return yesno_string(oh);
		case T_SCTP_CORK:
			return yesno_string(oh);
		case T_SCTP_PPI:
			return number_string(oh);;
		case T_SCTP_SID:
			/* sid[child] = *((t_uscalar_t *) T_OPT_DATA(oh)); */
			return number_string(oh);;
		case T_SCTP_SSN:
		case T_SCTP_TSN:
			return number_string(oh);;
		case T_SCTP_RECVOPT:
			return yesno_string(oh);
		case T_SCTP_COOKIE_LIFE:
		case T_SCTP_SACK_DELAY:
		case T_SCTP_PATH_MAX_RETRANS:
		case T_SCTP_ASSOC_MAX_RETRANS:
		case T_SCTP_MAX_INIT_RETRIES:
		case T_SCTP_HEARTBEAT_ITVL:
		case T_SCTP_RTO_INITIAL:
		case T_SCTP_RTO_MIN:
		case T_SCTP_RTO_MAX:
		case T_SCTP_OSTREAMS:
		case T_SCTP_ISTREAMS:
		case T_SCTP_COOKIE_INC:
		case T_SCTP_THROTTLE_ITVL:
			return number_string(oh);;
		case T_SCTP_MAC_TYPE:
			break;
		case T_SCTP_CKSUM_TYPE:
			break;
		case T_SCTP_ECN:
			break;
		case T_SCTP_ALI:
			break;
		case T_SCTP_ADD:
			break;
		case T_SCTP_SET:
			break;
		case T_SCTP_ADD_IP:
			break;
		case T_SCTP_DEL_IP:
			break;
		case T_SCTP_SET_IP:
			break;
		case T_SCTP_PR:
			break;
		case T_SCTP_LIFETIME:
		case T_SCTP_DISPOSITION:
		case T_SCTP_MAX_BURST:
		case T_SCTP_HB:
		case T_SCTP_RTO:
		case T_SCTP_MAXSEG:
			return number_string(oh);
		case T_SCTP_STATUS:
			break;
		case T_SCTP_DEBUG:
			break;
		}
		break;
#endif
	}
	return ("(unknown value)");
}

#if 0
void
parse_options(int fd, char *opt_ptr, size_t opt_len)
{
	struct t_opthdr *oh;

	for (oh = _T_OPT_FIRSTHDR_OFS(opt_ptr, opt_len, 0); oh; oh = _T_OPT_NEXTHDR_OFS(opt_ptr, opt_len, oh, 0)) {
		if (oh->len == sizeof(*oh))
			continue;
		switch (oh->level) {
		case T_INET_SCTP:
			switch (oh->name) {
			case T_SCTP_PPI:
				ppi[fd] = *((t_uscalar_t *) T_OPT_DATA(oh));
				continue;
			case T_SCTP_SID:
				sid[fd] = *((t_uscalar_t *) T_OPT_DATA(oh));
				continue;
			case T_SCTP_SSN:
				ssn[fd] = *((t_uscalar_t *) T_OPT_DATA(oh));
				continue;
			case T_SCTP_TSN:
				tsn[fd] = *((t_uscalar_t *) T_OPT_DATA(oh));
				continue;
			}
		}
	}
}
#endif

char *
mgmtflag_string(t_uscalar_t flag)
{
	switch (flag) {
	case T_NEGOTIATE:
		return ("T_NEGOTIATE");
	case T_CHECK:
		return ("T_CHECK");
	case T_DEFAULT:
		return ("T_DEFAULT");
	case T_SUCCESS:
		return ("T_SUCCESS");
	case T_FAILURE:
		return ("T_FAILURE");
	case T_CURRENT:
		return ("T_CURRENT");
	case T_PARTSUCCESS:
		return ("T_PARTSUCCESS");
	case T_READONLY:
		return ("T_READONLY");
	case T_NOTSUPPORT:
		return ("T_NOTSUPPORT");
	}
	return "(unknown flag)";
}

char *
size_string(ulong size)
{
	static char buf[128];

	switch (size) {
	case T_INFINITE:
		return ("T_INFINITE");
	case T_INVALID:
		return ("T_INVALID");
	case T_UNSPEC:
		return ("T_UNSPEC");
	}
	snprintf(buf, sizeof(buf), "%lu", size);
	return buf;
}

const char *
prim_string(ulong prim)
{
	switch (prim) {
	case T_CONN_REQ:
		return ("T_CONN_REQ------");
	case T_CONN_RES:
		return ("T_CONN_RES------");
	case T_DISCON_REQ:
		return ("T_DISCON_REQ----");
	case T_DATA_REQ:
		return ("T_DATA_REQ------");
	case T_EXDATA_REQ:
		return ("T_EXDATA_REQ----");
	case T_INFO_REQ:
		return ("T_INFO_REQ------");
	case T_BIND_REQ:
		return ("T_BIND_REQ------");
	case T_UNBIND_REQ:
		return ("T_UNBIND_REQ----");
	case T_UNITDATA_REQ:
		return ("T_UNITDATA_REQ--");
	case T_OPTMGMT_REQ:
		return ("T_OPTMGMT_REQ---");
	case T_ORDREL_REQ:
		return ("T_ORDREL_REQ----");
	case T_OPTDATA_REQ:
		return ("T_OPTDATA_REQ---");
	case T_ADDR_REQ:
		return ("T_ADDR_REQ------");
	case T_CAPABILITY_REQ:
		return ("T_CAPABILITY_REQ");
	case T_CONN_IND:
		return ("T_CONN_IND------");
	case T_CONN_CON:
		return ("T_CONN_CON------");
	case T_DISCON_IND:
		return ("T_DISCON_IND----");
	case T_DATA_IND:
		return ("T_DATA_IND------");
	case T_EXDATA_IND:
		return ("T_EXDATA_IND----");
	case T_INFO_ACK:
		return ("T_INFO_ACK------");
	case T_BIND_ACK:
		return ("T_BIND_ACK------");
	case T_ERROR_ACK:
		return ("T_ERROR_ACK-----");
	case T_OK_ACK:
		return ("T_OK_ACK--------");
	case T_UNITDATA_IND:
		return ("T_UNITDATA_IND--");
	case T_UDERROR_IND:
		return ("T_UDERROR_IND---");
	case T_OPTMGMT_ACK:
		return ("T_OPTMGMT_ACK---");
	case T_ORDREL_IND:
		return ("T_ORDREL_IND----");
	case T_OPTDATA_IND:
		return ("T_OPTDATA_IND---");
	case T_ADDR_ACK:
		return ("T_ADDR_ACK------");
	case T_CAPABILITY_ACK:
		return ("T_CAPABILITY_ACK");
	default:
		return ("T_????_??? -----");
	}
}

char *
t_errno_string(long err, long syserr)
{
	switch (err) {
	case 0:
		return ("ok");
	case TBADADDR:
		return ("[TBADADDR]");
	case TBADOPT:
		return ("[TBADOPT]");
	case TACCES:
		return ("[TACCES]");
	case TBADF:
		return ("[TBADF]");
	case TNOADDR:
		return ("[TNOADDR]");
	case TOUTSTATE:
		return ("[TOUTSTATE]");
	case TBADSEQ:
		return ("[TBADSEQ]");
	case TSYSERR:
		return errno_string(syserr);
	case TLOOK:
		return ("[TLOOK]");
	case TBADDATA:
		return ("[TBADDATA]");
	case TBUFOVFLW:
		return ("[TBUFOVFLW]");
	case TFLOW:
		return ("[TFLOW]");
	case TNODATA:
		return ("[TNODATA]");
	case TNODIS:
		return ("[TNODIS]");
	case TNOUDERR:
		return ("[TNOUDERR]");
	case TBADFLAG:
		return ("[TBADFLAG]");
	case TNOREL:
		return ("[TNOREL]");
	case TNOTSUPPORT:
		return ("[TNOTSUPPORT]");
	case TSTATECHNG:
		return ("[TSTATECHNG]");
	case TNOSTRUCTYPE:
		return ("[TNOSTRUCTYPE]");
	case TBADNAME:
		return ("[TBADNAME]");
	case TBADQLEN:
		return ("[TBADQLEN]");
	case TADDRBUSY:
		return ("[TADDRBUSY]");
	case TINDOUT:
		return ("[TINDOUT]");
	case TPROVMISMATCH:
		return ("[TPROVMISMATCH]");
	case TRESQLEN:
		return ("[TRESQLEN]");
	case TRESADDR:
		return ("[TRESADDR]");
	case TQFULL:
		return ("[TQFULL]");
	case TPROTO:
		return ("[TPROTO]");
	default:
	{
		static char buf[32];

		snprintf(buf, sizeof(buf), "[%ld]", err);
		return buf;
	}
	}
}

char *
t_look_string(int look)
{
	switch (look) {
	case 0:
		return ("(NO EVENT)");
	case T_LISTEN:
		return ("(T_LISTEN)");
	case T_CONNECT:
		return ("(T_CONNECT)");
	case T_DATA:
		return ("(T_DATA)");
	case T_EXDATA:
		return ("(T_EXDATA)");
	case T_DISCONNECT:
		return ("(T_DISCONNECT)");
	case T_UDERR:
		return ("(T_UDERR)");
	case T_ORDREL:
		return ("(T_ORDREL)");
	case T_GODATA:
		return ("(T_GODATA)");
	case T_GOEXDATA:
		return ("(T_GOEXDATA)");
	default:
	{
		static char buf[32];

		snprintf(buf, sizeof(buf), "(%d)", look);
		return buf;
	}
	}
}

void
print_less(int child)
{
	if (verbose < 1 || !show)
		return;
	dummy = lockf(fileno(stdout), F_LOCK, 0);
	switch (child) {
	case 0:
		fprintf(stdout, " .         .  <---->|               .               :  :                    \n");
		fprintf(stdout, " .  (more) .  <---->|               .               :  :                     [%d:%03d]\n", child, state);
		fprintf(stdout, " .         .  <---->|               .               :  :                    \n");
		break;
	case 1:
		fprintf(stdout, "                    :               .               :  |<-->  .         .   \n");
		fprintf(stdout, "                    :               .               :  |<-->  . (more)  .    [%d:%03d]\n", child, state);
		fprintf(stdout, "                    :               .               :  |<-->  .         .   \n");
		break;
	case 2:
		fprintf(stdout, "                    :               .               |<-:--->  .         .   \n");
		fprintf(stdout, "                    :               .               |<-:--->  . (more)  .    [%d:%03d]\n", child, state);
		fprintf(stdout, "                    :               .               |<-:--->  .         .   \n");
		break;
	}
	fflush(stdout);
	dummy = lockf(fileno(stdout), F_ULOCK, 0);
	show = 0;
	return;
}

void
print_more(void)
{
	show = 1;
}

void
print_simple(int child, const char *msgs[])
{
	dummy = lockf(fileno(stdout), F_LOCK, 0);
	fprintf(stdout, msgs[child]);
	fflush(stdout);
	dummy = lockf(fileno(stdout), F_ULOCK, 0);
}

void
print_simple_int(int child, const char *msgs[], int val)
{
	dummy = lockf(fileno(stdout), F_LOCK, 0);
	fprintf(stdout, msgs[child], val);
	fflush(stdout);
	dummy = lockf(fileno(stdout), F_ULOCK, 0);
}

void
print_double_int(int child, const char *msgs[], int val, int val2)
{
	dummy = lockf(fileno(stdout), F_LOCK, 0);
	fprintf(stdout, msgs[child], val, val2);
	fflush(stdout);
	dummy = lockf(fileno(stdout), F_ULOCK, 0);
}

void
print_triple_int(int child, const char *msgs[], int val, int val2, int val3)
{
	dummy = lockf(fileno(stdout), F_LOCK, 0);
	fprintf(stdout, msgs[child], val, val2, val3);
	fflush(stdout);
	dummy = lockf(fileno(stdout), F_ULOCK, 0);
}

void
print_simple_string(int child, const char *msgs[], const char *string)
{
	dummy = lockf(fileno(stdout), F_LOCK, 0);
	fprintf(stdout, msgs[child], string);
	fflush(stdout);
	dummy = lockf(fileno(stdout), F_ULOCK, 0);
}

void
print_pipe(int child)
{
	static const char *msgs[] = {
		"  pipe()      ----->v  v<------------------------------>v                   \n",
		"                    v  v<------------------------------>v<-----     pipe()  \n",
		"                    .  .                                .                   \n",
	};

	if (verbose > 3)
		print_simple(child, msgs);
}

void
print_open(int child)
{
	static const char *msgs[] = {
		"  open()      ----->v  .                                .                   \n",
		"                    |  v                                v<-----     open()  \n",
		"                    |  v<-------------------------------.------     open()  \n",
		"                    .  .                                .                   \n",
	};

	if (verbose > 3)
		print_simple(child, msgs);
}

void
print_close(int child)
{
	static const char *msgs[] = {
		"  close()     ----->X  |                                |                   \n",
		"                    .  X                                X<-----    close()  \n",
		"                    .  X<-------------------------------.------    close()  \n",
		"                    .  .                                .                   \n",
	};

	if (verbose > 3)
		print_simple(child, msgs);
}

void
print_preamble(int child)
{
	static const char *msgs[] = {
		"--------------------+  +----------Preamble--------------+                   \n",
		"                    +  +----------Preamble--------------+-------------------\n",
		"                    +--+----------Preamble--------------+                   \n",
		"--------------------+--+----------Preamble--------------+-------------------\n",
	};

	if (verbose > 0)
		print_simple(child, msgs);
}

void
print_notapplicable(int child)
{
	static const char *msgs[] = {
		"X-X-X-X-X-X-X-X-X-X-|  |-X-X-X NOT APPLICABLE -X-X-X-X-X|                    [%d:%03d]\n",
		"                    |  |-X-X-X NOT APPLICABLE -X-X-X-X-X|X-X-X-X-X-X-X-X-X-X [%d:%03d]\n",
		"                    |X-|-X-X-X NOT APPLICABLE -X-X-X-X-X|                    [%d:%03d]\n",
		"X-X-X-X-X-X-X-X-X-X-|X-|-X-X-X NOT APPLICABLE -X-X-X-X-X|X-X-X-X-X-X-X-X-X-X [%d:%03d]\n",
	};

	if (verbose > 0)
		print_double_int(child, msgs, child, state);
}

void
print_skipped(int child)
{
	static const char *msgs[] = {
		"::::::::::::::::::::|  |::::::::: SKIPPED ::::::::::::::|                    [%d:%03d]\n",
		"                    |  |::::::::: SKIPPED ::::::::::::::|::::::::::::::::::: [%d:%03d]\n",
		"                    |::|::::::::: SKIPPED ::::::::::::::|                    [%d:%03d]\n",
		"::::::::::::::::::::|::|::::::::: SKIPPED ::::::::::::::|::::::::::::::::::: [%d:%03d]\n",
	};

	if (verbose > 0)
		print_double_int(child, msgs, child, state);
}

void
print_inconclusive(int child)
{
	static const char *msgs[] = {
		"????????????????????|  |??????? INCONCLUSIVE ???????????|                    [%d:%03d]\n",
		"                    |  |??????? INCONCLUSIVE ???????????|??????????????????? [%d:%03d]\n",
		"                    |??|??????? INCONCLUSIVE ???????????|                    [%d:%03d]\n",
		"????????????????????|??|??????? INCONCLUSIVE ???????????|??????????????????? [%d:%03d]\n",
	};

	if (verbose > 0)
		print_double_int(child, msgs, child, state);
}

void
print_test(int child)
{
	static const char *msgs[] = {
		"--------------------+  +------------Test----------------+                   \n",
		"                    +  +------------Test----------------+-------------------\n",
		"                    +--+------------Test----------------+                   \n",
		"--------------------+--+------------Test----------------+-------------------\n",
	};

	if (verbose > 0)
		print_simple(child, msgs);
}

void
print_failed(int child)
{
	static const char *msgs[] = {
		"XXXXXXXXXXXXXXXXXXXX|  |XXXXXXXXXX FAILED XXXXXXXXXXXXXX|                    [%d:%03d]\n",
		"                    |  |XXXXXXXXXX FAILED XXXXXXXXXXXXXX|XXXXXXXXXXXXXXXXXXX [%d:%03d]\n",
		"                    |XX|XXXXXXXXXX FAILED XXXXXXXXXXXXXX|                    [%d:%03d]\n",
		"XXXXXXXXXXXXXXXXXXXX|XX|XXXXXXXXXX FAILED XXXXXXXXXXXXXX|XXXXXXXXXXXXXXXXXXX [%d:%03d]\n",
	};

	if (verbose > 0)
		print_double_int(child, msgs, child, state);
}

void
print_script_error(int child)
{
	static const char *msgs[] = {
		"####################|  |######## SCRIPT ERROR ##########|                    [%d:%03d]\n",
		"                    |  |######## SCRIPT ERROR ##########|################### [%d:%03d]\n",
		"                    |##|######## SCRIPT ERROR ##########|                    [%d:%03d]\n",
		"####################|##|######## SCRIPT ERROR ##########|################### [%d:%03d]\n",
	};

	if (verbose > 0)
		print_double_int(child, msgs, child, state);
}

void
print_passed(int child)
{
	static const char *msgs[] = {
		"********************|  |********** PASSED **************|                    [%d:%03d]\n",
		"                    |  |********** PASSED **************|******************* [%d:%03d]\n",
		"                    |**|********** PASSED **************|                    [%d:%03d]\n",
		"********************|**|********** PASSED **************|******************* [%d:%03d]\n",
	};

	if (verbose > 2)
		print_double_int(child, msgs, child, state);
}

void
print_postamble(int child)
{
	static const char *msgs[] = {
		"--------------------+  +----------Postamble-------------+                   \n",
		"                    +  +----------Postamble-------------+-------------------\n",
		"                    +--+----------Postamble-------------+                   \n",
		"--------------------+--+----------Postamble-------------+-------------------\n",
	};

	if (verbose > 0)
		print_simple(child, msgs);
}

void
print_test_end(int child)
{
	static const char *msgs[] = {
		"--------------------+  +--------------------------------+                   \n",
		"                    +  +--------------------------------+-------------------\n",
		"                    +--+--------------------------------+                   \n",
		"--------------------+--+--------------------------------+-------------------\n",
	};

	if (verbose > 0)
		print_simple(child, msgs);
}

void
print_terminated(int child, int signal)
{
	static const char *msgs[] = {
		"@@@@@@@@@@@@@@@@@@@@|  |@@@@@@@@ TERMINATED @@@@@@@@@@@@|                    {%d:%03d}\n",
		"                    |  |@@@@@@@@ TERMINATED @@@@@@@@@@@@|@@@@@@@@@@@@@@@@@@@ {%d:%03d}\n",
		"                    |@@|@@@@@@@@ TERMINATED @@@@@@@@@@@@|                    {%d:%03d}\n",
		"@@@@@@@@@@@@@@@@@@@@|@@|@@@@@@@@ TERMINATED @@@@@@@@@@@@|@@@@@@@@@@@@@@@@@@@ {%d:%03d}\n",
	};

	if (verbose > 0)
		print_double_int(child, msgs, child, signal);
}

void
print_stopped(int child, int signal)
{
	static const char *msgs[] = {
		"&&&&&&&&&&&&&&&&&&&&|  |&&&&&&&&& STOPPED &&&&&&&&&&&&&&|                    {%d:%03d}\n",
		"                    |  |&&&&&&&&& STOPPED &&&&&&&&&&&&&&|&&&&&&&&&&&&&&&&&&& {%d:%03d}\n",
		"                    |&&|&&&&&&&&& STOPPED &&&&&&&&&&&&&&|                    {%d:%03d}\n",
		"&&&&&&&&&&&&&&&&&&&&|&&|&&&&&&&&& STOPPED &&&&&&&&&&&&&&|&&&&&&&&&&&&&&&&&&& {%d:%03d}\n",
	};

	if (verbose > 0)
		print_double_int(child, msgs, child, signal);
}

void
print_timeout(int child)
{
	static const char *msgs[] = {
		"++++++++++++++++++++|  |+++++++++ TIMEOUT! +++++++++++++|                    [%d:%03d]\n",
		"                    |  |+++++++++ TIMEOUT! +++++++++++++|+++++++++++++++++++ [%d:%03d]\n",
		"                    |++|+++++++++ TIMEOUT! +++++++++++++|                    [%d:%03d]\n",
		"++++++++++++++++++++|++|+++++++++ TIMEOUT! +++++++++++++|+++++++++++++++++++ [%d:%03d]\n",
	};

	if (show_timeout || verbose > 0) {
		print_double_int(child, msgs, child, state);
		show_timeout--;
	}
}

void
print_nothing(int child)
{
	static const char *msgs[] = {
		"- - - - - - - - - - |  |- - - - - nothing! - - - - - - -|                    [%d:%03d]\n",
		"                    |  |- - - - - nothing! - - - - - - -|- - - - - - - - - - [%d:%03d]\n",
		"                    |- |- - - - - nothing! - - - - - - -|                    [%d:%03d]\n",
		"- - - - - - - - - - |- |- - - - - nothing! - - - - - - -|- - - - - - - - - - [%d:%03d]\n",
	};

	if (verbose > 1)
		print_double_int(child, msgs, child, state);
}

void
print_string_state(int child, const char *msgs[], const char *string)
{
	dummy = lockf(fileno(stdout), F_LOCK, 0);
	fprintf(stdout, msgs[child], string, child, state);
	fflush(stdout);
	dummy = lockf(fileno(stdout), F_ULOCK, 0);
}

void
print_syscall(int child, const char *command)
{
	static const char *msgs[] = {
		"  %-14s--->|  |                                |                    [%d:%03d]\n",
		"                    |  |                                |<---%14s  [%d:%03d]\n",
		"                    |  |                                |<---%14s  [%d:%03d]\n",
		"                    |  |                                |                    [%d:%03d]\n",
	};

	if (verbose > 0)
		print_string_state(child, msgs, command);
}

void
print_tx_prim(int child, const char *command)
{
	static const char *msgs[] = {
		"--%16s->|  |                                |                    [%d:%03d]\n",
		"                    |  |<- - - - - - - - - - - - - - - -|<-%16s- [%d:%03d]\n",
		"                    |  |<- - - - - - - - - - - - - - - -|<-%16s- [%d:%03d]\n",
		"                    |  |                                |                    [%d:%03d]\n",
	};

	if (verbose > 0)
		print_string_state(child, msgs, command);
}

void
print_rx_prim(int child, const char *command)
{
	static const char *msgs[] = {
		"<-%16s--|  |                                |                    [%d:%03d]\n",
		"                    |  |- - - - - - - - - - - - - - - ->|-%16s-> [%d:%03d]\n",
		"                    |  |- - - - - - - - - - - - - - - ->|-%16s-> [%d:%03d]\n",
		"                    |  |      <%16s>        |                    [%d:%03d]\n",
	};

	if (verbose > 0)
		print_string_state(child, msgs, command);
}

void
print_ack_prim(int child, const char *command)
{
	static const char *msgs[] = {
		"<-%16s-/|  |                                |                    [%d:%03d]\n",
		"                    |  |- - - - - - - - - - - - - - - ->|\\%16s-> [%d:%03d]\n",
		"                    |  |- - - - - - - - - - - - - - - ->|\\%16s-> [%d:%03d]\n",
		"                    |  |      <%16s>        |                    [%d:%03d]\n",
	};

	if (verbose > 0)
		print_string_state(child, msgs, command);
}

void
print_long_state(int child, const char *msgs[], long value)
{
	dummy = lockf(fileno(stdout), F_LOCK, 0);
	fprintf(stdout, msgs[child], value, child, state);
	fflush(stdout);
	dummy = lockf(fileno(stdout), F_ULOCK, 0);
}

void
print_no_prim(int child, long prim)
{
	static const char *msgs[] = {
		"????%4ld????  ?----?|  | ?- - - - - - - - - - - - - -? |                     [%d:%03d]\n",
		"                    |  | ?- - - - - - - - - - - - - -? |?--? ????%4ld????    [%d:%03d]\n",
		"                    |  | ?- - - - - - - - - - - - - -? |?--? ????%4ld????    [%d:%03d]\n",
		"                    |  | ?- - - - - %4ld  - - - - - -? |                     [%d:%03d]\n",
	};

	if (verbose > 0)
		print_long_state(child, msgs, prim);
}

void
print_string_int_state(int child, const char *msgs[], const char *string, int val)
{
	dummy = lockf(fileno(stdout), F_LOCK, 0);
	fprintf(stdout, msgs[child], string, val, child, state);
	fflush(stdout);
	dummy = lockf(fileno(stdout), F_ULOCK, 0);
}

void
print_rx_data(int child, const char *command, size_t bytes)
{
	static const char *msgs[] = {
		"<-%1$16s--|  |  %2$4d bytes                    |                    [%3$d:%4$03d]\n",
		"                    |  |- %2$4d bytes - - - - - - - - - >|--%1$16s> [%3$d:%4$03d]\n",
		"                    |  |- %2$4d bytes - - - - - - - - - >|--%1$16s> [%3$d:%4$03d]\n",
		"                    |  |  %2$4d bytes  %1$16s  |                    [%3$d:%4$03d]\n",
	};

	if ((verbose && show_data) || verbose > 1)
		print_string_int_state(child, msgs, command, bytes);
}

void
print_errno(int child, long error)
{
	static const char *msgs[] = {
		"  %-14s<--/|  |                                |                    [%d:%03d]\n",
		"                    |  |                                |\\-->%14s  [%d:%03d]\n",
		"                    |  |                                |\\-->%14s  [%d:%03d]\n",
		"                    |  |       [%14s]         |                    [%d:%03d]\n",
	};

	if (verbose > 3)
		print_string_state(child, msgs, errno_string(error));
}

void
print_success(int child)
{
	static const char *msgs[] = {
		"  ok          <----/|  |                                |                    [%d:%03d]\n",
		"                    |  |                                |\\---->         ok   [%d:%03d]\n",
		"                    |  |                                |\\---->         ok   [%d:%03d]\n",
		"                    |  |              ok                |                    [%d:%03d]\n",
	};

	if (verbose > 4)
		print_double_int(child, msgs, child, state);
}

void
print_success_value(int child, int value)
{
	static const char *msgs[] = {
		"  %10d  <----/|  |                                |                    [%d:%03d]\n",
		"                    |  |                                |\\---->  %10d  [%d:%03d]\n",
		"                    |  |                                |\\---->  %10d  [%d:%03d]\n",
		"                    |  |         [%10d]           |                    [%d:%03d]\n",
	};

	if (verbose)
		print_triple_int(child, msgs, value, child, state);
}

void
print_ti_ioctl(int child, int cmd, intptr_t arg)
{
	static const char *msgs[] = {
		"--ioctl(2)--------->|  |    %16s            |                    [%d:%03d]\n",
		"                    |  |    %16s            |<---ioctl(2)------  [%d:%03d]\n",
		"                    |  |    %16s            |<---ioctl(2)------  [%d:%03d]\n",
		"                    |  |    %16s ioctl(2)   |                    [%d:%03d]\n",
	};

	if (verbose > 0)
		print_string_state(child, msgs, ioctl_string(cmd, arg));
}

void
print_ioctl(int child, int cmd, intptr_t arg)
{
	if (verbose > 3)
		print_ti_ioctl(child, cmd, arg);
}

void
print_datcall(int child, const char *command, size_t bytes)
{
	static const char *msgs[] = {
		"  %1$16s->|- | %2$4d bytes- - - - - - - - - - >|                    [%3$d:%4$03d]\n",
		"                    |< + %2$4d bytes- - - - - - - - - - -|<-%1$16s  [%3$d:%4$03d]\n",
		"                    |< + %2$4d bytes- - - - - - - - - - -|<-%1$16s  [%3$d:%4$03d]\n",
		"                    |< + %2$4d bytes %1$16s |  |                    [%3$d:%4$03d]\n",
	};

	if ((verbose && show_data) || verbose > 1)
		print_string_int_state(child, msgs, command, bytes);
}

void
print_libcall(int child, const char *command)
{
	static const char *msgs[] = {
		"  %-16s->|  |                                |                    [%d:%03d]\n",
		"                    |  |                                |<-%16s  [%d:%03d]\n",
		"                    |  |                                |<-%16s  [%d:%03d]\n",
		"                    |  |     [%16s]         |                    [%d:%03d]\n",
	};

	if (verbose > 0)
		print_string_state(child, msgs, command);
}

void
print_terror(int child, long error, long terror)
{
	static const char *msgs[] = {
		"  %-14s<--/|  |                                |                    [%d:%03d]\n",
		"                    |  |                                |\\-->%14s  [%d:%03d]\n",
		"                    |  |                                |\\-->%14s  [%d:%03d]\n",
		"                    |  |       [%14s]         |                    [%d:%03d]\n",
	};

	if (verbose > 0)
		print_string_state(child, msgs, t_errno_string(terror, error));
}

void
print_tlook(int child, int tlook)
{
	static const char *msgs[] = {
		"  %-14s<--/|  |                                |                    [%d:%03d]\n",
		"                    |  |                                |\\-->%14s  [%d:%03d]\n",
		"                    |  |                                |\\-->%14s  [%d:%03d]\n",
		"                    |  |       [%14s]         |                    [%d:%03d]\n",
	};

	if (verbose > 0)
		print_string_state(child, msgs, t_look_string(tlook));
}

void
print_expect(int child, int want)
{
	static const char *msgs[] = {
		" (%-16s) |  | - - - -[Expected]- - - - - - - |                     [%d:%03d]\n",
		"                    |  | - - - -[Expected]- - - - - - - | (%-16s)  [%d:%03d]\n",
		"                    |  | - - - -[Expected]- - - - - - - | (%-16s)  [%d:%03d]\n",
		"                    |- |- [Expected %-16s ] -|                     [%d:%03d]\n",
	};

	if (verbose > 1 && show)
		print_string_state(child, msgs, event_string(want));
}

void
print_string(int child, const char *string)
{
	static const char *msgs[] = {
		"%-20s|  |                                |                    \n",
		"                    |  |                                |%-20s\n",
		"                    |  |                                |%-20s\n",
		"                    |  |    %-20s        |                    \n",
	};

	if (verbose > 1 && show)
		print_simple_string(child, msgs, string);
}

void
print_time_state(int child, const char *msgs[], ulong time)
{
	dummy = lockf(fileno(stdout), F_LOCK, 0);
	fprintf(stdout, msgs[child], time, child, state);
	fflush(stdout);
	dummy = lockf(fileno(stdout), F_ULOCK, 0);
}

void
print_waiting(int child, ulong time)
{
	static const char *msgs[] = {
		"/ / / / / / / / / / | /|/ / Waiting %03lu seconds / / / / |                    [%d:%03d]\n",
		"                    |  |/ / Waiting %03lu seconds / / / / | / / / / / / / / /  [%d:%03d]\n",
		"                    | /|/ / Waiting %03lu seconds / / / / |                    [%d:%03d]\n",
		"/ / / / / / / / / / | /|/ / Waiting %03lu seconds / / / / | / / / / / / / / /  [%d:%03d]\n",
	};

	if (verbose > 0 && show)
		print_time_state(child, msgs, time);
}

void
print_float_state(int child, const char *msgs[], float time)
{
	dummy = lockf(fileno(stdout), F_LOCK, 0);
	fprintf(stdout, msgs[child], time, child, state);
	fflush(stdout);
	dummy = lockf(fileno(stdout), F_ULOCK, 0);
}

void
print_mwaiting(int child, struct timespec *time)
{
	static const char *msgs[] = {
		"/ / / / / / / / / / |  |/ Waiting %8.4f seconds / / /|                    [%d:%03d]\n",
		"                    |  |/ Waiting %8.4f seconds / / /| / / / / / / / / /  [%d:%03d]\n",
		"                    | /|/ Waiting %8.4f seconds / / /|                    [%d:%03d]\n",
		"/ / / / / / / / / / | /|/ Waiting %8.4f seconds / / /| / / / / / / / / /  [%d:%03d]\n",
	};

	if (verbose > 0 && show) {
		float delay;

		delay = time->tv_nsec;
		delay = delay / 1000000000;
		delay = delay + time->tv_sec;
		print_float_state(child, msgs, delay);
	}
}

void
print_mgmtflag(int child, t_uscalar_t flag)
{
	print_string(child, mgmtflag_string(flag));
}

void
print_opt_level(int child, struct t_opthdr *oh)
{
	char *level = level_string(oh);

	if (level)
		print_string(child, level);
}

void
print_opt_name(int child, struct t_opthdr *oh)
{
	char *name = name_string(oh);

	if (name)
		print_string(child, name);
}

void
print_opt_status(int child, struct t_opthdr *oh)
{
	char *status = status_string(oh);

	if (status)
		print_string(child, status);
}

void
print_opt_length(int child, struct t_opthdr *oh)
{
	int len = oh->len - sizeof(*oh);

	if (len) {
		char buf[32];

		snprintf(buf, sizeof(buf), "(len=%d)", len);
		print_string(child, buf);
	}
}
void
print_opt_value(int child, struct t_opthdr *oh)
{
	char *value = value_string(child, oh);

	if (value)
		print_string(child, value);
}

void
print_options(int child, const char *cmd_buf, size_t opt_ofs, size_t opt_len)
{
	struct t_opthdr *oh;
	const char *opt_ptr = cmd_buf + opt_ofs;
	char buf[64];

	if (verbose < 4)
		return;
	snprintf(buf, sizeof(buf), "opt len = %lu", (ulong) opt_len);
	print_string(child, buf);
	snprintf(buf, sizeof(buf), "opt ofs = %lu", (ulong) opt_ofs);
	print_string(child, buf);
	oh = _T_OPT_FIRSTHDR_OFS(opt_ptr, opt_len, 0);
	if (oh) {
		for (; oh; oh = _T_OPT_NEXTHDR_OFS(opt_ptr, opt_len, oh, 0)) {
			int len = oh->len - sizeof(*oh);

			print_opt_level(child, oh);
			print_opt_name(child, oh);
			print_opt_status(child, oh);
			print_opt_length(child, oh);
			if (len < 0)
				break;
			print_opt_value(child, oh);
		}
	} else {
		oh = (typeof(oh)) opt_ptr;
		print_opt_level(child, oh);
		print_opt_name(child, oh);
		print_opt_status(child, oh);
		print_opt_length(child, oh);
	}
}

void
print_info(int child, struct T_info_ack *info)
{
	char buf[64];

	if (verbose < 4)
		return;
	snprintf(buf, sizeof(buf), "TSDU  = %ld", (long) info->TSDU_size);
	print_string(child, buf);
	snprintf(buf, sizeof(buf), "ETSDU = %ld", (long) info->ETSDU_size);
	print_string(child, buf);
	snprintf(buf, sizeof(buf), "CDATA = %ld", (long) info->CDATA_size);
	print_string(child, buf);
	snprintf(buf, sizeof(buf), "DDATA = %ld", (long) info->DDATA_size);
	print_string(child, buf);
	snprintf(buf, sizeof(buf), "ADDR  = %ld", (long) info->ADDR_size);
	print_string(child, buf);
	snprintf(buf, sizeof(buf), "OPT   = %ld", (long) info->OPT_size);
	print_string(child, buf);
	snprintf(buf, sizeof(buf), "TIDU  = %ld", (long) info->TIDU_size);
	print_string(child, buf);
	snprintf(buf, sizeof(buf), "<%s>", service_type(info->SERV_type));
	print_string(child, buf);
	snprintf(buf, sizeof(buf), "<%s>", state_string(info->CURRENT_state));
	print_string(child, buf);
	snprintf(buf, sizeof(buf), "PROV  = %ld", (long) info->PROVIDER_flag);
	print_string(child, buf);
}

/*
 *  -------------------------------------------------------------------------
 *
 *  Driver actions.
 *
 *  -------------------------------------------------------------------------
 */
int
test_ioctl(int child, int cmd, intptr_t arg)
{
	print_ioctl(child, cmd, arg);
	for (;;) {
		if ((last_retval = ioctl(test_fd[child], cmd, arg)) == -1) {
			print_errno(child, (last_errno = errno));
			if (last_errno == EINTR || last_errno == ERESTART)
				continue;
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	}
}

int
test_insertfd(int child, int resfd, int offset, struct strbuf *ctrl, struct strbuf *data, int flags)
{
	struct strfdinsert fdi;

	if (ctrl) {
		fdi.ctlbuf.maxlen = ctrl->maxlen;
		fdi.ctlbuf.len = ctrl->len;
		fdi.ctlbuf.buf = ctrl->buf;
	} else {
		fdi.ctlbuf.maxlen = -1;
		fdi.ctlbuf.len = 0;
		fdi.ctlbuf.buf = NULL;
	}
	if (data) {
		fdi.databuf.maxlen = data->maxlen;
		fdi.databuf.len = data->len;
		fdi.databuf.buf = data->buf;
	} else {
		fdi.databuf.maxlen = -1;
		fdi.databuf.len = 0;
		fdi.databuf.buf = NULL;
	}
	fdi.flags = flags;
	fdi.fildes = resfd;
	fdi.offset = offset;
	if (test_ioctl(child, I_FDINSERT, (intptr_t) & fdi) != __RESULT_SUCCESS)
		return __RESULT_FAILURE;
	return __RESULT_SUCCESS;
}

int
test_putpmsg(int child, struct strbuf *ctrl, struct strbuf *data, int band, int flags)
{
	if (flags & MSG_BAND || band) {
		if (verbose > 3) {
			dummy = lockf(fileno(stdout), F_LOCK, 0);
			fprintf(stdout, "putpmsg to %d: [%d,%d]\n", child, ctrl ? ctrl->len : -1, data ? data->len : -1);
			dummy = lockf(fileno(stdout), F_ULOCK, 0);
			fflush(stdout);
		}
		if (ctrl == NULL || data != NULL)
			print_datcall(child, "putpmsg(2)----", data ? data->len : 0);
		for (;;) {
			if ((last_retval = putpmsg(test_fd[child], ctrl, data, band, flags)) == -1) {
				print_errno(child, (last_errno = errno));
				if (last_errno == EAGAIN || last_errno == EINTR || last_errno == ERESTART)
					continue;
				return (__RESULT_FAILURE);
			}
			if (verbose > 3)
				print_success_value(child, last_retval);
			return (__RESULT_SUCCESS);
		}
	} else {
		if (verbose > 3) {
			dummy = lockf(fileno(stdout), F_LOCK, 0);
			fprintf(stdout, "putmsg to %d: [%d,%d]\n", child, ctrl ? ctrl->len : -1, data ? data->len : -1);
			dummy = lockf(fileno(stdout), F_ULOCK, 0);
			fflush(stdout);
		}
		if (ctrl == NULL || data != NULL)
			print_datcall(child, "putmsg(2)-----", data ? data->len : 0);
		for (;;) {
			if ((last_retval = putmsg(test_fd[child], ctrl, data, flags)) == -1) {
				print_errno(child, (last_errno = errno));
				if (last_errno == EAGAIN || last_errno == EINTR || last_errno == ERESTART)
					continue;
				return (__RESULT_FAILURE);
			}
			if (verbose > 3)
				print_success_value(child, last_retval);
			return (__RESULT_SUCCESS);
		}
	}
}

int
test_write(int child, const void *buf, size_t len)
{
	print_syscall(child, "write(2)------");
	for (;;) {
		if ((last_retval = write(test_fd[child], buf, len)) == -1) {
			print_errno(child, (last_errno = errno));
			if (last_errno == EAGAIN || last_errno == EINTR || last_errno == ERESTART)
				continue;
			return (__RESULT_FAILURE);
		}
		print_success_value(child, last_retval);
		break;
	}
	return (__RESULT_SUCCESS);
}

int
test_writev(int child, const struct iovec *iov, int num)
{
	print_syscall(child, "writev(2)-----");
	for (;;) {
		if ((last_retval = writev(test_fd[child], iov, num)) == -1) {
			print_errno(child, (last_errno = errno));
			if (last_errno == EAGAIN || last_errno == EINTR || last_errno == ERESTART)
				continue;
			return (__RESULT_FAILURE);
		}
		print_success_value(child, last_retval);
		break;
	}
	return (__RESULT_SUCCESS);
}

int
test_getmsg(int child, struct strbuf *ctrl, struct strbuf *data, int *flagp)
{
	print_syscall(child, "getmsg(2)-----");
	for (;;) {
		if ((last_retval = getmsg(test_fd[child], ctrl, data, flagp)) == -1) {
			print_errno(child, (last_errno = errno));
			return (__RESULT_FAILURE);
		}
		print_success_value(child, last_retval);
		break;
	}
	return (__RESULT_SUCCESS);
}

int
test_getpmsg(int child, struct strbuf *ctrl, struct strbuf *data, int *bandp, int *flagp)
{
	print_syscall(child, "getpmsg(2)----");
	for (;;) {
		if ((last_retval = getpmsg(test_fd[child], ctrl, data, bandp, flagp)) == -1) {
			print_errno(child, (last_errno = errno));
			return (__RESULT_FAILURE);
		}
		print_success_value(child, last_retval);
		break;
	}
	return (__RESULT_SUCCESS);
}

int
test_read(int child, void *buf, size_t count)
{
	print_syscall(child, "read(2)-------");
	for (;;) {
		if ((last_retval = read(test_fd[child], buf, count)) == -1) {
			print_errno(child, (last_errno = errno));
			return (__RESULT_FAILURE);
		}
		print_success_value(child, last_retval);
		break;
	}
	return (__RESULT_SUCCESS);
}

int
test_readv(int child, const struct iovec *iov, int count)
{
	print_syscall(child, "readv(2)------");
	for (;;) {
		if ((last_retval = readv(test_fd[child], iov, count)) == -1) {
			print_errno(child, (last_errno = errno));
			return (__RESULT_FAILURE);
		}
		print_success_value(child, last_retval);
		break;
	}
	return (__RESULT_SUCCESS);
}

int
test_ti_ioctl(int child, int cmd, intptr_t arg)
{
	int tpi_error;

	if (cmd == I_STR && verbose > 3) {
		struct strioctl *icp = (struct strioctl *) arg;

		dummy = lockf(fileno(stdout), F_LOCK, 0);
		fprintf(stdout, "ioctl from %d: cmd=%d, timout=%d, len=%d, dp=%p\n", child, icp->ic_cmd, icp->ic_timout, icp->ic_len, icp->ic_dp);
		fflush(stdout);
		dummy = lockf(fileno(stdout), F_ULOCK, 0);
	}
	print_ti_ioctl(child, cmd, arg);
	for (;;) {
		if ((last_retval = ioctl(test_fd[child], cmd, arg)) == -1) {
			print_errno(child, (last_errno = errno));
			if (last_errno == EINTR || last_errno == ERESTART)
				continue;
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		break;
	}
	if (cmd == I_STR && verbose > 3) {
		struct strioctl *icp = (struct strioctl *) arg;

		dummy = lockf(fileno(stdout), F_LOCK, 0);
		fprintf(stdout, "got ioctl from %d: cmd=%d, timout=%d, len=%d, dp=%p\n", child, icp->ic_cmd, icp->ic_timout, icp->ic_len, icp->ic_dp);
		fflush(stdout);
		dummy = lockf(fileno(stdout), F_ULOCK, 0);
	}
	if (last_retval == 0)
		return __RESULT_SUCCESS;
	tpi_error = last_retval & 0x00ff;
	if (tpi_error == TSYSERR)
		last_errno = (last_retval >> 8) & 0x00ff;
	else
		last_errno = 0;
	if (verbose) {
		dummy = lockf(fileno(stdout), F_LOCK, 0);
		fprintf(stdout, "***************ERROR: ioctl failed\n");
		if (verbose > 3)
			fprintf(stdout, "                    : %s; result = %d\n", __FUNCTION__, last_retval);
		fprintf(stdout, "                    : %s; TPI error = %d\n", __FUNCTION__, tpi_error);
		if (tpi_error == TSYSERR)
			fprintf(stdout, "                    : %s; %s\n", __FUNCTION__, strerror(last_errno));
		dummy = lockf(fileno(stdout), F_ULOCK, 0);
		fflush(stdout);
	}
	return (__RESULT_FAILURE);
}

int
test_nonblock(int child)
{
	long flags;

	print_syscall(child, "fcntl(2)------");
	for (;;) {
		if ((flags = last_retval = fcntl(test_fd[child], F_GETFL)) == -1) {
			print_errno(child, (last_errno = errno));
			if (last_errno == EINTR || last_errno == ERESTART)
				continue;
			return (__RESULT_FAILURE);
		}
		print_success_value(child, last_retval);
		break;
	}
	print_syscall(child, "fcntl(2)------");
	for (;;) {
		if ((last_retval = fcntl(test_fd[child], F_SETFL, flags | O_NONBLOCK)) == -1) {
			print_errno(child, (last_errno = errno));
			if (last_errno == EINTR || last_errno == ERESTART)
				continue;
			return (__RESULT_FAILURE);
		}
		print_success_value(child, last_retval);
		break;
	}
	return (__RESULT_SUCCESS);
}

int
test_block(int child)
{
	long flags;

	print_syscall(child, "fcntl(2)------");
	for (;;) {
		if ((flags = last_retval = fcntl(test_fd[child], F_GETFL)) == -1) {
			print_errno(child, (last_errno = errno));
			if (last_errno == EINTR || last_errno == ERESTART)
				continue;
			return (__RESULT_FAILURE);
		}
		print_success_value(child, last_retval);
		break;
	}
	print_syscall(child, "fcntl(2)------");
	for (;;) {
		if ((last_retval = fcntl(test_fd[child], F_SETFL, flags & ~O_NONBLOCK)) == -1) {
			print_errno(child, (last_errno = errno));
			if (last_errno == EINTR || last_errno == ERESTART)
				continue;
			return (__RESULT_FAILURE);
		}
		print_success_value(child, last_retval);
		break;
	}
	return (__RESULT_SUCCESS);
}

int
test_pipe(int child)
{
	int fds[2];

	for (;;) {
		print_pipe(child);
		if (pipe(fds) >= 0) {
			test_fd[child + 0] = fds[0];
			test_fd[child + 1] = fds[1];
			print_success(child);
			return (__RESULT_SUCCESS);
		}
		print_errno(child, (last_errno = errno));
		if (last_errno == EAGAIN || last_errno == EINTR || last_errno == ERESTART)
			continue;
		return (__RESULT_FAILURE);
	}
}

int
test_open(int child, const char *name)
{
	int fd;

	for (;;) {
		print_open(child);
		if ((fd = open(name, O_NONBLOCK | O_RDWR)) >= 0) {
			test_fd[child] = fd;
			print_success(child);
			return (__RESULT_SUCCESS);
		}
		print_errno(child, (last_errno = errno));
		if (last_errno == EAGAIN || last_errno == EINTR || last_errno == ERESTART)
			continue;
		return (__RESULT_FAILURE);
	}
}

int
test_close(int child)
{
	int fd = test_fd[child];

	test_fd[child] = 0;
	for (;;) {
		print_close(child);
		if (close(fd) >= 0) {
			print_success(child);
			return __RESULT_SUCCESS;
		}
		print_errno(child, (last_errno = errno));
		if (last_errno == EAGAIN || last_errno == EINTR || last_errno == ERESTART)
			continue;
		return __RESULT_FAILURE;
	}
}

/*
 *  -------------------------------------------------------------------------
 *
 *  STREAM Initialization
 *
 *  -------------------------------------------------------------------------
 */

static int
stream_start(int child, int index)
{
	switch (child) {
	case 1:
		if (test_pipe(0) != __RESULT_SUCCESS)
			return __RESULT_FAILURE;
		if (test_ioctl(1, I_SRDOPT, (intptr_t) RMSGD) != __RESULT_SUCCESS)
			return __RESULT_FAILURE;
		if (test_ioctl(1, I_PUSH, (intptr_t) "pipemod") != __RESULT_SUCCESS)
			return __RESULT_FAILURE;
		return __RESULT_SUCCESS;
	case 2:
		return __RESULT_SUCCESS;
	case 0:
#if 0
		if (test_ioctl(0, I_SRDOPT, (intptr_t) RMSGD) != __RESULT_SUCCESS)
			return __RESULT_FAILURE;
		if (test_ioctl(0, I_PUSH, (intptr_t) "timod") != __RESULT_SUCCESS)
			return __RESULT_FAILURE;
#endif
		return __RESULT_SUCCESS;
	default:
		return __RESULT_FAILURE;
	}
}

static int
stream_stop(int child)
{
	switch (child) {
	case 1:
		if (test_ioctl(1, I_POP, (intptr_t) NULL) != __RESULT_SUCCESS)
			return __RESULT_FAILURE;
		if (test_close(1) != __RESULT_SUCCESS)
			return __RESULT_FAILURE;
		if (test_close(0) != __RESULT_SUCCESS)
			return __RESULT_FAILURE;
		return __RESULT_SUCCESS;
	case 2:
		return __RESULT_SUCCESS;
	case 0:
#if 0
		if (test_ioctl(0, I_POP, (intptr_t) NULL) != __RESULT_SUCCESS)
			return __RESULT_FAILURE;
#endif
		return __RESULT_SUCCESS;
	default:
		return __RESULT_FAILURE;
	}
}

/*
 *  -------------------------------------------------------------------------
 *
 *  Test harness initialization and termination.
 *
 *  -------------------------------------------------------------------------
 */

static int
begin_tests(int index)
{
	state = 0;
	if (stream_start(0, index) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (stream_start(1, index) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (stream_start(2, index) != __RESULT_SUCCESS)
		goto failure;
	state++;
	show_acks = 1;
	return __RESULT_SUCCESS;
      failure:
	return __RESULT_FAILURE;
}

static int
end_tests(int index)
{
	show_acks = 0;
	if (stream_stop(2) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (stream_stop(1) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (stream_stop(0) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return __RESULT_SUCCESS;
      failure:
	return __RESULT_FAILURE;
}

/*
 *  -------------------------------------------------------------------------
 *
 *  Injected event encoding and display functions.
 *
 *  -------------------------------------------------------------------------
 */

static int
do_signal(int child, int action)
{
	struct strbuf ctrl_buf, data_buf, *ctrl = &ctrl_buf, *data = &data_buf;
	char cbuf[BUFSIZE], dbuf[BUFSIZE];
	union T_primitives *p = (typeof(p)) cbuf;
	struct strioctl ic;

	ic.ic_cmd = 0;
	ic.ic_timout = test_timout;
	ic.ic_len = sizeof(cbuf);
	ic.ic_dp = cbuf;
	ctrl->maxlen = 0;
	ctrl->buf = cbuf;
	data->maxlen = 0;
	data->buf = dbuf;
	test_pflags = MSG_BAND;
	test_pband = 0;
	switch (action) {
	case __TEST_WRITE:
		data->len = snprintf(dbuf, BUFSIZE, "%s", "Write test data.");
		return test_write(child, dbuf, data->len);
	case __TEST_WRITEV:
	{
		struct iovec vector[4];

		vector[0].iov_base = dbuf;
		vector[0].iov_len = sprintf(vector[0].iov_base, "Writev test datum for vector 0.");
		vector[1].iov_base = dbuf + vector[0].iov_len;
		vector[1].iov_len = sprintf(vector[1].iov_base, "Writev test datum for vector 1.");
		vector[2].iov_base = dbuf + vector[1].iov_len;
		vector[2].iov_len = sprintf(vector[2].iov_base, "Writev test datum for vector 2.");
		vector[3].iov_base = dbuf + vector[2].iov_len;
		vector[3].iov_len = sprintf(vector[3].iov_base, "Writev test datum for vector 3.");
		return test_writev(child, vector, 4);
	}
	}
	switch (action) {
	case __TEST_PUSH:
		return test_ti_ioctl(child, I_PUSH, (intptr_t) "timod");
	case __TEST_POP:
		return test_ti_ioctl(child, I_POP, (intptr_t) NULL);
	case __TEST_O_NONBLOCK:
		return test_nonblock(child);
	case __TEST_O_BLOCK:
		return test_block(child);
	case __TEST_PUTMSG_DATA:
		ctrl = NULL;
		data->len = snprintf(dbuf, BUFSIZE, "%s", "Putmsg test data.");
		test_pflags = MSG_BAND;
		test_pband = 0;
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_PUTPMSG_DATA:
		ctrl = NULL;
		data->len = snprintf(dbuf, BUFSIZE, "%s", "Putpmsg band test data.");
		test_pflags = MSG_BAND;
		test_pband = 1;
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_CONN_REQ:
		ctrl->len = sizeof(p->conn_req)
		    + (test_addr ? test_alen : 0)
		    + (test_opts ? test_olen : 0);
		p->conn_req.PRIM_type = T_CONN_REQ;
		p->conn_req.DEST_length = test_addr ? test_alen : 0;
		p->conn_req.DEST_offset = test_addr ? sizeof(p->conn_req) : 0;
		p->conn_req.OPT_length = test_opts ? test_olen : 0;
		p->conn_req.OPT_offset = test_opts ? sizeof(p->conn_req) + p->conn_req.DEST_length : 0;
		if (test_addr)
			bcopy(test_addr, ctrl->buf + p->conn_req.DEST_offset, p->conn_req.DEST_length);
		if (test_opts)
			bcopy(test_opts, ctrl->buf + p->conn_req.OPT_offset, p->conn_req.OPT_length);
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		print_string(child, addr_string(cbuf + p->conn_req.DEST_offset, p->conn_req.DEST_length));
		print_options(child, cbuf, p->conn_req.OPT_offset, p->conn_req.OPT_length);
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_CONN_IND:
		ctrl->len = sizeof(p->conn_ind);
		p->conn_ind.PRIM_type = T_CONN_IND;
		p->conn_ind.SRC_length = 0;
		p->conn_ind.SRC_offset = 0;
		p->conn_ind.OPT_length = 0;
		p->conn_ind.OPT_offset = 0;
		p->conn_ind.SEQ_number = last_sequence;
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		print_options(child, cbuf, p->conn_ind.OPT_offset, p->conn_ind.OPT_length);
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_CONN_RES:
		ctrl->len = sizeof(p->conn_res)
		    + (test_opts ? test_olen : 0);
		p->conn_res.PRIM_type = T_CONN_RES;
		p->conn_res.ACCEPTOR_id = 0;
		p->conn_res.OPT_length = test_opts ? test_olen : 0;
		p->conn_res.OPT_offset = test_opts ? sizeof(p->conn_res) : 0;
		p->conn_res.SEQ_number = last_sequence;
		if (test_opts)
			bcopy(test_opts, ctrl->buf + p->conn_res.OPT_offset, p->conn_res.OPT_length);
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		print_options(child, cbuf, p->conn_res.OPT_offset, p->conn_res.OPT_length);
		if (test_resfd == -1)
			return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
		else
			return test_insertfd(child, test_resfd, 4, ctrl, data, 0);
	case __TEST_CONN_CON:
		ctrl->len = sizeof(p->conn_con);
		p->conn_con.PRIM_type = T_CONN_CON;
		p->conn_con.RES_length = 0;
		p->conn_con.RES_offset = 0;
		p->conn_con.OPT_length = 0;
		p->conn_con.OPT_offset = 0;
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		print_options(child, cbuf, p->conn_con.OPT_offset, p->conn_con.OPT_length);
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_DISCON_REQ:
		ctrl->len = sizeof(p->discon_req);
		p->discon_req.PRIM_type = T_DISCON_REQ;
		p->discon_req.SEQ_number = last_sequence;
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_DISCON_IND:
		ctrl->len = sizeof(p->discon_ind);
		p->discon_ind.PRIM_type = T_DISCON_IND;
		p->discon_ind.DISCON_reason = 0;
		p->discon_ind.SEQ_number = last_sequence;
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_DATA_REQ:
		ctrl->len = sizeof(p->data_req);
		p->data_req.PRIM_type = T_DATA_REQ;
		p->data_req.MORE_flag = MORE_flag;
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_DATA_IND:
		ctrl->len = sizeof(p->data_ind);
		p->data_ind.PRIM_type = T_DATA_IND;
		p->data_ind.MORE_flag = MORE_flag;
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_EXDATA_REQ:
		ctrl->len = sizeof(p->exdata_req);
		p->exdata_req.PRIM_type = T_EXDATA_REQ;
		p->exdata_req.MORE_flag = MORE_flag;
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 1;
		print_tx_prim(child, prim_string(p->type));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_EXDATA_IND:
		ctrl->len = sizeof(p->exdata_ind);
		p->data_ind.PRIM_type = T_EXDATA_IND;
		p->data_ind.MORE_flag = MORE_flag;
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 1;
		print_tx_prim(child, prim_string(p->type));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_INFO_REQ:
		ctrl->len = sizeof(p->info_req);
		p->info_req.PRIM_type = T_INFO_REQ;
		data = NULL;
		test_pflags = MSG_HIPRI;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_INFO_ACK:
		ctrl->len = sizeof(p->info_ack);
		p->info_ack.PRIM_type = T_INFO_ACK;
		p->info_ack.TSDU_size = test_bufsize;
		p->info_ack.ETSDU_size = test_bufsize;
		p->info_ack.CDATA_size = test_bufsize;
		p->info_ack.DDATA_size = test_bufsize;
		p->info_ack.ADDR_size = test_bufsize;
		p->info_ack.OPT_size = test_bufsize;
		p->info_ack.TIDU_size = test_tidu;
		p->info_ack.SERV_type = last_servtype;
		p->info_ack.CURRENT_state = last_tstate;
		p->info_ack.PROVIDER_flag = last_provflag;
		data = NULL;
		test_pflags = MSG_HIPRI;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		print_info(child, &p->info_ack);
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_BIND_REQ:
		ctrl->len = sizeof(p->bind_req) + (test_addr ? test_alen : 0);
		p->bind_req.PRIM_type = T_BIND_REQ;
		p->bind_req.ADDR_length = test_addr ? test_alen : 0;
		p->bind_req.ADDR_offset = test_addr ? sizeof(p->bind_req) : 0;
		p->bind_req.CONIND_number = last_qlen;
		if (test_addr)
			bcopy(test_addr, ctrl->buf + p->bind_req.ADDR_offset, p->bind_req.ADDR_length);
		data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		print_string(child, addr_string(cbuf + p->bind_ack.ADDR_offset, p->bind_ack.ADDR_length));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_BIND_ACK:
		ctrl->len = sizeof(p->bind_ack);
		p->bind_ack.PRIM_type = T_BIND_ACK;
		p->bind_ack.ADDR_length = 0;
		p->bind_ack.ADDR_offset = 0;
		p->bind_ack.CONIND_number = last_qlen;
		data = NULL;
		test_pflags = MSG_HIPRI;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		print_string(child, addr_string(cbuf + p->bind_ack.ADDR_offset, p->bind_ack.ADDR_length));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_UNBIND_REQ:
		ctrl->len = sizeof(p->unbind_req);
		p->unbind_req.PRIM_type = T_UNBIND_REQ;
		data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_ERROR_ACK:
		ctrl->len = sizeof(p->error_ack);
		p->error_ack.PRIM_type = T_ERROR_ACK;
		p->error_ack.ERROR_prim = last_prim;
		p->error_ack.TLI_error = last_t_errno;
		p->error_ack.UNIX_error = last_errno;
		data = NULL;
		test_pflags = MSG_HIPRI;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		print_string(child, terrno_string(p->error_ack.TLI_error, p->error_ack.UNIX_error));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_OK_ACK:
		ctrl->len = sizeof(p->ok_ack);
		p->ok_ack.PRIM_type = T_OK_ACK;
		p->ok_ack.CORRECT_prim = 0;
		data = NULL;
		test_pflags = MSG_HIPRI;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_UNITDATA_REQ:
		ctrl->len = sizeof(p->unitdata_req)
		    + (test_addr ? test_alen : 0)
		    + (test_opts ? test_olen : 0);
		p->unitdata_req.PRIM_type = T_UNITDATA_REQ;
		p->unitdata_req.DEST_length = test_addr ? test_alen : 0;
		p->unitdata_req.DEST_offset = test_addr ? sizeof(p->unitdata_req) : 0;
		p->unitdata_req.OPT_length = test_opts ? test_olen : 0;
		p->unitdata_req.OPT_offset = test_opts ? sizeof(p->unitdata_req) + p->unitdata_req.DEST_length : 0;
		if (test_addr)
			bcopy(test_addr, ctrl->buf + p->unitdata_req.DEST_offset, p->unitdata_req.DEST_length);
		if (test_opts)
			bcopy(test_opts, ctrl->buf + p->unitdata_req.OPT_offset, p->unitdata_req.OPT_length);
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		print_string(child, addr_string(cbuf + p->unitdata_req.DEST_offset, p->unitdata_req.DEST_length));
		print_options(child, cbuf, p->unitdata_req.OPT_offset, p->unitdata_req.OPT_length);
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_UNITDATA_IND:
		ctrl->len = sizeof(p->unitdata_ind);
		p->unitdata_ind.PRIM_type = T_UNITDATA_IND;
		p->unitdata_ind.SRC_length = 0;
		p->unitdata_ind.SRC_offset = 0;
		p->unitdata_ind.OPT_length = 0;
		p->unitdata_ind.OPT_offset = 0;
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		print_string(child, addr_string(cbuf + p->unitdata_ind.SRC_offset, p->unitdata_ind.SRC_length));
		print_options(child, cbuf, p->unitdata_ind.OPT_offset, p->unitdata_ind.OPT_length);
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_UDERROR_IND:
		ctrl->len = sizeof(p->uderror_ind);
		p->uderror_ind.PRIM_type = T_UDERROR_IND;
		p->uderror_ind.DEST_length = 0;
		p->uderror_ind.DEST_offset = 0;
		p->uderror_ind.OPT_length = 0;
		p->uderror_ind.OPT_offset = 0;
		p->uderror_ind.ERROR_type = 0;
		data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_OPTMGMT_REQ:
		ctrl->len = sizeof(p->optmgmt_req)
		    + (test_opts ? test_olen : 0);
		p->optmgmt_req.PRIM_type = T_OPTMGMT_REQ;
		p->optmgmt_req.OPT_length = test_opts ? test_olen : 0;
		p->optmgmt_req.OPT_offset = test_opts ? sizeof(p->optmgmt_req) : 0;
		p->optmgmt_req.MGMT_flags = test_mgmtflags;
		if (test_opts)
			bcopy(test_opts, ctrl->buf + p->optmgmt_req.OPT_offset, p->optmgmt_req.OPT_length);
		data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		print_mgmtflag(child, p->optmgmt_req.MGMT_flags);
		print_options(child, cbuf, p->optmgmt_req.OPT_offset, p->optmgmt_req.OPT_length);
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_OPTMGMT_ACK:
		ctrl->len = sizeof(p->optmgmt_ack);
		p->optmgmt_ack.PRIM_type = T_OPTMGMT_ACK;
		data = NULL;
		test_pflags = MSG_HIPRI;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		print_mgmtflag(child, p->optmgmt_ack.MGMT_flags);
		print_options(child, cbuf, p->optmgmt_ack.OPT_offset, p->optmgmt_ack.OPT_length);
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_ORDREL_REQ:
		ctrl->len = sizeof(p->ordrel_req);
		p->ordrel_req.PRIM_type = T_ORDREL_REQ;
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_ORDREL_IND:
		ctrl->len = sizeof(p->ordrel_ind);
		p->ordrel_ind.PRIM_type = T_ORDREL_IND;
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_OPTDATA_REQ:
		ctrl->len = sizeof(p->optdata_req)
		    + (test_opts ? test_olen : 0);
		p->optdata_req.PRIM_type = T_OPTDATA_REQ;
		p->optdata_req.DATA_flag = DATA_flag;
		p->optdata_req.OPT_length = test_opts ? test_olen : 0;
		p->optdata_req.OPT_offset = test_opts ? sizeof(p->optdata_req) : 0;
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		if (test_opts)
			bcopy(test_opts, ctrl->buf + p->optdata_req.OPT_offset, p->optdata_req.OPT_length);
		test_pflags = MSG_BAND;
		test_pband = (DATA_flag & T_ODF_EX) ? 1 : 0;
		if (p->optdata_req.DATA_flag & T_ODF_EX) {
			if (p->optdata_req.DATA_flag & T_ODF_MORE)
				print_tx_prim(child, "T_OPTDATA_REQ!+ ");
			else
				print_tx_prim(child, "T_OPTDATA_REQ!  ");
		} else {
			if (p->optdata_req.DATA_flag & T_ODF_MORE)
				print_tx_prim(child, "T_OPTDATA_REQ+  ");
			else
				print_tx_prim(child, "T_OPTDATA_REQ   ");
		}
		print_options(child, cbuf, p->optdata_req.OPT_offset, p->optdata_req.OPT_length);
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_NRM_OPTDATA_IND:
		ctrl->len = sizeof(p->optdata_ind);
		p->optdata_ind.PRIM_type = T_OPTDATA_IND;
		p->optdata_ind.DATA_flag = 0;
		p->optdata_ind.OPT_length = 0;
		p->optdata_ind.OPT_offset = 0;
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 0;
		if (p->optdata_ind.DATA_flag & T_ODF_MORE)
			print_tx_prim(child, "T_OPTDATA_IND+  ");
		else
			print_tx_prim(child, "T_OPTDATA_IND   ");
		print_options(child, cbuf, p->optdata_ind.OPT_offset, p->optdata_ind.OPT_length);
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_EXP_OPTDATA_IND:
		ctrl->len = sizeof(p->optdata_ind);
		p->optdata_ind.PRIM_type = T_OPTDATA_IND;
		p->optdata_ind.DATA_flag = T_ODF_EX;
		p->optdata_ind.OPT_length = 0;
		p->optdata_ind.OPT_offset = 0;
		if (test_data)
			data->len = snprintf(dbuf, BUFSIZE, "%s", test_data);
		else
			data = NULL;
		test_pflags = MSG_BAND;
		test_pband = 1;
		if (p->optdata_ind.DATA_flag & T_ODF_MORE)
			print_tx_prim(child, "T_OPTDATA_IND!+ ");
		else
			print_tx_prim(child, "T_OPTDATA_IND!  ");
		print_options(child, cbuf, p->optdata_ind.OPT_offset, p->optdata_ind.OPT_length);
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_ADDR_REQ:
		ctrl->len = sizeof(p->addr_req);
		p->addr_req.PRIM_type = T_ADDR_REQ;
		data = NULL;
		test_pflags = MSG_HIPRI;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_ADDR_ACK:
		ctrl->len = sizeof(p->addr_ack);
		p->addr_ack.PRIM_type = T_ADDR_ACK;
		p->addr_ack.LOCADDR_length = 0;
		p->addr_ack.LOCADDR_offset = 0;
		p->addr_ack.REMADDR_length = 0;
		p->addr_ack.REMADDR_offset = 0;
		data = NULL;
		test_pflags = MSG_HIPRI;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		print_string(child, addr_string(cbuf + p->addr_ack.LOCADDR_offset, p->addr_ack.LOCADDR_length));
		print_string(child, addr_string(cbuf + p->addr_ack.REMADDR_offset, p->addr_ack.REMADDR_length));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_CAPABILITY_REQ:
		ctrl->len = sizeof(p->capability_req);
		p->capability_req.PRIM_type = T_CAPABILITY_REQ;
		p->capability_req.CAP_bits1 = TC1_INFO | TC1_ACCEPTOR_ID;
		data = NULL;
		test_pflags = test_prio ? MSG_HIPRI : MSG_BAND;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_CAPABILITY_ACK:
		ctrl->len = sizeof(p->capability_ack);
		p->capability_ack.PRIM_type = T_CAPABILITY_ACK;
		p->capability_ack.CAP_bits1 = TC1_INFO | TC1_ACCEPTOR_ID;
		p->capability_ack.INFO_ack.TSDU_size = test_bufsize;
		p->capability_ack.INFO_ack.ETSDU_size = test_bufsize;
		p->capability_ack.INFO_ack.CDATA_size = test_bufsize;
		p->capability_ack.INFO_ack.DDATA_size = test_bufsize;
		p->capability_ack.INFO_ack.ADDR_size = test_bufsize;
		p->capability_ack.INFO_ack.OPT_size = test_bufsize;
		p->capability_ack.INFO_ack.TIDU_size = test_tidu;
		p->capability_ack.INFO_ack.SERV_type = last_servtype;
		p->capability_ack.INFO_ack.CURRENT_state = last_tstate;
		p->capability_ack.INFO_ack.PROVIDER_flag = last_provflag;
		p->capability_ack.ACCEPTOR_id = 0;
		data = NULL;
		test_pflags = MSG_HIPRI;
		test_pband = 0;
		print_tx_prim(child, prim_string(p->type));
		return test_putpmsg(child, ctrl, data, test_pband, test_pflags);
	case __TEST_T_ACCEPT:
		test_acccall.sequence = last_sequence;
		print_libcall(child, "t_accept(3)-----");
		if ((last_retval = t_accept(test_fd[child], test_fd[child], &test_acccall)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_BIND:
		print_libcall(child, "t_bind(3)-------");
		test_bindreq.qlen = last_qlen;
		if ((last_retval = t_bind(test_fd[child], &test_bindreq, &test_bindret)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_CLOSE:
		print_libcall(child, "t_close(3)------");
		if ((last_retval = t_close(test_fd[child])) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_CONNECT:
		print_libcall(child, "t_connect(3)----");
		if ((last_retval = t_connect(test_fd[child], &test_sndcall, &test_rcvcall)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_GETINFO:
		print_libcall(child, "t_getinfo(3)----");
		if ((last_retval = t_getinfo(test_fd[child], &test_info)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_GETPROTADDR:
		print_libcall(child, "t_getprotaddr(3)");
		if ((last_retval = t_getprotaddr(test_fd[child], &test_addrloc, &test_addrrem)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_GETSTATE:
		print_libcall(child, "t_getstate(3)---");
		if ((last_tstate = t_getstate(test_fd[child])) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_LISTEN:
		print_libcall(child, "t_listen(3)-----");
		if ((last_retval = t_listen(test_fd[child], &test_liscall)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		last_sequence = test_liscall.sequence;
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_LOOK:
		print_libcall(child, "t_look(3)-------");
		if ((last_tevent = t_look(test_fd[child])) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		print_tlook(child, last_tevent);
		return (__RESULT_SUCCESS);
	case __TEST_T_OPTMGMT:
		print_libcall(child, "t_optmgmt(3)----");
		if ((last_retval = t_optmgmt(test_fd[child], &test_req, &test_ret)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_RCV:
		print_libcall(child, "t_rcv(3)--------");
		if ((last_retval = t_rcv(test_fd[child], test_udata, test_bufsize, &test_rcvflags)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose) {
			dummy = lockf(fileno(stdout), F_LOCK, 0);
			fprintf(stdout, "  %4d bytes%c%c      |  |                                |                    [%d:%03d]\n", last_retval, test_rcvflags & T_MORE ? '+' : '$', test_rcvflags & T_EXPEDITED ? '!' : ' ', child, state);
			fflush(stdout);
			dummy = lockf(fileno(stdout), F_ULOCK, 0);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_RCVCONNECT:
		print_libcall(child, "t_rcvconnect(3)-");
		if ((last_retval = t_rcvconnect(test_fd[child], &test_rcvcall)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose) {
			dummy = lockf(fileno(stdout), F_LOCK, 0);
			fprintf(stdout, "  %4d bytes%c%c      |  |                                |                    [%d:%03d]\n", test_rcvcall.udata.len, ' ', ' ', child, state);
			fflush(stdout);
			dummy = lockf(fileno(stdout), F_ULOCK, 0);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_RCVDIS:
		print_libcall(child, "t_rcvdis(3)-----");
		if ((last_retval = t_rcvdis(test_fd[child], &test_rcvdis)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose) {
			dummy = lockf(fileno(stdout), F_LOCK, 0);
			fprintf(stdout, "  %4d bytes%c%c      |  |                                |                    [%d:%03d]\n", test_rcvdis.udata.len, ' ', ' ', child, state);
			fflush(stdout);
			dummy = lockf(fileno(stdout), F_ULOCK, 0);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_RCVREL:
		print_libcall(child, "t_rcvrel(3)-----");
		if ((last_retval = t_rcvrel(test_fd[child])) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose) {
			dummy = lockf(fileno(stdout), F_LOCK, 0);
			fprintf(stdout, "  %4d bytes%c%c      |  |                                |                    [%d:%03d]\n", 0, ' ', ' ', child, state);
			fflush(stdout);
			dummy = lockf(fileno(stdout), F_ULOCK, 0);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_RCVRELDATA:
		print_libcall(child, "t_rcvreldata(3)-");
		if ((last_retval = t_rcvreldata(test_fd[child], &test_rcvdis)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose) {
			dummy = lockf(fileno(stdout), F_LOCK, 0);
			fprintf(stdout, "  %4d bytes%c%c      |  |                                |                    [%d:%03d]\n", test_rcvdis.udata.len, ' ', ' ', child, state);
			fflush(stdout);
			dummy = lockf(fileno(stdout), F_ULOCK, 0);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_RCVUDATA:
		print_libcall(child, "t_rcvudata(3)---");
		if ((last_retval = t_rcvudata(test_fd[child], &test_rcvudata, &test_rcvflags)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose) {
			dummy = lockf(fileno(stdout), F_LOCK, 0);
			fprintf(stdout, "  %4d bytes%c%c      |  |                                |                    [%d:%03d]\n", test_rcvudata.udata.len, ' ', ' ', child, state);
			fflush(stdout);
			dummy = lockf(fileno(stdout), F_ULOCK, 0);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_RCVUDERR:
		print_libcall(child, "t_rcvuderr(3)---");
		if ((last_retval = t_rcvuderr(test_fd[child], &test_uderr)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_RCVV:
		test_t_iov[0].iov_base = test_udata;
		test_t_iov[0].iov_len = 64;
		test_t_iov[1].iov_base = test_udata + 64;
		test_t_iov[1].iov_len = 64;
		test_t_iov[2].iov_base = test_udata + 64 + 64;
		test_t_iov[2].iov_len = 64;
		test_t_iov[3].iov_base = test_udata + 64 + 64 + 64;
		test_t_iov[3].iov_len = 64;
		print_libcall(child, "t_rcvv(3)-------");
		if ((last_retval = t_rcvv(test_fd[child], test_t_iov, test_iovcount, &test_rcvflags)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose) {
			dummy = lockf(fileno(stdout), F_LOCK, 0);
			fprintf(stdout, "  %4d bytes%c%c      |  |                                |                    [%d:%03d]\n", last_retval, test_rcvflags & T_MORE ? '+' : '$', test_rcvflags & T_EXPEDITED ? '!' : ' ', child, state);
			fflush(stdout);
			dummy = lockf(fileno(stdout), F_ULOCK, 0);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_RCVVUDATA:
		test_t_iov[0].iov_base = test_udata;
		test_t_iov[0].iov_len = 64;
		test_t_iov[1].iov_base = test_udata + 64;
		test_t_iov[1].iov_len = 64;
		test_t_iov[2].iov_base = test_udata + 64 + 64;
		test_t_iov[2].iov_len = 64;
		test_t_iov[3].iov_base = test_udata + 64 + 64 + 64;
		test_t_iov[3].iov_len = 64;
		print_libcall(child, "t_rcvvudata(3)--");
		if ((last_retval = t_rcvvudata(test_fd[child], &test_rcvudata, test_t_iov, test_iovcount, &test_rcvflags)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose) {
			dummy = lockf(fileno(stdout), F_LOCK, 0);
			fprintf(stdout, "  %4d bytes%c%c      |  |                                |                    [%d:%03d]\n", test_rcvudata.udata.len, ' ', ' ', child, state);
			fflush(stdout);
			dummy = lockf(fileno(stdout), F_ULOCK, 0);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_SND:
	{
		int nbytes = sprintf(test_udata, "Test t_snd data for transmission.");

		print_datcall(child, "t_snd(3)--------", nbytes);
		if ((last_retval = t_snd(test_fd[child], test_udata, nbytes, test_sndflags)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	}
	case __TEST_T_SNDDIS:
		test_discall.sequence = last_sequence;
		print_datcall(child, "t_snddis(3)-----", test_discall.udata.len > 0 ? test_discall.udata.len : 0);
		if ((last_retval = t_snddis(test_fd[child], &test_discall)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_SNDREL:
		print_datcall(child, "t_sndrel(3)-----", 0);
		if ((last_retval = t_sndrel(test_fd[child])) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_SNDRELDATA:
		test_snddis.udata.len = sprintf(test_udata, "Test t_sndreldata data.");
		test_snddis.sequence = last_sequence;
		print_datcall(child, "t_sndreldata(3)-", test_snddis.udata.len > 0 ? test_snddis.udata.len : 0);
		if ((last_retval = t_sndreldata(test_fd[child], &test_snddis)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_SNDUDATA:
		test_sndudata.udata.len = sprintf(test_udata, "Test t_sndudata data.");
		print_datcall(child, "t_sndudata(3)---", test_sndudata.udata.len);
		if ((last_retval = t_sndudata(test_fd[child], &test_sndudata)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_SNDV:
		test_t_iov[0].iov_base = dbuf;
		test_t_iov[0].iov_len = sprintf(test_t_iov[0].iov_base, "Writev test datum for vector 0.");
		test_t_iov[1].iov_base = dbuf + test_t_iov[0].iov_len;
		test_t_iov[1].iov_len = sprintf(test_t_iov[1].iov_base, "Writev test datum for vector 1.");
		test_t_iov[2].iov_base = dbuf + test_t_iov[1].iov_len;
		test_t_iov[2].iov_len = sprintf(test_t_iov[2].iov_base, "Writev test datum for vector 2.");
		test_t_iov[3].iov_base = dbuf + test_t_iov[2].iov_len;
		test_t_iov[3].iov_len = sprintf(test_t_iov[3].iov_base, "Writev test datum for vector 3.");
		print_datcall(child, "t_sndv(3)-------", test_iovcount * test_t_iov[0].iov_len);
		if ((last_retval = t_sndv(test_fd[child], test_t_iov, test_iovcount, test_sndflags)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_SNDVUDATA:
		test_t_iov[0].iov_base = dbuf;
		test_t_iov[0].iov_len = sprintf(test_t_iov[0].iov_base, "Writev test datum for vector 0.");
		test_t_iov[1].iov_base = dbuf + test_t_iov[0].iov_len;
		test_t_iov[1].iov_len = sprintf(test_t_iov[1].iov_base, "Writev test datum for vector 1.");
		test_t_iov[2].iov_base = dbuf + test_t_iov[1].iov_len;
		test_t_iov[2].iov_len = sprintf(test_t_iov[2].iov_base, "Writev test datum for vector 2.");
		test_t_iov[3].iov_base = dbuf + test_t_iov[2].iov_len;
		test_t_iov[3].iov_len = sprintf(test_t_iov[3].iov_base, "Writev test datum for vector 3.");
		test_sndudata.udata.len = 0;
		print_datcall(child, "t_sndvudata(3)--", test_iovcount * test_t_iov[0].iov_len);
		if ((last_retval = t_sndvudata(test_fd[child], &test_sndudata, test_t_iov, test_iovcount)) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_SYNC:
		print_libcall(child, "t_sync(3)-------");
		if ((last_retval = t_sync(test_fd[child])) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_T_UNBIND:
		print_libcall(child, "t_unbind(3)-----");
		if ((last_retval = t_unbind(test_fd[child])) == -1) {
			print_terror(child, (last_errno = errno), (last_t_errno = t_errno));
			return (__RESULT_FAILURE);
		}
		if (verbose > 3)
			print_success_value(child, last_retval);
		return (__RESULT_SUCCESS);
	case __TEST_O_TI_GETINFO:
		ic.ic_cmd = O_TI_GETINFO;
		ic.ic_len = sizeof(p->info_ack);
		p->info_req.PRIM_type = T_INFO_REQ;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_O_TI_OPTMGMT:
		ic.ic_cmd = O_TI_OPTMGMT;
		ic.ic_len = sizeof(p->optmgmt_ack)
		    + (test_opts ? test_olen : 0);
		p->optmgmt_req.PRIM_type = T_OPTMGMT_REQ;
		p->optmgmt_req.OPT_length = test_opts ? test_olen : 0;
		p->optmgmt_req.OPT_offset = test_opts ? sizeof(p->optmgmt_req) : 0;
		p->optmgmt_req.MGMT_flags = test_mgmtflags;
		if (test_opts)
			bcopy(test_opts, ctrl->buf + p->optmgmt_req.OPT_offset, p->optmgmt_req.OPT_length);
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_O_TI_BIND:
		ic.ic_cmd = O_TI_BIND;
		ic.ic_len = sizeof(p->bind_ack);
		p->bind_req.PRIM_type = T_BIND_REQ;
		p->bind_req.ADDR_length = 0;
		p->bind_req.ADDR_offset = 0;
		p->bind_req.CONIND_number = last_qlen;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_O_TI_UNBIND:
		ic.ic_cmd = O_TI_UNBIND;
		ic.ic_len = sizeof(p->ok_ack);
		p->unbind_req.PRIM_type = T_UNBIND_REQ;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST__O_TI_GETINFO:
		ic.ic_cmd = _O_TI_GETINFO;
		ic.ic_len = sizeof(p->info_ack);
		p->info_req.PRIM_type = T_INFO_REQ;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST__O_TI_OPTMGMT:
		ic.ic_cmd = _O_TI_OPTMGMT;
		ic.ic_len = sizeof(p->optmgmt_ack)
		    + (test_opts ? test_olen : 0);
		p->optmgmt_req.PRIM_type = T_OPTMGMT_REQ;
		p->optmgmt_req.OPT_length = test_opts ? test_olen : 0;
		p->optmgmt_req.OPT_offset = test_opts ? sizeof(p->optmgmt_req) : 0;
		p->optmgmt_req.MGMT_flags = test_mgmtflags;
		if (test_opts)
			bcopy(test_opts, ctrl->buf + p->optmgmt_req.OPT_offset, p->optmgmt_req.OPT_length);
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST__O_TI_BIND:
		ic.ic_cmd = _O_TI_BIND;
		ic.ic_len = sizeof(p->bind_ack);
		p->bind_req.PRIM_type = T_BIND_REQ;
		p->bind_req.ADDR_length = 0;
		p->bind_req.ADDR_offset = 0;
		p->bind_req.CONIND_number = last_qlen;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST__O_TI_UNBIND:
		ic.ic_cmd = _O_TI_UNBIND;
		ic.ic_len = sizeof(p->ok_ack);
		p->unbind_req.PRIM_type = T_UNBIND_REQ;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST__O_TI_GETMYNAME:
		ic.ic_cmd = _O_TI_GETMYNAME;
		ic.ic_len = sizeof(p->addr_ack);
		p->addr_req.PRIM_type = T_ADDR_REQ;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST__O_TI_GETPEERNAME:
		ic.ic_cmd = _O_TI_GETPEERNAME;
		ic.ic_len = sizeof(p->addr_ack);
		p->addr_req.PRIM_type = T_ADDR_REQ;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST__O_TI_XTI_HELLO:
		ic.ic_cmd = _O_TI_XTI_HELLO;
		ic.ic_len = 0;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST__O_TI_XTI_GET_STATE:
		ic.ic_cmd = _O_TI_XTI_GET_STATE;
		ic.ic_len = 0;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST__O_TI_XTI_CLEAR_EVENT:
		ic.ic_cmd = _O_TI_XTI_CLEAR_EVENT;
		ic.ic_len = 0;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST__O_TI_XTI_MODE:
		ic.ic_cmd = _O_TI_XTI_MODE;
		ic.ic_len = 0;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST__O_TI_TLI_MODE:
		ic.ic_cmd = _O_TI_TLI_MODE;
		ic.ic_len = 0;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_TI_GETINFO:
		ic.ic_cmd = TI_GETINFO;
		ic.ic_len = sizeof(p->info_ack);
		p->info_req.PRIM_type = T_INFO_REQ;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_TI_OPTMGMT:
		ic.ic_cmd = TI_OPTMGMT;
		ic.ic_len = sizeof(p->optmgmt_ack)
		    + (test_opts ? test_olen : 0);
		p->optmgmt_req.PRIM_type = T_OPTMGMT_REQ;
		p->optmgmt_req.OPT_length = test_opts ? test_olen : 0;
		p->optmgmt_req.OPT_offset = test_opts ? sizeof(p->optmgmt_req) : 0;
		p->optmgmt_req.MGMT_flags = test_mgmtflags;
		if (test_opts)
			bcopy(test_opts, ctrl->buf + p->optmgmt_req.OPT_offset, p->optmgmt_req.OPT_length);
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_TI_BIND:
		ic.ic_cmd = TI_BIND;
		ic.ic_len = sizeof(p->bind_ack);
		p->bind_req.PRIM_type = T_BIND_REQ;
		p->bind_req.ADDR_length = 0;
		p->bind_req.ADDR_offset = 0;
		p->bind_req.CONIND_number = last_qlen;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_TI_UNBIND:
		ic.ic_cmd = TI_UNBIND;
		ic.ic_len = sizeof(p->ok_ack);
		p->unbind_req.PRIM_type = T_UNBIND_REQ;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_TI_GETMYNAME:
		ic.ic_cmd = TI_GETMYNAME;
		ic.ic_len = sizeof(p->addr_ack);
		p->addr_req.PRIM_type = T_ADDR_REQ;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_TI_GETPEERNAME:
		ic.ic_cmd = TI_GETPEERNAME;
		ic.ic_len = sizeof(p->addr_ack);
		p->addr_req.PRIM_type = T_ADDR_REQ;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_TI_SETMYNAME:
		ic.ic_cmd = TI_SETMYNAME;
		ic.ic_len = sizeof(p->conn_res);
		p->conn_res.PRIM_type = T_CONN_RES;
		p->conn_res.ACCEPTOR_id = 0;
		p->conn_res.OPT_length = 0;
		p->conn_res.OPT_offset = 0;
		p->conn_res.SEQ_number = last_sequence;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_TI_SETPEERNAME:
		ic.ic_cmd = TI_SETPEERNAME;
		ic.ic_len = sizeof(p->conn_req);
		p->conn_req.PRIM_type = T_CONN_REQ;
		p->conn_req.DEST_length = 0;
		p->conn_req.DEST_offset = 0;
		p->conn_req.OPT_length = 0;
		p->conn_req.OPT_offset = 0;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_TI_SETMYNAME_DISC:
		ic.ic_cmd = TI_SETMYNAME;
		ic.ic_len = sizeof(p->discon_req);
		p->discon_req.PRIM_type = T_DISCON_REQ;
		p->discon_req.SEQ_number = last_sequence;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_TI_SETPEERNAME_DISC:
		ic.ic_cmd = TI_SETPEERNAME;
		ic.ic_len = sizeof(p->discon_req);
		p->discon_req.PRIM_type = T_DISCON_REQ;
		p->discon_req.SEQ_number = last_sequence;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_TI_SETMYNAME_DATA:
		ic.ic_cmd = TI_SETMYNAME;
		ic.ic_len = sizeof(p->conn_res) + sprintf(cbuf + sizeof(p->conn_res), "IO control test data.");
		p->conn_res.PRIM_type = T_CONN_RES;
		p->conn_res.ACCEPTOR_id = 0;
		p->conn_res.OPT_length = 0;
		p->conn_res.OPT_offset = 0;
		p->conn_res.SEQ_number = last_sequence;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_TI_SETPEERNAME_DATA:
		ic.ic_cmd = TI_SETPEERNAME;
		ic.ic_len = sizeof(p->conn_req) + sprintf(cbuf + sizeof(p->conn_res), "IO control test data.");
		p->conn_req.PRIM_type = T_CONN_REQ;
		p->conn_req.DEST_length = 0;
		p->conn_req.DEST_offset = 0;
		p->conn_req.OPT_length = 0;
		p->conn_req.OPT_offset = 0;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_TI_SETMYNAME_DISC_DATA:
		ic.ic_cmd = TI_SETMYNAME;
		ic.ic_len = sizeof(p->discon_req) + sprintf(cbuf + sizeof(p->conn_res), "IO control test data.");
		p->discon_req.PRIM_type = T_DISCON_REQ;
		p->discon_req.SEQ_number = last_sequence;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_TI_SETPEERNAME_DISC_DATA:
		ic.ic_cmd = TI_SETPEERNAME;
		ic.ic_len = sizeof(p->discon_req) + sprintf(cbuf + sizeof(p->conn_res), "IO control test data.");
		p->discon_req.PRIM_type = T_DISCON_REQ;
		p->discon_req.SEQ_number = last_sequence;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_TI_SYNC:
	{
		union {
			struct ti_sync_req req;
			struct ti_sync_ack ack;
		} *s = (typeof(s)) p;

		ic.ic_cmd = TI_SYNC;
		ic.ic_len = sizeof(*s);
		s->req.tsr_flags = TSRF_INFO_REQ | TSRF_IS_EXP_IN_RCVBUF | TSRF_QLEN_REQ;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	}
	case __TEST_TI_GETADDRS:
		ic.ic_cmd = TI_GETADDRS;
		ic.ic_len = sizeof(p->addr_ack);
		p->addr_req.PRIM_type = T_ADDR_REQ;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	case __TEST_TI_CAPABILITY:
		ic.ic_cmd = TI_CAPABILITY;
		ic.ic_len = sizeof(p->capability_ack);
		p->capability_req.PRIM_type = T_CAPABILITY_REQ;
		p->capability_req.CAP_bits1 = TC1_INFO | TC1_ACCEPTOR_ID;
		return test_ti_ioctl(child, I_STR, (intptr_t) & ic);
	default:
		return __RESULT_SCRIPT_ERROR;
	}
	return __RESULT_SCRIPT_ERROR;
}

/*
 *  -------------------------------------------------------------------------
 *
 *  Received event decoding and display functions.
 *
 *  -------------------------------------------------------------------------
 */

static int
do_decode_data(int child, struct strbuf *data)
{
	int event = __RESULT_DECODE_ERROR;

	if (data->len >= 0) {
		event = __TEST_DATA;
		print_rx_prim(child, "DATA------------");
	}
	return ((last_event = event));
}

static int
do_decode_ctrl(int child, struct strbuf *ctrl, struct strbuf *data)
{
	int event = __RESULT_DECODE_ERROR;
	union T_primitives *p = (union T_primitives *) ctrl->buf;

	if (ctrl->len >= sizeof(p->type)) {
		switch ((last_prim = p->type)) {
		case T_CONN_REQ:
			event = __TEST_CONN_REQ;
			print_rx_prim(child, prim_string(p->type));
			print_options(child, cbuf, p->conn_req.OPT_offset, p->conn_req.OPT_length);
			break;
		case T_CONN_RES:
			event = __TEST_CONN_RES;
			print_rx_prim(child, prim_string(p->type));
			print_options(child, cbuf, p->conn_res.OPT_offset, p->conn_res.OPT_length);
			break;
		case T_DISCON_REQ:
			event = __TEST_DISCON_REQ;
			last_sequence = p->discon_req.SEQ_number;
			print_rx_prim(child, prim_string(p->type));
			break;
		case T_DATA_REQ:
			event = __TEST_DATA_REQ;
			MORE_flag = p->data_req.MORE_flag;
			print_rx_prim(child, prim_string(p->type));
			break;
		case T_EXDATA_REQ:
			event = __TEST_EXDATA_REQ;
			MORE_flag = p->data_req.MORE_flag;
			print_rx_prim(child, prim_string(p->type));
			break;
		case T_INFO_REQ:
			event = __TEST_INFO_REQ;
			print_rx_prim(child, prim_string(p->type));
			break;
		case T_BIND_REQ:
			event = __TEST_BIND_REQ;
			last_qlen = p->bind_req.CONIND_number;
			print_rx_prim(child, prim_string(p->type));
			break;
		case T_UNBIND_REQ:
			event = __TEST_UNBIND_REQ;
			print_rx_prim(child, prim_string(p->type));
			break;
		case T_UNITDATA_REQ:
			event = __TEST_UNITDATA_REQ;
			print_rx_prim(child, prim_string(p->type));
			print_string(child, addr_string(cbuf + p->unitdata_req.DEST_offset, p->unitdata_req.DEST_length));
			print_options(child, cbuf, p->unitdata_req.OPT_offset, p->unitdata_req.OPT_length);
			break;
		case T_OPTMGMT_REQ:
			event = __TEST_OPTMGMT_REQ;
			print_rx_prim(child, prim_string(p->type));
			print_mgmtflag(child, p->optmgmt_req.MGMT_flags);
			print_options(child, cbuf, p->optmgmt_req.OPT_offset, p->optmgmt_req.OPT_length);
			break;
		case T_ORDREL_REQ:
			event = __TEST_ORDREL_REQ;
			print_rx_prim(child, prim_string(p->type));
			break;
		case T_OPTDATA_REQ:
			event = __TEST_OPTDATA_REQ;
			if (p->optdata_req.DATA_flag & T_ODF_EX) {
				if (p->optdata_req.DATA_flag & T_ODF_MORE)
					print_rx_prim(child, "T_OPTDATA_REQ!+ ");
				else
					print_rx_prim(child, "T_OPTDATA_REQ!  ");
			} else {
				if (p->optdata_req.DATA_flag & T_ODF_MORE)
					print_rx_prim(child, "T_OPTDATA_REQ+  ");
				else
					print_rx_prim(child, "T_OPTDATA_REQ   ");
			}
			print_options(child, cbuf, p->optdata_req.OPT_offset, p->optdata_req.OPT_length);
			break;
		case T_ADDR_REQ:
			event = __TEST_ADDR_REQ;
			print_rx_prim(child, prim_string(p->type));
			break;
		case T_CAPABILITY_REQ:
			event = __TEST_CAPABILITY_REQ;
			print_rx_prim(child, prim_string(p->type));
			break;
		case T_CONN_IND:
			event = __TEST_CONN_IND;
			last_sequence = p->conn_ind.SEQ_number;
			print_rx_prim(child, prim_string(p->type));
			print_string(child, addr_string(cbuf + p->conn_ind.SRC_offset, p->conn_ind.SRC_length));
			print_options(child, cbuf, p->conn_ind.OPT_offset, p->conn_ind.OPT_length);
			break;
		case T_CONN_CON:
			event = __TEST_CONN_CON;
			print_rx_prim(child, prim_string(p->type));
			print_options(child, cbuf, p->conn_con.OPT_offset, p->conn_con.OPT_length);
			break;
		case T_DISCON_IND:
			event = __TEST_DISCON_IND;
			print_rx_prim(child, prim_string(p->type));
			break;
		case T_DATA_IND:
			event = __TEST_DATA_IND;
			print_rx_prim(child, prim_string(p->type));
			break;
		case T_EXDATA_IND:
			event = __TEST_EXDATA_IND;
			print_rx_prim(child, prim_string(p->type));
			break;
		case T_INFO_ACK:
			event = __TEST_INFO_ACK;
			last_info = p->info_ack;
			print_ack_prim(child, prim_string(p->type));
			print_info(child, &p->info_ack);
			break;
		case T_BIND_ACK:
			event = __TEST_BIND_ACK;
			last_qlen = p->bind_ack.CONIND_number;
			print_ack_prim(child, prim_string(p->type));
			print_string(child, addr_string(cbuf + p->bind_ack.ADDR_offset, p->bind_ack.ADDR_length));
			break;
		case T_ERROR_ACK:
			event = __TEST_ERROR_ACK;
			last_t_errno = p->error_ack.TLI_error;
			last_errno = p->error_ack.UNIX_error;
			print_ack_prim(child, prim_string(p->type));
			print_string(child, terrno_string(p->error_ack.TLI_error, p->error_ack.UNIX_error));
			break;
		case T_OK_ACK:
			event = __TEST_OK_ACK;
			print_ack_prim(child, prim_string(p->type));
			break;
		case T_UNITDATA_IND:
			event = __TEST_UNITDATA_IND;
			print_rx_prim(child, prim_string(p->type));
			print_string(child, addr_string(cbuf + p->unitdata_ind.SRC_offset, p->unitdata_ind.SRC_length));
			print_options(child, cbuf, p->unitdata_ind.OPT_offset, p->unitdata_ind.OPT_length);
			break;
		case T_UDERROR_IND:
			event = __TEST_UDERROR_IND;
			print_rx_prim(child, prim_string(p->type));
			break;
		case T_OPTMGMT_ACK:
			event = __TEST_OPTMGMT_ACK;
			print_ack_prim(child, prim_string(p->type));
			print_mgmtflag(child, p->optmgmt_ack.MGMT_flags);
			print_options(child, cbuf, p->optmgmt_ack.OPT_offset, p->optmgmt_ack.OPT_length);
			break;
		case T_ORDREL_IND:
			event = __TEST_ORDREL_IND;
			print_rx_prim(child, prim_string(p->type));
			break;
		case T_OPTDATA_IND:
			if (p->optdata_ind.DATA_flag & T_ODF_EX) {
				event = __TEST_EXP_OPTDATA_IND;
				if (p->optdata_ind.DATA_flag & T_ODF_MORE)
					print_rx_prim(child, "T_OPTDATA_IND!+ ");
				else
					print_rx_prim(child, "T_OPTDATA_IND!  ");
			} else {
				event = __TEST_NRM_OPTDATA_IND;
				if (p->optdata_ind.DATA_flag & T_ODF_MORE)
					print_rx_prim(child, "T_OPTDATA_IND+  ");
				else
					print_rx_prim(child, "T_OPTDATA_IND   ");
			}
			print_options(child, cbuf, p->optdata_ind.OPT_offset, p->optdata_ind.OPT_length);
			break;
		case T_ADDR_ACK:
			event = __TEST_ADDR_ACK;
			print_ack_prim(child, prim_string(p->type));
			print_string(child, addr_string(cbuf + p->addr_ack.LOCADDR_offset, p->addr_ack.LOCADDR_length));
			print_string(child, addr_string(cbuf + p->addr_ack.REMADDR_offset, p->addr_ack.REMADDR_length));
			break;
		case T_CAPABILITY_ACK:
			event = __TEST_CAPABILITY_ACK;
			last_info = p->capability_ack.INFO_ack;
			print_ack_prim(child, prim_string(p->type));
			break;
		default:
			event = __EVENT_UNKNOWN;
			print_no_prim(child, p->type);
			break;
		}
		if (data && data->len >= 0)
			if (do_decode_data(child, data) != __TEST_DATA)
				event = __RESULT_FAILURE;
	}
	return ((last_event = event));
}

static int
do_decode_msg(int child, struct strbuf *ctrl, struct strbuf *data)
{
	if (ctrl->len > 0) {
		if ((last_event = do_decode_ctrl(child, ctrl, data)) != __EVENT_UNKNOWN)
			return (time_event(child, last_event));
	} else if (data->len > 0) {
		if ((last_event = do_decode_data(child, data)) != __EVENT_UNKNOWN)
			return (time_event(child, last_event));
	} else if (data->len == 0) {
		return (time_event(child, (last_event = __EVENT_EOF)));
	}
	return (time_event(child, (last_event = __EVENT_NO_MSG)));
}

#if 0
#define IUT 0x00000001
#define PT  0x00000002
#define ANY 0x00000003

int
any_wait_event(int source, int wait)
{
	while (1) {
		struct pollfd pfd[] = {
			{test_fd[0], POLLIN | POLLPRI, 0},
			{test_fd[1], POLLIN | POLLPRI, 0}
		};

		if (timer_timeout) {
			timer_timeout = 0;
			print_timeout(3);
			last_event = __EVENT_TIMEOUT;
			return time_event(__EVENT_TIMEOUT);
		}
		if (verbose > 3) {
			dummy = lockf(fileno(stdout), F_LOCK, 0);
			fprintf(stdout, "polling:\n");
			fflush(stdout);
			dummy = lockf(fileno(stdout), F_ULOCK, 0);
		}
		pfd[0].fd = test_fd[0];
		pfd[0].events = (source & IUT) ? (POLLIN | POLLPRI | POLLRDNORM | POLLRDBAND | POLLMSG | POLLERR | POLLHUP) : 0;
		pfd[0].revents = 0;
		pfd[1].fd = test_fd[1];
		pfd[1].events = (source & PT) ? (POLLIN | POLLPRI | POLLRDNORM | POLLRDBAND | POLLMSG | POLLERR | POLLHUP) : 0;
		pfd[1].revents = 0;
		switch (poll(pfd, 2, wait)) {
		case -1:
			if ((errno == EAGAIN || errno == EINTR))
				break;
			print_errno(3, (last_errno = errno));
			break;
		case 0:
			print_nothing(3);
			last_event = __EVENT_NO_MSG;
			return time_event(__EVENT_NO_MSG);
		case 1:
		case 2:
			if (pfd[0].revents) {
				int flags = 0;
				char cbuf[BUFSIZE];
				char dbuf[BUFSIZE];
				struct strbuf ctrl = { BUFSIZE, 0, cbuf };
				struct strbuf data = { BUFSIZE, 0, dbuf };

				if (verbose > 3) {
					dummy = lockf(fileno(stdout), F_LOCK, 0);
					fprintf(stdout, "getmsg from top:\n");
					fflush(stdout);
					dummy = lockf(fileno(stdout), F_ULOCK, 0);
				}
				if (getmsg(test_fd[0], &ctrl, &data, &flags) == 0) {
					if (verbose > 3) {
						dummy = lockf(fileno(stdout), F_LOCK, 0);
						fprintf(stdout, "gotmsg from top [%d,%d]:\n", ctrl.len, data.len);
						fflush(stdout);
						dummy = lockf(fileno(stdout), F_ULOCK, 0);
					}
					if ((last_event = do_decode_msg(0, &ctrl, &data)) != __EVENT_NO_MSG)
						return time_event(last_event);
				}
			}
			if (pfd[1].revents) {
				int flags = 0;
				char cbuf[BUFSIZE];
				char dbuf[BUFSIZE];
				struct strbuf ctrl = { BUFSIZE, 0, cbuf };
				struct strbuf data = { BUFSIZE, 0, dbuf };

				if (verbose > 3) {
					dummy = lockf(fileno(stdout), F_LOCK, 0);
					fprintf(stdout, "getmsg from bot:\n");
					fflush(stdout);
					dummy = lockf(fileno(stdout), F_ULOCK, 0);
				}
				if (getmsg(test_fd[1], &ctrl, &data, &flags) == 0) {
					if (verbose > 3) {
						dummy = lockf(fileno(stdout), F_LOCK, 0);
						fprintf(stdout, "gotmsg from bot [%d,%d,%d]:\n", ctrl.len, data.len, flags);
						fflush(stdout);
						dummy = lockf(fileno(stdout), F_ULOCK, 0);
					}
					if ((last_event = do_decode_msg(1, &ctrl, &data)) != __EVENT_NO_MSG)
						return time_event(last_event);
				}
			}
		default:
			break;
		}
	}
}
#endif

int
wait_event(int child, int wait)
{
	while (1) {
		struct pollfd pfd[] = { {test_fd[child], POLLIN | POLLPRI, 0} };

		if (timer_timeout) {
			timer_timeout = 0;
			print_timeout(child);
			last_event = __EVENT_TIMEOUT;
			return time_event(child, __EVENT_TIMEOUT);
		}
		if (verbose > 4)
			print_syscall(child, "poll()");
		pfd[0].fd = test_fd[child];
		pfd[0].events = POLLIN | POLLPRI | POLLRDNORM | POLLRDBAND | POLLMSG | POLLERR | POLLHUP;
		pfd[0].revents = 0;
		switch (poll(pfd, 1, wait)) {
		case -1:
			print_errno(child, (last_errno = errno));
			if (last_errno == EAGAIN || last_errno == EINTR || last_errno == ERESTART)
				break;
			return (__RESULT_FAILURE);
		case 0:
			if (verbose > 4)
				print_success(child);
			print_nothing(child);
			last_event = __EVENT_NO_MSG;
			return time_event(child, __EVENT_NO_MSG);
		case 1:
			if (verbose > 4)
				print_success(child);
			if (pfd[0].revents) {
				int ret;

				ctrl.maxlen = BUFSIZE;
				ctrl.len = -1;
				ctrl.buf = cbuf;
				data.maxlen = BUFSIZE;
				data.len = -1;
				data.buf = dbuf;
				flags = 0;
				for (;;) {
					if (verbose > 4)
						print_syscall(child, "getmsg()");
					if ((ret = getmsg(test_fd[child], &ctrl, &data, &flags)) >= 0)
						break;
					print_errno(child, (last_errno = errno));
					if (last_errno == EINTR || last_errno == ERESTART)
						continue;
					if (last_errno == EAGAIN)
						break;
					return __RESULT_FAILURE;
				}
				if (ret < 0)
					break;
				if (ret == 0) {
					if (verbose > 4)
						print_success(child);
					if (verbose > 4) {
						dummy = lockf(fileno(stdout), F_LOCK, 0);
						fprintf(stdout, "gotmsg from %d [%d,%d]:\n", child, ctrl.len, data.len);
						fflush(stdout);
						dummy = lockf(fileno(stdout), F_ULOCK, 0);
					}
					last_prio = (flags == RS_HIPRI);
					if ((last_event = do_decode_msg(child, &ctrl, &data)) != __EVENT_NO_MSG)
						return time_event(child, last_event);
				}
			}
		default:
			break;
		}
	}
	return __EVENT_UNKNOWN;
}

int
get_event(int child)
{
	return wait_event(child, -1);
}

int
get_data(int child, int action)
{
	int ret = 0;

	switch (action) {
	case __TEST_READ:
	{
		char buf[BUFSIZE];

		test_read(child, buf, BUFSIZE);
		ret = last_retval;
		break;
	}
	case __TEST_READV:
	{
		char buf[BUFSIZE];
		static const size_t count = 4;
		struct iovec vector[] = {
			{buf, (BUFSIZE >> 2)},
			{buf + (BUFSIZE >> 2), (BUFSIZE >> 2)},
			{buf + (BUFSIZE >> 2) + (BUFSIZE >> 2), (BUFSIZE >> 2)},
			{buf + (BUFSIZE >> 2) + (BUFSIZE >> 2) + (BUFSIZE >> 2), (BUFSIZE >> 2)}
		};

		test_readv(child, vector, count);
		ret = last_retval;
		break;
	}
	case __TEST_GETMSG:
	{
		char buf[BUFSIZE];
		struct strbuf data = { BUFSIZE, 0, buf };

		if (test_getmsg(child, NULL, &data, &test_gflags) == __RESULT_FAILURE) {
			ret = last_retval;
			break;
		}
		ret = data.len;
		break;
	}
	case __TEST_GETPMSG:
	{
		char buf[BUFSIZE];
		struct strbuf data = { BUFSIZE, 0, buf };

		if (test_getpmsg(child, NULL, &data, &test_gband, &test_gflags) == __RESULT_FAILURE) {
			ret = last_retval;
			break;
		}
		ret = data.len;
		break;
	}
	}
	return (ret);
}

int
expect(int child, int wait, int want)
{
	if ((last_event = wait_event(child, wait)) == want)
		return (__RESULT_SUCCESS);
	print_expect(child, want);
	return (__RESULT_FAILURE);
}

void
test_sleep(int child, unsigned long t)
{
	print_waiting(child, t);
	sleep(t);
}

void
test_msleep(int child, unsigned long m)
{
	struct timespec time;

	time.tv_sec = m / 1000;
	time.tv_nsec = (m % 1000) * 1000000;
	print_mwaiting(child, &time);
	nanosleep(&time, NULL);
}

/*
 *  -------------------------------------------------------------------------
 *
 *  Preambles and postambles
 *
 *  -------------------------------------------------------------------------
 */
static int
preamble_0(int child)
{
	if (start_tt(TEST_DURATION) != __RESULT_SUCCESS)
		goto failure;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

static int
postamble_0(int child)
{
	int failed = -1;

	while (1) {
		switch (wait_event(child, NORMAL_WAIT)) {
		case __EVENT_NO_MSG:
		case __EVENT_EOF:
		case __EVENT_TIMEOUT:
		case __TEST_DISCON_REQ:
			break;
		case __RESULT_FAILURE:
			break;
		default:
			failed = (failed == -1) ? state : failed;
			state++;
			continue;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (failed != -1)
		goto failure;
	return (__RESULT_SUCCESS);
      failure:
	state = failed;
	return (__RESULT_FAILURE);
}

static int
preamble_1_top(int child)
{
	if (preamble_0(child) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_PUSH) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SYNC) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

static int
postamble_1_top(int child)
{
	int failed = -1;

	if (do_signal(child, __TEST_T_CLOSE) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (failed != -1)
		goto failure;
	return (__RESULT_SUCCESS);
      failure:
	state = failed;
	return (__RESULT_FAILURE);
}

static int
preamble_1_bot(int child)
{
	if (preamble_0(child) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_CAPABILITY_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_CAPABILITY_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

static int
postamble_1_bot(int child)
{
	return postamble_0(child);
}

static int
preamble_1cl_top(int child)
{
	last_qlen = 0;
	last_servtype = T_CLTS;
	return preamble_1_top(child);
}
static int
postamble_1cl_top(int child)
{
	last_qlen = 0;
	last_servtype = T_CLTS;
	return postamble_1_top(child);
}
static int
preamble_1cl_bot(int child)
{
	last_qlen = 0;
	last_servtype = T_CLTS;
	return preamble_1_bot(child);
}
static int
postamble_1cl_bot(int child)
{
	last_qlen = 0;
	last_servtype = T_CLTS;
	return postamble_1_bot(child);
}

static int
preamble_2_top(int child)
{
	if (preamble_0(child) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_PUSH) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SYNC) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_BIND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}
static int
postamble_2_top(int child)
{
	int failed = -1;

	if (do_signal(child, __TEST_T_UNBIND) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (do_signal(child, __TEST_T_CLOSE) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (failed != -1)
		goto failure;
	return (__RESULT_SUCCESS);
      failure:
	state = failed;
	return (__RESULT_FAILURE);
}
static int
preamble_2_bot(int child)
{
	if (preamble_0(child) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_CAPABILITY_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_CAPABILITY_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_BIND_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_BIND_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}
static int
postamble_2_bot(int child)
{
	int failed = -1;

	if (expect(child, INFINITE_WAIT, __TEST_UNBIND_REQ) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	test_msleep(child, NORMAL_WAIT);
	state++;
	while (1) {
		switch (wait_event(child, NORMAL_WAIT)) {
		case __EVENT_NO_MSG:
		case __EVENT_EOF:
		case __EVENT_TIMEOUT:
		case __TEST_DISCON_REQ:
			break;
		case __RESULT_FAILURE:
			break;
		default:
			failed = (failed == -1) ? state : failed;
			state++;
			continue;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (failed != -1)
		goto failure;
	return (__RESULT_SUCCESS);
      failure:
	state = failed;
	return (__RESULT_FAILURE);
}

static int
preamble_2cl_top(int child)
{
	last_qlen = 0;
	last_servtype = T_CLTS;
	return preamble_2_top(child);
}
static int
postamble_2cl_top(int child)
{
	last_qlen = 0;
	last_servtype = T_CLTS;
	return postamble_2_top(child);
}
static int
preamble_2cl_bot(int child)
{
	last_qlen = 0;
	last_servtype = T_CLTS;
	return preamble_2_bot(child);
}
static int
postamble_2cl_bot(int child)
{
	last_qlen = 0;
	last_servtype = T_CLTS;
	return postamble_2_bot(child);
}

static int
preamble_3_top(int child)
{
	if (preamble_0(child) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_PUSH) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SYNC) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_BIND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_CONNECT) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}
static int
postamble_3_top(int child)
{
	int failed = -1;

	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (do_signal(child, __TEST_T_UNBIND) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (do_signal(child, __TEST_T_CLOSE) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (failed != -1)
		goto failure;
	return (__RESULT_SUCCESS);
      failure:
	state = failed;
	return (__RESULT_FAILURE);
}
static int
preamble_3_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_CAPABILITY_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_CAPABILITY_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_BIND_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_BIND_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_CONN_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_data = "Connection confirmation test data.";
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_CONN_CON) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}
static int
postamble_3_bot(int child)
{
	int failed = -1;

	if (expect(child, INFINITE_WAIT, __TEST_DISCON_REQ) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_UNBIND_REQ) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	test_msleep(child, NORMAL_WAIT);
	state++;
	while (1) {
		switch (wait_event(child, NORMAL_WAIT)) {
		case __EVENT_NO_MSG:
		case __EVENT_EOF:
		case __EVENT_TIMEOUT:
		case __TEST_DISCON_REQ:
			break;
		case __RESULT_FAILURE:
			break;
		default:
			failed = (failed == -1) ? state : failed;
			state++;
			continue;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (failed != -1)
		goto failure;
	return (__RESULT_SUCCESS);
      failure:
	state = failed;
	return (__RESULT_FAILURE);
}

static int
preamble_3s_top(int child)
{
	test_tidu = 13;
	return preamble_3_top(child);
}
static int
postamble_3s_top(int child)
{
	test_tidu = test_bufsize;
	return postamble_3_top(child);
}
static int
preamble_3s_bot(int child)
{
	test_tidu = 13;
	return preamble_3_bot(child);
}
static int
postamble_3s_bot(int child)
{
	test_tidu = test_bufsize;
	return postamble_3_bot(child);
}

#if 0
static int
preamble_1(int child)
{
	union T_primitives *p = (typeof(p)) cbuf;

	test_mgmtflags = T_NEGOTIATE;
	test_opts = &opt_optm;
	test_olen = sizeof(opt_optm);
	if (do_signal(child, __TEST_OPTMGMT_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_OPTMGMT_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (p->optmgmt_ack.MGMT_flags != T_SUCCESS)
		goto failure;
	state++;
	test_addr = &addrs[child];
	test_alen = sizeof(addrs[child]);
	last_qlen = (child == 2) ? 5 : 0;
	if (do_signal(child, __TEST_BIND_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_BIND_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

static int
preamble_1s(int child)
{
	if (preamble_1(child) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

static int
postamble_1(int child)
{
	int failed = -1;

	while (1) {
		switch (wait_event(child, NORMAL_WAIT)) {
		case __EVENT_NO_MSG:
		case __EVENT_TIMEOUT:
			break;
		case __RESULT_FAILURE:
			failed = (failed == -1) ? state : failed;
			break;
		default:
			failed = (failed == -1) ? state : failed;
			state++;
			continue;
		}
		break;
	}
	state++;
	if (do_signal(child, __TEST_UNBIND_REQ) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_OK_ACK) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (failed != -1)
		goto failure;
	return (__RESULT_SUCCESS);
      failure:
	state = failed;
	return (__RESULT_FAILURE);
}

static int
postamble_1e(int child)
{
	int failed = -1;

	while (1) {
		switch (wait_event(child, NORMAL_WAIT)) {
		case __EVENT_NO_MSG:
		case __EVENT_TIMEOUT:
			break;
		case __RESULT_FAILURE:
			break;
		default:
			failed = (failed == -1) ? state : failed;
			state++;
			continue;
		}
		break;
	}
	state++;
	if (do_signal(child, __TEST_UNBIND_REQ) == __RESULT_SUCCESS || last_errno != EPROTO) {
		expect(child, NORMAL_WAIT, __TEST_OK_ACK);
		failed = (failed == -1) ? state : failed;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (failed != -1)
		goto failure;
	return (__RESULT_SUCCESS);
      failure:
	state = failed;
	return (__RESULT_FAILURE);
}

static int
preamble_2_conn(int child)
{
	if (preamble_1(child) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, NORMAL_WAIT, __EVENT_NO_MSG) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_addr = &addrs[2];
	test_alen = sizeof(addrs[2]);
	test_data = NULL;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	if (do_signal(child, __TEST_CONN_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_addr = &addrs[0];
	test_alen = sizeof(addrs[0]);
	test_data = "Connection confirmation test data.";
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	if (expect(child, INFINITE_WAIT, __TEST_CONN_CON) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

static int
preamble_2_resp(int child)
{
	if (preamble_1(child) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

static int
preamble_2_list(int child)
{
	if (preamble_1(child) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_CONN_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_resfd = test_fd[1];
	test_data = NULL;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	if (do_signal(child, __TEST_CONN_RES) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

static int
postamble_2_conn(int child)
{
	int failed = -1;

	while (1) {
		switch (wait_event(child, NORMAL_WAIT)) {
		case __EVENT_NO_MSG:
		case __EVENT_TIMEOUT:
			break;
		case __RESULT_FAILURE:
			failed = (failed == -1) ? state : failed;
			break;
		default:
			failed = (failed == -1) ? state : failed;
			state++;
			continue;
		}
		break;
	}
	state++;
	test_data = NULL;
	last_sequence = 0;
	if (do_signal(child, __TEST_DISCON_REQ) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_OK_ACK) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (postamble_1(child) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (failed != -1)
		goto failure;
	return (__RESULT_SUCCESS);
      failure:
	state = failed;
	return (__RESULT_FAILURE);
}

static int
postamble_2_resp(int child)
{
	int failed = -1;

	while (1) {
		switch (wait_event(child, NORMAL_WAIT)) {
		case __EVENT_NO_MSG:
		case __EVENT_TIMEOUT:
			break;
		case __RESULT_FAILURE:
			failed = (failed == -1) ? state : failed;
			break;
		case __TEST_DISCON_IND:
			goto got_disconnect;
		default:
			failed = (failed == -1) ? state : failed;
			state++;
			continue;
		}
		break;
	}
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_DISCON_IND) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
      got_disconnect:
	state++;
	if (postamble_1(child) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (failed != -1)
		goto failure;
	return (__RESULT_SUCCESS);
      failure:
	state = failed;
	return (__RESULT_FAILURE);
}

static int
postamble_2_list(int child)
{
	int failed = -1;

	while (1) {
		switch (wait_event(child, NORMAL_WAIT)) {
		case __EVENT_NO_MSG:
		case __EVENT_TIMEOUT:
			break;
		case __RESULT_FAILURE:
			failed = (failed == -1) ? state : failed;
			break;
		default:
			failed = (failed == -1) ? state : failed;
			state++;
			continue;
		}
		break;
	}
	state++;
	if (postamble_1(child) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (failed != -1)
		goto failure;
	return (__RESULT_SUCCESS);
      failure:
	state = failed;
	return (__RESULT_FAILURE);
}

#if 0
static int
preamble_2b_conn(int child)
{
	if (preamble_1(child) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, NORMAL_WAIT, __EVENT_NO_MSG) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_addr = &addrs[1];
	test_alen = sizeof(addrs[1]);
	test_data = "Hello World";
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	if (do_signal(child, __TEST_CONN_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_addr = &addrs[0];
	test_alen = sizeof(addrs[0]);
	test_data = "Connection confirmation test data.";
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	if (expect(child, INFINITE_WAIT, __TEST_CONN_CON) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_EXDATA_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

static int
preamble_2b_resp(int child)
{
	if (preamble_1(child) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_EXDATA_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

static int
preamble_2b_list(int child)
{
	if (preamble_1(child) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_CONN_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_resfd = test_fd[1];
	test_data = "Hello There!";
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	if (do_signal(child, __TEST_CONN_RES) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}
#endif

static int
postamble_3_conn(int child)
{
	int failed = -1;

	while (1) {
		switch (wait_event(child, NORMAL_WAIT)) {
		case __EVENT_NO_MSG:
		case __EVENT_TIMEOUT:
			break;
		case __RESULT_FAILURE:
			failed = (failed == -1) ? state : failed;
			break;
		default:
			failed = (failed == -1) ? state : failed;
			state++;
			continue;
		}
		break;
	}
	state++;
	test_data = NULL;
	if (do_signal(child, __TEST_ORDREL_REQ) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_ORDREL_IND) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (failed != -1) {
		state += 10;
		if (postamble_2_conn(child) != __RESULT_SUCCESS)
			failed = (failed == -1) ? state : failed;
	} else {
		if (postamble_1(child) != __RESULT_SUCCESS)
			failed = (failed == -1) ? state : failed;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (failed != -1)
		goto failure;
	return (__RESULT_SUCCESS);
      failure:
	state = failed;
	return (__RESULT_FAILURE);
}

static int
postamble_3_resp(int child)
{
	int failed = -1;

	while (1) {
		switch (wait_event(child, NORMAL_WAIT)) {
		case __EVENT_NO_MSG:
		case __EVENT_TIMEOUT:
			break;
		case __RESULT_FAILURE:
			failed = (failed == -1) ? state : failed;
			break;
		case __TEST_ORDREL_IND:
			goto got_release;
		default:
			failed = (failed == -1) ? state : failed;
			state++;
			continue;
		}
		break;
	}
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_ORDREL_IND) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
      got_release:
	state++;
	test_data = NULL;
	if (do_signal(child, __TEST_ORDREL_REQ) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (expect(child, NORMAL_WAIT, __EVENT_NO_MSG) != __RESULT_SUCCESS)
		failed = (failed == -1) ? state : failed;
	state++;
	if (failed != -1) {
		state += 10;
		if (postamble_2_resp(child) != __RESULT_SUCCESS)
			failed = (failed == -1) ? state : failed;
	} else {
		if (postamble_1(child) != __RESULT_SUCCESS)
			failed = (failed == -1) ? state : failed;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (failed != -1)
		goto failure;
	return (__RESULT_SUCCESS);
      failure:
	state = failed;
	return (__RESULT_FAILURE);
}

static int
postamble_3_list(int child)
{
	return postamble_2_list(child);
}
#endif

#if 0
static int
preamble_3b_conn(int child)
{
	opt_optm.rcv_val = T_YES;
	opt_optm.ist_val = 32;
	opt_optm.ost_val = 32;
	opt_conn.ist_val = 32;
	opt_conn.ost_val = 32;
	return preamble_2_conn(child);
}
static int
preamble_3b_resp(int child)
{
	opt_optm.rcv_val = T_YES;
	opt_optm.ist_val = 32;
	opt_optm.ost_val = 32;
	opt_conn.ist_val = 32;
	opt_conn.ost_val = 32;
	return preamble_2_resp(child);
}
static int
preamble_3b_list(int child)
{
	opt_optm.rcv_val = T_YES;
	opt_optm.ist_val = 32;
	opt_optm.ost_val = 32;
	opt_conn.ist_val = 32;
	opt_conn.ost_val = 32;
	return preamble_2_list(child);
}

static int
preamble_4_conn(int child)
{
	opt_optm.dbg_val = SCTP_OPTION_DROPPING;
	return preamble_2_conn(child);
}
static int
preamble_4_list(int child)
{
	opt_optm.dbg_val = SCTP_OPTION_DROPPING;
	return preamble_2_list(child);
}
static int
preamble_4_resp(int child)
{
	opt_optm.dbg_val = SCTP_OPTION_DROPPING;
	return preamble_2_resp(child);
}

static int
preamble_4b_conn(int child)
{
	opt_optm.dbg_val = SCTP_OPTION_RANDOM;
	return preamble_2_conn(child);
}
static int
preamble_4b_list(int child)
{
	opt_optm.dbg_val = SCTP_OPTION_RANDOM;
	return preamble_2_list(child);
}
static int
preamble_4b_resp(int child)
{
	opt_optm.dbg_val = SCTP_OPTION_RANDOM;
	return preamble_2_resp(child);
}

static int
preamble_5_conn(int child)
{
	// opt_optm.dbg_val = SCTP_OPTION_BREAK|SCTP_OPTION_DBREAK|SCTP_OPTION_DROPPING;
	opt_optm.dbg_val = SCTP_OPTION_BREAK;
	return preamble_2_conn(child);
}
static int
preamble_5_list(int child)
{
	// opt_optm.dbg_val = SCTP_OPTION_BREAK|SCTP_OPTION_DBREAK|SCTP_OPTION_DROPPING;
	opt_optm.dbg_val = SCTP_OPTION_BREAK;
	return preamble_2_list(child);
}
static int
preamble_5_resp(int child)
{
	// opt_optm.dbg_val = SCTP_OPTION_BREAK|SCTP_OPTION_DBREAK|SCTP_OPTION_DROPPING;
	opt_optm.dbg_val = SCTP_OPTION_BREAK;
	return preamble_2_resp(child);
}

static int
preamble_6_conn(int child)
{
	opt_optm.dbg_val = SCTP_OPTION_RANDOM;
	return preamble_3b_conn(child);
}
static int
preamble_6_list(int child)
{
	opt_optm.dbg_val = SCTP_OPTION_RANDOM;
	return preamble_3b_list(child);
}
static int
preamble_6_resp(int child)
{
	opt_optm.dbg_val = SCTP_OPTION_RANDOM;
	return preamble_3b_resp(child);
}

static int
preamble_7_conn(int child)
{
	opt_optm.mac_val = T_SCTP_HMAC_SHA1;
	return preamble_1(child);
}
static int
preamble_7_list(int child)
{
	opt_optm.mac_val = T_SCTP_HMAC_SHA1;
	return preamble_1(child);
}
static int
preamble_7_resp(int child)
{
	opt_optm.mac_val = T_SCTP_HMAC_SHA1;
	return preamble_1(child);
}

static int
preamble_8_conn(int child)
{
	opt_optm.mac_val = T_SCTP_HMAC_MD5;
	return preamble_1(child);
}
static int
preamble_8_list(int child)
{
	opt_optm.mac_val = T_SCTP_HMAC_MD5;
	return preamble_1(child);
}
static int
preamble_8_resp(int child)
{
	opt_optm.mac_val = T_SCTP_HMAC_MD5;
	return preamble_1(child);
}
#endif

/*
 *  =========================================================================
 *
 *  The Test Cases...
 *
 *  =========================================================================
 */

struct test_stream {
	int (*preamble) (int);		/* test preamble */
	int (*testcase) (int);		/* test case */
	int (*postamble) (int);		/* test postamble */
};

/*
 *  Check test case guard timer.
 */
#define test_group_0 "Sanity checks"
#define tgrp_case_0_1 test_group_0
#define numb_case_0_1 "0.1"
#define name_case_0_1 "Check test case guard timer."
#define sref_case_0_1 "(none)"
#define desc_case_0_1 "\
Checks that the test case guard timer will fire and bring down the children."

int
test_case_0_1(int child)
{
	test_sleep(child, 40);
	return (__RESULT_SUCCESS);
}

#define preamble_0_1		preamble_0
#define postamble_0_1		postamble_0

struct test_stream test_0_1_conn = { &preamble_0_1, &test_case_0_1, &postamble_0_1 };
struct test_stream test_0_1_resp = { &preamble_0_1, &test_case_0_1, &postamble_0_1 };
struct test_stream test_0_1_list = { &preamble_0_1, &test_case_0_1, &postamble_0_1 };

#define test_group_1 "Pushing and popping the timod module"
#define tgrp_case_1_1 test_group_1
#define numb_case_1_1 "1.1"
#define name_case_1_1 "Normal push and pop sequence"
#define sref_case_1_1 "(none)"
#define desc_case_1_1 "\
This test case tests the simple pushing and popping of the timod module.  It\n\
ensures that pushing the module on an empty stream has no effect, while\n\
popping the module from the stream results in a T_DISCON_REQ being sent to the\n\
transport peer."

int
test_case_1_1_top(int child)
{
	if (do_signal(child, __TEST_PUSH) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_POP) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_1_1_bot(int child)
{
	switch (wait_event(child, NORMAL_WAIT)) {
	case __TEST_DISCON_REQ:
	case __EVENT_NO_MSG:
		break;
	default:
		goto failure;
	}
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_1_1_top	preamble_0
#define preamble_1_1_bot	preamble_0

#define postamble_1_1_top	postamble_0
#define postamble_1_1_bot	postamble_0

struct test_stream test_1_1_top = { &preamble_1_1_top, &test_case_1_1_top, &postamble_1_1_top };
struct test_stream test_1_1_bot = { &preamble_1_1_bot, &test_case_1_1_bot, &postamble_1_1_bot };

#define test_group_2 "Syncrhonizing the library to a file descriptor"
#define tgrp_case_1_2_1 test_group_2
#define numb_case_1_2_1 "1.2.1"
#define name_case_1_2_1 "Syncrhonization after \"timod\" pushed -- normal operation"
#define sref_case_1_2_1 "(none)"
#define desc_case_1_2_1 "\
This test case tests syncrhonization of the XTI library to a file descriptor\n\
on the test harness upon which the \"timod\" module has been pushed."

int
test_case_1_2_1_top(int child)
{
	if (do_signal(child, __TEST_PUSH) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SYNC) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_CLOSE) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_1_2_1_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_CAPABILITY_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_CAPABILITY_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, NORMAL_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_1_2_1_top preamble_0
#define preamble_1_2_1_bot preamble_0

#define postamble_1_2_1_top postamble_0
#define postamble_1_2_1_bot postamble_0

static struct test_stream test_1_2_1_top = { &preamble_1_2_1_top, &test_case_1_2_1_top, &postamble_1_2_1_top };
static struct test_stream test_1_2_1_bot = { &preamble_1_2_1_bot, &test_case_1_2_1_bot, &postamble_1_2_1_bot };

#define tgrp_case_1_2_2 test_group_2
#define numb_case_1_2_2 "1.2.2"
#define name_case_1_2_2 "Syncrhonization after \"timod\" pushed -- closed file descriptor"
#define sref_case_1_2_2 "(none)"
#define desc_case_1_2_2 "\
This test case tests synchronization of the XTI library to a file descriptor\n\
that is closed."

int
test_case_1_2_2_top(int child)
{
	int old_fd = test_fd[child];

	close(10);
	state++;
	test_fd[child] = 10;
	if (do_signal(child, __TEST_T_SYNC) == __RESULT_SUCCESS || last_t_errno != TBADF)
		goto failure;
	test_fd[child] = old_fd;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_1_2_2_bot(int child)
{
	if (expect(child, NORMAL_WAIT, __EVENT_NO_MSG) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_1_2_2_top preamble_0
#define preamble_1_2_2_bot preamble_0

#define postamble_1_2_2_top postamble_0
#define postamble_1_2_2_bot postamble_0

static struct test_stream test_1_2_2_top = { &preamble_1_2_2_top, &test_case_1_2_2_top, &postamble_1_2_2_top };
static struct test_stream test_1_2_2_bot = { &preamble_1_2_2_bot, &test_case_1_2_2_bot, &postamble_1_2_2_bot };

#define tgrp_case_1_2_3 test_group_2
#define numb_case_1_2_3 "1.2.3"
#define name_case_1_2_3 "Synchronization after \"timod\" pushed -- system error"
#define sref_case_1_2_3 "(none)"
#define desc_case_1_2_3 "\
This test case tests synchronization of the XTI library to a file descriptor\n\
on the test harness with a system error (ENOMEM)."

int
test_case_1_2_3_top(int child)
{
	if (do_signal(child, __TEST_PUSH) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SYNC) == __RESULT_SUCCESS || last_t_errno != TSYSERR || last_errno != ENOMEM)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_1_2_3_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_CAPABILITY_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	last_t_errno = TSYSERR;
	last_errno = ENOMEM;
	if (do_signal(child, __TEST_ERROR_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, NORMAL_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_1_2_3_top preamble_0
#define preamble_1_2_3_bot preamble_0

#define postamble_1_2_3_top postamble_0
#define postamble_1_2_3_bot postamble_0

static struct test_stream test_1_2_3_top = { &preamble_1_2_3_top, &test_case_1_2_3_top, &postamble_1_2_3_top };
static struct test_stream test_1_2_3_bot = { &preamble_1_2_3_bot, &test_case_1_2_3_bot, &postamble_1_2_3_bot };

#define test_group_3 "Binding to an address"
#define tgrp_case_2_1_1 test_group_3
#define numb_case_2_1_1 "2.1.1"
#define name_case_2_1_1 "Successful bind operation"
#define sref_case_2_1_1 "(none)"
#define desc_case_2_1_1 "\
This test case tests successful bind of the XTI stream."

int
test_case_2_1_1_top(int child)
{
	if (do_signal(child, __TEST_T_BIND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_2_1_1_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_BIND_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_BIND_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_2_1_1_top preamble_1_top
#define preamble_2_1_1_bot preamble_1_bot

#define postamble_2_1_1_top postamble_1_top
#define postamble_2_1_1_bot postamble_1_bot

static struct test_stream test_2_1_1_top = { &preamble_2_1_1_top, &test_case_2_1_1_top, &postamble_2_1_1_top };
static struct test_stream test_2_1_1_bot = { &preamble_2_1_1_bot, &test_case_2_1_1_bot, &postamble_2_1_1_bot };

#define tgrp_case_2_1_2 test_group_3
#define numb_case_2_1_2 "2.1.2"
#define name_case_2_1_2 "Unsuccessful bind operation -- TACCES error"
#define sref_case_2_1_2 "(none)"
#define desc_case_2_1_2 "\
This test case test unsuccessful bind of the XTI stream."

int
test_case_2_1_x_top(int child, int terror)
{
	if (do_signal(child, __TEST_T_BIND) == __RESULT_SUCCESS || last_t_errno != terror)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_2_1_x_bot(int child, int terror)
{
	if (expect(child, INFINITE_WAIT, __TEST_BIND_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	last_t_errno = terror;
	last_errno = 0;
	if (do_signal(child, __TEST_ERROR_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_2_1_2_top(int child)
{
	return test_case_2_1_x_top(child, TACCES);
}

int
test_case_2_1_2_bot(int child)
{
	return test_case_2_1_x_bot(child, TACCES);
}

#define preamble_2_1_2_top preamble_1_top
#define preamble_2_1_2_bot preamble_1_bot

#define postamble_2_1_2_top postamble_1_top
#define postamble_2_1_2_bot postamble_1_bot

static struct test_stream test_2_1_2_top = { &preamble_2_1_2_top, &test_case_2_1_2_top, &postamble_2_1_2_top };
static struct test_stream test_2_1_2_bot = { &preamble_2_1_2_bot, &test_case_2_1_2_bot, &postamble_2_1_2_bot };

#define tgrp_case_2_1_3 test_group_3
#define numb_case_2_1_3 "2.1.3"
#define name_case_2_1_3 "Unsuccessful bind operation -- TADDRBUSY error"
#define sref_case_2_1_3 "(none)"
#define desc_case_2_1_3 "\
This test case test unsuccessful bind of the XTI stream."

int
test_case_2_1_3_top(int child)
{
	return test_case_2_1_x_top(child, TADDRBUSY);
}

int
test_case_2_1_3_bot(int child)
{
	return test_case_2_1_x_bot(child, TADDRBUSY);
}

#define preamble_2_1_3_top preamble_1_top
#define preamble_2_1_3_bot preamble_1_bot

#define postamble_2_1_3_top postamble_1_top
#define postamble_2_1_3_bot postamble_1_bot

static struct test_stream test_2_1_3_top = { &preamble_2_1_3_top, &test_case_2_1_3_top, &postamble_2_1_3_top };
static struct test_stream test_2_1_3_bot = { &preamble_2_1_3_bot, &test_case_2_1_3_bot, &postamble_2_1_3_bot };

#define tgrp_case_2_1_4 test_group_3
#define numb_case_2_1_4 "2.1.4"
#define name_case_2_1_4 "Unsuccessful bind operation -- TBADADDR error"
#define sref_case_2_1_4 "(none)"
#define desc_case_2_1_4 "\
This test case test unsuccessful bind of the XTI stream."

int
test_case_2_1_4_top(int child)
{
	return test_case_2_1_x_top(child, TBADADDR);
}

int
test_case_2_1_4_bot(int child)
{
	return test_case_2_1_x_bot(child, TBADADDR);
}

#define preamble_2_1_4_top preamble_1_top
#define preamble_2_1_4_bot preamble_1_bot

#define postamble_2_1_4_top postamble_1_top
#define postamble_2_1_4_bot postamble_1_bot

static struct test_stream test_2_1_4_top = { &preamble_2_1_4_top, &test_case_2_1_4_top, &postamble_2_1_4_top };
static struct test_stream test_2_1_4_bot = { &preamble_2_1_4_bot, &test_case_2_1_4_bot, &postamble_2_1_4_bot };

#define tgrp_case_2_1_5 test_group_3
#define numb_case_2_1_5 "2.1.5"
#define name_case_2_1_5 "Unsuccessful bind operation -- TOUTSTATE error"
#define sref_case_2_1_5 "(none)"
#define desc_case_2_1_5 "\
This test case test unsuccessful bind of the XTI stream."

int
test_case_2_1_5_top(int child)
{
	return test_case_2_1_x_top(child, TOUTSTATE);
}

int
test_case_2_1_5_bot(int child)
{
	return test_case_2_1_x_bot(child, TOUTSTATE);
}

#define preamble_2_1_5_top preamble_1_top
#define preamble_2_1_5_bot preamble_1_bot

#define postamble_2_1_5_top postamble_1_top
#define postamble_2_1_5_bot postamble_1_bot

static struct test_stream test_2_1_5_top = { &preamble_2_1_5_top, &test_case_2_1_5_top, &postamble_2_1_5_top };
static struct test_stream test_2_1_5_bot = { &preamble_2_1_5_bot, &test_case_2_1_5_bot, &postamble_2_1_5_bot };

#define tgrp_case_2_1_6 test_group_3
#define numb_case_2_1_6 "2.1.6"
#define name_case_2_1_6 "Unsuccessful bind operation -- TNOADDR error"
#define sref_case_2_1_6 "(none)"
#define desc_case_2_1_6 "\
This test case test unsuccessful bind of the XTI stream."

int
test_case_2_1_6_top(int child)
{
	return test_case_2_1_x_top(child, TNOADDR);
}

int
test_case_2_1_6_bot(int child)
{
	return test_case_2_1_x_bot(child, TNOADDR);
}

#define preamble_2_1_6_top preamble_1_top
#define preamble_2_1_6_bot preamble_1_bot

#define postamble_2_1_6_top postamble_1_top
#define postamble_2_1_6_bot postamble_1_bot

static struct test_stream test_2_1_6_top = { &preamble_2_1_6_top, &test_case_2_1_6_top, &postamble_2_1_6_top };
static struct test_stream test_2_1_6_bot = { &preamble_2_1_6_bot, &test_case_2_1_6_bot, &postamble_2_1_6_bot };

#define tgrp_case_2_1_7 test_group_3
#define numb_case_2_1_7 "2.1.7"
#define name_case_2_1_7 "Unsuccessful bind operation -- TOUTSTATE error"
#define sref_case_2_1_7 "(none)"
#define desc_case_2_1_7 "\
This test case test unsuccessful bind of the XTI stream."

int
test_case_2_1_7_top(int child)
{
	return test_case_2_1_x_top(child, TOUTSTATE);
}

int
test_case_2_1_7_bot(int child)
{
	return test_case_2_1_x_bot(child, TOUTSTATE);
}

#define preamble_2_1_7_top preamble_1_top
#define preamble_2_1_7_bot preamble_1_bot

#define postamble_2_1_7_top postamble_1_top
#define postamble_2_1_7_bot postamble_1_bot

static struct test_stream test_2_1_7_top = { &preamble_2_1_7_top, &test_case_2_1_7_top, &postamble_2_1_7_top };
static struct test_stream test_2_1_7_bot = { &preamble_2_1_7_bot, &test_case_2_1_7_bot, &postamble_2_1_7_bot };

#define tgrp_case_2_1_8 test_group_3
#define numb_case_2_1_8 "2.1.8"
#define name_case_2_1_8 "Unsuccessful bind operation -- double bind error"
#define sref_case_2_1_8 "(none)"
#define desc_case_2_1_8 "\
This test case tests unsuccessful bind of the XTI stream."

int
test_case_2_1_8_top(int child)
{
	if (do_signal(child, __TEST_T_BIND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_BIND) == __RESULT_SUCCESS || last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_UNBIND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_2_1_8_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_BIND_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_BIND_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_UNBIND_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, NORMAL_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_2_1_8_top preamble_1_top
#define preamble_2_1_8_bot preamble_1_bot

#define postamble_2_1_8_top postamble_1_top
#define postamble_2_1_8_bot postamble_1_bot

static struct test_stream test_2_1_8_top = { &preamble_2_1_8_top, &test_case_2_1_8_top, &postamble_2_1_8_top };
static struct test_stream test_2_1_8_bot = { &preamble_2_1_8_bot, &test_case_2_1_8_bot, &postamble_2_1_8_bot };

#define tgrp_case_2_2 test_group_3
#define numb_case_2_2 "2.2"
#define name_case_2_2 "Successful bind and unbind operation"
#define sref_case_2_2 "(none)"
#define desc_case_2_2 "\
This test case tests successful bind and unbind of the XTI stream."

int
test_case_2_2_top(int child)
{
	if (do_signal(child, __TEST_T_BIND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_UNBIND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_2_2_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_BIND_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_BIND_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_UNBIND_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_2_2_top preamble_1_top
#define preamble_2_2_bot preamble_1_bot

#define postamble_2_2_top postamble_1_top
#define postamble_2_2_bot postamble_1_bot

static struct test_stream test_2_2_top = { &preamble_2_2_top, &test_case_2_2_top, &postamble_2_2_top };
static struct test_stream test_2_2_bot = { &preamble_2_2_bot, &test_case_2_2_bot, &postamble_2_2_bot };

#define test_group_4 "Connecting to an address"
#define tgrp_case_2_3_1 test_group_4
#define numb_case_2_3_1 "2.3.1"
#define name_case_2_3_1 "Successful connect and disconnect operation -- immediate confirmation, synchronous mode"
#define sref_case_2_3_1 "(none)"
#define desc_case_2_3_1 "\
This test case tests successful connect and disconnect of the XTI stream."

int
test_case_2_3_1_top(int child)
{
	if (do_signal(child, __TEST_T_CONNECT) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_2_3_1_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_CONN_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_addr = NULL;
	test_alen = 0;
	test_data = "Connection confirmation test data.";
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_CONN_CON) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_DISCON_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_2_3_1_top preamble_2_top
#define preamble_2_3_1_bot preamble_2_bot

#define postamble_2_3_1_top postamble_2_top
#define postamble_2_3_1_bot postamble_2_bot

static struct test_stream test_2_3_1_top = { &preamble_2_3_1_top, &test_case_2_3_1_top, &postamble_2_3_1_top };
static struct test_stream test_2_3_1_bot = { &preamble_2_3_1_bot, &test_case_2_3_1_bot, &postamble_2_3_1_bot };

#define tgrp_case_2_3_2 test_group_4
#define numb_case_2_3_2 "2.3.2"
#define name_case_2_3_2 "Successful connect and disconnect operation -- delayed confirmation, synchronous mode"
#define sref_case_2_3_2 "(none)"
#define desc_case_2_3_2 "\
This test case tests successful connect and disconnect of the XTI stream."

int
test_case_2_3_2_top(int child)
{
	if (do_signal(child, __TEST_T_CONNECT) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_2_3_2_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_CONN_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	test_addr = NULL;
	test_alen = 0;
	test_data = "Connection confirmation test data.";
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_CONN_CON) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_DISCON_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_2_3_2_top preamble_2_top
#define preamble_2_3_2_bot preamble_2_bot

#define postamble_2_3_2_top postamble_2_top
#define postamble_2_3_2_bot postamble_2_bot

static struct test_stream test_2_3_2_top = { &preamble_2_3_2_top, &test_case_2_3_2_top, &postamble_2_3_2_top };
static struct test_stream test_2_3_2_bot = { &preamble_2_3_2_bot, &test_case_2_3_2_bot, &postamble_2_3_2_bot };

#define tgrp_case_2_3_3 test_group_4
#define numb_case_2_3_3 "2.3.3"
#define name_case_2_3_3 "Successful connect and disconnect operation -- delayed confirmation, asynchronous mode"
#define sref_case_2_3_3 "(none)"
#define desc_case_2_3_3 "\
This test case tests successful connect and disconnect of the XTI stream."

int
test_case_2_3_3_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_CONNECT) == __RESULT_SUCCESS || last_t_errno != TNODATA)
		goto failure;
	state++;
	test_msleep(child, LONG_WAIT + NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_RCVCONNECT) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_2_3_3_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_CONN_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	test_addr = NULL;
	test_alen = 0;
	test_data = "Connection confirmation test data.";
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_CONN_CON) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_DISCON_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_2_3_3_top preamble_2_top
#define preamble_2_3_3_bot preamble_2_bot

#define postamble_2_3_3_top postamble_2_top
#define postamble_2_3_3_bot postamble_2_bot

static struct test_stream test_2_3_3_top = { &preamble_2_3_3_top, &test_case_2_3_3_top, &postamble_2_3_3_top };
static struct test_stream test_2_3_3_bot = { &preamble_2_3_3_bot, &test_case_2_3_3_bot, &postamble_2_3_3_bot };

#define tgrp_case_2_3_4 test_group_4
#define numb_case_2_3_4 "2.3.4"
#define name_case_2_3_4 "Unsuccessful connect operation -- TACCES error"
#define sref_case_2_3_4 "(none)"
#define desc_case_2_3_4 "\
This test case test unsuccessful connection of the XTI stream with a\n\
TACCES error."

int
test_case_2_3_x_top(int child, int terror)
{
	if (do_signal(child, __TEST_T_CONNECT) == __RESULT_SUCCESS || last_t_errno != terror)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_2_3_x_bot(int child, int terror)
{
	if (expect(child, INFINITE_WAIT, __TEST_CONN_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	last_t_errno = terror;
	last_errno = 0;
	if (do_signal(child, __TEST_ERROR_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_2_3_4_top(int child)
{
	return test_case_2_3_x_top(child, TACCES);
}

int
test_case_2_3_4_bot(int child)
{
	return test_case_2_3_x_bot(child, TACCES);
}

#define preamble_2_3_4_top preamble_2_top
#define preamble_2_3_4_bot preamble_2_bot

#define postamble_2_3_4_top postamble_2_top
#define postamble_2_3_4_bot postamble_2_bot

static struct test_stream test_2_3_4_top = { &preamble_2_3_4_top, &test_case_2_3_4_top, &postamble_2_3_4_top };
static struct test_stream test_2_3_4_bot = { &preamble_2_3_4_bot, &test_case_2_3_4_bot, &postamble_2_3_4_bot };

#define tgrp_case_2_3_5 test_group_4
#define numb_case_2_3_5 "2.3.5"
#define name_case_2_3_5 "Unsuccessful connect operation -- TADDRBUSY error"
#define sref_case_2_3_5 "(none)"
#define desc_case_2_3_5 "\
This test case test unsuccessful connection of the XTI stream with a\n\
TADDRBUSY error."

int
test_case_2_3_5_top(int child)
{
	return test_case_2_3_x_top(child, TADDRBUSY);
}

int
test_case_2_3_5_bot(int child)
{
	return test_case_2_3_x_bot(child, TADDRBUSY);
}

#define preamble_2_3_5_top preamble_2_top
#define preamble_2_3_5_bot preamble_2_bot

#define postamble_2_3_5_top postamble_2_top
#define postamble_2_3_5_bot postamble_2_bot

static struct test_stream test_2_3_5_top = { &preamble_2_3_5_top, &test_case_2_3_5_top, &postamble_2_3_5_top };
static struct test_stream test_2_3_5_bot = { &preamble_2_3_5_bot, &test_case_2_3_5_bot, &postamble_2_3_5_bot };

#define tgrp_case_2_3_6 test_group_4
#define numb_case_2_3_6 "2.3.6"
#define name_case_2_3_6 "Unsuccessful connect operation -- TBADADDR error"
#define sref_case_2_3_6 "(none)"
#define desc_case_2_3_6 "\
This test case test unsuccessful connection of the XTI stream with a\n\
TBADADDR error."

int
test_case_2_3_6_top(int child)
{
	return test_case_2_3_x_top(child, TBADADDR);
}

int
test_case_2_3_6_bot(int child)
{
	return test_case_2_3_x_bot(child, TBADADDR);
}

#define preamble_2_3_6_top preamble_2_top
#define preamble_2_3_6_bot preamble_2_bot

#define postamble_2_3_6_top postamble_2_top
#define postamble_2_3_6_bot postamble_2_bot

static struct test_stream test_2_3_6_top = { &preamble_2_3_6_top, &test_case_2_3_6_top, &postamble_2_3_6_top };
static struct test_stream test_2_3_6_bot = { &preamble_2_3_6_bot, &test_case_2_3_6_bot, &postamble_2_3_6_bot };

#define tgrp_case_2_3_7 test_group_4
#define numb_case_2_3_7 "2.3.7"
#define name_case_2_3_7 "Unsuccessful connect operation -- TBADOPT error"
#define sref_case_2_3_7 "(none)"
#define desc_case_2_3_7 "\
This test case test unsuccessful connection of the XTI stream with a\n\
TBADOPT error."

int
test_case_2_3_7_top(int child)
{
	return test_case_2_3_x_top(child, TBADOPT);
}

int
test_case_2_3_7_bot(int child)
{
	return test_case_2_3_x_bot(child, TBADOPT);
}

#define preamble_2_3_7_top preamble_2_top
#define preamble_2_3_7_bot preamble_2_bot

#define postamble_2_3_7_top postamble_2_top
#define postamble_2_3_7_bot postamble_2_bot

static struct test_stream test_2_3_7_top = { &preamble_2_3_7_top, &test_case_2_3_7_top, &postamble_2_3_7_top };
static struct test_stream test_2_3_7_bot = { &preamble_2_3_7_bot, &test_case_2_3_7_bot, &postamble_2_3_7_bot };

#define tgrp_case_2_3_8 test_group_4
#define numb_case_2_3_8 "2.3.8"
#define name_case_2_3_8 "Unsuccessful connect operation -- TNOTSUPPORT error"
#define sref_case_2_3_8 "(none)"
#define desc_case_2_3_8 "\
This test case test unsuccessful connection of the XTI stream with a\n\
TNOTSUPPORT error."

int
test_case_2_3_8_top(int child)
{
	return test_case_2_3_x_top(child, TNOTSUPPORT);
}

int
test_case_2_3_8_bot(int child)
{
	return test_case_2_3_x_bot(child, TNOTSUPPORT);
}

#define preamble_2_3_8_top preamble_2_top
#define preamble_2_3_8_bot preamble_2_bot

#define postamble_2_3_8_top postamble_2_top
#define postamble_2_3_8_bot postamble_2_bot

static struct test_stream test_2_3_8_top = { &preamble_2_3_8_top, &test_case_2_3_8_top, &postamble_2_3_8_top };
static struct test_stream test_2_3_8_bot = { &preamble_2_3_8_bot, &test_case_2_3_8_bot, &postamble_2_3_8_bot };

#define tgrp_case_2_3_9 test_group_4
#define numb_case_2_3_9 "2.3.9"
#define name_case_2_3_9 "Unsuccessful connect operation -- TOUTSTATE error"
#define sref_case_2_3_9 "(none)"
#define desc_case_2_3_9 "\
This test case test unsuccessful connection of the XTI stream with a\n\
TOUTSTATE error."

int
test_case_2_3_9_top(int child)
{
	return test_case_2_3_x_top(child, TOUTSTATE);
}

int
test_case_2_3_9_bot(int child)
{
	return test_case_2_3_x_bot(child, TOUTSTATE);
}

#define preamble_2_3_9_top preamble_2_top
#define preamble_2_3_9_bot preamble_2_bot

#define postamble_2_3_9_top postamble_2_top
#define postamble_2_3_9_bot postamble_2_bot

static struct test_stream test_2_3_9_top = { &preamble_2_3_9_top, &test_case_2_3_9_top, &postamble_2_3_9_top };
static struct test_stream test_2_3_9_bot = { &preamble_2_3_9_bot, &test_case_2_3_9_bot, &postamble_2_3_9_bot };

#define test_group_5 "Connection from an address"
#define tgrp_case_2_4_1 test_group_5
#define numb_case_2_4_1 "2.4.1"
#define name_case_2_4_1 "Successful connection and disconnection operation -- synchronous mode"
#define sref_case_2_4_1 "(none)"
#define desc_case_2_4_1 "\
This test case tests successful incoming connection and disconnection of\n\
the XTI stream."

int
test_case_2_4_1_top(int child)
{
	if (do_signal(child, __TEST_T_LISTEN) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_2_4_1_bot(int child)
{
	last_sequence = 1;
	test_addr = NULL;
	test_alen = 0;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_data = "Connection indication test data.";
	if (do_signal(child, __TEST_CONN_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_DISCON_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_2_4_1_top preamble_2_top
#define preamble_2_4_1_bot preamble_2_bot

#define postamble_2_4_1_top postamble_2_top
#define postamble_2_4_1_bot postamble_2_bot

static struct test_stream test_2_4_1_top = { &preamble_2_4_1_top, &test_case_2_4_1_top, &postamble_2_4_1_top };
static struct test_stream test_2_4_1_bot = { &preamble_2_4_1_bot, &test_case_2_4_1_bot, &postamble_2_4_1_bot };

#define test_group_6 "Connection collision"
#define tgrp_case_2_5_1 test_group_6
#define numb_case_2_5_1 "2.5.1"
#define name_case_2_5_1 "Incoming connection before outgoing"
#define sref_case_2_5_1 "(none)"
#define desc_case_2_5_1 "\
This test case tests unsuccessful outgoing connection due to incoming\n\
connection indication on an XTI stream."

int
test_case_2_5_1_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_CONNECT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	switch (last_t_errno) {
	case TLOOK:
	case TOUTSTATE:
		break;
	default:
		state++;
		goto failure;
	}
	state++;
	if (do_signal(child, __TEST_T_LISTEN) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_2_5_1_bot(int child)
{
	last_sequence = 1;
	test_addr = NULL;
	test_alen = 0;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_data = "Connection indication test data.";
	if (do_signal(child, __TEST_CONN_IND) != __RESULT_SUCCESS)
		goto failure;
      again:
	state++;
	switch (wait_event(child, LONG_WAIT)) {
	case __TEST_DISCON_REQ:
		test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
		if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
			goto failure;
		break;
	case __TEST_CONN_REQ:
		test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
		last_t_errno = TOUTSTATE;
		last_errno = 0;
		if (do_signal(child, __TEST_ERROR_ACK) != __RESULT_SUCCESS)
			goto failure;
		goto again;
	default:
		goto failure;
	}
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_2_5_1_top preamble_2_top
#define preamble_2_5_1_bot preamble_2_bot

#define postamble_2_5_1_top postamble_2_top
#define postamble_2_5_1_bot postamble_2_bot

static struct test_stream test_2_5_1_top = { &preamble_2_5_1_top, &test_case_2_5_1_top, &postamble_2_5_1_top };
static struct test_stream test_2_5_1_bot = { &preamble_2_5_1_bot, &test_case_2_5_1_bot, &postamble_2_5_1_bot };

#define test_group_7 "t_look response to events"
#define tgrp_case_3_1_1 test_group_7
#define numb_case_3_1_1 "3.1.1"
#define name_case_3_1_1 "T_CONN_IND event"
#define sref_case_3_1_1 "(none)"
#define desc_case_3_1_1 "\
This test case tests the response of the t_look function to events.\n\
This test case tests the response to the T_CONN_IND TPI event."

int
test_case_3_1_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LISTEN) == __RESULT_SUCCESS)
		return (__RESULT_INCONCLUSIVE);
	state++;
	if (last_t_errno != TNODATA)
		goto failure;
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_LISTEN)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LISTEN) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_3_1_1_bot(int child)
{
	test_msleep(child, SHORT_WAIT);
	state++;
	last_sequence = 1;
	test_addr = NULL;
	test_alen = 0;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_data = "Connection indication test data.";
	if (do_signal(child, __TEST_CONN_IND) != 0)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_DISCON_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_3_1_1_top preamble_2_top
#define preamble_3_1_1_bot preamble_2_bot

#define postamble_3_1_1_top postamble_2_top
#define postamble_3_1_1_bot postamble_2_bot

static struct test_stream test_3_1_1_top = { &preamble_3_1_1_top, &test_case_3_1_1_top, &postamble_3_1_1_top };
static struct test_stream test_3_1_1_bot = { &preamble_3_1_1_bot, &test_case_3_1_1_bot, &postamble_3_1_1_bot };

#define tgrp_case_3_2_1 test_group_7
#define numb_case_3_2_1 "3.2.1"
#define name_case_3_2_1 "T_CONN_CON event"
#define sref_case_3_2_1 "(none)"
#define desc_case_3_2_1 "\
This test case tests the response of the t_look function to events.\n\
This test case tests the response to the T_CONN_CON TPI event."

int
test_case_3_2_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_CONNECT) == __RESULT_SUCCESS)
		return (__RESULT_INCONCLUSIVE);
	state++;
	if (last_t_errno != TNODATA)
		goto failure;
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_CONNECT)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVCONNECT) != __RESULT_SUCCESS)
		goto failure;
	state++;
	last_sequence = 0;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_3_2_1_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_CONN_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, NORMAL_WAIT);
	state++;
	test_addr = NULL;
	test_alen = 0;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_data = "Connection confirmation test data.";
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_CONN_CON) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_DISCON_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_3_2_1_top preamble_2_top
#define preamble_3_2_1_bot preamble_2_bot

#define postamble_3_2_1_top postamble_2_top
#define postamble_3_2_1_bot postamble_2_bot

static struct test_stream test_3_2_1_top = { &preamble_3_2_1_top, &test_case_3_2_1_top, &postamble_3_2_1_top };
static struct test_stream test_3_2_1_bot = { &preamble_3_2_1_bot, &test_case_3_2_1_bot, &postamble_3_2_1_bot };

#define tgrp_case_3_3_1 test_group_7
#define numb_case_3_3_1 "3.3.1"
#define name_case_3_3_1 "T_DISCON_IND event"
#define sref_case_3_3_1 "(none)"
#define desc_case_3_3_1 "\
This test case tests the response of the t_look function to events.\n\
This test case tests the response to the T_DISCON_IND TPI event."

int
test_case_3_3_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_CONNECT) == __RESULT_SUCCESS)
		return (__RESULT_INCONCLUSIVE);
	state++;
	if (last_t_errno != TNODATA)
		goto failure;
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DISCONNECT)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_3_3_1_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_CONN_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, NORMAL_WAIT);
	state++;
	last_sequence = 0;
	test_data = "Disconnect indication test data.";
	if (do_signal(child, __TEST_DISCON_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_3_3_1_top preamble_2_top
#define preamble_3_3_1_bot preamble_2_bot

#define postamble_3_3_1_top postamble_2_top
#define postamble_3_3_1_bot postamble_2_bot

static struct test_stream test_3_3_1_top = { &preamble_3_3_1_top, &test_case_3_3_1_top, &postamble_3_3_1_top };
static struct test_stream test_3_3_1_bot = { &preamble_3_3_1_bot, &test_case_3_3_1_bot, &postamble_3_3_1_bot };

#define tgrp_case_3_4_1 test_group_7
#define numb_case_3_4_1 "3.4.1"
#define name_case_3_4_1 "T_ORDREL_IND event"
#define sref_case_3_4_1 "(none)"
#define desc_case_3_4_1 "\
This test case tests the response of the t_look function to events.\n\
This test case tests the response to the T_ORDREL_IND TPI event."

int
test_case_3_4_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_CONNECT) != __RESULT_SUCCESS) {
		state++;
		if (last_t_errno != TNODATA)
			goto failure;
		state++;
		test_msleep(child, NORMAL_WAIT);
		state++;
		if (do_signal(child, __TEST_T_RCVCONNECT) != __RESULT_SUCCESS)
			goto failure;
	}
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_ORDREL)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVREL) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDREL) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_3_4_1_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_CONN_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_addr = NULL;
	test_alen = 0;
	test_data = "Connection confirmation test data.";
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_CONN_CON) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, NORMAL_WAIT);
	state++;
	last_sequence = 0;
	test_data = "Orderly release indication test data.";
	if (do_signal(child, __TEST_ORDREL_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_ORDREL_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_3_4_1_top preamble_2_top
#define preamble_3_4_1_bot preamble_2_bot

#define postamble_3_4_1_top postamble_2_top
#define postamble_3_4_1_bot postamble_2_bot

static struct test_stream test_3_4_1_top = { &preamble_3_4_1_top, &test_case_3_4_1_top, &postamble_3_4_1_top };
static struct test_stream test_3_4_1_bot = { &preamble_3_4_1_bot, &test_case_3_4_1_bot, &postamble_3_4_1_bot };

#define tgrp_case_3_4_2 test_group_7
#define numb_case_3_4_2 "3.4.2"
#define name_case_3_4_2 "T_ORDREL_IND w/ DATA event"
#define sref_case_3_4_2 "(none)"
#define desc_case_3_4_2 "\
This test case tests the response of the t_look function to events.\n\
This test case tests the response to the T_ORDREL_IND w/ DATA TPI event."

int
test_case_3_4_2_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_CONNECT) != __RESULT_SUCCESS) {
		state++;
		if (last_t_errno != TNODATA)
			goto failure;
		state++;
		test_msleep(child, NORMAL_WAIT);
		state++;
		if (do_signal(child, __TEST_T_RCVCONNECT) != __RESULT_SUCCESS)
			goto failure;
	}
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_ORDREL)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVRELDATA) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDRELDATA) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_3_4_2_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_CONN_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_addr = NULL;
	test_alen = 0;
	test_data = "Connection confirmation test data.";
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_CONN_CON) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, NORMAL_WAIT);
	state++;
	last_sequence = 0;
	test_data = "Orderly release indication test data.";
	if (do_signal(child, __TEST_ORDREL_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_ORDREL_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_3_4_2_top preamble_2_top
#define preamble_3_4_2_bot preamble_2_bot

#define postamble_3_4_2_top postamble_2_top
#define postamble_3_4_2_bot postamble_2_bot

static struct test_stream test_3_4_2_top = { &preamble_3_4_2_top, &test_case_3_4_2_top, &postamble_3_4_2_top };
static struct test_stream test_3_4_2_bot = { &preamble_3_4_2_bot, &test_case_3_4_2_bot, &postamble_3_4_2_bot };

#define tgrp_case_3_5_1 test_group_7
#define numb_case_3_5_1 "3.5.1"
#define name_case_3_5_1 "T_DATA_IND event -- t_rcv used as receive function"
#define sref_case_3_5_1 "(none)"
#define desc_case_3_5_1 "\
This test case tests the response of the t_look function to events.\n\
This test case tests the response to the T_DATA_IND TPI event."

int
test_case_3_5_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCV) == __RESULT_SUCCESS)
		return (__RESULT_INCONCLUSIVE);
	state++;
	if (last_t_errno != TNODATA)
		goto failure;
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DATA)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCV) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCV) == __RESULT_SUCCESS || last_t_errno != TNODATA)
		goto failure;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_3_5_1_bot(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	test_data = "Normal test data.";
	if (do_signal(child, __TEST_DATA_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_3_5_1_top preamble_3_top
#define preamble_3_5_1_bot preamble_3_bot

#define postamble_3_5_1_top postamble_3_top
#define postamble_3_5_1_bot postamble_3_bot

static struct test_stream test_3_5_1_top = { &preamble_3_5_1_top, &test_case_3_5_1_top, &postamble_3_5_1_top };
static struct test_stream test_3_5_1_bot = { &preamble_3_5_1_bot, &test_case_3_5_1_bot, &postamble_3_5_1_bot };

#define tgrp_case_3_5_2 test_group_7
#define numb_case_3_5_2 "3.5.2"
#define name_case_3_5_2 "T_DATA_IND event -- t_rcvv used as receive function"
#define sref_case_3_5_2 "(none)"
#define desc_case_3_5_2 "\
This test case tests the response of the t_look function to events.\n\
This test case tests the response to the T_DATA_IND TPI event."

int
test_case_3_5_2_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCV) == __RESULT_SUCCESS)
		return (__RESULT_INCONCLUSIVE);
	state++;
	if (last_t_errno != TNODATA)
		goto failure;
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DATA)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVV) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVV) == __RESULT_SUCCESS || last_t_errno != TNODATA)
		goto failure;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_3_5_2_bot(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	test_data = "Normal test data.";
	if (do_signal(child, __TEST_DATA_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_3_5_2_top preamble_3_top
#define preamble_3_5_2_bot preamble_3_bot

#define postamble_3_5_2_top postamble_3_top
#define postamble_3_5_2_bot postamble_3_bot

static struct test_stream test_3_5_2_top = { &preamble_3_5_2_top, &test_case_3_5_2_top, &postamble_3_5_2_top };
static struct test_stream test_3_5_2_bot = { &preamble_3_5_2_bot, &test_case_3_5_2_bot, &postamble_3_5_2_bot };

#define tgrp_case_3_6_1 test_group_7
#define numb_case_3_6_1 "3.6.1"
#define name_case_3_6_1 "T_UNITDATA_IND event"
#define sref_case_3_6_1 "(none)"
#define desc_case_3_6_1 "\
This test case tests the response of the t_look function to events.\n\
This test case tests the response to the T_UNITDATA_IND TPI event."

int
test_case_3_6_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVUDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNODATA)
		goto failure;
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DATA)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVUDATA) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVUDATA) == __RESULT_SUCCESS || last_t_errno != TNODATA)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_3_6_1_bot(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	test_data = "Unit test data.";
	if (do_signal(child, __TEST_UNITDATA_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_3_6_1_top preamble_2cl_top
#define preamble_3_6_1_bot preamble_2cl_bot

#define postamble_3_6_1_top postamble_2cl_top
#define postamble_3_6_1_bot postamble_2cl_bot

static struct test_stream test_3_6_1_top = { &preamble_3_6_1_top, &test_case_3_6_1_top, &postamble_3_6_1_top };
static struct test_stream test_3_6_1_bot = { &preamble_3_6_1_bot, &test_case_3_6_1_bot, &postamble_3_6_1_bot };

#define tgrp_case_3_7_1 test_group_7
#define numb_case_3_7_1 "3.7.1"
#define name_case_3_7_1 "T_UDERROR_IND event"
#define sref_case_3_7_1 "(none)"
#define desc_case_3_7_1 "\
This test case tests the response of the t_look function to events.\n\
This test case tests the response to the T_UDERROR_IND TPI event."

int
test_case_3_7_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVUDERR) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOUDERR)
		goto failure;
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_UDERR)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVUDERR) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVUDERR) == __RESULT_SUCCESS || last_t_errno != TNOUDERR)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_3_7_1_bot(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_UDERROR_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_3_7_1_top preamble_2cl_top
#define preamble_3_7_1_bot preamble_2cl_bot

#define postamble_3_7_1_top postamble_2cl_top
#define postamble_3_7_1_bot postamble_2cl_bot

static struct test_stream test_3_7_1_top = { &preamble_3_7_1_top, &test_case_3_7_1_top, &postamble_3_7_1_top };
static struct test_stream test_3_7_1_bot = { &preamble_3_7_1_bot, &test_case_3_7_1_bot, &postamble_3_7_1_bot };

#define test_group_8 "Asynchronous operation -- TLOOK response to various calls"
#define tgrp_case_4_1_1 test_group_8
#define numb_case_4_1_1 "4.1.1"
#define name_case_4_1_1 "t_accept library call"
#define sref_case_4_1_1 "(none)"
#define desc_case_4_1_1 "\
This test case tests the response of the t_accept library call to an\n\
asynchronous T_DISCONNECT event."

int
test_case_4_1_1_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_LISTEN) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, NORMAL_WAIT);
	state++;
	last_sequence = 1;
	if (do_signal(child, __TEST_T_ACCEPT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DISCONNECT)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_1_1_bot(int child)
{
	int begstate = state;

	last_sequence = 1;
	test_addr = NULL;
	test_alen = 0;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_data = "Connection indication test data.";
	if (do_signal(child, __TEST_CONN_IND) != __RESULT_SUCCESS)
		goto failure;
	state = begstate + 1;
	test_data = "Disconnect indication test data.";
	if (do_signal(child, __TEST_DISCON_IND) != __RESULT_SUCCESS)
		goto failure;
	state = begstate + 2;
	state = begstate + 3;
	for (;;) {
		switch (wait_event(child, LONG_WAIT)) {
		case __EVENT_NO_MSG:
			state++;
			return (__RESULT_SUCCESS);
		case __TEST_CONN_RES:
			if (state != begstate + 3)
				goto failure;
			state = begstate + 5;
			test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
			last_t_errno = TOUTSTATE;
			last_errno = 0;
			if (do_signal(child, __TEST_ERROR_ACK) != __RESULT_SUCCESS)
				goto failure;
			state = begstate + 6;
			return (__RESULT_SUCCESS);
		default:
			state = begstate + 7;
			goto failure;
		}
	}
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_1_1_top preamble_2_top
#define preamble_4_1_1_bot preamble_2_bot

#define postamble_4_1_1_top postamble_2_top
#define postamble_4_1_1_bot postamble_2_bot

static struct test_stream test_4_1_1_top = { &preamble_4_1_1_top, &test_case_4_1_1_top, &postamble_4_1_1_top };
static struct test_stream test_4_1_1_bot = { &preamble_4_1_1_bot, &test_case_4_1_1_bot, &postamble_4_1_1_bot };

#define tgrp_case_4_1_2 test_group_8
#define numb_case_4_1_2 "4.1.2"
#define name_case_4_1_2 "t_accept library call"
#define sref_case_4_1_2 "(none)"
#define desc_case_4_1_2 "\
This test case tests the response of the t_accept library call to an\n\
asynchronous T_LISTEN event."

int
test_case_4_1_2_top(int child)
{
	int seq;

	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_LISTEN) != __RESULT_SUCCESS)
		goto failure;
	seq = last_sequence;
	state++;
	test_msleep(child, NORMAL_WAIT);
	state++;
	last_sequence = 1;
	if (do_signal(child, __TEST_T_ACCEPT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_LISTEN)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LISTEN) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	last_sequence = seq;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_1_2_bot(int child)
{
	int begstate = state;

	last_sequence = 1;
	test_addr = NULL;
	test_alen = 0;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_data = "Connection indication test data.";
	if (do_signal(child, __TEST_CONN_IND) != __RESULT_SUCCESS)
		goto failure;
	state = begstate + 1;
	last_sequence = 2;
	test_addr = NULL;
	test_alen = 0;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_data = "Connection indication test data.";
	if (do_signal(child, __TEST_CONN_IND) != __RESULT_SUCCESS)
		goto failure;
	state = begstate + 2;
	state = begstate + 3;
	for (;;) {
		switch (wait_event(child, LONG_WAIT)) {
		case __EVENT_NO_MSG:
			state = begstate + 4;
			goto failure;
		case __TEST_CONN_RES:
			if (state != begstate + 3)
				goto failure;
			state = begstate + 5;
			test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
			last_t_errno = TOUTSTATE;
			last_errno = 0;
			if (do_signal(child, __TEST_ERROR_ACK) != __RESULT_SUCCESS)
				goto failure;
			state = begstate + 6;
			continue;
		case __TEST_DISCON_REQ:
			if (state != begstate + 8) {
				state = begstate + 7;
				test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
				if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
					goto failure;
				state = begstate + 8;
				continue;
			}
			state = begstate + 9;
			test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
			if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
				goto failure;
			state = begstate + 10;
			return (__RESULT_SUCCESS);
		default:
			state = begstate + 11;
			goto failure;
		}
	}
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_1_2_top preamble_2_top
#define preamble_4_1_2_bot preamble_2_bot

#define postamble_4_1_2_top postamble_2_top
#define postamble_4_1_2_bot postamble_2_bot

static struct test_stream test_4_1_2_top = { &preamble_4_1_2_top, &test_case_4_1_2_top, &postamble_4_1_2_top };
static struct test_stream test_4_1_2_bot = { &preamble_4_1_2_bot, &test_case_4_1_2_bot, &postamble_4_1_2_bot };

#define tgrp_case_4_2_1 test_group_8
#define numb_case_4_2_1 "4.2.1"
#define name_case_4_2_1 "t_connect library call"
#define sref_case_4_2_1 "(none)"
#define desc_case_4_2_1 "\
This test case tests the response of the t_connect library call to an\n\
asynchronous T_DISCONNECT event."

int
test_case_4_2_1_top(int child)
{
	if (do_signal(child, __TEST_T_CONNECT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DISCONNECT)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_2_1_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_CONN_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_data = "Disconnect indication test data.";
	if (do_signal(child, __TEST_DISCON_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_2_1_top preamble_2_top
#define preamble_4_2_1_bot preamble_2_bot

#define postamble_4_2_1_top postamble_2_top
#define postamble_4_2_1_bot postamble_2_bot

static struct test_stream test_4_2_1_top = { &preamble_4_2_1_top, &test_case_4_2_1_top, &postamble_4_2_1_top };
static struct test_stream test_4_2_1_bot = { &preamble_4_2_1_bot, &test_case_4_2_1_bot, &postamble_4_2_1_bot };

#define tgrp_case_4_2_2 test_group_8
#define numb_case_4_2_2 "4.2.2"
#define name_case_4_2_2 "t_connect library call"
#define sref_case_4_2_2 "(none)"
#define desc_case_4_2_2 "\
This test case tests the response of the t_connect library call to an\n\
asynchronous T_LISTEN event."

int
test_case_4_2_2_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_CONNECT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_LISTEN)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LISTEN) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_2_2_bot(int child)
{
	int begstate = state;

	last_sequence = 1;
	test_addr = NULL;
	test_alen = 0;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_data = "Connection indication test data.";
	if (do_signal(child, __TEST_CONN_IND) != __RESULT_SUCCESS)
		goto failure;
	state = begstate + 1;
	state = begstate + 2;
	for (;;) {
		switch (wait_event(child, LONG_WAIT)) {
		case __TEST_CONN_REQ:
			if (state != begstate + 2)
				goto failure;
			state = begstate + 3;
			test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
			last_t_errno = TOUTSTATE;
			last_errno = 0;
			if (do_signal(child, __TEST_ERROR_ACK) != __RESULT_SUCCESS)
				goto failure;
			state = begstate + 4;
			continue;
		case __TEST_DISCON_REQ:
			state = begstate + 5;
			test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
			if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
				goto failure;
			state = begstate + 6;
			return (__RESULT_SUCCESS);
		case __EVENT_TIMEOUT:
			state = begstate + 7;
			return (__RESULT_SUCCESS);
		default:
			state = begstate + 8;
			goto failure;
		}
	}
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_2_2_top preamble_2_top
#define preamble_4_2_2_bot preamble_2_bot

#define postamble_4_2_2_top postamble_2_top
#define postamble_4_2_2_bot postamble_2_bot

static struct test_stream test_4_2_2_top = { &preamble_4_2_2_top, &test_case_4_2_2_top, &postamble_4_2_2_top };
static struct test_stream test_4_2_2_bot = { &preamble_4_2_2_bot, &test_case_4_2_2_bot, &postamble_4_2_2_bot };

#define tgrp_case_4_3_1 test_group_8
#define numb_case_4_3_1 "4.3.1"
#define name_case_4_3_1 "t_listen library call"
#define sref_case_4_3_1 "(none)"
#define desc_case_4_3_1 "\
This test case tests the response of the t_listen library call to an\n\
asynchronous T_DISCONNECT event."

int
test_case_4_3_1_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_LISTEN) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LISTEN) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DISCONNECT)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_3_1_bot(int child)
{
	last_sequence = 1;
	test_addr = NULL;
	test_alen = 0;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_data = "Connection indication test data.";
	if (do_signal(child, __TEST_CONN_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_data = "Disconnect indication test data.";
	if (do_signal(child, __TEST_DISCON_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_3_1_top preamble_2_top
#define preamble_4_3_1_bot preamble_2_bot

#define postamble_4_3_1_top postamble_2_top
#define postamble_4_3_1_bot postamble_2_bot

static struct test_stream test_4_3_1_top = { &preamble_4_3_1_top, &test_case_4_3_1_top, &postamble_4_3_1_top };
static struct test_stream test_4_3_1_bot = { &preamble_4_3_1_bot, &test_case_4_3_1_bot, &postamble_4_3_1_bot };

#define tgrp_case_4_4_1 test_group_8
#define numb_case_4_4_1 "4.4.1"
#define name_case_4_4_1 "t_rcv library call"
#define sref_case_4_4_1 "(none)"
#define desc_case_4_4_1 "\
This test case tests the response of the t_rcv library call to an\n\
asynchronous T_DISCONNECT event."

int
test_case_4_4_1_top(int child)
{
	if (do_signal(child, __TEST_T_RCV) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DISCONNECT)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_4_1_bot(int child)
{
	test_data = "Disconnect indication test data.";
	if (do_signal(child, __TEST_DISCON_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_4_1_top preamble_3_top
#define preamble_4_4_1_bot preamble_3_bot

#define postamble_4_4_1_top postamble_2_top
#define postamble_4_4_1_bot postamble_2_bot

static struct test_stream test_4_4_1_top = { &preamble_4_4_1_top, &test_case_4_4_1_top, &postamble_4_4_1_top };
static struct test_stream test_4_4_1_bot = { &preamble_4_4_1_bot, &test_case_4_4_1_bot, &postamble_4_4_1_bot };

#define tgrp_case_4_4_2 test_group_8
#define numb_case_4_4_2 "4.4.2"
#define name_case_4_4_2 "t_rcv library call"
#define sref_case_4_4_2 "(none)"
#define desc_case_4_4_2 "\
This test case tests the response of the t_rcv library call to an\n\
asynchronous T_ORDREL event."

int
test_case_4_4_2_top(int child)
{
	if (do_signal(child, __TEST_T_RCV) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_ORDREL)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVREL) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDREL) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_4_2_bot(int child)
{
	test_data = "Orderly release indication test data.";
	if (do_signal(child, __TEST_ORDREL_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_ORDREL_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_4_2_top preamble_3_top
#define preamble_4_4_2_bot preamble_3_bot

#define postamble_4_4_2_top postamble_2_top
#define postamble_4_4_2_bot postamble_2_bot

static struct test_stream test_4_4_2_top = { &preamble_4_4_2_top, &test_case_4_4_2_top, &postamble_4_4_2_top };
static struct test_stream test_4_4_2_bot = { &preamble_4_4_2_bot, &test_case_4_4_2_bot, &postamble_4_4_2_bot };

#define tgrp_case_4_4_3 test_group_8
#define numb_case_4_4_3 "4.4.3"
#define name_case_4_4_3 "t_rcvv library call"
#define sref_case_4_4_3 "(none)"
#define desc_case_4_4_3 "\
This test case tests the response of the t_rcvv library call to an\n\
asynchronous T_DISCONNECT event."

int
test_case_4_4_3_top(int child)
{
	if (do_signal(child, __TEST_T_RCVV) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DISCONNECT)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_4_3_bot(int child)
{
	test_data = "Disconnect indication test data.";
	if (do_signal(child, __TEST_DISCON_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_4_3_top preamble_3_top
#define preamble_4_4_3_bot preamble_3_bot

#define postamble_4_4_3_top postamble_2_top
#define postamble_4_4_3_bot postamble_2_bot

static struct test_stream test_4_4_3_top = { &preamble_4_4_3_top, &test_case_4_4_3_top, &postamble_4_4_3_top };
static struct test_stream test_4_4_3_bot = { &preamble_4_4_3_bot, &test_case_4_4_3_bot, &postamble_4_4_3_bot };

#define tgrp_case_4_4_4 test_group_8
#define numb_case_4_4_4 "4.4.4"
#define name_case_4_4_4 "t_rcvv library call"
#define sref_case_4_4_4 "(none)"
#define desc_case_4_4_4 "\
This test case tests the response of the t_rcvv library call to an\n\
asynchronous T_ORDREL event."

int
test_case_4_4_4_top(int child)
{
	if (do_signal(child, __TEST_T_RCVV) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_ORDREL)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVREL) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDREL) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_4_4_bot(int child)
{
	test_data = "Orderly release indication test data.";
	if (do_signal(child, __TEST_ORDREL_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_ORDREL_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_4_4_top preamble_3_top
#define preamble_4_4_4_bot preamble_3_bot

#define postamble_4_4_4_top postamble_2_top
#define postamble_4_4_4_bot postamble_2_bot

static struct test_stream test_4_4_4_top = { &preamble_4_4_4_top, &test_case_4_4_4_top, &postamble_4_4_4_top };
static struct test_stream test_4_4_4_bot = { &preamble_4_4_4_bot, &test_case_4_4_4_bot, &postamble_4_4_4_bot };

#define tgrp_case_4_4_5 test_group_8
#define numb_case_4_4_5 "4.4.5"
#define name_case_4_4_5 "t_snd library call"
#define sref_case_4_4_5 "(none)"
#define desc_case_4_4_5 "\
This test case tests the response of the t_snd library call to an\n\
asynchronous T_DISCONNECT event."

int
test_case_4_4_5_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_SND) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DISCONNECT)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_4_5_bot(int child)
{
	test_data = "Disconnect indication test data.";
	if (do_signal(child, __TEST_DISCON_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_4_5_top preamble_3_top
#define preamble_4_4_5_bot preamble_3_bot

#define postamble_4_4_5_top postamble_2_top
#define postamble_4_4_5_bot postamble_2_bot

static struct test_stream test_4_4_5_top = { &preamble_4_4_5_top, &test_case_4_4_5_top, &postamble_4_4_5_top };
static struct test_stream test_4_4_5_bot = { &preamble_4_4_5_bot, &test_case_4_4_5_bot, &postamble_4_4_5_bot };

#define tgrp_case_4_4_6 test_group_8
#define numb_case_4_4_6 "4.4.6"
#define name_case_4_4_6 "t_snd library call"
#define sref_case_4_4_6 "(none)"
#define desc_case_4_4_6 "\
This test case tests the response of the t_snd library call to an\n\
asynchronous T_ORDREL event."

int
test_case_4_4_6_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_SND) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_ORDREL)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVREL) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDREL) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_4_6_bot(int child)
{
	test_data = "Orderly release indication test data.";
	if (do_signal(child, __TEST_ORDREL_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_ORDREL_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_4_6_top preamble_3_top
#define preamble_4_4_6_bot preamble_3_bot

#define postamble_4_4_6_top postamble_2_top
#define postamble_4_4_6_bot postamble_2_bot

static struct test_stream test_4_4_6_top = { &preamble_4_4_6_top, &test_case_4_4_6_top, &postamble_4_4_6_top };
static struct test_stream test_4_4_6_bot = { &preamble_4_4_6_bot, &test_case_4_4_6_bot, &postamble_4_4_6_bot };

#define tgrp_case_4_4_7 test_group_8
#define numb_case_4_4_7 "4.4.7"
#define name_case_4_4_7 "t_sndv library call"
#define sref_case_4_4_7 "(none)"
#define desc_case_4_4_7 "\
This test case tests the response of the t_sndv library call to an\n\
asynchronous T_DISCONNECT event."

int
test_case_4_4_7_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_SNDV) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DISCONNECT)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_4_7_bot(int child)
{
	test_data = "Disconnect indication test data.";
	if (do_signal(child, __TEST_DISCON_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_4_7_top preamble_3_top
#define preamble_4_4_7_bot preamble_3_bot

#define postamble_4_4_7_top postamble_2_top
#define postamble_4_4_7_bot postamble_2_bot

static struct test_stream test_4_4_7_top = { &preamble_4_4_7_top, &test_case_4_4_7_top, &postamble_4_4_7_top };
static struct test_stream test_4_4_7_bot = { &preamble_4_4_7_bot, &test_case_4_4_7_bot, &postamble_4_4_7_bot };

#define tgrp_case_4_4_8 test_group_8
#define numb_case_4_4_8 "4.4.8"
#define name_case_4_4_8 "t_sndv library call"
#define sref_case_4_4_8 "(none)"
#define desc_case_4_4_8 "\
This test case tests the response of the t_sndv library call to an\n\
asynchronous T_ORDREL event."

int
test_case_4_4_8_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_SNDV) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_ORDREL)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVREL) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDREL) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_4_8_bot(int child)
{
	test_data = "Orderly release indication test data.";
	if (do_signal(child, __TEST_ORDREL_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_ORDREL_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_4_8_top preamble_3_top
#define preamble_4_4_8_bot preamble_3_bot

#define postamble_4_4_8_top postamble_2_top
#define postamble_4_4_8_bot postamble_2_bot

static struct test_stream test_4_4_8_top = { &preamble_4_4_8_top, &test_case_4_4_8_top, &postamble_4_4_8_top };
static struct test_stream test_4_4_8_bot = { &preamble_4_4_8_bot, &test_case_4_4_8_bot, &postamble_4_4_8_bot };

#define tgrp_case_4_5_1 test_group_8
#define numb_case_4_5_1 "4.5.1"
#define name_case_4_5_1 "t_rcvconnect library call"
#define sref_case_4_5_1 "(none)"
#define desc_case_4_5_1 "\
This test case tests the response of the t_rcvconnect library call to an\n\
asynchronous T_DISCONNECT event."

int
test_case_4_5_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_CONNECT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNODATA)
		goto failure;
	state++;
	test_msleep(child, LONG_WAIT);
	state++;
	if (do_signal(child, __TEST_T_RCVCONNECT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DISCONNECT)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_5_1_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_CONN_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, NORMAL_WAIT);
	state++;
	test_data = "Disconnect indication test data.";
	if (do_signal(child, __TEST_DISCON_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_5_1_top preamble_2_top
#define preamble_4_5_1_bot preamble_2_bot

#define postamble_4_5_1_top postamble_2_top
#define postamble_4_5_1_bot postamble_2_bot

static struct test_stream test_4_5_1_top = { &preamble_4_5_1_top, &test_case_4_5_1_top, &postamble_4_5_1_top };
static struct test_stream test_4_5_1_bot = { &preamble_4_5_1_bot, &test_case_4_5_1_bot, &postamble_4_5_1_bot };

#define tgrp_case_4_6_1 test_group_8
#define numb_case_4_6_1 "4.6.1"
#define name_case_4_6_1 "t_rcvrel library call"
#define sref_case_4_6_1 "(none)"
#define desc_case_4_6_1 "\
This test case tests the response of the t_rcvrel library call to an\n\
asynchronous T_DISCONNECT event."

int
test_case_4_6_1_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_RCVREL) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DISCONNECT)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_6_1_bot(int child)
{
	test_data = "Disconnect indication test data.";
	if (do_signal(child, __TEST_DISCON_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_6_1_top preamble_3_top
#define preamble_4_6_1_bot preamble_3_bot

#define postamble_4_6_1_top postamble_2_top
#define postamble_4_6_1_bot postamble_2_bot

static struct test_stream test_4_6_1_top = { &preamble_4_6_1_top, &test_case_4_6_1_top, &postamble_4_6_1_top };
static struct test_stream test_4_6_1_bot = { &preamble_4_6_1_bot, &test_case_4_6_1_bot, &postamble_4_6_1_bot };

#define tgrp_case_4_6_2 test_group_8
#define numb_case_4_6_2 "4.6.2"
#define name_case_4_6_2 "t_rcvreldata library call"
#define sref_case_4_6_2 "(none)"
#define desc_case_4_6_2 "\
This test case tests the response of the t_rcvreldata library call to an\n\
asynchronous T_DISCONNECT event."

int
test_case_4_6_2_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_RCVRELDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DISCONNECT)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_6_2_bot(int child)
{
	test_data = "Disconnect indication test data.";
	if (do_signal(child, __TEST_DISCON_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_6_2_top preamble_3_top
#define preamble_4_6_2_bot preamble_3_bot

#define postamble_4_6_2_top postamble_2_top
#define postamble_4_6_2_bot postamble_2_bot

static struct test_stream test_4_6_2_top = { &preamble_4_6_2_top, &test_case_4_6_2_top, &postamble_4_6_2_top };
static struct test_stream test_4_6_2_bot = { &preamble_4_6_2_bot, &test_case_4_6_2_bot, &postamble_4_6_2_bot };

#define tgrp_case_4_6_3 test_group_8
#define numb_case_4_6_3 "4.6.3"
#define name_case_4_6_3 "t_sndrel library call"
#define sref_case_4_6_3 "(none)"
#define desc_case_4_6_3 "\
This test case tests the response of the t_sndrel library call to an\n\
asynchronous T_DISCONNECT event."

int
test_case_4_6_3_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_SNDREL) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DISCONNECT)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_6_3_bot(int child)
{
	test_data = "Disconnect indication test data.";
	if (do_signal(child, __TEST_DISCON_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_6_3_top preamble_3_top
#define preamble_4_6_3_bot preamble_3_bot

#define postamble_4_6_3_top postamble_2_top
#define postamble_4_6_3_bot postamble_2_bot

static struct test_stream test_4_6_3_top = { &preamble_4_6_3_top, &test_case_4_6_3_top, &postamble_4_6_3_top };
static struct test_stream test_4_6_3_bot = { &preamble_4_6_3_bot, &test_case_4_6_3_bot, &postamble_4_6_3_bot };

#define tgrp_case_4_6_4 test_group_8
#define numb_case_4_6_4 "4.6.4"
#define name_case_4_6_4 "t_sndreldata library call"
#define sref_case_4_6_4 "(none)"
#define desc_case_4_6_4 "\
This test case tests the response of the t_sndreldata library call to an\n\
asynchronous T_DISCONNECT event."

int
test_case_4_6_4_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_SNDRELDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DISCONNECT)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_6_4_bot(int child)
{
	test_data = "Disconnect indication test data.";
	if (do_signal(child, __TEST_DISCON_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_6_4_top preamble_3_top
#define preamble_4_6_4_bot preamble_3_bot

#define postamble_4_6_4_top postamble_2_top
#define postamble_4_6_4_bot postamble_2_bot

static struct test_stream test_4_6_4_top = { &preamble_4_6_4_top, &test_case_4_6_4_top, &postamble_4_6_4_top };
static struct test_stream test_4_6_4_bot = { &preamble_4_6_4_bot, &test_case_4_6_4_bot, &postamble_4_6_4_bot };

#define tgrp_case_4_7_1 test_group_8
#define numb_case_4_7_1 "4.7.1"
#define name_case_4_7_1 "t_rcvudata library call"
#define sref_case_4_7_1 "(none)"
#define desc_case_4_7_1 "\
This test case tests the response of the t_rcvudata library call to an\n\
asynchronous T_UDERR event."

int
test_case_4_7_1_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_RCVUDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_UDERR)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVUDERR) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_7_1_bot(int child)
{
	if (do_signal(child, __TEST_UDERROR_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_7_1_top preamble_2cl_top
#define preamble_4_7_1_bot preamble_2cl_bot

#define postamble_4_7_1_top postamble_2cl_top
#define postamble_4_7_1_bot postamble_2cl_bot

static struct test_stream test_4_7_1_top = { &preamble_4_7_1_top, &test_case_4_7_1_top, &postamble_4_7_1_top };
static struct test_stream test_4_7_1_bot = { &preamble_4_7_1_bot, &test_case_4_7_1_bot, &postamble_4_7_1_bot };

#define tgrp_case_4_7_2 test_group_8
#define numb_case_4_7_2 "4.7.2"
#define name_case_4_7_2 "t_rcvvudata library call"
#define sref_case_4_7_2 "(none)"
#define desc_case_4_7_2 "\
This test case tests the response of the t_rcvvudata library call to an\n\
asynchronous T_UDERR event."

int
test_case_4_7_2_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_RCVVUDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_UDERR)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVUDERR) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_7_2_bot(int child)
{
	if (do_signal(child, __TEST_UDERROR_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_7_2_top preamble_2cl_top
#define preamble_4_7_2_bot preamble_2cl_bot

#define postamble_4_7_2_top postamble_2cl_top
#define postamble_4_7_2_bot postamble_2cl_bot

static struct test_stream test_4_7_2_top = { &preamble_4_7_2_top, &test_case_4_7_2_top, &postamble_4_7_2_top };
static struct test_stream test_4_7_2_bot = { &preamble_4_7_2_bot, &test_case_4_7_2_bot, &postamble_4_7_2_bot };

#define tgrp_case_4_7_3 test_group_8
#define numb_case_4_7_3 "4.7.3"
#define name_case_4_7_3 "t_sndudata library call"
#define sref_case_4_7_3 "(none)"
#define desc_case_4_7_3 "\
This test case tests the response of the t_sndudata library call to an\n\
asynchronous T_UDERR event."

int
test_case_4_7_3_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_SNDUDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_UDERR)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVUDERR) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_7_3_bot(int child)
{
	if (do_signal(child, __TEST_UDERROR_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_7_3_top preamble_2cl_top
#define preamble_4_7_3_bot preamble_2cl_bot

#define postamble_4_7_3_top postamble_2cl_top
#define postamble_4_7_3_bot postamble_2cl_bot

static struct test_stream test_4_7_3_top = { &preamble_4_7_3_top, &test_case_4_7_3_top, &postamble_4_7_3_top };
static struct test_stream test_4_7_3_bot = { &preamble_4_7_3_bot, &test_case_4_7_3_bot, &postamble_4_7_3_bot };

#define tgrp_case_4_7_4 test_group_8
#define numb_case_4_7_4 "4.7.4"
#define name_case_4_7_4 "t_sndvudata library call"
#define sref_case_4_7_4 "(none)"
#define desc_case_4_7_4 "\
This test case tests the response of the t_sndvudata library call to an\n\
asynchronous T_UDERR event."

int
test_case_4_7_4_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_SNDVUDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_UDERR)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVUDERR) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_7_4_bot(int child)
{
	if (do_signal(child, __TEST_UDERROR_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_7_4_top preamble_2cl_top
#define preamble_4_7_4_bot preamble_2cl_bot

#define postamble_4_7_4_top postamble_2cl_top
#define postamble_4_7_4_bot postamble_2cl_bot

static struct test_stream test_4_7_4_top = { &preamble_4_7_4_top, &test_case_4_7_4_top, &postamble_4_7_4_top };
static struct test_stream test_4_7_4_bot = { &preamble_4_7_4_bot, &test_case_4_7_4_bot, &postamble_4_7_4_bot };

#define tgrp_case_4_8_1 test_group_8
#define numb_case_4_8_1 "4.8.1"
#define name_case_4_8_1 "t_unbind library call"
#define sref_case_4_8_1 "(none)"
#define desc_case_4_8_1 "\
This test case tests the response of the t_unbind library call to an\n\
asynchronous T_LISTEN event."

int
test_case_4_8_1_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_UNBIND) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_LISTEN)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LISTEN) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_8_1_bot(int child)
{
	last_sequence = 1;
	test_addr = NULL;
	test_alen = 0;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_data = "Connection indication test data.";
	if (do_signal(child, __TEST_CONN_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_DISCON_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_8_1_top preamble_2_top
#define preamble_4_8_1_bot preamble_2_bot

#define postamble_4_8_1_top postamble_2_top
#define postamble_4_8_1_bot postamble_2_bot

static struct test_stream test_4_8_1_top = { &preamble_4_8_1_top, &test_case_4_8_1_top, &postamble_4_8_1_top };
static struct test_stream test_4_8_1_bot = { &preamble_4_8_1_bot, &test_case_4_8_1_bot, &postamble_4_8_1_bot };

#define tgrp_case_4_8_2 test_group_8
#define numb_case_4_8_2 "4.8.2"
#define name_case_4_8_2 "t_unbind library call"
#define sref_case_4_8_2 "(none)"
#define desc_case_4_8_2 "\
This test case tests the response of the t_unbind library call to an\n\
asynchronous T_DATA event."

int
test_case_4_8_2_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_UNBIND) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DATA)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVUDATA) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_8_2_bot(int child)
{
	test_data = "Unit test data.";
	if (do_signal(child, __TEST_UNITDATA_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_8_2_top preamble_2cl_top
#define preamble_4_8_2_bot preamble_2cl_bot

#define postamble_4_8_2_top postamble_2cl_top
#define postamble_4_8_2_bot postamble_2cl_bot

static struct test_stream test_4_8_2_top = { &preamble_4_8_2_top, &test_case_4_8_2_top, &postamble_4_8_2_top };
static struct test_stream test_4_8_2_bot = { &preamble_4_8_2_bot, &test_case_4_8_2_bot, &postamble_4_8_2_bot };

#define tgrp_case_4_8_3 test_group_8
#define numb_case_4_8_3 "4.8.3"
#define name_case_4_8_3 "t_unbind library call"
#define sref_case_4_8_3 "(none)"
#define desc_case_4_8_3 "\
This test case tests the response of the t_unbind library call to an\n\
asynchronous T_UDERR event."

int
test_case_4_8_3_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_UNBIND) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_UDERR)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVUDERR) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_8_3_bot(int child)
{
	if (do_signal(child, __TEST_UDERROR_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_8_3_top preamble_2cl_top
#define preamble_4_8_3_bot preamble_2cl_bot

#define postamble_4_8_3_top postamble_2cl_top
#define postamble_4_8_3_bot postamble_2cl_bot

static struct test_stream test_4_8_3_top = { &preamble_4_8_3_top, &test_case_4_8_3_top, &postamble_4_8_3_top };
static struct test_stream test_4_8_3_bot = { &preamble_4_8_3_bot, &test_case_4_8_3_bot, &postamble_4_8_3_bot };

#define tgrp_case_4_9_1 test_group_8
#define numb_case_4_9_1 "4.9.1"
#define name_case_4_9_1 "t_snddis library call"
#define sref_case_4_9_1 "(none)"
#define desc_case_4_9_1 "\
This test case tests the response of the t_snddis library call to an\n\
asynchronous T_DISCONNECT event."

int
test_case_4_9_1_top(int child)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_SNDDIS) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TLOOK)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LOOK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_tevent != T_DISCONNECT)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_4_9_1_bot(int child)
{
	test_data = "Disconnect indication test data.";
	if (do_signal(child, __TEST_DISCON_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_4_9_1_top preamble_3_top
#define preamble_4_9_1_bot preamble_3_bot

#define postamble_4_9_1_top postamble_2_top
#define postamble_4_9_1_bot postamble_2_bot

static struct test_stream test_4_9_1_top = { &preamble_4_9_1_top, &test_case_4_9_1_top, &postamble_4_9_1_top };
static struct test_stream test_4_9_1_bot = { &preamble_4_9_1_bot, &test_case_4_9_1_bot, &postamble_4_9_1_bot };

#define test_group_9 "Operations on a closed file descriptor"
#define tgrp_case_5_1_1 test_group_9
#define numb_case_5_1_1 "5.1.1"
#define name_case_5_1_1 "t_accept operation"
#define sref_case_5_1_1 "(none)"
#define desc_case_5_1_1 "\
This test case tests the t_accept operation on a closed file descriptor."

int
test_case_5_1_x_top(int child, int function)
{
	int old_fd = test_fd[child];

	close(10);
	state++;
	test_fd[child] = 10;
	if (do_signal(child, function) == __RESULT_SUCCESS || last_t_errno != TBADF)
		goto failure;
	test_fd[child] = old_fd;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_5_1_x_bot(int child, int function)
{
	return (__RESULT_SUCCESS);
}

int
test_case_5_1_1_top(int child)
{
	last_sequence = 1;
	return test_case_5_1_x_top(child, __TEST_T_ACCEPT);
}

int
test_case_5_1_1_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_ACCEPT);
}

#define preamble_5_1_1_top preamble_0
#define preamble_5_1_1_bot preamble_0

#define postamble_5_1_1_top postamble_0
#define postamble_5_1_1_bot postamble_0

static struct test_stream test_5_1_1_top = { &preamble_5_1_1_top, &test_case_5_1_1_top, &postamble_5_1_1_top };
static struct test_stream test_5_1_1_bot = { &preamble_5_1_1_bot, &test_case_5_1_1_bot, &postamble_5_1_1_bot };

#define tgrp_case_5_1_2 test_group_9
#define numb_case_5_1_2 "5.1.2"
#define name_case_5_1_2 "t_bind operation"
#define sref_case_5_1_2 "(none)"
#define desc_case_5_1_2 "\
This test case tests the t_bind operation on a closed file descriptor."

int
test_case_5_1_2_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_BIND);
}

int
test_case_5_1_2_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_BIND);
}

#define preamble_5_1_2_top preamble_0
#define preamble_5_1_2_bot preamble_0

#define postamble_5_1_2_top postamble_0
#define postamble_5_1_2_bot postamble_0

static struct test_stream test_5_1_2_top = { &preamble_5_1_2_top, &test_case_5_1_2_top, &postamble_5_1_2_top };
static struct test_stream test_5_1_2_bot = { &preamble_5_1_2_bot, &test_case_5_1_2_bot, &postamble_5_1_2_bot };

#define tgrp_case_5_1_3 test_group_9
#define numb_case_5_1_3 "5.1.3"
#define name_case_5_1_3 "t_close operation"
#define sref_case_5_1_3 "(none)"
#define desc_case_5_1_3 "\
This test case tests the t_close operation on a closed file descriptor."

int
test_case_5_1_3_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_CLOSE);
}

int
test_case_5_1_3_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_CLOSE);
}

#define preamble_5_1_3_top preamble_0
#define preamble_5_1_3_bot preamble_0

#define postamble_5_1_3_top postamble_0
#define postamble_5_1_3_bot postamble_0

static struct test_stream test_5_1_3_top = { &preamble_5_1_3_top, &test_case_5_1_3_top, &postamble_5_1_3_top };
static struct test_stream test_5_1_3_bot = { &preamble_5_1_3_bot, &test_case_5_1_3_bot, &postamble_5_1_3_bot };

#define tgrp_case_5_1_4 test_group_9
#define numb_case_5_1_4 "5.1.4"
#define name_case_5_1_4 "t_connect operation"
#define sref_case_5_1_4 "(none)"
#define desc_case_5_1_4 "\
This test case tests the t_connect operation on a closed file descriptor."

int
test_case_5_1_4_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_CONNECT);
}

int
test_case_5_1_4_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_CONNECT);
}

#define preamble_5_1_4_top preamble_0
#define preamble_5_1_4_bot preamble_0

#define postamble_5_1_4_top postamble_0
#define postamble_5_1_4_bot postamble_0

static struct test_stream test_5_1_4_top = { &preamble_5_1_4_top, &test_case_5_1_4_top, &postamble_5_1_4_top };
static struct test_stream test_5_1_4_bot = { &preamble_5_1_4_bot, &test_case_5_1_4_bot, &postamble_5_1_4_bot };

#define tgrp_case_5_1_5 test_group_9
#define numb_case_5_1_5 "5.1.5"
#define name_case_5_1_5 "t_getinfo operation"
#define sref_case_5_1_5 "(none)"
#define desc_case_5_1_5 "\
This test case tests the t_getinfo operation on a closed file descriptor."

int
test_case_5_1_5_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_GETINFO);
}

int
test_case_5_1_5_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_GETINFO);
}

#define preamble_5_1_5_top preamble_0
#define preamble_5_1_5_bot preamble_0

#define postamble_5_1_5_top postamble_0
#define postamble_5_1_5_bot postamble_0

static struct test_stream test_5_1_5_top = { &preamble_5_1_5_top, &test_case_5_1_5_top, &postamble_5_1_5_top };
static struct test_stream test_5_1_5_bot = { &preamble_5_1_5_bot, &test_case_5_1_5_bot, &postamble_5_1_5_bot };

#define tgrp_case_5_1_6 test_group_9
#define numb_case_5_1_6 "5.1.6"
#define name_case_5_1_6 "t_getprotaddr operation"
#define sref_case_5_1_6 "(none)"
#define desc_case_5_1_6 "\
This test case tests the t_getprotaddr operation on a closed file descriptor."

int
test_case_5_1_6_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_GETPROTADDR);
}

int
test_case_5_1_6_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_GETPROTADDR);
}

#define preamble_5_1_6_top preamble_0
#define preamble_5_1_6_bot preamble_0

#define postamble_5_1_6_top postamble_0
#define postamble_5_1_6_bot postamble_0

static struct test_stream test_5_1_6_top = { &preamble_5_1_6_top, &test_case_5_1_6_top, &postamble_5_1_6_top };
static struct test_stream test_5_1_6_bot = { &preamble_5_1_6_bot, &test_case_5_1_6_bot, &postamble_5_1_6_bot };

#define tgrp_case_5_1_7 test_group_9
#define numb_case_5_1_7 "5.1.7"
#define name_case_5_1_7 "t_getstate operation"
#define sref_case_5_1_7 "(none)"
#define desc_case_5_1_7 "\
This test case tests the t_getstate operation on a closed file descriptor."

int
test_case_5_1_7_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_GETSTATE);
}

int
test_case_5_1_7_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_GETSTATE);
}

#define preamble_5_1_7_top preamble_0
#define preamble_5_1_7_bot preamble_0

#define postamble_5_1_7_top postamble_0
#define postamble_5_1_7_bot postamble_0

static struct test_stream test_5_1_7_top = { &preamble_5_1_7_top, &test_case_5_1_7_top, &postamble_5_1_7_top };
static struct test_stream test_5_1_7_bot = { &preamble_5_1_7_bot, &test_case_5_1_7_bot, &postamble_5_1_7_bot };

#define tgrp_case_5_1_8 test_group_9
#define numb_case_5_1_8 "5.1.8"
#define name_case_5_1_8 "t_listen operation"
#define sref_case_5_1_8 "(none)"
#define desc_case_5_1_8 "\
This test case tests the t_listen operation on a closed file descriptor."

int
test_case_5_1_8_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_LISTEN);
}

int
test_case_5_1_8_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_LISTEN);
}

#define preamble_5_1_8_top preamble_0
#define preamble_5_1_8_bot preamble_0

#define postamble_5_1_8_top postamble_0
#define postamble_5_1_8_bot postamble_0

static struct test_stream test_5_1_8_top = { &preamble_5_1_8_top, &test_case_5_1_8_top, &postamble_5_1_8_top };
static struct test_stream test_5_1_8_bot = { &preamble_5_1_8_bot, &test_case_5_1_8_bot, &postamble_5_1_8_bot };

#define tgrp_case_5_1_9 test_group_9
#define numb_case_5_1_9 "5.1.9"
#define name_case_5_1_9 "t_look operation"
#define sref_case_5_1_9 "(none)"
#define desc_case_5_1_9 "\
This test case tests the t_look operation on a closed file descriptor."

int
test_case_5_1_9_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_LOOK);
}

int
test_case_5_1_9_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_LOOK);
}

#define preamble_5_1_9_top preamble_0
#define preamble_5_1_9_bot preamble_0

#define postamble_5_1_9_top postamble_0
#define postamble_5_1_9_bot postamble_0

static struct test_stream test_5_1_9_top = { &preamble_5_1_9_top, &test_case_5_1_9_top, &postamble_5_1_9_top };
static struct test_stream test_5_1_9_bot = { &preamble_5_1_9_bot, &test_case_5_1_9_bot, &postamble_5_1_9_bot };

#define tgrp_case_5_1_10 test_group_9
#define numb_case_5_1_10 "5.1.10"
#define name_case_5_1_10 "t_optmgmt operation"
#define sref_case_5_1_10 "(none)"
#define desc_case_5_1_10 "\
This test case tests the t_optmgmt operation on a closed file descriptor."

int
test_case_5_1_10_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_OPTMGMT);
}

int
test_case_5_1_10_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_OPTMGMT);
}

#define preamble_5_1_10_top preamble_0
#define preamble_5_1_10_bot preamble_0

#define postamble_5_1_10_top postamble_0
#define postamble_5_1_10_bot postamble_0

static struct test_stream test_5_1_10_top = { &preamble_5_1_10_top, &test_case_5_1_10_top, &postamble_5_1_10_top };
static struct test_stream test_5_1_10_bot = { &preamble_5_1_10_bot, &test_case_5_1_10_bot, &postamble_5_1_10_bot };

#define tgrp_case_5_1_11 test_group_9
#define numb_case_5_1_11 "5.1.11"
#define name_case_5_1_11 "t_rcv operation"
#define sref_case_5_1_11 "(none)"
#define desc_case_5_1_11 "\
This test case tests the t_rcv operation on a closed file descriptor."

int
test_case_5_1_11_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_RCV);
}

int
test_case_5_1_11_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_RCV);
}

#define preamble_5_1_11_top preamble_0
#define preamble_5_1_11_bot preamble_0

#define postamble_5_1_11_top postamble_0
#define postamble_5_1_11_bot postamble_0

static struct test_stream test_5_1_11_top = { &preamble_5_1_11_top, &test_case_5_1_11_top, &postamble_5_1_11_top };
static struct test_stream test_5_1_11_bot = { &preamble_5_1_11_bot, &test_case_5_1_11_bot, &postamble_5_1_11_bot };

#define tgrp_case_5_1_12 test_group_9
#define numb_case_5_1_12 "5.1.12"
#define name_case_5_1_12 "t_rcvconnect operation"
#define sref_case_5_1_12 "(none)"
#define desc_case_5_1_12 "\
This test case tests the t_rcvconnect operation on a closed file descriptor."

int
test_case_5_1_12_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_RCVCONNECT);
}

int
test_case_5_1_12_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_RCVCONNECT);
}

#define preamble_5_1_12_top preamble_0
#define preamble_5_1_12_bot preamble_0

#define postamble_5_1_12_top postamble_0
#define postamble_5_1_12_bot postamble_0

static struct test_stream test_5_1_12_top = { &preamble_5_1_12_top, &test_case_5_1_12_top, &postamble_5_1_12_top };
static struct test_stream test_5_1_12_bot = { &preamble_5_1_12_bot, &test_case_5_1_12_bot, &postamble_5_1_12_bot };

#define tgrp_case_5_1_13 test_group_9
#define numb_case_5_1_13 "5.1.13"
#define name_case_5_1_13 "t_rcvdis operation"
#define sref_case_5_1_13 "(none)"
#define desc_case_5_1_13 "\
This test case tests the t_rcvdis operation on a closed file descriptor."

int
test_case_5_1_13_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_RCVDIS);
}

int
test_case_5_1_13_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_RCVDIS);
}

#define preamble_5_1_13_top preamble_0
#define preamble_5_1_13_bot preamble_0

#define postamble_5_1_13_top postamble_0
#define postamble_5_1_13_bot postamble_0

static struct test_stream test_5_1_13_top = { &preamble_5_1_13_top, &test_case_5_1_13_top, &postamble_5_1_13_top };
static struct test_stream test_5_1_13_bot = { &preamble_5_1_13_bot, &test_case_5_1_13_bot, &postamble_5_1_13_bot };

#define tgrp_case_5_1_14 test_group_9
#define numb_case_5_1_14 "5.1.14"
#define name_case_5_1_14 "t_rcvrel operation"
#define sref_case_5_1_14 "(none)"
#define desc_case_5_1_14 "\
This test case tests the t_rcvrel operation on a closed file descriptor."

int
test_case_5_1_14_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_RCVREL);
}

int
test_case_5_1_14_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_RCVREL);
}

#define preamble_5_1_14_top preamble_0
#define preamble_5_1_14_bot preamble_0

#define postamble_5_1_14_top postamble_0
#define postamble_5_1_14_bot postamble_0

static struct test_stream test_5_1_14_top = { &preamble_5_1_14_top, &test_case_5_1_14_top, &postamble_5_1_14_top };
static struct test_stream test_5_1_14_bot = { &preamble_5_1_14_bot, &test_case_5_1_14_bot, &postamble_5_1_14_bot };

#define tgrp_case_5_1_15 test_group_9
#define numb_case_5_1_15 "5.1.15"
#define name_case_5_1_15 "t_rcvreldata operation"
#define sref_case_5_1_15 "(none)"
#define desc_case_5_1_15 "\
This test case tests the t_rcvreldata operation on a closed file descriptor."

int
test_case_5_1_15_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_RCVRELDATA);
}

int
test_case_5_1_15_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_RCVRELDATA);
}

#define preamble_5_1_15_top preamble_0
#define preamble_5_1_15_bot preamble_0

#define postamble_5_1_15_top postamble_0
#define postamble_5_1_15_bot postamble_0

static struct test_stream test_5_1_15_top = { &preamble_5_1_15_top, &test_case_5_1_15_top, &postamble_5_1_15_top };
static struct test_stream test_5_1_15_bot = { &preamble_5_1_15_bot, &test_case_5_1_15_bot, &postamble_5_1_15_bot };

#define tgrp_case_5_1_16 test_group_9
#define numb_case_5_1_16 "5.1.16"
#define name_case_5_1_16 "t_rcvudata operation"
#define sref_case_5_1_16 "(none)"
#define desc_case_5_1_16 "\
This test case tests the t_rcvudata operation on a closed file descriptor."

int
test_case_5_1_16_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_RCVUDATA);
}

int
test_case_5_1_16_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_RCVUDATA);
}

#define preamble_5_1_16_top preamble_0
#define preamble_5_1_16_bot preamble_0

#define postamble_5_1_16_top postamble_0
#define postamble_5_1_16_bot postamble_0

static struct test_stream test_5_1_16_top = { &preamble_5_1_16_top, &test_case_5_1_16_top, &postamble_5_1_16_top };
static struct test_stream test_5_1_16_bot = { &preamble_5_1_16_bot, &test_case_5_1_16_bot, &postamble_5_1_16_bot };

#define tgrp_case_5_1_17 test_group_9
#define numb_case_5_1_17 "5.1.17"
#define name_case_5_1_17 "t_rcvuderr operation"
#define sref_case_5_1_17 "(none)"
#define desc_case_5_1_17 "\
This test case tests the t_rcvuderr operation on a closed file descriptor."

int
test_case_5_1_17_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_RCVUDERR);
}

int
test_case_5_1_17_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_RCVUDERR);
}

#define preamble_5_1_17_top preamble_0
#define preamble_5_1_17_bot preamble_0

#define postamble_5_1_17_top postamble_0
#define postamble_5_1_17_bot postamble_0

static struct test_stream test_5_1_17_top = { &preamble_5_1_17_top, &test_case_5_1_17_top, &postamble_5_1_17_top };
static struct test_stream test_5_1_17_bot = { &preamble_5_1_17_bot, &test_case_5_1_17_bot, &postamble_5_1_17_bot };

#define tgrp_case_5_1_18 test_group_9
#define numb_case_5_1_18 "5.1.18"
#define name_case_5_1_18 "t_rcvv operation"
#define sref_case_5_1_18 "(none)"
#define desc_case_5_1_18 "\
This test case tests the t_rcvv operation on a closed file descriptor."

int
test_case_5_1_18_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_RCVV);
}

int
test_case_5_1_18_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_RCVV);
}

#define preamble_5_1_18_top preamble_0
#define preamble_5_1_18_bot preamble_0

#define postamble_5_1_18_top postamble_0
#define postamble_5_1_18_bot postamble_0

static struct test_stream test_5_1_18_top = { &preamble_5_1_18_top, &test_case_5_1_18_top, &postamble_5_1_18_top };
static struct test_stream test_5_1_18_bot = { &preamble_5_1_18_bot, &test_case_5_1_18_bot, &postamble_5_1_18_bot };

#define tgrp_case_5_1_19 test_group_9
#define numb_case_5_1_19 "5.1.19"
#define name_case_5_1_19 "t_rcvvudata operation"
#define sref_case_5_1_19 "(none)"
#define desc_case_5_1_19 "\
This test case tests the t_rcvvudata operation on a closed file descriptor."

int
test_case_5_1_19_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_RCVVUDATA);
}

int
test_case_5_1_19_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_RCVVUDATA);
}

#define preamble_5_1_19_top preamble_0
#define preamble_5_1_19_bot preamble_0

#define postamble_5_1_19_top postamble_0
#define postamble_5_1_19_bot postamble_0

static struct test_stream test_5_1_19_top = { &preamble_5_1_19_top, &test_case_5_1_19_top, &postamble_5_1_19_top };
static struct test_stream test_5_1_19_bot = { &preamble_5_1_19_bot, &test_case_5_1_19_bot, &postamble_5_1_19_bot };

#define tgrp_case_5_1_20 test_group_9
#define numb_case_5_1_20 "5.1.20"
#define name_case_5_1_20 "t_snd operation"
#define sref_case_5_1_20 "(none)"
#define desc_case_5_1_20 "\
This test case tests the t_snd operation on a closed file descriptor."

int
test_case_5_1_20_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_SND);
}

int
test_case_5_1_20_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_SND);
}

#define preamble_5_1_20_top preamble_0
#define preamble_5_1_20_bot preamble_0

#define postamble_5_1_20_top postamble_0
#define postamble_5_1_20_bot postamble_0

static struct test_stream test_5_1_20_top = { &preamble_5_1_20_top, &test_case_5_1_20_top, &postamble_5_1_20_top };
static struct test_stream test_5_1_20_bot = { &preamble_5_1_20_bot, &test_case_5_1_20_bot, &postamble_5_1_20_bot };

#define tgrp_case_5_1_21 test_group_9
#define numb_case_5_1_21 "5.1.21"
#define name_case_5_1_21 "t_snddis operation"
#define sref_case_5_1_21 "(none)"
#define desc_case_5_1_21 "\
This test case tests the t_snddis operation on a closed file descriptor."

int
test_case_5_1_21_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_SNDDIS);
}

int
test_case_5_1_21_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_SNDDIS);
}

#define preamble_5_1_21_top preamble_0
#define preamble_5_1_21_bot preamble_0

#define postamble_5_1_21_top postamble_0
#define postamble_5_1_21_bot postamble_0

static struct test_stream test_5_1_21_top = { &preamble_5_1_21_top, &test_case_5_1_21_top, &postamble_5_1_21_top };
static struct test_stream test_5_1_21_bot = { &preamble_5_1_21_bot, &test_case_5_1_21_bot, &postamble_5_1_21_bot };

#define tgrp_case_5_1_22 test_group_9
#define numb_case_5_1_22 "5.1.22"
#define name_case_5_1_22 "t_sndrel operation"
#define sref_case_5_1_22 "(none)"
#define desc_case_5_1_22 "\
This test case tests the t_sndrel operation on a closed file descriptor."

int
test_case_5_1_22_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_SNDREL);
}

int
test_case_5_1_22_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_SNDREL);
}

#define preamble_5_1_22_top preamble_0
#define preamble_5_1_22_bot preamble_0

#define postamble_5_1_22_top postamble_0
#define postamble_5_1_22_bot postamble_0

static struct test_stream test_5_1_22_top = { &preamble_5_1_22_top, &test_case_5_1_22_top, &postamble_5_1_22_top };
static struct test_stream test_5_1_22_bot = { &preamble_5_1_22_bot, &test_case_5_1_22_bot, &postamble_5_1_22_bot };

#define tgrp_case_5_1_23 test_group_9
#define numb_case_5_1_23 "5.1.23"
#define name_case_5_1_23 "t_sndreldata operation"
#define sref_case_5_1_23 "(none)"
#define desc_case_5_1_23 "\
This test case tests the t_sndreldata operation on a closed file descriptor."

int
test_case_5_1_23_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_SNDRELDATA);
}

int
test_case_5_1_23_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_SNDRELDATA);
}

#define preamble_5_1_23_top preamble_0
#define preamble_5_1_23_bot preamble_0

#define postamble_5_1_23_top postamble_0
#define postamble_5_1_23_bot postamble_0

static struct test_stream test_5_1_23_top = { &preamble_5_1_23_top, &test_case_5_1_23_top, &postamble_5_1_23_top };
static struct test_stream test_5_1_23_bot = { &preamble_5_1_23_bot, &test_case_5_1_23_bot, &postamble_5_1_23_bot };

#define tgrp_case_5_1_24 test_group_9
#define numb_case_5_1_24 "5.1.24"
#define name_case_5_1_24 "t_sndudata operation"
#define sref_case_5_1_24 "(none)"
#define desc_case_5_1_24 "\
This test case tests the t_sndudata operation on a closed file descriptor."

int
test_case_5_1_24_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_SNDUDATA);
}

int
test_case_5_1_24_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_SNDUDATA);
}

#define preamble_5_1_24_top preamble_0
#define preamble_5_1_24_bot preamble_0

#define postamble_5_1_24_top postamble_0
#define postamble_5_1_24_bot postamble_0

static struct test_stream test_5_1_24_top = { &preamble_5_1_24_top, &test_case_5_1_24_top, &postamble_5_1_24_top };
static struct test_stream test_5_1_24_bot = { &preamble_5_1_24_bot, &test_case_5_1_24_bot, &postamble_5_1_24_bot };

#define tgrp_case_5_1_25 test_group_9
#define numb_case_5_1_25 "5.1.25"
#define name_case_5_1_25 "t_sndv operation"
#define sref_case_5_1_25 "(none)"
#define desc_case_5_1_25 "\
This test case tests the t_sndv operation on a closed file descriptor."

int
test_case_5_1_25_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_SNDV);
}

int
test_case_5_1_25_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_SNDV);
}

#define preamble_5_1_25_top preamble_0
#define preamble_5_1_25_bot preamble_0

#define postamble_5_1_25_top postamble_0
#define postamble_5_1_25_bot postamble_0

static struct test_stream test_5_1_25_top = { &preamble_5_1_25_top, &test_case_5_1_25_top, &postamble_5_1_25_top };
static struct test_stream test_5_1_25_bot = { &preamble_5_1_25_bot, &test_case_5_1_25_bot, &postamble_5_1_25_bot };

#define tgrp_case_5_1_26 test_group_9
#define numb_case_5_1_26 "5.1.26"
#define name_case_5_1_26 "t_sndvudata operation"
#define sref_case_5_1_26 "(none)"
#define desc_case_5_1_26 "\
This test case tests the t_sndvudata operation on a closed file descriptor."

int
test_case_5_1_26_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_SNDVUDATA);
}

int
test_case_5_1_26_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_SNDVUDATA);
}

#define preamble_5_1_26_top preamble_0
#define preamble_5_1_26_bot preamble_0

#define postamble_5_1_26_top postamble_0
#define postamble_5_1_26_bot postamble_0

static struct test_stream test_5_1_26_top = { &preamble_5_1_26_top, &test_case_5_1_26_top, &postamble_5_1_26_top };
static struct test_stream test_5_1_26_bot = { &preamble_5_1_26_bot, &test_case_5_1_26_bot, &postamble_5_1_26_bot };

#define tgrp_case_5_1_27 test_group_9
#define numb_case_5_1_27 "5.1.27"
#define name_case_5_1_27 "t_sync operation"
#define sref_case_5_1_27 "(none)"
#define desc_case_5_1_27 "\
This test case tests the t_sync operation on a closed file descriptor."

int
test_case_5_1_27_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_SYNC);
}

int
test_case_5_1_27_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_SYNC);
}

#define preamble_5_1_27_top preamble_0
#define preamble_5_1_27_bot preamble_0

#define postamble_5_1_27_top postamble_0
#define postamble_5_1_27_bot postamble_0

static struct test_stream test_5_1_27_top = { &preamble_5_1_27_top, &test_case_5_1_27_top, &postamble_5_1_27_top };
static struct test_stream test_5_1_27_bot = { &preamble_5_1_27_bot, &test_case_5_1_27_bot, &postamble_5_1_27_bot };

#define tgrp_case_5_1_28 test_group_9
#define numb_case_5_1_28 "5.1.28"
#define name_case_5_1_28 "t_unbind operation"
#define sref_case_5_1_28 "(none)"
#define desc_case_5_1_28 "\
This test case tests the t_unbind operation on a closed file descriptor."

int
test_case_5_1_28_top(int child)
{
	return test_case_5_1_x_top(child, __TEST_T_UNBIND);
}

int
test_case_5_1_28_bot(int child)
{
	return test_case_5_1_x_bot(child, __TEST_T_UNBIND);
}

#define preamble_5_1_28_top preamble_0
#define preamble_5_1_28_bot preamble_0

#define postamble_5_1_28_top postamble_0
#define postamble_5_1_28_bot postamble_0

static struct test_stream test_5_1_28_top = { &preamble_5_1_28_top, &test_case_5_1_28_top, &postamble_5_1_28_top };
static struct test_stream test_5_1_28_bot = { &preamble_5_1_28_bot, &test_case_5_1_28_bot, &postamble_5_1_28_bot };

#define test_group_10 "Operations on a non-STREAMS file descriptor"
#define tgrp_case_5_2_1 test_group_10
#define numb_case_5_2_1 "5.2.1"
#define name_case_5_2_1 "t_accept operation"
#define sref_case_5_2_1 "(none)"
#define desc_case_5_2_1 "\
This test case tests the t_accept operation on a non-STREAMS file descriptor."

int
test_case_5_2_x_top(int child, int function)
{
	test_fd[0] = fileno(stderr);
	state++;
	if (do_signal(child, function) == __RESULT_SUCCESS || last_t_errno != TBADF)
		goto failure;
	test_fd[0] = child;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_5_2_x_bot(int child, int function)
{
	if (expect(child, NORMAL_WAIT, __EVENT_NO_MSG) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_5_2_1_top(int child)
{
	last_sequence = 1;
	return test_case_5_2_x_top(child, __TEST_T_ACCEPT);
}

int
test_case_5_2_1_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_ACCEPT);
}

#define preamble_5_2_1_top preamble_0
#define preamble_5_2_1_bot preamble_0

#define postamble_5_2_1_top postamble_0
#define postamble_5_2_1_bot postamble_0

static struct test_stream test_5_2_1_top = { &preamble_5_2_1_top, &test_case_5_2_1_top, &postamble_5_2_1_top };
static struct test_stream test_5_2_1_bot = { &preamble_5_2_1_bot, &test_case_5_2_1_bot, &postamble_5_2_1_bot };

#define tgrp_case_5_2_2 test_group_10
#define numb_case_5_2_2 "5.2.2"
#define name_case_5_2_2 "t_bind operation"
#define sref_case_5_2_2 "(none)"
#define desc_case_5_2_2 "\
This test case tests the t_bind operation on a non-STREAMS file descriptor."

int
test_case_5_2_2_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_BIND);
}

int
test_case_5_2_2_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_BIND);
}

#define preamble_5_2_2_top preamble_0
#define preamble_5_2_2_bot preamble_0

#define postamble_5_2_2_top postamble_0
#define postamble_5_2_2_bot postamble_0

static struct test_stream test_5_2_2_top = { &preamble_5_2_2_top, &test_case_5_2_2_top, &postamble_5_2_2_top };
static struct test_stream test_5_2_2_bot = { &preamble_5_2_2_bot, &test_case_5_2_2_bot, &postamble_5_2_2_bot };

#define tgrp_case_5_2_3 test_group_10
#define numb_case_5_2_3 "5.2.3"
#define name_case_5_2_3 "t_close operation"
#define sref_case_5_2_3 "(none)"
#define desc_case_5_2_3 "\
This test case tests the t_close operation on a non-STREAMS file descriptor."

int
test_case_5_2_3_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_CLOSE);
}

int
test_case_5_2_3_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_CLOSE);
}

#define preamble_5_2_3_top preamble_0
#define preamble_5_2_3_bot preamble_0

#define postamble_5_2_3_top postamble_0
#define postamble_5_2_3_bot postamble_0

static struct test_stream test_5_2_3_top = { &preamble_5_2_3_top, &test_case_5_2_3_top, &postamble_5_2_3_top };
static struct test_stream test_5_2_3_bot = { &preamble_5_2_3_bot, &test_case_5_2_3_bot, &postamble_5_2_3_bot };

#define tgrp_case_5_2_4 test_group_10
#define numb_case_5_2_4 "5.2.4"
#define name_case_5_2_4 "t_connect operation"
#define sref_case_5_2_4 "(none)"
#define desc_case_5_2_4 "\
This test case tests the t_connect operation on a non-STREAMS file descriptor."

int
test_case_5_2_4_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_CONNECT);
}

int
test_case_5_2_4_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_CONNECT);
}

#define preamble_5_2_4_top preamble_0
#define preamble_5_2_4_bot preamble_0

#define postamble_5_2_4_top postamble_0
#define postamble_5_2_4_bot postamble_0

static struct test_stream test_5_2_4_top = { &preamble_5_2_4_top, &test_case_5_2_4_top, &postamble_5_2_4_top };
static struct test_stream test_5_2_4_bot = { &preamble_5_2_4_bot, &test_case_5_2_4_bot, &postamble_5_2_4_bot };

#define tgrp_case_5_2_5 test_group_10
#define numb_case_5_2_5 "5.2.5"
#define name_case_5_2_5 "t_getinfo operation"
#define sref_case_5_2_5 "(none)"
#define desc_case_5_2_5 "\
This test case tests the t_getinfo operation on a non-STREAMS file descriptor."

int
test_case_5_2_5_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_GETINFO);
}

int
test_case_5_2_5_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_GETINFO);
}

#define preamble_5_2_5_top preamble_0
#define preamble_5_2_5_bot preamble_0

#define postamble_5_2_5_top postamble_0
#define postamble_5_2_5_bot postamble_0

static struct test_stream test_5_2_5_top = { &preamble_5_2_5_top, &test_case_5_2_5_top, &postamble_5_2_5_top };
static struct test_stream test_5_2_5_bot = { &preamble_5_2_5_bot, &test_case_5_2_5_bot, &postamble_5_2_5_bot };

#define tgrp_case_5_2_6 test_group_10
#define numb_case_5_2_6 "5.2.6"
#define name_case_5_2_6 "t_getprotaddr operation"
#define sref_case_5_2_6 "(none)"
#define desc_case_5_2_6 "\
This test case tests the t_getprotaddr operation on a non-STREAMS file descriptor."

int
test_case_5_2_6_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_GETPROTADDR);
}

int
test_case_5_2_6_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_GETPROTADDR);
}

#define preamble_5_2_6_top preamble_0
#define preamble_5_2_6_bot preamble_0

#define postamble_5_2_6_top postamble_0
#define postamble_5_2_6_bot postamble_0

static struct test_stream test_5_2_6_top = { &preamble_5_2_6_top, &test_case_5_2_6_top, &postamble_5_2_6_top };
static struct test_stream test_5_2_6_bot = { &preamble_5_2_6_bot, &test_case_5_2_6_bot, &postamble_5_2_6_bot };

#define tgrp_case_5_2_7 test_group_10
#define numb_case_5_2_7 "5.2.7"
#define name_case_5_2_7 "t_getstate operation"
#define sref_case_5_2_7 "(none)"
#define desc_case_5_2_7 "\
This test case tests the t_getstate operation on a non-STREAMS file descriptor."

int
test_case_5_2_7_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_GETSTATE);
}

int
test_case_5_2_7_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_GETSTATE);
}

#define preamble_5_2_7_top preamble_0
#define preamble_5_2_7_bot preamble_0

#define postamble_5_2_7_top postamble_0
#define postamble_5_2_7_bot postamble_0

static struct test_stream test_5_2_7_top = { &preamble_5_2_7_top, &test_case_5_2_7_top, &postamble_5_2_7_top };
static struct test_stream test_5_2_7_bot = { &preamble_5_2_7_bot, &test_case_5_2_7_bot, &postamble_5_2_7_bot };

#define tgrp_case_5_2_8 test_group_10
#define numb_case_5_2_8 "5.2.8"
#define name_case_5_2_8 "t_listen operation"
#define sref_case_5_2_8 "(none)"
#define desc_case_5_2_8 "\
This test case tests the t_listen operation on a non-STREAMS file descriptor."

int
test_case_5_2_8_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_LISTEN);
}

int
test_case_5_2_8_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_LISTEN);
}

#define preamble_5_2_8_top preamble_0
#define preamble_5_2_8_bot preamble_0

#define postamble_5_2_8_top postamble_0
#define postamble_5_2_8_bot postamble_0

static struct test_stream test_5_2_8_top = { &preamble_5_2_8_top, &test_case_5_2_8_top, &postamble_5_2_8_top };
static struct test_stream test_5_2_8_bot = { &preamble_5_2_8_bot, &test_case_5_2_8_bot, &postamble_5_2_8_bot };

#define tgrp_case_5_2_9 test_group_10
#define numb_case_5_2_9 "5.2.9"
#define name_case_5_2_9 "t_look operation"
#define sref_case_5_2_9 "(none)"
#define desc_case_5_2_9 "\
This test case tests the t_look operation on a non-STREAMS file descriptor."

int
test_case_5_2_9_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_LOOK);
}

int
test_case_5_2_9_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_LOOK);
}

#define preamble_5_2_9_top preamble_0
#define preamble_5_2_9_bot preamble_0

#define postamble_5_2_9_top postamble_0
#define postamble_5_2_9_bot postamble_0

static struct test_stream test_5_2_9_top = { &preamble_5_2_9_top, &test_case_5_2_9_top, &postamble_5_2_9_top };
static struct test_stream test_5_2_9_bot = { &preamble_5_2_9_bot, &test_case_5_2_9_bot, &postamble_5_2_9_bot };

#define tgrp_case_5_2_10 test_group_10
#define numb_case_5_2_10 "5.2.10"
#define name_case_5_2_10 "t_optmgmt operation"
#define sref_case_5_2_10 "(none)"
#define desc_case_5_2_10 "\
This test case tests the t_optmgmt operation on a non-STREAMS file descriptor."

int
test_case_5_2_10_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_OPTMGMT);
}

int
test_case_5_2_10_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_OPTMGMT);
}

#define preamble_5_2_10_top preamble_0
#define preamble_5_2_10_bot preamble_0

#define postamble_5_2_10_top postamble_0
#define postamble_5_2_10_bot postamble_0

static struct test_stream test_5_2_10_top = { &preamble_5_2_10_top, &test_case_5_2_10_top, &postamble_5_2_10_top };
static struct test_stream test_5_2_10_bot = { &preamble_5_2_10_bot, &test_case_5_2_10_bot, &postamble_5_2_10_bot };

#define tgrp_case_5_2_11 test_group_10
#define numb_case_5_2_11 "5.2.11"
#define name_case_5_2_11 "t_rcv operation"
#define sref_case_5_2_11 "(none)"
#define desc_case_5_2_11 "\
This test case tests the t_rcv operation on a non-STREAMS file descriptor."

int
test_case_5_2_11_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_RCV);
}

int
test_case_5_2_11_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_RCV);
}

#define preamble_5_2_11_top preamble_0
#define preamble_5_2_11_bot preamble_0

#define postamble_5_2_11_top postamble_0
#define postamble_5_2_11_bot postamble_0

static struct test_stream test_5_2_11_top = { &preamble_5_2_11_top, &test_case_5_2_11_top, &postamble_5_2_11_top };
static struct test_stream test_5_2_11_bot = { &preamble_5_2_11_bot, &test_case_5_2_11_bot, &postamble_5_2_11_bot };

#define tgrp_case_5_2_12 test_group_10
#define numb_case_5_2_12 "5.2.12"
#define name_case_5_2_12 "t_rcvconnect operation"
#define sref_case_5_2_12 "(none)"
#define desc_case_5_2_12 "\
This test case tests the t_rcvconnect operation on a non-STREAMS file descriptor."

int
test_case_5_2_12_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_RCVCONNECT);
}

int
test_case_5_2_12_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_RCVCONNECT);
}

#define preamble_5_2_12_top preamble_0
#define preamble_5_2_12_bot preamble_0

#define postamble_5_2_12_top postamble_0
#define postamble_5_2_12_bot postamble_0

static struct test_stream test_5_2_12_top = { &preamble_5_2_12_top, &test_case_5_2_12_top, &postamble_5_2_12_top };
static struct test_stream test_5_2_12_bot = { &preamble_5_2_12_bot, &test_case_5_2_12_bot, &postamble_5_2_12_bot };

#define tgrp_case_5_2_13 test_group_10
#define numb_case_5_2_13 "5.2.13"
#define name_case_5_2_13 "t_rcvdis operation"
#define sref_case_5_2_13 "(none)"
#define desc_case_5_2_13 "\
This test case tests the t_rcvdis operation on a non-STREAMS file descriptor."

int
test_case_5_2_13_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_RCVDIS);
}

int
test_case_5_2_13_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_RCVDIS);
}

#define preamble_5_2_13_top preamble_0
#define preamble_5_2_13_bot preamble_0

#define postamble_5_2_13_top postamble_0
#define postamble_5_2_13_bot postamble_0

static struct test_stream test_5_2_13_top = { &preamble_5_2_13_top, &test_case_5_2_13_top, &postamble_5_2_13_top };
static struct test_stream test_5_2_13_bot = { &preamble_5_2_13_bot, &test_case_5_2_13_bot, &postamble_5_2_13_bot };

#define tgrp_case_5_2_14 test_group_10
#define numb_case_5_2_14 "5.2.14"
#define name_case_5_2_14 "t_rcvrel operation"
#define sref_case_5_2_14 "(none)"
#define desc_case_5_2_14 "\
This test case tests the t_rcvrel operation on a non-STREAMS file descriptor."

int
test_case_5_2_14_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_RCVREL);
}

int
test_case_5_2_14_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_RCVREL);
}

#define preamble_5_2_14_top preamble_0
#define preamble_5_2_14_bot preamble_0

#define postamble_5_2_14_top postamble_0
#define postamble_5_2_14_bot postamble_0

static struct test_stream test_5_2_14_top = { &preamble_5_2_14_top, &test_case_5_2_14_top, &postamble_5_2_14_top };
static struct test_stream test_5_2_14_bot = { &preamble_5_2_14_bot, &test_case_5_2_14_bot, &postamble_5_2_14_bot };

#define tgrp_case_5_2_15 test_group_10
#define numb_case_5_2_15 "5.2.15"
#define name_case_5_2_15 "t_rcvreldata operation"
#define sref_case_5_2_15 "(none)"
#define desc_case_5_2_15 "\
This test case tests the t_rcvreldata operation on a non-STREAMS file descriptor."

int
test_case_5_2_15_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_RCVRELDATA);
}

int
test_case_5_2_15_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_RCVRELDATA);
}

#define preamble_5_2_15_top preamble_0
#define preamble_5_2_15_bot preamble_0

#define postamble_5_2_15_top postamble_0
#define postamble_5_2_15_bot postamble_0

static struct test_stream test_5_2_15_top = { &preamble_5_2_15_top, &test_case_5_2_15_top, &postamble_5_2_15_top };
static struct test_stream test_5_2_15_bot = { &preamble_5_2_15_bot, &test_case_5_2_15_bot, &postamble_5_2_15_bot };

#define tgrp_case_5_2_16 test_group_10
#define numb_case_5_2_16 "5.2.16"
#define name_case_5_2_16 "t_rcvudata operation"
#define sref_case_5_2_16 "(none)"
#define desc_case_5_2_16 "\
This test case tests the t_rcvudata operation on a non-STREAMS file descriptor."

int
test_case_5_2_16_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_RCVUDATA);
}

int
test_case_5_2_16_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_RCVUDATA);
}

#define preamble_5_2_16_top preamble_0
#define preamble_5_2_16_bot preamble_0

#define postamble_5_2_16_top postamble_0
#define postamble_5_2_16_bot postamble_0

static struct test_stream test_5_2_16_top = { &preamble_5_2_16_top, &test_case_5_2_16_top, &postamble_5_2_16_top };
static struct test_stream test_5_2_16_bot = { &preamble_5_2_16_bot, &test_case_5_2_16_bot, &postamble_5_2_16_bot };

#define tgrp_case_5_2_17 test_group_10
#define numb_case_5_2_17 "5.2.17"
#define name_case_5_2_17 "t_rcvuderr operation"
#define sref_case_5_2_17 "(none)"
#define desc_case_5_2_17 "\
This test case tests the t_rcvuderr operation on a non-STREAMS file descriptor."

int
test_case_5_2_17_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_RCVUDERR);
}

int
test_case_5_2_17_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_RCVUDERR);
}

#define preamble_5_2_17_top preamble_0
#define preamble_5_2_17_bot preamble_0

#define postamble_5_2_17_top postamble_0
#define postamble_5_2_17_bot postamble_0

static struct test_stream test_5_2_17_top = { &preamble_5_2_17_top, &test_case_5_2_17_top, &postamble_5_2_17_top };
static struct test_stream test_5_2_17_bot = { &preamble_5_2_17_bot, &test_case_5_2_17_bot, &postamble_5_2_17_bot };

#define tgrp_case_5_2_18 test_group_10
#define numb_case_5_2_18 "5.2.18"
#define name_case_5_2_18 "t_rcvv operation"
#define sref_case_5_2_18 "(none)"
#define desc_case_5_2_18 "\
This test case tests the t_rcvv operation on a non-STREAMS file descriptor."

int
test_case_5_2_18_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_RCVV);
}

int
test_case_5_2_18_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_RCVV);
}

#define preamble_5_2_18_top preamble_0
#define preamble_5_2_18_bot preamble_0

#define postamble_5_2_18_top postamble_0
#define postamble_5_2_18_bot postamble_0

static struct test_stream test_5_2_18_top = { &preamble_5_2_18_top, &test_case_5_2_18_top, &postamble_5_2_18_top };
static struct test_stream test_5_2_18_bot = { &preamble_5_2_18_bot, &test_case_5_2_18_bot, &postamble_5_2_18_bot };

#define tgrp_case_5_2_19 test_group_10
#define numb_case_5_2_19 "5.2.19"
#define name_case_5_2_19 "t_rcvvudata operation"
#define sref_case_5_2_19 "(none)"
#define desc_case_5_2_19 "\
This test case tests the t_rcvvudata operation on a non-STREAMS file descriptor."

int
test_case_5_2_19_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_RCVVUDATA);
}

int
test_case_5_2_19_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_RCVVUDATA);
}

#define preamble_5_2_19_top preamble_0
#define preamble_5_2_19_bot preamble_0

#define postamble_5_2_19_top postamble_0
#define postamble_5_2_19_bot postamble_0

static struct test_stream test_5_2_19_top = { &preamble_5_2_19_top, &test_case_5_2_19_top, &postamble_5_2_19_top };
static struct test_stream test_5_2_19_bot = { &preamble_5_2_19_bot, &test_case_5_2_19_bot, &postamble_5_2_19_bot };

#define tgrp_case_5_2_20 test_group_10
#define numb_case_5_2_20 "5.2.20"
#define name_case_5_2_20 "t_snd operation"
#define sref_case_5_2_20 "(none)"
#define desc_case_5_2_20 "\
This test case tests the t_snd operation on a non-STREAMS file descriptor."

int
test_case_5_2_20_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_SND);
}

int
test_case_5_2_20_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_SND);
}

#define preamble_5_2_20_top preamble_0
#define preamble_5_2_20_bot preamble_0

#define postamble_5_2_20_top postamble_0
#define postamble_5_2_20_bot postamble_0

static struct test_stream test_5_2_20_top = { &preamble_5_2_20_top, &test_case_5_2_20_top, &postamble_5_2_20_top };
static struct test_stream test_5_2_20_bot = { &preamble_5_2_20_bot, &test_case_5_2_20_bot, &postamble_5_2_20_bot };

#define tgrp_case_5_2_21 test_group_10
#define numb_case_5_2_21 "5.2.21"
#define name_case_5_2_21 "t_snddis operation"
#define sref_case_5_2_21 "(none)"
#define desc_case_5_2_21 "\
This test case tests the t_snddis operation on a non-STREAMS file descriptor."

int
test_case_5_2_21_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_SNDDIS);
}

int
test_case_5_2_21_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_SNDDIS);
}

#define preamble_5_2_21_top preamble_0
#define preamble_5_2_21_bot preamble_0

#define postamble_5_2_21_top postamble_0
#define postamble_5_2_21_bot postamble_0

static struct test_stream test_5_2_21_top = { &preamble_5_2_21_top, &test_case_5_2_21_top, &postamble_5_2_21_top };
static struct test_stream test_5_2_21_bot = { &preamble_5_2_21_bot, &test_case_5_2_21_bot, &postamble_5_2_21_bot };

#define tgrp_case_5_2_22 test_group_10
#define numb_case_5_2_22 "5.2.22"
#define name_case_5_2_22 "t_sndrel operation"
#define sref_case_5_2_22 "(none)"
#define desc_case_5_2_22 "\
This test case tests the t_sndrel operation on a non-STREAMS file descriptor."

int
test_case_5_2_22_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_SNDREL);
}

int
test_case_5_2_22_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_SNDREL);
}

#define preamble_5_2_22_top preamble_0
#define preamble_5_2_22_bot preamble_0

#define postamble_5_2_22_top postamble_0
#define postamble_5_2_22_bot postamble_0

static struct test_stream test_5_2_22_top = { &preamble_5_2_22_top, &test_case_5_2_22_top, &postamble_5_2_22_top };
static struct test_stream test_5_2_22_bot = { &preamble_5_2_22_bot, &test_case_5_2_22_bot, &postamble_5_2_22_bot };

#define tgrp_case_5_2_23 test_group_10
#define numb_case_5_2_23 "5.2.23"
#define name_case_5_2_23 "t_sndreldata operation"
#define sref_case_5_2_23 "(none)"
#define desc_case_5_2_23 "\
This test case tests the t_sndreldata operation on a non-STREAMS file descriptor."

int
test_case_5_2_23_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_SNDRELDATA);
}

int
test_case_5_2_23_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_SNDRELDATA);
}

#define preamble_5_2_23_top preamble_0
#define preamble_5_2_23_bot preamble_0

#define postamble_5_2_23_top postamble_0
#define postamble_5_2_23_bot postamble_0

static struct test_stream test_5_2_23_top = { &preamble_5_2_23_top, &test_case_5_2_23_top, &postamble_5_2_23_top };
static struct test_stream test_5_2_23_bot = { &preamble_5_2_23_bot, &test_case_5_2_23_bot, &postamble_5_2_23_bot };

#define tgrp_case_5_2_24 test_group_10
#define numb_case_5_2_24 "5.2.24"
#define name_case_5_2_24 "t_sndudata operation"
#define sref_case_5_2_24 "(none)"
#define desc_case_5_2_24 "\
This test case tests the t_sndudata operation on a non-STREAMS file descriptor."

int
test_case_5_2_24_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_SNDUDATA);
}

int
test_case_5_2_24_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_SNDUDATA);
}

#define preamble_5_2_24_top preamble_0
#define preamble_5_2_24_bot preamble_0

#define postamble_5_2_24_top postamble_0
#define postamble_5_2_24_bot postamble_0

static struct test_stream test_5_2_24_top = { &preamble_5_2_24_top, &test_case_5_2_24_top, &postamble_5_2_24_top };
static struct test_stream test_5_2_24_bot = { &preamble_5_2_24_bot, &test_case_5_2_24_bot, &postamble_5_2_24_bot };

#define tgrp_case_5_2_25 test_group_10
#define numb_case_5_2_25 "5.2.25"
#define name_case_5_2_25 "t_sndv operation"
#define sref_case_5_2_25 "(none)"
#define desc_case_5_2_25 "\
This test case tests the t_sndv operation on a non-STREAMS file descriptor."

int
test_case_5_2_25_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_SNDV);
}

int
test_case_5_2_25_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_SNDV);
}

#define preamble_5_2_25_top preamble_0
#define preamble_5_2_25_bot preamble_0

#define postamble_5_2_25_top postamble_0
#define postamble_5_2_25_bot postamble_0

static struct test_stream test_5_2_25_top = { &preamble_5_2_25_top, &test_case_5_2_25_top, &postamble_5_2_25_top };
static struct test_stream test_5_2_25_bot = { &preamble_5_2_25_bot, &test_case_5_2_25_bot, &postamble_5_2_25_bot };

#define tgrp_case_5_2_26 test_group_10
#define numb_case_5_2_26 "5.2.26"
#define name_case_5_2_26 "t_sndvudata operation"
#define sref_case_5_2_26 "(none)"
#define desc_case_5_2_26 "\
This test case tests the t_sndvudata operation on a non-STREAMS file descriptor."

int
test_case_5_2_26_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_SNDVUDATA);
}

int
test_case_5_2_26_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_SNDVUDATA);
}

#define preamble_5_2_26_top preamble_0
#define preamble_5_2_26_bot preamble_0

#define postamble_5_2_26_top postamble_0
#define postamble_5_2_26_bot postamble_0

static struct test_stream test_5_2_26_top = { &preamble_5_2_26_top, &test_case_5_2_26_top, &postamble_5_2_26_top };
static struct test_stream test_5_2_26_bot = { &preamble_5_2_26_bot, &test_case_5_2_26_bot, &postamble_5_2_26_bot };

#define tgrp_case_5_2_27 test_group_10
#define numb_case_5_2_27 "5.2.27"
#define name_case_5_2_27 "t_sync operation"
#define sref_case_5_2_27 "(none)"
#define desc_case_5_2_27 "\
This test case tests the t_sync operation on a non-STREAMS file descriptor."

int
test_case_5_2_27_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_SYNC);
}

int
test_case_5_2_27_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_SYNC);
}

#define preamble_5_2_27_top preamble_0
#define preamble_5_2_27_bot preamble_0

#define postamble_5_2_27_top postamble_0
#define postamble_5_2_27_bot postamble_0

static struct test_stream test_5_2_27_top = { &preamble_5_2_27_top, &test_case_5_2_27_top, &postamble_5_2_27_top };
static struct test_stream test_5_2_27_bot = { &preamble_5_2_27_bot, &test_case_5_2_27_bot, &postamble_5_2_27_bot };

#define tgrp_case_5_2_28 test_group_10
#define numb_case_5_2_28 "5.2.28"
#define name_case_5_2_28 "t_unbind operation"
#define sref_case_5_2_28 "(none)"
#define desc_case_5_2_28 "\
This test case tests the t_unbind operation on a non-STREAMS file descriptor."

int
test_case_5_2_28_top(int child)
{
	return test_case_5_2_x_top(child, __TEST_T_UNBIND);
}

int
test_case_5_2_28_bot(int child)
{
	return test_case_5_2_x_bot(child, __TEST_T_UNBIND);
}

#define preamble_5_2_28_top preamble_0
#define preamble_5_2_28_bot preamble_0

#define postamble_5_2_28_top postamble_0
#define postamble_5_2_28_bot postamble_0

static struct test_stream test_5_2_28_top = { &preamble_5_2_28_top, &test_case_5_2_28_top, &postamble_5_2_28_top };
static struct test_stream test_5_2_28_bot = { &preamble_5_2_28_bot, &test_case_5_2_28_bot, &postamble_5_2_28_bot };

#define test_group_11 "Operations on a STREAMS file descriptor"
#define tgrp_case_5_3_1 test_group_11
#define numb_case_5_3_1 "5.3.1"
#define name_case_5_3_1 "Without \"timod\" pushed -- t_accept operation"
#define sref_case_5_3_1 "(none)"
#define desc_case_5_3_1 "\
This test case tests the t_accept operation on a STREAMS file descriptor."

int
test_case_5_3_x_top(int child, int function)
{
	if (do_signal(child, function) == __RESULT_SUCCESS || last_t_errno != TBADF)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_5_3_x_bot(int child, int function)
{
	if (expect(child, NORMAL_WAIT, __EVENT_NO_MSG) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_5_3_1_top(int child)
{
	last_sequence = 1;
	return test_case_5_3_x_top(child, __TEST_T_ACCEPT);
}

int
test_case_5_3_1_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_ACCEPT);
}

#define preamble_5_3_1_top preamble_0
#define preamble_5_3_1_bot preamble_0

#define postamble_5_3_1_top postamble_0
#define postamble_5_3_1_bot postamble_0

static struct test_stream test_5_3_1_top = { &preamble_5_3_1_top, &test_case_5_3_1_top, &postamble_5_3_1_top };
static struct test_stream test_5_3_1_bot = { &preamble_5_3_1_bot, &test_case_5_3_1_bot, &postamble_5_3_1_bot };

#define tgrp_case_5_3_2 test_group_11
#define numb_case_5_3_2 "5.3.2"
#define name_case_5_3_2 "Without \"timod\" pushed -- t_bind operation"
#define sref_case_5_3_2 "(none)"
#define desc_case_5_3_2 "\
This test case tests the t_bind operation on a STREAMS file descriptor."

int
test_case_5_3_2_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_BIND);
}

int
test_case_5_3_2_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_BIND);
}

#define preamble_5_3_2_top preamble_0
#define preamble_5_3_2_bot preamble_0

#define postamble_5_3_2_top postamble_0
#define postamble_5_3_2_bot postamble_0

static struct test_stream test_5_3_2_top = { &preamble_5_3_2_top, &test_case_5_3_2_top, &postamble_5_3_2_top };
static struct test_stream test_5_3_2_bot = { &preamble_5_3_2_bot, &test_case_5_3_2_bot, &postamble_5_3_2_bot };

#define tgrp_case_5_3_3 test_group_11
#define numb_case_5_3_3 "5.3.3"
#define name_case_5_3_3 "Without \"timod\" pushed -- t_close operation"
#define sref_case_5_3_3 "(none)"
#define desc_case_5_3_3 "\
This test case tests the t_close operation on a STREAMS file descriptor."

int
test_case_5_3_3_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_CLOSE);
}

int
test_case_5_3_3_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_CLOSE);
}

#define preamble_5_3_3_top preamble_0
#define preamble_5_3_3_bot preamble_0

#define postamble_5_3_3_top postamble_0
#define postamble_5_3_3_bot postamble_0

static struct test_stream test_5_3_3_top = { &preamble_5_3_3_top, &test_case_5_3_3_top, &postamble_5_3_3_top };
static struct test_stream test_5_3_3_bot = { &preamble_5_3_3_bot, &test_case_5_3_3_bot, &postamble_5_3_3_bot };

#define tgrp_case_5_3_4 test_group_11
#define numb_case_5_3_4 "5.3.4"
#define name_case_5_3_4 "Without \"timod\" pushed -- t_connect operation"
#define sref_case_5_3_4 "(none)"
#define desc_case_5_3_4 "\
This test case tests the t_connect operation on a STREAMS file descriptor."

int
test_case_5_3_4_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_CONNECT);
}

int
test_case_5_3_4_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_CONNECT);
}

#define preamble_5_3_4_top preamble_0
#define preamble_5_3_4_bot preamble_0

#define postamble_5_3_4_top postamble_0
#define postamble_5_3_4_bot postamble_0

static struct test_stream test_5_3_4_top = { &preamble_5_3_4_top, &test_case_5_3_4_top, &postamble_5_3_4_top };
static struct test_stream test_5_3_4_bot = { &preamble_5_3_4_bot, &test_case_5_3_4_bot, &postamble_5_3_4_bot };

#define tgrp_case_5_3_5 test_group_11
#define numb_case_5_3_5 "5.3.5"
#define name_case_5_3_5 "Without \"timod\" pushed -- t_getinfo operation"
#define sref_case_5_3_5 "(none)"
#define desc_case_5_3_5 "\
This test case tests the t_getinfo operation on a STREAMS file descriptor."

int
test_case_5_3_5_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_GETINFO);
}

int
test_case_5_3_5_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_GETINFO);
}

#define preamble_5_3_5_top preamble_0
#define preamble_5_3_5_bot preamble_0

#define postamble_5_3_5_top postamble_0
#define postamble_5_3_5_bot postamble_0

static struct test_stream test_5_3_5_top = { &preamble_5_3_5_top, &test_case_5_3_5_top, &postamble_5_3_5_top };
static struct test_stream test_5_3_5_bot = { &preamble_5_3_5_bot, &test_case_5_3_5_bot, &postamble_5_3_5_bot };

#define tgrp_case_5_3_6 test_group_11
#define numb_case_5_3_6 "5.3.6"
#define name_case_5_3_6 "Without \"timod\" pushed -- t_getprotaddr operation"
#define sref_case_5_3_6 "(none)"
#define desc_case_5_3_6 "\
This test case tests the t_getprotaddr operation on a STREAMS file descriptor."

int
test_case_5_3_6_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_GETPROTADDR);
}

int
test_case_5_3_6_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_GETPROTADDR);
}

#define preamble_5_3_6_top preamble_0
#define preamble_5_3_6_bot preamble_0

#define postamble_5_3_6_top postamble_0
#define postamble_5_3_6_bot postamble_0

static struct test_stream test_5_3_6_top = { &preamble_5_3_6_top, &test_case_5_3_6_top, &postamble_5_3_6_top };
static struct test_stream test_5_3_6_bot = { &preamble_5_3_6_bot, &test_case_5_3_6_bot, &postamble_5_3_6_bot };

#define tgrp_case_5_3_7 test_group_11
#define numb_case_5_3_7 "5.3.7"
#define name_case_5_3_7 "Without \"timod\" pushed -- t_getstate operation"
#define sref_case_5_3_7 "(none)"
#define desc_case_5_3_7 "\
This test case tests the t_getstate operation on a STREAMS file descriptor."

int
test_case_5_3_7_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_GETSTATE);
}

int
test_case_5_3_7_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_GETSTATE);
}

#define preamble_5_3_7_top preamble_0
#define preamble_5_3_7_bot preamble_0

#define postamble_5_3_7_top postamble_0
#define postamble_5_3_7_bot postamble_0

static struct test_stream test_5_3_7_top = { &preamble_5_3_7_top, &test_case_5_3_7_top, &postamble_5_3_7_top };
static struct test_stream test_5_3_7_bot = { &preamble_5_3_7_bot, &test_case_5_3_7_bot, &postamble_5_3_7_bot };

#define tgrp_case_5_3_8 test_group_11
#define numb_case_5_3_8 "5.3.8"
#define name_case_5_3_8 "Without \"timod\" pushed -- t_listen operation"
#define sref_case_5_3_8 "(none)"
#define desc_case_5_3_8 "\
This test case tests the t_listen operation on a STREAMS file descriptor."

int
test_case_5_3_8_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_LISTEN);
}

int
test_case_5_3_8_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_LISTEN);
}

#define preamble_5_3_8_top preamble_0
#define preamble_5_3_8_bot preamble_0

#define postamble_5_3_8_top postamble_0
#define postamble_5_3_8_bot postamble_0

static struct test_stream test_5_3_8_top = { &preamble_5_3_8_top, &test_case_5_3_8_top, &postamble_5_3_8_top };
static struct test_stream test_5_3_8_bot = { &preamble_5_3_8_bot, &test_case_5_3_8_bot, &postamble_5_3_8_bot };

#define tgrp_case_5_3_9 test_group_11
#define numb_case_5_3_9 "5.3.9"
#define name_case_5_3_9 "Without \"timod\" pushed -- t_look operation"
#define sref_case_5_3_9 "(none)"
#define desc_case_5_3_9 "\
This test case tests the t_look operation on a STREAMS file descriptor."

int
test_case_5_3_9_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_LOOK);
}

int
test_case_5_3_9_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_LOOK);
}

#define preamble_5_3_9_top preamble_0
#define preamble_5_3_9_bot preamble_0

#define postamble_5_3_9_top postamble_0
#define postamble_5_3_9_bot postamble_0

static struct test_stream test_5_3_9_top = { &preamble_5_3_9_top, &test_case_5_3_9_top, &postamble_5_3_9_top };
static struct test_stream test_5_3_9_bot = { &preamble_5_3_9_bot, &test_case_5_3_9_bot, &postamble_5_3_9_bot };

#define tgrp_case_5_3_10 test_group_11
#define numb_case_5_3_10 "5.3.10"
#define name_case_5_3_10 "Without \"timod\" pushed -- t_optmgmt operation"
#define sref_case_5_3_10 "(none)"
#define desc_case_5_3_10 "\
This test case tests the t_optmgmt operation on a STREAMS file descriptor."

int
test_case_5_3_10_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_OPTMGMT);
}

int
test_case_5_3_10_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_OPTMGMT);
}

#define preamble_5_3_10_top preamble_0
#define preamble_5_3_10_bot preamble_0

#define postamble_5_3_10_top postamble_0
#define postamble_5_3_10_bot postamble_0

static struct test_stream test_5_3_10_top = { &preamble_5_3_10_top, &test_case_5_3_10_top, &postamble_5_3_10_top };
static struct test_stream test_5_3_10_bot = { &preamble_5_3_10_bot, &test_case_5_3_10_bot, &postamble_5_3_10_bot };

#define tgrp_case_5_3_11 test_group_11
#define numb_case_5_3_11 "5.3.11"
#define name_case_5_3_11 "Without \"timod\" pushed -- t_rcv operation"
#define sref_case_5_3_11 "(none)"
#define desc_case_5_3_11 "\
This test case tests the t_rcv operation on a STREAMS file descriptor."

int
test_case_5_3_11_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_RCV);
}

int
test_case_5_3_11_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_RCV);
}

#define preamble_5_3_11_top preamble_0
#define preamble_5_3_11_bot preamble_0

#define postamble_5_3_11_top postamble_0
#define postamble_5_3_11_bot postamble_0

static struct test_stream test_5_3_11_top = { &preamble_5_3_11_top, &test_case_5_3_11_top, &postamble_5_3_11_top };
static struct test_stream test_5_3_11_bot = { &preamble_5_3_11_bot, &test_case_5_3_11_bot, &postamble_5_3_11_bot };

#define tgrp_case_5_3_12 test_group_11
#define numb_case_5_3_12 "5.3.12"
#define name_case_5_3_12 "Without \"timod\" pushed -- t_rcvconnect operation"
#define sref_case_5_3_12 "(none)"
#define desc_case_5_3_12 "\
This test case tests the t_rcvconnect operation on a STREAMS file descriptor."

int
test_case_5_3_12_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_RCVCONNECT);
}

int
test_case_5_3_12_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_RCVCONNECT);
}

#define preamble_5_3_12_top preamble_0
#define preamble_5_3_12_bot preamble_0

#define postamble_5_3_12_top postamble_0
#define postamble_5_3_12_bot postamble_0

static struct test_stream test_5_3_12_top = { &preamble_5_3_12_top, &test_case_5_3_12_top, &postamble_5_3_12_top };
static struct test_stream test_5_3_12_bot = { &preamble_5_3_12_bot, &test_case_5_3_12_bot, &postamble_5_3_12_bot };

#define tgrp_case_5_3_13 test_group_11
#define numb_case_5_3_13 "5.3.13"
#define name_case_5_3_13 "Without \"timod\" pushed -- t_rcvdis operation"
#define sref_case_5_3_13 "(none)"
#define desc_case_5_3_13 "\
This test case tests the t_rcvdis operation on a STREAMS file descriptor."

int
test_case_5_3_13_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_RCVDIS);
}

int
test_case_5_3_13_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_RCVDIS);
}

#define preamble_5_3_13_top preamble_0
#define preamble_5_3_13_bot preamble_0

#define postamble_5_3_13_top postamble_0
#define postamble_5_3_13_bot postamble_0

static struct test_stream test_5_3_13_top = { &preamble_5_3_13_top, &test_case_5_3_13_top, &postamble_5_3_13_top };
static struct test_stream test_5_3_13_bot = { &preamble_5_3_13_bot, &test_case_5_3_13_bot, &postamble_5_3_13_bot };

#define tgrp_case_5_3_14 test_group_11
#define numb_case_5_3_14 "5.3.14"
#define name_case_5_3_14 "Without \"timod\" pushed -- t_rcvrel operation"
#define sref_case_5_3_14 "(none)"
#define desc_case_5_3_14 "\
This test case tests the t_rcvrel operation on a STREAMS file descriptor."

int
test_case_5_3_14_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_RCVREL);
}

int
test_case_5_3_14_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_RCVREL);
}

#define preamble_5_3_14_top preamble_0
#define preamble_5_3_14_bot preamble_0

#define postamble_5_3_14_top postamble_0
#define postamble_5_3_14_bot postamble_0

static struct test_stream test_5_3_14_top = { &preamble_5_3_14_top, &test_case_5_3_14_top, &postamble_5_3_14_top };
static struct test_stream test_5_3_14_bot = { &preamble_5_3_14_bot, &test_case_5_3_14_bot, &postamble_5_3_14_bot };

#define tgrp_case_5_3_15 test_group_11
#define numb_case_5_3_15 "5.3.15"
#define name_case_5_3_15 "Without \"timod\" pushed -- t_rcvreldata operation"
#define sref_case_5_3_15 "(none)"
#define desc_case_5_3_15 "\
This test case tests the t_rcvreldata operation on a STREAMS file descriptor."

int
test_case_5_3_15_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_RCVRELDATA);
}

int
test_case_5_3_15_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_RCVRELDATA);
}

#define preamble_5_3_15_top preamble_0
#define preamble_5_3_15_bot preamble_0

#define postamble_5_3_15_top postamble_0
#define postamble_5_3_15_bot postamble_0

static struct test_stream test_5_3_15_top = { &preamble_5_3_15_top, &test_case_5_3_15_top, &postamble_5_3_15_top };
static struct test_stream test_5_3_15_bot = { &preamble_5_3_15_bot, &test_case_5_3_15_bot, &postamble_5_3_15_bot };

#define tgrp_case_5_3_16 test_group_11
#define numb_case_5_3_16 "5.3.16"
#define name_case_5_3_16 "Without \"timod\" pushed -- t_rcvudata operation"
#define sref_case_5_3_16 "(none)"
#define desc_case_5_3_16 "\
This test case tests the t_rcvudata operation on a STREAMS file descriptor."

int
test_case_5_3_16_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_RCVUDATA);
}

int
test_case_5_3_16_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_RCVUDATA);
}

#define preamble_5_3_16_top preamble_0
#define preamble_5_3_16_bot preamble_0

#define postamble_5_3_16_top postamble_0
#define postamble_5_3_16_bot postamble_0

static struct test_stream test_5_3_16_top = { &preamble_5_3_16_top, &test_case_5_3_16_top, &postamble_5_3_16_top };
static struct test_stream test_5_3_16_bot = { &preamble_5_3_16_bot, &test_case_5_3_16_bot, &postamble_5_3_16_bot };

#define tgrp_case_5_3_17 test_group_11
#define numb_case_5_3_17 "5.3.17"
#define name_case_5_3_17 "Without \"timod\" pushed -- t_rcvuderr operation"
#define sref_case_5_3_17 "(none)"
#define desc_case_5_3_17 "\
This test case tests the t_rcvuderr operation on a STREAMS file descriptor."

int
test_case_5_3_17_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_RCVUDERR);
}

int
test_case_5_3_17_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_RCVUDERR);
}

#define preamble_5_3_17_top preamble_0
#define preamble_5_3_17_bot preamble_0

#define postamble_5_3_17_top postamble_0
#define postamble_5_3_17_bot postamble_0

static struct test_stream test_5_3_17_top = { &preamble_5_3_17_top, &test_case_5_3_17_top, &postamble_5_3_17_top };
static struct test_stream test_5_3_17_bot = { &preamble_5_3_17_bot, &test_case_5_3_17_bot, &postamble_5_3_17_bot };

#define tgrp_case_5_3_18 test_group_11
#define numb_case_5_3_18 "5.3.18"
#define name_case_5_3_18 "Without \"timod\" pushed -- t_rcvv operation"
#define sref_case_5_3_18 "(none)"
#define desc_case_5_3_18 "\
This test case tests the t_rcvv operation on a STREAMS file descriptor."

int
test_case_5_3_18_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_RCVV);
}

int
test_case_5_3_18_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_RCVV);
}

#define preamble_5_3_18_top preamble_0
#define preamble_5_3_18_bot preamble_0

#define postamble_5_3_18_top postamble_0
#define postamble_5_3_18_bot postamble_0

static struct test_stream test_5_3_18_top = { &preamble_5_3_18_top, &test_case_5_3_18_top, &postamble_5_3_18_top };
static struct test_stream test_5_3_18_bot = { &preamble_5_3_18_bot, &test_case_5_3_18_bot, &postamble_5_3_18_bot };

#define tgrp_case_5_3_19 test_group_11
#define numb_case_5_3_19 "5.3.19"
#define name_case_5_3_19 "Without \"timod\" pushed -- t_rcvvudata operation"
#define sref_case_5_3_19 "(none)"
#define desc_case_5_3_19 "\
This test case tests the t_rcvvudata operation on a STREAMS file descriptor."

int
test_case_5_3_19_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_RCVVUDATA);
}

int
test_case_5_3_19_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_RCVVUDATA);
}

#define preamble_5_3_19_top preamble_0
#define preamble_5_3_19_bot preamble_0

#define postamble_5_3_19_top postamble_0
#define postamble_5_3_19_bot postamble_0

static struct test_stream test_5_3_19_top = { &preamble_5_3_19_top, &test_case_5_3_19_top, &postamble_5_3_19_top };
static struct test_stream test_5_3_19_bot = { &preamble_5_3_19_bot, &test_case_5_3_19_bot, &postamble_5_3_19_bot };

#define tgrp_case_5_3_20 test_group_11
#define numb_case_5_3_20 "5.3.20"
#define name_case_5_3_20 "Without \"timod\" pushed -- t_snd operation"
#define sref_case_5_3_20 "(none)"
#define desc_case_5_3_20 "\
This test case tests the t_snd operation on a STREAMS file descriptor."

int
test_case_5_3_20_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_SND);
}

int
test_case_5_3_20_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_SND);
}

#define preamble_5_3_20_top preamble_0
#define preamble_5_3_20_bot preamble_0

#define postamble_5_3_20_top postamble_0
#define postamble_5_3_20_bot postamble_0

static struct test_stream test_5_3_20_top = { &preamble_5_3_20_top, &test_case_5_3_20_top, &postamble_5_3_20_top };
static struct test_stream test_5_3_20_bot = { &preamble_5_3_20_bot, &test_case_5_3_20_bot, &postamble_5_3_20_bot };

#define tgrp_case_5_3_21 test_group_11
#define numb_case_5_3_21 "5.3.21"
#define name_case_5_3_21 "Without \"timod\" pushed -- t_snddis operation"
#define sref_case_5_3_21 "(none)"
#define desc_case_5_3_21 "\
This test case tests the t_snddis operation on a STREAMS file descriptor."

int
test_case_5_3_21_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_SNDDIS);
}

int
test_case_5_3_21_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_SNDDIS);
}

#define preamble_5_3_21_top preamble_0
#define preamble_5_3_21_bot preamble_0

#define postamble_5_3_21_top postamble_0
#define postamble_5_3_21_bot postamble_0

static struct test_stream test_5_3_21_top = { &preamble_5_3_21_top, &test_case_5_3_21_top, &postamble_5_3_21_top };
static struct test_stream test_5_3_21_bot = { &preamble_5_3_21_bot, &test_case_5_3_21_bot, &postamble_5_3_21_bot };

#define tgrp_case_5_3_22 test_group_11
#define numb_case_5_3_22 "5.3.22"
#define name_case_5_3_22 "Without \"timod\" pushed -- t_sndrel operation"
#define sref_case_5_3_22 "(none)"
#define desc_case_5_3_22 "\
This test case tests the t_sndrel operation on a STREAMS file descriptor."

int
test_case_5_3_22_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_SNDREL);
}

int
test_case_5_3_22_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_SNDREL);
}

#define preamble_5_3_22_top preamble_0
#define preamble_5_3_22_bot preamble_0

#define postamble_5_3_22_top postamble_0
#define postamble_5_3_22_bot postamble_0

static struct test_stream test_5_3_22_top = { &preamble_5_3_22_top, &test_case_5_3_22_top, &postamble_5_3_22_top };
static struct test_stream test_5_3_22_bot = { &preamble_5_3_22_bot, &test_case_5_3_22_bot, &postamble_5_3_22_bot };

#define tgrp_case_5_3_23 test_group_11
#define numb_case_5_3_23 "5.3.23"
#define name_case_5_3_23 "Without \"timod\" pushed -- t_sndreldata operation"
#define sref_case_5_3_23 "(none)"
#define desc_case_5_3_23 "\
This test case tests the t_sndreldata operation on a STREAMS file descriptor."

int
test_case_5_3_23_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_SNDRELDATA);
}

int
test_case_5_3_23_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_SNDRELDATA);
}

#define preamble_5_3_23_top preamble_0
#define preamble_5_3_23_bot preamble_0

#define postamble_5_3_23_top postamble_0
#define postamble_5_3_23_bot postamble_0

static struct test_stream test_5_3_23_top = { &preamble_5_3_23_top, &test_case_5_3_23_top, &postamble_5_3_23_top };
static struct test_stream test_5_3_23_bot = { &preamble_5_3_23_bot, &test_case_5_3_23_bot, &postamble_5_3_23_bot };

#define tgrp_case_5_3_24 test_group_11
#define numb_case_5_3_24 "5.3.24"
#define name_case_5_3_24 "Without \"timod\" pushed -- t_sndudata operation"
#define sref_case_5_3_24 "(none)"
#define desc_case_5_3_24 "\
This test case tests the t_sndudata operation on a STREAMS file descriptor."

int
test_case_5_3_24_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_SNDUDATA);
}

int
test_case_5_3_24_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_SNDUDATA);
}

#define preamble_5_3_24_top preamble_0
#define preamble_5_3_24_bot preamble_0

#define postamble_5_3_24_top postamble_0
#define postamble_5_3_24_bot postamble_0

static struct test_stream test_5_3_24_top = { &preamble_5_3_24_top, &test_case_5_3_24_top, &postamble_5_3_24_top };
static struct test_stream test_5_3_24_bot = { &preamble_5_3_24_bot, &test_case_5_3_24_bot, &postamble_5_3_24_bot };

#define tgrp_case_5_3_25 test_group_11
#define numb_case_5_3_25 "5.3.25"
#define name_case_5_3_25 "Without \"timod\" pushed -- t_sndv operation"
#define sref_case_5_3_25 "(none)"
#define desc_case_5_3_25 "\
This test case tests the t_sndv operation on a STREAMS file descriptor."

int
test_case_5_3_25_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_SNDV);
}

int
test_case_5_3_25_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_SNDV);
}

#define preamble_5_3_25_top preamble_0
#define preamble_5_3_25_bot preamble_0

#define postamble_5_3_25_top postamble_0
#define postamble_5_3_25_bot postamble_0

static struct test_stream test_5_3_25_top = { &preamble_5_3_25_top, &test_case_5_3_25_top, &postamble_5_3_25_top };
static struct test_stream test_5_3_25_bot = { &preamble_5_3_25_bot, &test_case_5_3_25_bot, &postamble_5_3_25_bot };

#define tgrp_case_5_3_26 test_group_11
#define numb_case_5_3_26 "5.3.26"
#define name_case_5_3_26 "Without \"timod\" pushed -- t_sndvudata operation"
#define sref_case_5_3_26 "(none)"
#define desc_case_5_3_26 "\
This test case tests the t_sndvudata operation on a STREAMS file descriptor."

int
test_case_5_3_26_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_SNDVUDATA);
}

int
test_case_5_3_26_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_SNDVUDATA);
}

#define preamble_5_3_26_top preamble_0
#define preamble_5_3_26_bot preamble_0

#define postamble_5_3_26_top postamble_0
#define postamble_5_3_26_bot postamble_0

static struct test_stream test_5_3_26_top = { &preamble_5_3_26_top, &test_case_5_3_26_top, &postamble_5_3_26_top };
static struct test_stream test_5_3_26_bot = { &preamble_5_3_26_bot, &test_case_5_3_26_bot, &postamble_5_3_26_bot };

#define tgrp_case_5_3_27 test_group_11
#define numb_case_5_3_27 "5.3.27"
#define name_case_5_3_27 "Without \"timod\" pushed -- t_sync operation"
#define sref_case_5_3_27 "(none)"
#define desc_case_5_3_27 "\
This test case tests the t_sync operation on a STREAMS file descriptor."

int
test_case_5_3_27_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_SYNC);
}

int
test_case_5_3_27_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_SYNC);
}

#define preamble_5_3_27_top preamble_0
#define preamble_5_3_27_bot preamble_0

#define postamble_5_3_27_top postamble_0
#define postamble_5_3_27_bot postamble_0

static struct test_stream test_5_3_27_top = { &preamble_5_3_27_top, &test_case_5_3_27_top, &postamble_5_3_27_top };
static struct test_stream test_5_3_27_bot = { &preamble_5_3_27_bot, &test_case_5_3_27_bot, &postamble_5_3_27_bot };

#define tgrp_case_5_3_28 test_group_11
#define numb_case_5_3_28 "5.3.28"
#define name_case_5_3_28 "Without \"timod\" pushed -- t_unbind operation"
#define sref_case_5_3_28 "(none)"
#define desc_case_5_3_28 "\
This test case tests the t_unbind operation on a STREAMS file descriptor."

int
test_case_5_3_28_top(int child)
{
	return test_case_5_3_x_top(child, __TEST_T_UNBIND);
}

int
test_case_5_3_28_bot(int child)
{
	return test_case_5_3_x_bot(child, __TEST_T_UNBIND);
}

#define preamble_5_3_28_top preamble_0
#define preamble_5_3_28_bot preamble_0

#define postamble_5_3_28_top postamble_0
#define postamble_5_3_28_bot postamble_0

static struct test_stream test_5_3_28_top = { &preamble_5_3_28_top, &test_case_5_3_28_top, &postamble_5_3_28_top };
static struct test_stream test_5_3_28_bot = { &preamble_5_3_28_bot, &test_case_5_3_28_bot, &postamble_5_3_28_bot };

#define test_group_12 "Valid TPI error acknowledgements for all calls"
#define tgrp_case_6_1_1 test_group_12
#define numb_case_6_1_1 "6.1.1"
#define name_case_6_1_1 "t_accept library call -- TACCES error"
#define sref_case_6_1_1 "(none)"
#define desc_case_6_1_1 "\
This test case tests the TACCES error acknowledgement from the TPI provider\n\
in response to the t_accept library call."

int
test_case_6_1_x_top(int child, int terror, int error)
{
	test_msleep(child, NORMAL_WAIT);
	state++;
	if (do_signal(child, __TEST_T_LISTEN) != __RESULT_SUCCESS)
		goto failure;
	state++;
	last_sequence = 1;
	if (do_signal(child, __TEST_T_ACCEPT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != terror)
		goto failure;
	state++;
	if (last_t_errno == TSYSERR && last_errno != error)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		return (__RESULT_INCONCLUSIVE);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_6_1_x_bot(int child, int terror, int error)
{
	last_sequence = 1;
	test_addr = NULL;
	test_alen = 0;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_data = "Connection indication test data.";
	if (do_signal(child, __TEST_CONN_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_CONN_RES) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	last_t_errno = terror;
	last_errno = error;
	if (do_signal(child, __TEST_ERROR_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_DISCON_REQ) != __RESULT_SUCCESS)
		return (__RESULT_INCONCLUSIVE);
	state++;
	test_msleep(child, NORMAL_WAIT);	/* delay to avoid LiS ioctl bug */
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_6_1_1_top(int child)
{
	return test_case_6_1_x_top(child, TACCES, 0);
}

int
test_case_6_1_1_bot(int child)
{
	return test_case_6_1_x_bot(child, TACCES, 0);
}

#define preamble_6_1_1_top preamble_2_top
#define preamble_6_1_1_bot preamble_2_bot

#define postamble_6_1_1_top postamble_2_top
#define postamble_6_1_1_bot postamble_2_bot

static struct test_stream test_6_1_1_top = { &preamble_6_1_1_top, &test_case_6_1_1_top, &postamble_6_1_1_top };
static struct test_stream test_6_1_1_bot = { &preamble_6_1_1_bot, &test_case_6_1_1_bot, &postamble_6_1_1_bot };

#define tgrp_case_6_1_2 test_group_12
#define numb_case_6_1_2 "6.1.2"
#define name_case_6_1_2 "t_accept library call -- TBADADDR error"
#define sref_case_6_1_2 "(none)"
#define desc_case_6_1_2 "\
This test case tests the TBADADDR error acknowledgement from the TPI provider\n\
in response to the t_accept library call."

int
test_case_6_1_2_top(int child)
{
	return test_case_6_1_x_top(child, TBADADDR, 0);
}

int
test_case_6_1_2_bot(int child)
{
	return test_case_6_1_x_bot(child, TBADADDR, 0);
}

#define preamble_6_1_2_top preamble_2_top
#define preamble_6_1_2_bot preamble_2_bot

#define postamble_6_1_2_top postamble_2_top
#define postamble_6_1_2_bot postamble_2_bot

static struct test_stream test_6_1_2_top = { &preamble_6_1_2_top, &test_case_6_1_2_top, &postamble_6_1_2_top };
static struct test_stream test_6_1_2_bot = { &preamble_6_1_2_bot, &test_case_6_1_2_bot, &postamble_6_1_2_bot };

#define tgrp_case_6_1_3 test_group_12
#define numb_case_6_1_3 "6.1.3"
#define name_case_6_1_3 "t_accept library call -- TBADDATA error"
#define sref_case_6_1_3 "(none)"
#define desc_case_6_1_3 "\
This test case tests the TBADDATA error acknowledgement from the TPI provider\n\
in response to the t_accept library call."

int
test_case_6_1_3_top(int child)
{
	return test_case_6_1_x_top(child, TBADDATA, 0);
}

int
test_case_6_1_3_bot(int child)
{
	return test_case_6_1_x_bot(child, TBADDATA, 0);
}

#define preamble_6_1_3_top preamble_2_top
#define preamble_6_1_3_bot preamble_2_bot

#define postamble_6_1_3_top postamble_2_top
#define postamble_6_1_3_bot postamble_2_bot

static struct test_stream test_6_1_3_top = { &preamble_6_1_3_top, &test_case_6_1_3_top, &postamble_6_1_3_top };
static struct test_stream test_6_1_3_bot = { &preamble_6_1_3_bot, &test_case_6_1_3_bot, &postamble_6_1_3_bot };

#define tgrp_case_6_1_4 test_group_12
#define numb_case_6_1_4 "6.1.4"
#define name_case_6_1_4 "t_accept library call -- TBADF error"
#define sref_case_6_1_4 "(none)"
#define desc_case_6_1_4 "\
This test case tests the TBADF error acknowledgement from the TPI provider\n\
in response to the t_accept library call."

int
test_case_6_1_4_top(int child)
{
	return test_case_6_1_x_top(child, TBADF, 0);
}

int
test_case_6_1_4_bot(int child)
{
	return test_case_6_1_x_bot(child, TBADF, 0);
}

#define preamble_6_1_4_top preamble_2_top
#define preamble_6_1_4_bot preamble_2_bot

#define postamble_6_1_4_top postamble_2_top
#define postamble_6_1_4_bot postamble_2_bot

static struct test_stream test_6_1_4_top = { &preamble_6_1_4_top, &test_case_6_1_4_top, &postamble_6_1_4_top };
static struct test_stream test_6_1_4_bot = { &preamble_6_1_4_bot, &test_case_6_1_4_bot, &postamble_6_1_4_bot };

#define tgrp_case_6_1_5 test_group_12
#define numb_case_6_1_5 "6.1.5"
#define name_case_6_1_5 "t_accept library call -- TBADOPT error"
#define sref_case_6_1_5 "(none)"
#define desc_case_6_1_5 "\
This test case tests the TBADOPT error acknowledgement from the TPI provider\n\
in response to the t_accept library call."

int
test_case_6_1_5_top(int child)
{
	return test_case_6_1_x_top(child, TBADOPT, 0);
}

int
test_case_6_1_5_bot(int child)
{
	return test_case_6_1_x_bot(child, TBADOPT, 0);
}

#define preamble_6_1_5_top preamble_2_top
#define preamble_6_1_5_bot preamble_2_bot

#define postamble_6_1_5_top postamble_2_top
#define postamble_6_1_5_bot postamble_2_bot

static struct test_stream test_6_1_5_top = { &preamble_6_1_5_top, &test_case_6_1_5_top, &postamble_6_1_5_top };
static struct test_stream test_6_1_5_bot = { &preamble_6_1_5_bot, &test_case_6_1_5_bot, &postamble_6_1_5_bot };

#define tgrp_case_6_1_6 test_group_12
#define numb_case_6_1_6 "6.1.6"
#define name_case_6_1_6 "t_accept library call -- TBADSEQ error"
#define sref_case_6_1_6 "(none)"
#define desc_case_6_1_6 "\
This test case tests the TBADSEQ error acknowledgement from the TPI provider\n\
in response to the t_accept library call."

int
test_case_6_1_6_top(int child)
{
	return test_case_6_1_x_top(child, TBADSEQ, 0);
}

int
test_case_6_1_6_bot(int child)
{
	return test_case_6_1_x_bot(child, TBADSEQ, 0);
}

#define preamble_6_1_6_top preamble_2_top
#define preamble_6_1_6_bot preamble_2_bot

#define postamble_6_1_6_top postamble_2_top
#define postamble_6_1_6_bot postamble_2_bot

static struct test_stream test_6_1_6_top = { &preamble_6_1_6_top, &test_case_6_1_6_top, &postamble_6_1_6_top };
static struct test_stream test_6_1_6_bot = { &preamble_6_1_6_bot, &test_case_6_1_6_bot, &postamble_6_1_6_bot };

#define tgrp_case_6_1_7 test_group_12
#define numb_case_6_1_7 "6.1.7"
#define name_case_6_1_7 "t_accept library call -- TNOTSUPPORT error"
#define sref_case_6_1_7 "(none)"
#define desc_case_6_1_7 "\
This test case tests the TNOTSUPPORT error acknowledgement from the TPI provider\n\
in response to the t_accept library call."

int
test_case_6_1_7_top(int child)
{
	return test_case_6_1_x_top(child, TNOTSUPPORT, 0);
}

int
test_case_6_1_7_bot(int child)
{
	return test_case_6_1_x_bot(child, TNOTSUPPORT, 0);
}

#define preamble_6_1_7_top preamble_2_top
#define preamble_6_1_7_bot preamble_2_bot

#define postamble_6_1_7_top postamble_2_top
#define postamble_6_1_7_bot postamble_2_bot

static struct test_stream test_6_1_7_top = { &preamble_6_1_7_top, &test_case_6_1_7_top, &postamble_6_1_7_top };
static struct test_stream test_6_1_7_bot = { &preamble_6_1_7_bot, &test_case_6_1_7_bot, &postamble_6_1_7_bot };

#define tgrp_case_6_1_8 test_group_12
#define numb_case_6_1_8 "6.1.8"
#define name_case_6_1_8 "t_accept library call -- TOUTSTATE error"
#define sref_case_6_1_8 "(none)"
#define desc_case_6_1_8 "\
This test case tests the TOUTSTATE error acknowledgement from the TPI provider\n\
in response to the t_accept library call."

int
test_case_6_1_8_top(int child)
{
	return test_case_6_1_x_top(child, TOUTSTATE, 0);
}

int
test_case_6_1_8_bot(int child)
{
	return test_case_6_1_x_bot(child, TOUTSTATE, 0);
}

#define preamble_6_1_8_top preamble_2_top
#define preamble_6_1_8_bot preamble_2_bot

#define postamble_6_1_8_top postamble_2_top
#define postamble_6_1_8_bot postamble_2_bot

static struct test_stream test_6_1_8_top = { &preamble_6_1_8_top, &test_case_6_1_8_top, &postamble_6_1_8_top };
static struct test_stream test_6_1_8_bot = { &preamble_6_1_8_bot, &test_case_6_1_8_bot, &postamble_6_1_8_bot };

#define tgrp_case_6_1_9 test_group_12
#define numb_case_6_1_9 "6.1.9"
#define name_case_6_1_9 "t_accept library call -- TPROVMISMATCH error"
#define sref_case_6_1_9 "(none)"
#define desc_case_6_1_9 "\
This test case tests the TPROVMISMATCH error acknowledgement from the TPI provider\n\
in response to the t_accept library call."

int
test_case_6_1_9_top(int child)
{
	return test_case_6_1_x_top(child, TPROVMISMATCH, 0);
}

int
test_case_6_1_9_bot(int child)
{
	return test_case_6_1_x_bot(child, TPROVMISMATCH, 0);
}

#define preamble_6_1_9_top preamble_2_top
#define preamble_6_1_9_bot preamble_2_bot

#define postamble_6_1_9_top postamble_2_top
#define postamble_6_1_9_bot postamble_2_bot

static struct test_stream test_6_1_9_top = { &preamble_6_1_9_top, &test_case_6_1_9_top, &postamble_6_1_9_top };
static struct test_stream test_6_1_9_bot = { &preamble_6_1_9_bot, &test_case_6_1_9_bot, &postamble_6_1_9_bot };

#define tgrp_case_6_1_10 test_group_12
#define numb_case_6_1_10 "6.1.10"
#define name_case_6_1_10 "t_accept library call -- TRESADDR error"
#define sref_case_6_1_10 "(none)"
#define desc_case_6_1_10 "\
This test case tests the TRESADDR error acknowledgement from the TPI provider\n\
in response to the t_accept library call."

int
test_case_6_1_10_top(int child)
{
	return test_case_6_1_x_top(child, TRESADDR, 0);
}

int
test_case_6_1_10_bot(int child)
{
	return test_case_6_1_x_bot(child, TRESADDR, 0);
}

#define preamble_6_1_10_top preamble_2_top
#define preamble_6_1_10_bot preamble_2_bot

#define postamble_6_1_10_top postamble_2_top
#define postamble_6_1_10_bot postamble_2_bot

static struct test_stream test_6_1_10_top = { &preamble_6_1_10_top, &test_case_6_1_10_top, &postamble_6_1_10_top };
static struct test_stream test_6_1_10_bot = { &preamble_6_1_10_bot, &test_case_6_1_10_bot, &postamble_6_1_10_bot };

#define tgrp_case_6_1_11 test_group_12
#define numb_case_6_1_11 "6.1.11"
#define name_case_6_1_11 "t_accept library call -- TRESQLEN error"
#define sref_case_6_1_11 "(none)"
#define desc_case_6_1_11 "\
This test case tests the TRESQLEN error acknowledgement from the TPI provider\n\
in response to the t_accept library call."

int
test_case_6_1_11_top(int child)
{
	return test_case_6_1_x_top(child, TRESQLEN, 0);
}

int
test_case_6_1_11_bot(int child)
{
	return test_case_6_1_x_bot(child, TRESQLEN, 0);
}

#define preamble_6_1_11_top preamble_2_top
#define preamble_6_1_11_bot preamble_2_bot

#define postamble_6_1_11_top postamble_2_top
#define postamble_6_1_11_bot postamble_2_bot

static struct test_stream test_6_1_11_top = { &preamble_6_1_11_top, &test_case_6_1_11_top, &postamble_6_1_11_top };
static struct test_stream test_6_1_11_bot = { &preamble_6_1_11_bot, &test_case_6_1_11_bot, &postamble_6_1_11_bot };

#define tgrp_case_6_1_12 test_group_12
#define numb_case_6_1_12 "6.1.12"
#define name_case_6_1_12 "t_accept library call -- TSYSERR error"
#define sref_case_6_1_12 "(none)"
#define desc_case_6_1_12 "\
This test case tests the TSYSERR error acknowledgement from the TPI provider\n\
in response to the t_accept library call."

int
test_case_6_1_12_top(int child)
{
	return test_case_6_1_x_top(child, TSYSERR, EPROTO);
}

int
test_case_6_1_12_bot(int child)
{
	return test_case_6_1_x_bot(child, TSYSERR, EPROTO);
}

#define preamble_6_1_12_top preamble_2_top
#define preamble_6_1_12_bot preamble_2_bot

#define postamble_6_1_12_top postamble_2_top
#define postamble_6_1_12_bot postamble_2_bot

static struct test_stream test_6_1_12_top = { &preamble_6_1_12_top, &test_case_6_1_12_top, &postamble_6_1_12_top };
static struct test_stream test_6_1_12_bot = { &preamble_6_1_12_bot, &test_case_6_1_12_bot, &postamble_6_1_12_bot };

#define tgrp_case_6_2_1 test_group_12
#define numb_case_6_2_1 "6.2.1"
#define name_case_6_2_1 "t_bind library call -- TACCES error"
#define sref_case_6_2_1 "(none)"
#define desc_case_6_2_1 "\
This test case tests the TACCES error acknowledgement from the TPI provider\n\
in response to the t_bind library call."

int
test_case_6_2_x_top(int child, int terror, int error)
{
	if (do_signal(child, __TEST_T_BIND) == __RESULT_SUCCESS)
		goto failure;
	if (last_t_errno != terror)
		goto failure;
	if (last_t_errno == TSYSERR && last_errno != error)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_6_2_x_bot(int child, int terror, int error)
{
	if (expect(child, INFINITE_WAIT, __TEST_BIND_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	last_t_errno = terror;
	last_errno = error;
	if (do_signal(child, __TEST_ERROR_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_6_2_1_top(int child)
{
	return test_case_6_2_x_top(child, TACCES, 0);
}

int
test_case_6_2_1_bot(int child)
{
	return test_case_6_2_x_bot(child, TACCES, 0);
}

#define preamble_6_2_1_top preamble_1_top
#define preamble_6_2_1_bot preamble_1_bot

#define postamble_6_2_1_top postamble_1_top
#define postamble_6_2_1_bot postamble_1_bot

static struct test_stream test_6_2_1_top = { &preamble_6_2_1_top, &test_case_6_2_1_top, &postamble_6_2_1_top };
static struct test_stream test_6_2_1_bot = { &preamble_6_2_1_bot, &test_case_6_2_1_bot, &postamble_6_2_1_bot };

#define tgrp_case_6_2_2 test_group_12
#define numb_case_6_2_2 "6.2.2"
#define name_case_6_2_2 "t_bind library call -- TADDRBUSY error"
#define sref_case_6_2_2 "(none)"
#define desc_case_6_2_2 "\
This test case tests the TADDRBUSY error acknowledgement from the TPI provider\n\
in response to the t_bind library call."

int
test_case_6_2_2_top(int child)
{
	return test_case_6_2_x_top(child, TADDRBUSY, 0);
}

int
test_case_6_2_2_bot(int child)
{
	return test_case_6_2_x_bot(child, TADDRBUSY, 0);
}

#define preamble_6_2_2_top preamble_1_top
#define preamble_6_2_2_bot preamble_1_bot

#define postamble_6_2_2_top postamble_1_top
#define postamble_6_2_2_bot postamble_1_bot

static struct test_stream test_6_2_2_top = { &preamble_6_2_2_top, &test_case_6_2_2_top, &postamble_6_2_2_top };
static struct test_stream test_6_2_2_bot = { &preamble_6_2_2_bot, &test_case_6_2_2_bot, &postamble_6_2_2_bot };

#define tgrp_case_6_2_3 test_group_12
#define numb_case_6_2_3 "6.2.3"
#define name_case_6_2_3 "t_bind library call -- TBADADDR error"
#define sref_case_6_2_3 "(none)"
#define desc_case_6_2_3 "\
This test case tests the TBADADDR error acknowledgement from the TPI provider\n\
in response to the t_bind library call."

int
test_case_6_2_3_top(int child)
{
	return test_case_6_2_x_top(child, TBADADDR, 0);
}

int
test_case_6_2_3_bot(int child)
{
	return test_case_6_2_x_bot(child, TBADADDR, 0);
}

#define preamble_6_2_3_top preamble_1_top
#define preamble_6_2_3_bot preamble_1_bot

#define postamble_6_2_3_top postamble_1_top
#define postamble_6_2_3_bot postamble_1_bot

static struct test_stream test_6_2_3_top = { &preamble_6_2_3_top, &test_case_6_2_3_top, &postamble_6_2_3_top };
static struct test_stream test_6_2_3_bot = { &preamble_6_2_3_bot, &test_case_6_2_3_bot, &postamble_6_2_3_bot };

#define tgrp_case_6_2_4 test_group_12
#define numb_case_6_2_4 "6.2.4"
#define name_case_6_2_4 "t_bind library call -- TNOADDR error"
#define sref_case_6_2_4 "(none)"
#define desc_case_6_2_4 "\
This test case tests the TNOADDR error acknowledgement from the TPI provider\n\
in response to the t_bind library call."

int
test_case_6_2_4_top(int child)
{
	return test_case_6_2_x_top(child, TNOADDR, 0);
}

int
test_case_6_2_4_bot(int child)
{
	return test_case_6_2_x_bot(child, TNOADDR, 0);
}

#define preamble_6_2_4_top preamble_1_top
#define preamble_6_2_4_bot preamble_1_bot

#define postamble_6_2_4_top postamble_1_top
#define postamble_6_2_4_bot postamble_1_bot

static struct test_stream test_6_2_4_top = { &preamble_6_2_4_top, &test_case_6_2_4_top, &postamble_6_2_4_top };
static struct test_stream test_6_2_4_bot = { &preamble_6_2_4_bot, &test_case_6_2_4_bot, &postamble_6_2_4_bot };

#define tgrp_case_6_2_5 test_group_12
#define numb_case_6_2_5 "6.2.5"
#define name_case_6_2_5 "t_bind library call -- TOUTSTATE error"
#define sref_case_6_2_5 "(none)"
#define desc_case_6_2_5 "\
This test case tests the TOUTSTATE error acknowledgement from the TPI provider\n\
in response to the t_bind library call."

int
test_case_6_2_5_top(int child)
{
	return test_case_6_2_x_top(child, TOUTSTATE, 0);
}

int
test_case_6_2_5_bot(int child)
{
	return test_case_6_2_x_bot(child, TOUTSTATE, 0);
}

#define preamble_6_2_5_top preamble_1_top
#define preamble_6_2_5_bot preamble_1_bot

#define postamble_6_2_5_top postamble_1_top
#define postamble_6_2_5_bot postamble_1_bot

static struct test_stream test_6_2_5_top = { &preamble_6_2_5_top, &test_case_6_2_5_top, &postamble_6_2_5_top };
static struct test_stream test_6_2_5_bot = { &preamble_6_2_5_bot, &test_case_6_2_5_bot, &postamble_6_2_5_bot };

#define tgrp_case_6_2_6 test_group_12
#define numb_case_6_2_6 "6.2.6"
#define name_case_6_2_6 "t_bind library call -- TSYSERR error"
#define sref_case_6_2_6 "(none)"
#define desc_case_6_2_6 "\
This test case tests the TSYSERR error acknowledgement from the TPI provider\n\
in response to the t_bind library call."

int
test_case_6_2_6_top(int child)
{
	return test_case_6_2_x_top(child, TSYSERR, EPROTO);
}

int
test_case_6_2_6_bot(int child)
{
	return test_case_6_2_x_bot(child, TSYSERR, EPROTO);
}

#define preamble_6_2_6_top preamble_1_top
#define preamble_6_2_6_bot preamble_1_bot

#define postamble_6_2_6_top postamble_1_top
#define postamble_6_2_6_bot postamble_1_bot

static struct test_stream test_6_2_6_top = { &preamble_6_2_6_top, &test_case_6_2_6_top, &postamble_6_2_6_top };
static struct test_stream test_6_2_6_bot = { &preamble_6_2_6_bot, &test_case_6_2_6_bot, &postamble_6_2_6_bot };

#define tgrp_case_6_3_1 test_group_12
#define numb_case_6_3_1 "6.3.1"
#define name_case_6_3_1 "t_connect library call -- TACCES error"
#define sref_case_6_3_1 "(none)"
#define desc_case_6_3_1 "\
This test case tests the TACCES error acknowledgement from the TPI provider\n\
in response to the t_connect library call."

int
test_case_6_3_x_top(int child, int terror, int error)
{
	if (do_signal(child, __TEST_T_CONNECT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != terror)
		goto failure;
	state++;
	if (last_t_errno == TSYSERR && last_errno != error)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_6_3_x_bot(int child, int terror, int error)
{
	if (expect(child, INFINITE_WAIT, __TEST_CONN_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	last_t_errno = terror;
	last_errno = error;
	if (do_signal(child, __TEST_ERROR_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_6_3_1_top(int child)
{
	return test_case_6_3_x_top(child, TACCES, 0);
}

int
test_case_6_3_1_bot(int child)
{
	return test_case_6_3_x_bot(child, TACCES, 0);
}

#define preamble_6_3_1_top preamble_2_top
#define preamble_6_3_1_bot preamble_2_bot

#define postamble_6_3_1_top postamble_2_top
#define postamble_6_3_1_bot postamble_2_bot

static struct test_stream test_6_3_1_top = { &preamble_6_3_1_top, &test_case_6_3_1_top, &postamble_6_3_1_top };
static struct test_stream test_6_3_1_bot = { &preamble_6_3_1_bot, &test_case_6_3_1_bot, &postamble_6_3_1_bot };

#define tgrp_case_6_3_2 test_group_12
#define numb_case_6_3_2 "6.3.2"
#define name_case_6_3_2 "t_connect library call -- TADDRBUSY error"
#define sref_case_6_3_2 "(none)"
#define desc_case_6_3_2 "\
This test case tests the TADDRBUSY error acknowledgement from the TPI provider\n\
in response to the t_connect library call."

int
test_case_6_3_2_top(int child)
{
	return test_case_6_3_x_top(child, TADDRBUSY, 0);
}

int
test_case_6_3_2_bot(int child)
{
	return test_case_6_3_x_bot(child, TADDRBUSY, 0);
}

#define preamble_6_3_2_top preamble_2_top
#define preamble_6_3_2_bot preamble_2_bot

#define postamble_6_3_2_top postamble_2_top
#define postamble_6_3_2_bot postamble_2_bot

static struct test_stream test_6_3_2_top = { &preamble_6_3_2_top, &test_case_6_3_2_top, &postamble_6_3_2_top };
static struct test_stream test_6_3_2_bot = { &preamble_6_3_2_bot, &test_case_6_3_2_bot, &postamble_6_3_2_bot };

#define tgrp_case_6_3_3 test_group_12
#define numb_case_6_3_3 "6.3.3"
#define name_case_6_3_3 "t_connect library call -- TBADADDR error"
#define sref_case_6_3_3 "(none)"
#define desc_case_6_3_3 "\
This test case tests the TBADADDR error acknowledgement from the TPI provider\n\
in response to the t_connect library call."

int
test_case_6_3_3_top(int child)
{
	return test_case_6_3_x_top(child, TBADADDR, 0);
}

int
test_case_6_3_3_bot(int child)
{
	return test_case_6_3_x_bot(child, TBADADDR, 0);
}

#define preamble_6_3_3_top preamble_2_top
#define preamble_6_3_3_bot preamble_2_bot

#define postamble_6_3_3_top postamble_2_top
#define postamble_6_3_3_bot postamble_2_bot

static struct test_stream test_6_3_3_top = { &preamble_6_3_3_top, &test_case_6_3_3_top, &postamble_6_3_3_top };
static struct test_stream test_6_3_3_bot = { &preamble_6_3_3_bot, &test_case_6_3_3_bot, &postamble_6_3_3_bot };

#define tgrp_case_6_3_4 test_group_12
#define numb_case_6_3_4 "6.3.4"
#define name_case_6_3_4 "t_connect library call -- TBADDATA error"
#define sref_case_6_3_4 "(none)"
#define desc_case_6_3_4 "\
This test case tests the TBADDATA error acknowledgement from the TPI provider\n\
in response to the t_connect library call."

int
test_case_6_3_4_top(int child)
{
	return test_case_6_3_x_top(child, TBADDATA, 0);
}

int
test_case_6_3_4_bot(int child)
{
	return test_case_6_3_x_bot(child, TBADDATA, 0);
}

#define preamble_6_3_4_top preamble_2_top
#define preamble_6_3_4_bot preamble_2_bot

#define postamble_6_3_4_top postamble_2_top
#define postamble_6_3_4_bot postamble_2_bot

static struct test_stream test_6_3_4_top = { &preamble_6_3_4_top, &test_case_6_3_4_top, &postamble_6_3_4_top };
static struct test_stream test_6_3_4_bot = { &preamble_6_3_4_bot, &test_case_6_3_4_bot, &postamble_6_3_4_bot };

#define tgrp_case_6_3_5 test_group_12
#define numb_case_6_3_5 "6.3.5"
#define name_case_6_3_5 "t_connect library call -- TBADOPT error"
#define sref_case_6_3_5 "(none)"
#define desc_case_6_3_5 "\
This test case tests the TBADOPT error acknowledgement from the TPI provider\n\
in response to the t_connect library call."

int
test_case_6_3_5_top(int child)
{
	return test_case_6_3_x_top(child, TBADOPT, 0);
}

int
test_case_6_3_5_bot(int child)
{
	return test_case_6_3_x_bot(child, TBADOPT, 0);
}

#define preamble_6_3_5_top preamble_2_top
#define preamble_6_3_5_bot preamble_2_bot

#define postamble_6_3_5_top postamble_2_top
#define postamble_6_3_5_bot postamble_2_bot

static struct test_stream test_6_3_5_top = { &preamble_6_3_5_top, &test_case_6_3_5_top, &postamble_6_3_5_top };
static struct test_stream test_6_3_5_bot = { &preamble_6_3_5_bot, &test_case_6_3_5_bot, &postamble_6_3_5_bot };

#define tgrp_case_6_3_6 test_group_12
#define numb_case_6_3_6 "6.3.6"
#define name_case_6_3_6 "t_connect library call -- TNOTSUPPORT error"
#define sref_case_6_3_6 "(none)"
#define desc_case_6_3_6 "\
This test case tests the TNOTSUPPORT error acknowledgement from the TPI provider\n\
in response to the t_connect library call."

int
test_case_6_3_6_top(int child)
{
	return test_case_6_3_x_top(child, TNOTSUPPORT, 0);
}

int
test_case_6_3_6_bot(int child)
{
	return test_case_6_3_x_bot(child, TNOTSUPPORT, 0);
}

#define preamble_6_3_6_top preamble_2_top
#define preamble_6_3_6_bot preamble_2_bot

#define postamble_6_3_6_top postamble_2_top
#define postamble_6_3_6_bot postamble_2_bot

static struct test_stream test_6_3_6_top = { &preamble_6_3_6_top, &test_case_6_3_6_top, &postamble_6_3_6_top };
static struct test_stream test_6_3_6_bot = { &preamble_6_3_6_bot, &test_case_6_3_6_bot, &postamble_6_3_6_bot };

#define tgrp_case_6_3_7 test_group_12
#define numb_case_6_3_7 "6.3.7"
#define name_case_6_3_7 "t_connect library call -- TOUTSTATE error"
#define sref_case_6_3_7 "(none)"
#define desc_case_6_3_7 "\
This test case tests the TOUTSTATE error acknowledgement from the TPI provider\n\
in response to the t_connect library call."

int
test_case_6_3_7_top(int child)
{
	return test_case_6_3_x_top(child, TOUTSTATE, 0);
}

int
test_case_6_3_7_bot(int child)
{
	return test_case_6_3_x_bot(child, TOUTSTATE, 0);
}

#define preamble_6_3_7_top preamble_2_top
#define preamble_6_3_7_bot preamble_2_bot

#define postamble_6_3_7_top postamble_2_top
#define postamble_6_3_7_bot postamble_2_bot

static struct test_stream test_6_3_7_top = { &preamble_6_3_7_top, &test_case_6_3_7_top, &postamble_6_3_7_top };
static struct test_stream test_6_3_7_bot = { &preamble_6_3_7_bot, &test_case_6_3_7_bot, &postamble_6_3_7_bot };

#define tgrp_case_6_3_8 test_group_12
#define numb_case_6_3_8 "6.3.8"
#define name_case_6_3_8 "t_connect library call -- TSYSERR error"
#define sref_case_6_3_8 "(none)"
#define desc_case_6_3_8 "\
This test case tests the TSYSERR error acknowledgement from the TPI provider\n\
in response to the t_connect library call."

int
test_case_6_3_8_top(int child)
{
	return test_case_6_3_x_top(child, TSYSERR, EPROTO);
}

int
test_case_6_3_8_bot(int child)
{
	return test_case_6_3_x_bot(child, TSYSERR, EPROTO);
}

#define preamble_6_3_8_top preamble_2_top
#define preamble_6_3_8_bot preamble_2_bot

#define postamble_6_3_8_top postamble_2_top
#define postamble_6_3_8_bot postamble_2_bot

static struct test_stream test_6_3_8_top = { &preamble_6_3_8_top, &test_case_6_3_8_top, &postamble_6_3_8_top };
static struct test_stream test_6_3_8_bot = { &preamble_6_3_8_bot, &test_case_6_3_8_bot, &postamble_6_3_8_bot };

#define tgrp_case_6_4_1 test_group_12
#define numb_case_6_4_1 "6.4.1"
#define name_case_6_4_1 "t_optmgmt library call -- TACCES error"
#define sref_case_6_4_1 "(none)"
#define desc_case_6_4_1 "\
This test case tests the TACCES error acknowledgement from the TPI provider\n\
in response to the t_optmgmt library call."

int
test_case_6_4_x_top(int child, int terror, int error)
{
	if (do_signal(child, __TEST_T_OPTMGMT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != terror)
		goto failure;
	state++;
	if (last_t_errno == TSYSERR && last_errno != error)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_6_4_x_bot(int child, int terror, int error)
{
	if (expect(child, INFINITE_WAIT, __TEST_OPTMGMT_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	last_t_errno = terror;
	last_errno = error;
	if (do_signal(child, __TEST_ERROR_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_6_4_1_top(int child)
{
	return test_case_6_4_x_top(child, TACCES, 0);
}

int
test_case_6_4_1_bot(int child)
{
	return test_case_6_4_x_bot(child, TACCES, 0);
}

#define preamble_6_4_1_top preamble_2_top
#define preamble_6_4_1_bot preamble_2_bot

#define postamble_6_4_1_top postamble_2_top
#define postamble_6_4_1_bot postamble_2_bot

static struct test_stream test_6_4_1_top = { &preamble_6_4_1_top, &test_case_6_4_1_top, &postamble_6_4_1_top };
static struct test_stream test_6_4_1_bot = { &preamble_6_4_1_bot, &test_case_6_4_1_bot, &postamble_6_4_1_bot };

#define tgrp_case_6_4_2 test_group_12
#define numb_case_6_4_2 "6.4.2"
#define name_case_6_4_2 "t_optmgmt library call -- TBADFLAG error"
#define sref_case_6_4_2 "(none)"
#define desc_case_6_4_2 "\
This test case tests the TBADFLAG error acknowledgement from the TPI provider\n\
in response to the t_optmgmt library call."

int
test_case_6_4_2_top(int child)
{
	return test_case_6_4_x_top(child, TBADFLAG, 0);
}

int
test_case_6_4_2_bot(int child)
{
	return test_case_6_4_x_bot(child, TBADFLAG, 0);
}

#define preamble_6_4_2_top preamble_2_top
#define preamble_6_4_2_bot preamble_2_bot

#define postamble_6_4_2_top postamble_2_top
#define postamble_6_4_2_bot postamble_2_bot

static struct test_stream test_6_4_2_top = { &preamble_6_4_2_top, &test_case_6_4_2_top, &postamble_6_4_2_top };
static struct test_stream test_6_4_2_bot = { &preamble_6_4_2_bot, &test_case_6_4_2_bot, &postamble_6_4_2_bot };

#define tgrp_case_6_4_3 test_group_12
#define numb_case_6_4_3 "6.4.3"
#define name_case_6_4_3 "t_optmgmt library call -- TBADOPT error"
#define sref_case_6_4_3 "(none)"
#define desc_case_6_4_3 "\
This test case tests the TBADOPT error acknowledgement from the TPI provider\n\
in response to the t_optmgmt library call."

int
test_case_6_4_3_top(int child)
{
	return test_case_6_4_x_top(child, TBADOPT, 0);
}

int
test_case_6_4_3_bot(int child)
{
	return test_case_6_4_x_bot(child, TBADOPT, 0);
}

#define preamble_6_4_3_top preamble_2_top
#define preamble_6_4_3_bot preamble_2_bot

#define postamble_6_4_3_top postamble_2_top
#define postamble_6_4_3_bot postamble_2_bot

static struct test_stream test_6_4_3_top = { &preamble_6_4_3_top, &test_case_6_4_3_top, &postamble_6_4_3_top };
static struct test_stream test_6_4_3_bot = { &preamble_6_4_3_bot, &test_case_6_4_3_bot, &postamble_6_4_3_bot };

#define tgrp_case_6_4_4 test_group_12
#define numb_case_6_4_4 "6.4.4"
#define name_case_6_4_4 "t_optmgmt library call -- TOUTSTATE error"
#define sref_case_6_4_4 "(none)"
#define desc_case_6_4_4 "\
This test case tests the TOUTSTATE error acknowledgement from the TPI provider\n\
in response to the t_optmgmt library call."

int
test_case_6_4_4_top(int child)
{
	return test_case_6_4_x_top(child, TOUTSTATE, 0);
}

int
test_case_6_4_4_bot(int child)
{
	return test_case_6_4_x_bot(child, TOUTSTATE, 0);
}

#define preamble_6_4_4_top preamble_2_top
#define preamble_6_4_4_bot preamble_2_bot

#define postamble_6_4_4_top postamble_2_top
#define postamble_6_4_4_bot postamble_2_bot

static struct test_stream test_6_4_4_top = { &preamble_6_4_4_top, &test_case_6_4_4_top, &postamble_6_4_4_top };
static struct test_stream test_6_4_4_bot = { &preamble_6_4_4_bot, &test_case_6_4_4_bot, &postamble_6_4_4_bot };

#define tgrp_case_6_4_5 test_group_12
#define numb_case_6_4_5 "6.4.5"
#define name_case_6_4_5 "t_optmgmt library call -- TNOTSUPPORT error"
#define sref_case_6_4_5 "(none)"
#define desc_case_6_4_5 "\
This test case tests the TNOTSUPPORT error acknowledgement from the TPI provider\n\
in response to the t_optmgmt library call."

int
test_case_6_4_5_top(int child)
{
	return test_case_6_4_x_top(child, TNOTSUPPORT, 0);
}

int
test_case_6_4_5_bot(int child)
{
	return test_case_6_4_x_bot(child, TNOTSUPPORT, 0);
}

#define preamble_6_4_5_top preamble_2_top
#define preamble_6_4_5_bot preamble_2_bot

#define postamble_6_4_5_top postamble_2_top
#define postamble_6_4_5_bot postamble_2_bot

static struct test_stream test_6_4_5_top = { &preamble_6_4_5_top, &test_case_6_4_5_top, &postamble_6_4_5_top };
static struct test_stream test_6_4_5_bot = { &preamble_6_4_5_bot, &test_case_6_4_5_bot, &postamble_6_4_5_bot };

#define tgrp_case_6_4_6 test_group_12
#define numb_case_6_4_6 "6.4.6"
#define name_case_6_4_6 "t_optmgmt library call -- TSYSERR error"
#define sref_case_6_4_6 "(none)"
#define desc_case_6_4_6 "\
This test case tests the TSYSERR error acknowledgement from the TPI provider\n\
in response to the t_optmgmt library call."

int
test_case_6_4_6_top(int child)
{
	return test_case_6_4_x_top(child, TSYSERR, EPROTO);
}

int
test_case_6_4_6_bot(int child)
{
	return test_case_6_4_x_bot(child, TSYSERR, EPROTO);
}

#define preamble_6_4_6_top preamble_2_top
#define preamble_6_4_6_bot preamble_2_bot

#define postamble_6_4_6_top postamble_2_top
#define postamble_6_4_6_bot postamble_2_bot

static struct test_stream test_6_4_6_top = { &preamble_6_4_6_top, &test_case_6_4_6_top, &postamble_6_4_6_top };
static struct test_stream test_6_4_6_bot = { &preamble_6_4_6_bot, &test_case_6_4_6_bot, &postamble_6_4_6_bot };

#define tgrp_case_6_5_1 test_group_12
#define numb_case_6_5_1 "6.5.1"
#define name_case_6_5_1 "t_snddis library call -- TBADDATA error"
#define sref_case_6_5_1 "(none)"
#define desc_case_6_5_1 "\
This test case tests the TBADDATA error acknowledgement from the TPI provider\n\
in response to the t_snddis library call."

int
test_case_6_5_x_top(int child, int terror, int error)
{
	if (do_signal(child, __TEST_T_SNDDIS) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != terror)
		goto failure;
	state++;
	if (last_t_errno == TSYSERR && last_errno != error)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_6_5_x_bot(int child, int terror, int error)
{
	if (expect(child, INFINITE_WAIT, __TEST_DISCON_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	last_t_errno = terror;
	last_errno = error;
	if (do_signal(child, __TEST_ERROR_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_6_5_1_top(int child)
{
	return test_case_6_5_x_top(child, TBADDATA, 0);
}

int
test_case_6_5_1_bot(int child)
{
	return test_case_6_5_x_bot(child, TBADDATA, 0);
}

#define preamble_6_5_1_top preamble_3_top
#define preamble_6_5_1_bot preamble_3_bot

#define postamble_6_5_1_top postamble_3_top
#define postamble_6_5_1_bot postamble_3_bot

static struct test_stream test_6_5_1_top = { &preamble_6_5_1_top, &test_case_6_5_1_top, &postamble_6_5_1_top };
static struct test_stream test_6_5_1_bot = { &preamble_6_5_1_bot, &test_case_6_5_1_bot, &postamble_6_5_1_bot };

#define tgrp_case_6_5_2 test_group_12
#define numb_case_6_5_2 "6.5.2"
#define name_case_6_5_2 "t_snddis library call -- TBADSEQ error"
#define sref_case_6_5_2 "(none)"
#define desc_case_6_5_2 "\
This test case tests the TBADSEQ error acknowledgement from the TPI provider\n\
in response to the t_snddis library call."

int
test_case_6_5_2_top(int child)
{
	return test_case_6_5_x_top(child, TBADSEQ, 0);
}

int
test_case_6_5_2_bot(int child)
{
	return test_case_6_5_x_bot(child, TBADSEQ, 0);
}

#define preamble_6_5_2_top preamble_3_top
#define preamble_6_5_2_bot preamble_3_bot

#define postamble_6_5_2_top postamble_3_top
#define postamble_6_5_2_bot postamble_3_bot

static struct test_stream test_6_5_2_top = { &preamble_6_5_2_top, &test_case_6_5_2_top, &postamble_6_5_2_top };
static struct test_stream test_6_5_2_bot = { &preamble_6_5_2_bot, &test_case_6_5_2_bot, &postamble_6_5_2_bot };

#define tgrp_case_6_5_3 test_group_12
#define numb_case_6_5_3 "6.5.3"
#define name_case_6_5_3 "t_snddis library call -- TNOTSUPPORT error"
#define sref_case_6_5_3 "(none)"
#define desc_case_6_5_3 "\
This test case tests the TNOTSUPPORT error acknowledgement from the TPI provider\n\
in response to the t_snddis library call."

int
test_case_6_5_3_top(int child)
{
	return test_case_6_5_x_top(child, TNOTSUPPORT, 0);
}

int
test_case_6_5_3_bot(int child)
{
	return test_case_6_5_x_bot(child, TNOTSUPPORT, 0);
}

#define preamble_6_5_3_top preamble_3_top
#define preamble_6_5_3_bot preamble_3_bot

#define postamble_6_5_3_top postamble_3_top
#define postamble_6_5_3_bot postamble_3_bot

static struct test_stream test_6_5_3_top = { &preamble_6_5_3_top, &test_case_6_5_3_top, &postamble_6_5_3_top };
static struct test_stream test_6_5_3_bot = { &preamble_6_5_3_bot, &test_case_6_5_3_bot, &postamble_6_5_3_bot };

#define tgrp_case_6_5_4 test_group_12
#define numb_case_6_5_4 "6.5.4"
#define name_case_6_5_4 "t_snddis library call -- TOUTSTATE error"
#define sref_case_6_5_4 "(none)"
#define desc_case_6_5_4 "\
This test case tests the TOUTSTATE error acknowledgement from the TPI provider\n\
in response to the t_snddis library call."

int
test_case_6_5_4_top(int child)
{
	return test_case_6_5_x_top(child, TOUTSTATE, 0);
}

int
test_case_6_5_4_bot(int child)
{
	return test_case_6_5_x_bot(child, TOUTSTATE, 0);
}

#define preamble_6_5_4_top preamble_3_top
#define preamble_6_5_4_bot preamble_3_bot

#define postamble_6_5_4_top postamble_3_top
#define postamble_6_5_4_bot postamble_3_bot

static struct test_stream test_6_5_4_top = { &preamble_6_5_4_top, &test_case_6_5_4_top, &postamble_6_5_4_top };
static struct test_stream test_6_5_4_bot = { &preamble_6_5_4_bot, &test_case_6_5_4_bot, &postamble_6_5_4_bot };

#define tgrp_case_6_5_5 test_group_12
#define numb_case_6_5_5 "6.5.5"
#define name_case_6_5_5 "t_snddis library call -- TSYSERR error"
#define sref_case_6_5_5 "(none)"
#define desc_case_6_5_5 "\
This test case tests the TSYSERR error acknowledgement from the TPI provider\n\
in response to the t_snddis library call."

int
test_case_6_5_5_top(int child)
{
	return test_case_6_5_x_top(child, TSYSERR, EPROTO);
}

int
test_case_6_5_5_bot(int child)
{
	return test_case_6_5_x_bot(child, TSYSERR, EPROTO);
}

#define preamble_6_5_5_top preamble_3_top
#define preamble_6_5_5_bot preamble_3_bot

#define postamble_6_5_5_top postamble_3_top
#define postamble_6_5_5_bot postamble_3_bot

static struct test_stream test_6_5_5_top = { &preamble_6_5_5_top, &test_case_6_5_5_top, &postamble_6_5_5_top };
static struct test_stream test_6_5_5_bot = { &preamble_6_5_5_bot, &test_case_6_5_5_bot, &postamble_6_5_5_bot };

#define tgrp_case_6_6_1 test_group_12
#define numb_case_6_6_1 "6.6.1"
#define name_case_6_6_1 "t_unbind library call -- TOUTSTATE error"
#define sref_case_6_6_1 "(none)"
#define desc_case_6_6_1 "\
This test case tests the TOUTSTATE error acknowledgement from the TPI provider\n\
in response to the t_unbind library call."

int
test_case_6_6_x_top(int child, int terror, int error)
{
	if (do_signal(child, __TEST_T_UNBIND) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != terror)
		goto failure;
	state++;
	if (last_t_errno == TSYSERR && last_errno != error)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_6_6_x_bot(int child, int terror, int error)
{
	if (expect(child, INFINITE_WAIT, __TEST_UNBIND_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	last_t_errno = terror;
	last_errno = error;
	if (do_signal(child, __TEST_ERROR_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_6_6_1_top(int child)
{
	return test_case_6_6_x_top(child, TOUTSTATE, 0);
}

int
test_case_6_6_1_bot(int child)
{
	return test_case_6_6_x_bot(child, TOUTSTATE, 0);
}

#define preamble_6_6_1_top preamble_2cl_top
#define preamble_6_6_1_bot preamble_2cl_bot

#define postamble_6_6_1_top postamble_2cl_top
#define postamble_6_6_1_bot postamble_2cl_bot

static struct test_stream test_6_6_1_top = { &preamble_6_6_1_top, &test_case_6_6_1_top, &postamble_6_6_1_top };
static struct test_stream test_6_6_1_bot = { &preamble_6_6_1_bot, &test_case_6_6_1_bot, &postamble_6_6_1_bot };

#define tgrp_case_6_6_2 test_group_12
#define numb_case_6_6_2 "6.6.2"
#define name_case_6_6_2 "t_unbind library call -- TSYSERR error"
#define sref_case_6_6_2 "(none)"
#define desc_case_6_6_2 "\
This test case tests the TSYSERR error acknowledgement from the TPI provider\n\
in response to the t_unbind library call."

int
test_case_6_6_2_top(int child)
{
	return test_case_6_6_x_top(child, TSYSERR, EPROTO);
}

int
test_case_6_6_2_bot(int child)
{
	return test_case_6_6_x_bot(child, TSYSERR, EPROTO);
}

#define preamble_6_6_2_top preamble_2cl_top
#define preamble_6_6_2_bot preamble_2cl_bot

#define postamble_6_6_2_top postamble_2cl_top
#define postamble_6_6_2_bot postamble_2cl_bot

static struct test_stream test_6_6_2_top = { &preamble_6_6_2_top, &test_case_6_6_2_top, &postamble_6_6_2_top };
static struct test_stream test_6_6_2_bot = { &preamble_6_6_2_bot, &test_case_6_6_2_bot, &postamble_6_6_2_bot };

#define test_group_13 "Error codes returned by the XTI library"
#define tgrp_case_7_1_1 test_group_13
#define numb_case_7_1_1 "7.1.1"
#define name_case_7_1_1 "t_accept library call -- TINDOUT error"
#define sref_case_7_1_1 "(none)"
#define desc_case_7_1_1 "\
This test case tests the TINDOUT error from the XTI library in response to\n\
the t_accept library call."

int
test_case_7_1_1_top(int child)
{
	if (do_signal(child, __TEST_T_LISTEN) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LISTEN) != __RESULT_SUCCESS)
		goto failure;
	state++;
	last_sequence = 1;
	if (do_signal(child, __TEST_T_ACCEPT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TINDOUT)
		goto failure;
	state++;
	last_sequence = 1;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	last_sequence = 2;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_1_1_bot(int child)
{
	last_sequence = 1;
	test_addr = NULL;
	test_alen = 0;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_data = "Connection indication test data.";
	if (do_signal(child, __TEST_CONN_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	last_sequence = 2;
	test_addr = NULL;
	test_alen = 0;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_data = "Connection indication test data.";
	if (do_signal(child, __TEST_CONN_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_DISCON_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_DISCON_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_7_1_1_top preamble_2_top
#define preamble_7_1_1_bot preamble_2_bot

#define postamble_7_1_1_top postamble_2_top
#define postamble_7_1_1_bot postamble_2_bot

static struct test_stream test_7_1_1_top = { &preamble_7_1_1_top, &test_case_7_1_1_top, &postamble_7_1_1_top };
static struct test_stream test_7_1_1_bot = { &preamble_7_1_1_bot, &test_case_7_1_1_bot, &postamble_7_1_1_bot };

#define tgrp_case_7_1_2 test_group_13
#define numb_case_7_1_2 "7.1.2"
#define name_case_7_1_2 "t_accept library call -- TNOTSUPPORT error"
#define sref_case_7_1_2 "(none)"
#define desc_case_7_1_2 "\
This test case tests the TNOTSUPPORT error from the XTI library in response to\n\
the t_accept library call."

int
test_case_7_1_2_top(int child)
{
	last_sequence = 1;
	if (do_signal(child, __TEST_T_ACCEPT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOTSUPPORT)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_1_2_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_1_2_top preamble_2cl_top
#define preamble_7_1_2_bot preamble_2cl_bot

#define postamble_7_1_2_top postamble_2cl_top
#define postamble_7_1_2_bot postamble_2cl_bot

static struct test_stream test_7_1_2_top = { &preamble_7_1_2_top, &test_case_7_1_2_top, &postamble_7_1_2_top };
static struct test_stream test_7_1_2_bot = { &preamble_7_1_2_bot, &test_case_7_1_2_bot, &postamble_7_1_2_bot };

#define tgrp_case_7_1_3 test_group_13
#define numb_case_7_1_3 "7.1.3"
#define name_case_7_1_3 "t_accept library call -- TOUTSTATE error"
#define sref_case_7_1_3 "(none)"
#define desc_case_7_1_3 "\
This test case tests the TOUTSTATE error from the XTI library in response to\n\
the t_accept library call."

int
test_case_7_1_3_top(int child)
{
	last_sequence = 1;
	if (do_signal(child, __TEST_T_ACCEPT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_1_3_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_1_3_top preamble_2_top
#define preamble_7_1_3_bot preamble_2_bot

#define postamble_7_1_3_top postamble_2_top
#define postamble_7_1_3_bot postamble_2_bot

static struct test_stream test_7_1_3_top = { &preamble_7_1_3_top, &test_case_7_1_3_top, &postamble_7_1_3_top };
static struct test_stream test_7_1_3_bot = { &preamble_7_1_3_bot, &test_case_7_1_3_bot, &postamble_7_1_3_bot };

#define tgrp_case_7_2_1 test_group_13
#define numb_case_7_2_1 "7.2.1"
#define name_case_7_2_1 "t_connect library call -- TOUTSTATE error"
#define sref_case_7_2_1 "(none)"
#define desc_case_7_2_1 "\
This test case tests the TOUTSTATE error from the XTI library in response to\n\
the t_connect library call."

int
test_case_7_2_1_top(int child)
{
	if (do_signal(child, __TEST_T_CONNECT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_2_1_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_2_1_top preamble_3_top
#define preamble_7_2_1_bot preamble_3_bot

#define postamble_7_2_1_top postamble_3_top
#define postamble_7_2_1_bot postamble_3_bot

static struct test_stream test_7_2_1_top = { &preamble_7_2_1_top, &test_case_7_2_1_top, &postamble_7_2_1_top };
static struct test_stream test_7_2_1_bot = { &preamble_7_2_1_bot, &test_case_7_2_1_bot, &postamble_7_2_1_bot };

#define tgrp_case_7_3_1 test_group_13
#define numb_case_7_3_1 "7.3.1"
#define name_case_7_3_1 "t_listen library call -- TBADQLEN error"
#define sref_case_7_3_1 "(none)"
#define desc_case_7_3_1 "\
This test case tests the TBADQLEN error from the XTI library in response to\n\
the t_listen library call."

int
test_case_7_3_1_top(int child)
{
	last_qlen = 0;
	if (do_signal(child, __TEST_T_BIND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LISTEN) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TBADQLEN)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_UNBIND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_3_1_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_BIND_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	last_qlen = 0;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_BIND_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_UNBIND_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_7_3_1_top preamble_1_top
#define preamble_7_3_1_bot preamble_1_bot

#define postamble_7_3_1_top postamble_1_top
#define postamble_7_3_1_bot postamble_1_bot

static struct test_stream test_7_3_1_top = { &preamble_7_3_1_top, &test_case_7_3_1_top, &postamble_7_3_1_top };
static struct test_stream test_7_3_1_bot = { &preamble_7_3_1_bot, &test_case_7_3_1_bot, &postamble_7_3_1_bot };

#define tgrp_case_7_3_2 test_group_13
#define numb_case_7_3_2 "7.3.2"
#define name_case_7_3_2 "t_listen library call -- TNOTSUPPORT error"
#define sref_case_7_3_2 "(none)"
#define desc_case_7_3_2 "\
This test case tests the TNOTSUPPORT error from the XTI library in response to\n\
the t_listen library call."

int
test_case_7_3_2_top(int child)
{
	if (do_signal(child, __TEST_T_LISTEN) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOTSUPPORT)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_3_2_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_3_2_top preamble_2cl_top
#define preamble_7_3_2_bot preamble_2cl_bot

#define postamble_7_3_2_top postamble_2cl_top
#define postamble_7_3_2_bot postamble_2cl_bot

static struct test_stream test_7_3_2_top = { &preamble_7_3_2_top, &test_case_7_3_2_top, &postamble_7_3_2_top };
static struct test_stream test_7_3_2_bot = { &preamble_7_3_2_bot, &test_case_7_3_2_bot, &postamble_7_3_2_bot };

#define tgrp_case_7_3_3 test_group_13
#define numb_case_7_3_3 "7.3.3"
#define name_case_7_3_3 "t_listen library call -- TOUTSTATE error"
#define sref_case_7_3_3 "(none)"
#define desc_case_7_3_3 "\
This test case tests the TOUTSTATE error from the XTI library in response to\n\
the t_listen library call."

int
test_case_7_3_3_top(int child)
{
	if (do_signal(child, __TEST_T_LISTEN) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_3_3_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_3_3_top preamble_3_top
#define preamble_7_3_3_bot preamble_3_bot

#define postamble_7_3_3_top postamble_3_top
#define postamble_7_3_3_bot postamble_3_bot

static struct test_stream test_7_3_3_top = { &preamble_7_3_3_top, &test_case_7_3_3_top, &postamble_7_3_3_top };
static struct test_stream test_7_3_3_bot = { &preamble_7_3_3_bot, &test_case_7_3_3_bot, &postamble_7_3_3_bot };

#define tgrp_case_7_3_4 test_group_13
#define numb_case_7_3_4 "7.3.4"
#define name_case_7_3_4 "t_listen library call -- TQFULL error"
#define sref_case_7_3_4 "(none)"
#define desc_case_7_3_4 "\
This test case tests the TQFULL error from the XTI library in response to\n\
the t_listen library call."

int
test_case_7_3_4_top(int child)
{
	if (do_signal(child, __TEST_T_LISTEN) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LISTEN) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LISTEN) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TQFULL)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_3_4_bot(int child)
{
	last_sequence = 1;
	test_data = "Connection indication test data.";
	test_addr = NULL;
	test_alen = 0;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	if (do_signal(child, __TEST_CONN_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	last_sequence = 2;
	test_addr = NULL;
	test_alen = 0;
	test_opts = &opt_conn;
	test_olen = sizeof(opt_conn);
	test_data = "Connection indication test data.";
	if (do_signal(child, __TEST_CONN_IND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_DISCON_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_DISCON_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_7_3_4_top preamble_2_top
#define preamble_7_3_4_bot preamble_2_bot

#define postamble_7_3_4_top postamble_2_top
#define postamble_7_3_4_bot postamble_2_bot

static struct test_stream test_7_3_4_top = { &preamble_7_3_4_top, &test_case_7_3_4_top, &postamble_7_3_4_top };
static struct test_stream test_7_3_4_bot = { &preamble_7_3_4_bot, &test_case_7_3_4_bot, &postamble_7_3_4_bot };

#define tgrp_case_7_3_5 test_group_13
#define numb_case_7_3_5 "7.3.5"
#define name_case_7_3_5 "t_listen library call -- TNODATA error"
#define sref_case_7_3_5 "(none)"
#define desc_case_7_3_5 "\
This test case tests the TNODATA error from the XTI library in response to\n\
the t_listen library call."

int
test_case_7_3_5_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_LISTEN) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNODATA)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_3_5_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_3_5_top preamble_2_top
#define preamble_7_3_5_bot preamble_2_bot

#define postamble_7_3_5_top postamble_2_top
#define postamble_7_3_5_bot postamble_2_bot

static struct test_stream test_7_3_5_top = { &preamble_7_3_5_top, &test_case_7_3_5_top, &postamble_7_3_5_top };
static struct test_stream test_7_3_5_bot = { &preamble_7_3_5_bot, &test_case_7_3_5_bot, &postamble_7_3_5_bot };

#define tgrp_case_7_4_1 test_group_13
#define numb_case_7_4_1 "7.4.1"
#define name_case_7_4_1 "t_rcv library call -- TNODATA error"
#define sref_case_7_4_1 "(none)"
#define desc_case_7_4_1 "\
This test case tests the TNODATA error from the XTI library in response to\n\
the t_rcv library call."

int
test_case_7_4_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCV) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNODATA)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_4_1_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_4_1_top preamble_3_top
#define preamble_7_4_1_bot preamble_3_bot

#define postamble_7_4_1_top postamble_3_top
#define postamble_7_4_1_bot postamble_3_bot

static struct test_stream test_7_4_1_top = { &preamble_7_4_1_top, &test_case_7_4_1_top, &postamble_7_4_1_top };
static struct test_stream test_7_4_1_bot = { &preamble_7_4_1_bot, &test_case_7_4_1_bot, &postamble_7_4_1_bot };

#define tgrp_case_7_4_2 test_group_13
#define numb_case_7_4_2 "7.4.2"
#define name_case_7_4_2 "t_rcv library call -- TNOTSUPPORT error"
#define sref_case_7_4_2 "(none)"
#define desc_case_7_4_2 "\
This test case tests the TNOTSUPPORT error from the XTI library in response to\n\
the t_rcv library call."

int
test_case_7_4_2_top(int child)
{
	if (do_signal(child, __TEST_T_RCV) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOTSUPPORT)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_4_2_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_4_2_top preamble_2cl_top
#define preamble_7_4_2_bot preamble_2cl_bot

#define postamble_7_4_2_top postamble_2cl_top
#define postamble_7_4_2_bot postamble_2cl_bot

static struct test_stream test_7_4_2_top = { &preamble_7_4_2_top, &test_case_7_4_2_top, &postamble_7_4_2_top };
static struct test_stream test_7_4_2_bot = { &preamble_7_4_2_bot, &test_case_7_4_2_bot, &postamble_7_4_2_bot };

#define tgrp_case_7_4_3 test_group_13
#define numb_case_7_4_3 "7.4.3"
#define name_case_7_4_3 "t_rcv library call -- TOUTSTATE error"
#define sref_case_7_4_3 "(none)"
#define desc_case_7_4_3 "\
This test case tests the TOUTSTATE error from the XTI library in response to\n\
the t_rcv library call."

int
test_case_7_4_3_top(int child)
{
	if (do_signal(child, __TEST_T_RCV) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_4_3_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_4_3_top preamble_2_top
#define preamble_7_4_3_bot preamble_2_bot

#define postamble_7_4_3_top postamble_2_top
#define postamble_7_4_3_bot postamble_2_bot

static struct test_stream test_7_4_3_top = { &preamble_7_4_3_top, &test_case_7_4_3_top, &postamble_7_4_3_top };
static struct test_stream test_7_4_3_bot = { &preamble_7_4_3_bot, &test_case_7_4_3_bot, &postamble_7_4_3_bot };

#define tgrp_case_7_5_1 test_group_13
#define numb_case_7_5_1 "7.5.1"
#define name_case_7_5_1 "t_rcvv library call -- TNODATA error"
#define sref_case_7_5_1 "(none)"
#define desc_case_7_5_1 "\
This test case tests the TNODATA error from the XTI library in response to\n\
the t_rcvv library call."

int
test_case_7_5_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVV) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNODATA)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_5_1_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_5_1_top preamble_3_top
#define preamble_7_5_1_bot preamble_3_bot

#define postamble_7_5_1_top postamble_3_top
#define postamble_7_5_1_bot postamble_3_bot

static struct test_stream test_7_5_1_top = { &preamble_7_5_1_top, &test_case_7_5_1_top, &postamble_7_5_1_top };
static struct test_stream test_7_5_1_bot = { &preamble_7_5_1_bot, &test_case_7_5_1_bot, &postamble_7_5_1_bot };

#define tgrp_case_7_5_2 test_group_13
#define numb_case_7_5_2 "7.5.2"
#define name_case_7_5_2 "t_rcvv library call -- TNOTSUPPORT error"
#define sref_case_7_5_2 "(none)"
#define desc_case_7_5_2 "\
This test case tests the TNOTSUPPORT error from the XTI library in response to\n\
the t_rcvv library call."

int
test_case_7_5_2_top(int child)
{
	if (do_signal(child, __TEST_T_RCVV) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOTSUPPORT)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_5_2_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_5_2_top preamble_2cl_top
#define preamble_7_5_2_bot preamble_2cl_bot

#define postamble_7_5_2_top postamble_2cl_top
#define postamble_7_5_2_bot postamble_2cl_bot

static struct test_stream test_7_5_2_top = { &preamble_7_5_2_top, &test_case_7_5_2_top, &postamble_7_5_2_top };
static struct test_stream test_7_5_2_bot = { &preamble_7_5_2_bot, &test_case_7_5_2_bot, &postamble_7_5_2_bot };

#define tgrp_case_7_5_3 test_group_13
#define numb_case_7_5_3 "7.5.3"
#define name_case_7_5_3 "t_rcvv library call -- TOUTSTATE error"
#define sref_case_7_5_3 "(none)"
#define desc_case_7_5_3 "\
This test case tests the TOUTSTATE error from the XTI library in response to\n\
the t_rcvv library call."

int
test_case_7_5_3_top(int child)
{
	if (do_signal(child, __TEST_T_RCVV) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_5_3_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_5_3_top preamble_2_top
#define preamble_7_5_3_bot preamble_2_bot

#define postamble_7_5_3_top postamble_2_top
#define postamble_7_5_3_bot postamble_2_bot

static struct test_stream test_7_5_3_top = { &preamble_7_5_3_top, &test_case_7_5_3_top, &postamble_7_5_3_top };
static struct test_stream test_7_5_3_bot = { &preamble_7_5_3_bot, &test_case_7_5_3_bot, &postamble_7_5_3_bot };

#define tgrp_case_7_6_1 test_group_13
#define numb_case_7_6_1 "7.6.1"
#define name_case_7_6_1 "t_rcvconnect library call -- TNODATA error"
#define sref_case_7_6_1 "(none)"
#define desc_case_7_6_1 "\
This test case tests the TNODATA error from the XTI library in response to\n\
the t_rcvconnect library call."

int
test_case_7_6_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_CONNECT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNODATA)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVCONNECT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNODATA)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDDIS) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_6_1_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_CONN_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (expect(child, INFINITE_WAIT, __TEST_DISCON_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_OK_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_7_6_1_top preamble_2_top
#define preamble_7_6_1_bot preamble_2_bot

#define postamble_7_6_1_top postamble_2_top
#define postamble_7_6_1_bot postamble_2_bot

static struct test_stream test_7_6_1_top = { &preamble_7_6_1_top, &test_case_7_6_1_top, &postamble_7_6_1_top };
static struct test_stream test_7_6_1_bot = { &preamble_7_6_1_bot, &test_case_7_6_1_bot, &postamble_7_6_1_bot };

#define tgrp_case_7_6_2 test_group_13
#define numb_case_7_6_2 "7.6.2"
#define name_case_7_6_2 "t_rcvconnect library call -- TNOTSUPPORT error"
#define sref_case_7_6_2 "(none)"
#define desc_case_7_6_2 "\
This test case tests the TNOTSUPPORT error from the XTI library in response to\n\
the t_rcvconnect library call."

int
test_case_7_6_2_top(int child)
{
	if (do_signal(child, __TEST_T_RCVCONNECT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOTSUPPORT)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_6_2_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_6_2_top preamble_2cl_top
#define preamble_7_6_2_bot preamble_2cl_bot

#define postamble_7_6_2_top postamble_2cl_top
#define postamble_7_6_2_bot postamble_2cl_bot

static struct test_stream test_7_6_2_top = { &preamble_7_6_2_top, &test_case_7_6_2_top, &postamble_7_6_2_top };
static struct test_stream test_7_6_2_bot = { &preamble_7_6_2_bot, &test_case_7_6_2_bot, &postamble_7_6_2_bot };

#define tgrp_case_7_6_3 test_group_13
#define numb_case_7_6_3 "7.6.3"
#define name_case_7_6_3 "t_rcvconnect library call -- TOUTSTATE error"
#define sref_case_7_6_3 "(none)"
#define desc_case_7_6_3 "\
This test case tests the TOUTSTATE error from the XTI library in response to\n\
the t_rcvconnect library call."

int
test_case_7_6_3_top(int child)
{
	if (do_signal(child, __TEST_T_RCVCONNECT) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_6_3_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_6_3_top preamble_2_top
#define preamble_7_6_3_bot preamble_2_bot

#define postamble_7_6_3_top postamble_2_top
#define postamble_7_6_3_bot postamble_2_bot

static struct test_stream test_7_6_3_top = { &preamble_7_6_3_top, &test_case_7_6_3_top, &postamble_7_6_3_top };
static struct test_stream test_7_6_3_bot = { &preamble_7_6_3_bot, &test_case_7_6_3_bot, &postamble_7_6_3_bot };

#define tgrp_case_7_7_1 test_group_13
#define numb_case_7_7_1 "7.7.1"
#define name_case_7_7_1 "t_rcvdis library call -- TNODIS error"
#define sref_case_7_7_1 "(none)"
#define desc_case_7_7_1 "\
This test case tests the TNODIS error from the XTI library in response to\n\
the t_rcvdis library call."

int
test_case_7_7_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVDIS) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNODIS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_7_1_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_7_1_top preamble_3_top
#define preamble_7_7_1_bot preamble_3_bot

#define postamble_7_7_1_top postamble_3_top
#define postamble_7_7_1_bot postamble_3_bot

static struct test_stream test_7_7_1_top = { &preamble_7_7_1_top, &test_case_7_7_1_top, &postamble_7_7_1_top };
static struct test_stream test_7_7_1_bot = { &preamble_7_7_1_bot, &test_case_7_7_1_bot, &postamble_7_7_1_bot };

#define tgrp_case_7_7_2 test_group_13
#define numb_case_7_7_2 "7.7.2"
#define name_case_7_7_2 "t_rcvdis library call -- TNOTSUPPORT error"
#define sref_case_7_7_2 "(none)"
#define desc_case_7_7_2 "\
This test case tests the TNOTSUPPORT error from the XTI library in response to\n\
the t_rcvdis library call."

int
test_case_7_7_2_top(int child)
{
	if (do_signal(child, __TEST_T_RCVDIS) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOTSUPPORT)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_7_2_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_7_2_top preamble_2cl_top
#define preamble_7_7_2_bot preamble_2cl_bot

#define postamble_7_7_2_top postamble_2cl_top
#define postamble_7_7_2_bot postamble_2cl_bot

static struct test_stream test_7_7_2_top = { &preamble_7_7_2_top, &test_case_7_7_2_top, &postamble_7_7_2_top };
static struct test_stream test_7_7_2_bot = { &preamble_7_7_2_bot, &test_case_7_7_2_bot, &postamble_7_7_2_bot };

#define tgrp_case_7_7_3 test_group_13
#define numb_case_7_7_3 "7.7.3"
#define name_case_7_7_3 "t_rcvdis library call -- TOUTSTATE error"
#define sref_case_7_7_3 "(none)"
#define desc_case_7_7_3 "\
This test case tests the TOUTSTATE error from the XTI library in response to\n\
the t_rcvdis library call."

int
test_case_7_7_3_top(int child)
{
	if (do_signal(child, __TEST_T_RCVDIS) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_7_3_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_7_3_top preamble_2_top
#define preamble_7_7_3_bot preamble_2_bot

#define postamble_7_7_3_top postamble_2_top
#define postamble_7_7_3_bot postamble_2_bot

static struct test_stream test_7_7_3_top = { &preamble_7_7_3_top, &test_case_7_7_3_top, &postamble_7_7_3_top };
static struct test_stream test_7_7_3_bot = { &preamble_7_7_3_bot, &test_case_7_7_3_bot, &postamble_7_7_3_bot };

#define tgrp_case_7_8_1 test_group_13
#define numb_case_7_8_1 "7.8.1"
#define name_case_7_8_1 "t_rcvrel library call -- TNOREL error"
#define sref_case_7_8_1 "(none)"
#define desc_case_7_8_1 "\
This test case tests the TNOREL error from the XTI library in response to\n\
the t_rcvrel library call."

int
test_case_7_8_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVREL) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOREL)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_8_1_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_8_1_top preamble_3_top
#define preamble_7_8_1_bot preamble_3_bot

#define postamble_7_8_1_top postamble_3_top
#define postamble_7_8_1_bot postamble_3_bot

static struct test_stream test_7_8_1_top = { &preamble_7_8_1_top, &test_case_7_8_1_top, &postamble_7_8_1_top };
static struct test_stream test_7_8_1_bot = { &preamble_7_8_1_bot, &test_case_7_8_1_bot, &postamble_7_8_1_bot };

#define tgrp_case_7_8_2 test_group_13
#define numb_case_7_8_2 "7.8.2"
#define name_case_7_8_2 "t_rcvrel library call -- TNOTSUPPORT error"
#define sref_case_7_8_2 "(none)"
#define desc_case_7_8_2 "\
This test case tests the TNOTSUPPORT error from the XTI library in response to\n\
the t_rcvrel library call."

int
test_case_7_8_2_top(int child)
{
	if (do_signal(child, __TEST_T_RCVREL) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOTSUPPORT)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_8_2_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_8_2_top preamble_2cl_top
#define preamble_7_8_2_bot preamble_2cl_bot

#define postamble_7_8_2_top postamble_2cl_top
#define postamble_7_8_2_bot postamble_2cl_bot

static struct test_stream test_7_8_2_top = { &preamble_7_8_2_top, &test_case_7_8_2_top, &postamble_7_8_2_top };
static struct test_stream test_7_8_2_bot = { &preamble_7_8_2_bot, &test_case_7_8_2_bot, &postamble_7_8_2_bot };

#define tgrp_case_7_8_3 test_group_13
#define numb_case_7_8_3 "7.8.3"
#define name_case_7_8_3 "t_rcvrel library call -- TOUTSTATE error"
#define sref_case_7_8_3 "(none)"
#define desc_case_7_8_3 "\
This test case tests the TOUTSTATE error from the XTI library in response to\n\
the t_rcvrel library call."

int
test_case_7_8_3_top(int child)
{
	if (do_signal(child, __TEST_T_RCVREL) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_8_3_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_8_3_top preamble_2_top
#define preamble_7_8_3_bot preamble_2_bot

#define postamble_7_8_3_top postamble_2_top
#define postamble_7_8_3_bot postamble_2_bot

static struct test_stream test_7_8_3_top = { &preamble_7_8_3_top, &test_case_7_8_3_top, &postamble_7_8_3_top };
static struct test_stream test_7_8_3_bot = { &preamble_7_8_3_bot, &test_case_7_8_3_bot, &postamble_7_8_3_bot };

#define tgrp_case_7_9_1 test_group_13
#define numb_case_7_9_1 "7.9.1"
#define name_case_7_9_1 "t_rcvreldata library call -- TNOREL error"
#define sref_case_7_9_1 "(none)"
#define desc_case_7_9_1 "\
This test case tests the TNOREL error from the XTI library in response to\n\
the t_rcvreldata library call."

int
test_case_7_9_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVRELDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOREL)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_9_1_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_9_1_top preamble_3_top
#define preamble_7_9_1_bot preamble_3_bot

#define postamble_7_9_1_top postamble_3_top
#define postamble_7_9_1_bot postamble_3_bot

static struct test_stream test_7_9_1_top = { &preamble_7_9_1_top, &test_case_7_9_1_top, &postamble_7_9_1_top };
static struct test_stream test_7_9_1_bot = { &preamble_7_9_1_bot, &test_case_7_9_1_bot, &postamble_7_9_1_bot };

#define tgrp_case_7_9_2 test_group_13
#define numb_case_7_9_2 "7.9.2"
#define name_case_7_9_2 "t_rcvreldata library call -- TNOTSUPPORT error"
#define sref_case_7_9_2 "(none)"
#define desc_case_7_9_2 "\
This test case tests the TNOTSUPPORT error from the XTI library in response to\n\
the t_rcvreldata library call."

int
test_case_7_9_2_top(int child)
{
	if (do_signal(child, __TEST_T_RCVRELDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOTSUPPORT)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_9_2_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_9_2_top preamble_2cl_top
#define preamble_7_9_2_bot preamble_2cl_bot

#define postamble_7_9_2_top postamble_2cl_top
#define postamble_7_9_2_bot postamble_2cl_bot

static struct test_stream test_7_9_2_top = { &preamble_7_9_2_top, &test_case_7_9_2_top, &postamble_7_9_2_top };
static struct test_stream test_7_9_2_bot = { &preamble_7_9_2_bot, &test_case_7_9_2_bot, &postamble_7_9_2_bot };

#define tgrp_case_7_9_3 test_group_13
#define numb_case_7_9_3 "7.9.3"
#define name_case_7_9_3 "t_rcvreldata library call -- TOUTSTATE error"
#define sref_case_7_9_3 "(none)"
#define desc_case_7_9_3 "\
This test case tests the TOUTSTATE error from the XTI library in response to\n\
the t_rcvreldata library call."

int
test_case_7_9_3_top(int child)
{
	if (do_signal(child, __TEST_T_RCVRELDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_9_3_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_9_3_top preamble_2_top
#define preamble_7_9_3_bot preamble_2_bot

#define postamble_7_9_3_top postamble_2_top
#define postamble_7_9_3_bot postamble_2_bot

static struct test_stream test_7_9_3_top = { &preamble_7_9_3_top, &test_case_7_9_3_top, &postamble_7_9_3_top };
static struct test_stream test_7_9_3_bot = { &preamble_7_9_3_bot, &test_case_7_9_3_bot, &postamble_7_9_3_bot };

#define tgrp_case_7_10_1 test_group_13
#define numb_case_7_10_1 "7.10.1"
#define name_case_7_10_1 "t_rcvudata library call -- TNODATA error"
#define sref_case_7_10_1 "(none)"
#define desc_case_7_10_1 "\
This test case tests the TNODATA error from the XTI library in response to\n\
the t_rcvudata library call."

int
test_case_7_10_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVUDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNODATA)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_10_1_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_10_1_top preamble_2cl_top
#define preamble_7_10_1_bot preamble_2cl_bot

#define postamble_7_10_1_top postamble_2cl_top
#define postamble_7_10_1_bot postamble_2cl_bot

static struct test_stream test_7_10_1_top = { &preamble_7_10_1_top, &test_case_7_10_1_top, &postamble_7_10_1_top };
static struct test_stream test_7_10_1_bot = { &preamble_7_10_1_bot, &test_case_7_10_1_bot, &postamble_7_10_1_bot };

#define tgrp_case_7_10_2 test_group_13
#define numb_case_7_10_2 "7.10.2"
#define name_case_7_10_2 "t_rcvudata library call -- TNOTSUPPORT error"
#define sref_case_7_10_2 "(none)"
#define desc_case_7_10_2 "\
This test case tests the TNOTSUPPORT error from the XTI library in response to\n\
the t_rcvudata library call."

int
test_case_7_10_2_top(int child)
{
	if (do_signal(child, __TEST_T_RCVUDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOTSUPPORT)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_10_2_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_10_2_top preamble_2_top
#define preamble_7_10_2_bot preamble_2_bot

#define postamble_7_10_2_top postamble_2_top
#define postamble_7_10_2_bot postamble_2_bot

static struct test_stream test_7_10_2_top = { &preamble_7_10_2_top, &test_case_7_10_2_top, &postamble_7_10_2_top };
static struct test_stream test_7_10_2_bot = { &preamble_7_10_2_bot, &test_case_7_10_2_bot, &postamble_7_10_2_bot };

#define tgrp_case_7_10_3 test_group_13
#define numb_case_7_10_3 "7.10.3"
#define name_case_7_10_3 "t_rcvudata library call -- TOUTSTATE error"
#define sref_case_7_10_3 "(none)"
#define desc_case_7_10_3 "\
This test case tests the TOUTSTATE error from the XTI library in response to\n\
the t_rcvudata library call."

int
test_case_7_10_3_top(int child)
{
	if (do_signal(child, __TEST_T_RCVUDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_10_3_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_10_3_top preamble_1cl_top
#define preamble_7_10_3_bot preamble_1cl_bot

#define postamble_7_10_3_top postamble_1cl_top
#define postamble_7_10_3_bot postamble_1cl_bot

static struct test_stream test_7_10_3_top = { &preamble_7_10_3_top, &test_case_7_10_3_top, &postamble_7_10_3_top };
static struct test_stream test_7_10_3_bot = { &preamble_7_10_3_bot, &test_case_7_10_3_bot, &postamble_7_10_3_bot };

#define tgrp_case_7_11_1 test_group_13
#define numb_case_7_11_1 "7.11.1"
#define name_case_7_11_1 "t_rcvvudata library call -- TNODATA error"
#define sref_case_7_11_1 "(none)"
#define desc_case_7_11_1 "\
This test case tests the TNODATA error from the XTI library in response to\n\
the t_rcvvudata library call."

int
test_case_7_11_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVVUDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNODATA)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_11_1_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_11_1_top preamble_2cl_top
#define preamble_7_11_1_bot preamble_2cl_bot

#define postamble_7_11_1_top postamble_2cl_top
#define postamble_7_11_1_bot postamble_2cl_bot

static struct test_stream test_7_11_1_top = { &preamble_7_11_1_top, &test_case_7_11_1_top, &postamble_7_11_1_top };
static struct test_stream test_7_11_1_bot = { &preamble_7_11_1_bot, &test_case_7_11_1_bot, &postamble_7_11_1_bot };

#define tgrp_case_7_11_2 test_group_13
#define numb_case_7_11_2 "7.11.2"
#define name_case_7_11_2 "t_rcvvudata library call -- TNOTSUPPORT error"
#define sref_case_7_11_2 "(none)"
#define desc_case_7_11_2 "\
This test case tests the TNOTSUPPORT error from the XTI library in response to\n\
the t_rcvvudata library call."

int
test_case_7_11_2_top(int child)
{
	if (do_signal(child, __TEST_T_RCVVUDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOTSUPPORT)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_11_2_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_11_2_top preamble_2_top
#define preamble_7_11_2_bot preamble_2_bot

#define postamble_7_11_2_top postamble_2_top
#define postamble_7_11_2_bot postamble_2_bot

static struct test_stream test_7_11_2_top = { &preamble_7_11_2_top, &test_case_7_11_2_top, &postamble_7_11_2_top };
static struct test_stream test_7_11_2_bot = { &preamble_7_11_2_bot, &test_case_7_11_2_bot, &postamble_7_11_2_bot };

#define tgrp_case_7_11_3 test_group_13
#define numb_case_7_11_3 "7.11.3"
#define name_case_7_11_3 "t_rcvvudata library call -- TOUTSTATE error"
#define sref_case_7_11_3 "(none)"
#define desc_case_7_11_3 "\
This test case tests the TOUTSTATE error from the XTI library in response to\n\
the t_rcvvudata library call."

int
test_case_7_11_3_top(int child)
{
	if (do_signal(child, __TEST_T_RCVVUDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_11_3_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_11_3_top preamble_1cl_top
#define preamble_7_11_3_bot preamble_1cl_bot

#define postamble_7_11_3_top postamble_1cl_top
#define postamble_7_11_3_bot postamble_1cl_bot

static struct test_stream test_7_11_3_top = { &preamble_7_11_3_top, &test_case_7_11_3_top, &postamble_7_11_3_top };
static struct test_stream test_7_11_3_bot = { &preamble_7_11_3_bot, &test_case_7_11_3_bot, &postamble_7_11_3_bot };

#define tgrp_case_7_12_1 test_group_13
#define numb_case_7_12_1 "7.12.1"
#define name_case_7_12_1 "t_rcvuderr library call -- TNOUDERR error"
#define sref_case_7_12_1 "(none)"
#define desc_case_7_12_1 "\
This test case tests the TNOUDERR error from the XTI library in response to\n\
the t_rcvuderr library call."

int
test_case_7_12_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_RCVUDERR) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOUDERR)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_12_1_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_12_1_top preamble_2cl_top
#define preamble_7_12_1_bot preamble_2cl_bot

#define postamble_7_12_1_top postamble_2cl_top
#define postamble_7_12_1_bot postamble_2cl_bot

static struct test_stream test_7_12_1_top = { &preamble_7_12_1_top, &test_case_7_12_1_top, &postamble_7_12_1_top };
static struct test_stream test_7_12_1_bot = { &preamble_7_12_1_bot, &test_case_7_12_1_bot, &postamble_7_12_1_bot };

#define tgrp_case_7_12_2 test_group_13
#define numb_case_7_12_2 "7.12.2"
#define name_case_7_12_2 "t_rcvuderr library call -- TNOTSUPPORT error"
#define sref_case_7_12_2 "(none)"
#define desc_case_7_12_2 "\
This test case tests the TNOTSUPPORT error from the XTI library in response to\n\
the t_rcvuderr library call."

int
test_case_7_12_2_top(int child)
{
	if (do_signal(child, __TEST_T_RCVUDERR) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOTSUPPORT)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_12_2_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_12_2_top preamble_2_top
#define preamble_7_12_2_bot preamble_2_bot

#define postamble_7_12_2_top postamble_2_top
#define postamble_7_12_2_bot postamble_2_bot

static struct test_stream test_7_12_2_top = { &preamble_7_12_2_top, &test_case_7_12_2_top, &postamble_7_12_2_top };
static struct test_stream test_7_12_2_bot = { &preamble_7_12_2_bot, &test_case_7_12_2_bot, &postamble_7_12_2_bot };

#define tgrp_case_7_12_3 test_group_13
#define numb_case_7_12_3 "7.12.3"
#define name_case_7_12_3 "t_rcvuderr library call -- TOUTSTATE error"
#define sref_case_7_12_3 "(none)"
#define desc_case_7_12_3 "\
This test case tests the TOUTSTATE error from the XTI library in response to\n\
the t_rcvuderr library call."

int
test_case_7_12_3_top(int child)
{
	if (do_signal(child, __TEST_T_RCVUDERR) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_12_3_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_12_3_top preamble_1cl_top
#define preamble_7_12_3_bot preamble_1cl_bot

#define postamble_7_12_3_top postamble_1cl_top
#define postamble_7_12_3_bot postamble_1cl_bot

static struct test_stream test_7_12_3_top = { &preamble_7_12_3_top, &test_case_7_12_3_top, &postamble_7_12_3_top };
static struct test_stream test_7_12_3_bot = { &preamble_7_12_3_bot, &test_case_7_12_3_bot, &postamble_7_12_3_bot };

#define tgrp_case_7_13_1 test_group_13
#define numb_case_7_13_1 "7.13.1"
#define name_case_7_13_1 "t_snd library call -- TFLOW error"
#define sref_case_7_13_1 "(none)"
#define desc_case_7_13_1 "\
This test case tests the TFLOW error from the XTI library in response to\n\
the t_snd library call."

int
test_case_7_13_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SND) != __RESULT_SUCCESS)
		goto failure;
	state++;
	show_data = 0;
	while (do_signal(child, __TEST_T_SND) == __RESULT_SUCCESS) ;
	show_data = 1;
	state++;
	if (last_t_errno != TFLOW)
		goto failure;
	state++;
	test_msleep(child, LONGER_WAIT << 2);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_13_1_bot(int child)
{
	test_msleep(child, LONGER_WAIT << 1);
	for (;;) {
		state++;
		switch (wait_event(child, NORMAL_WAIT)) {
		case __TEST_DATA_REQ:
			show_data = 0;
			continue;
		case __EVENT_NO_MSG:
			show_data = 1;
			break;
		default:
			show_data = 1;
			goto failure;
		}
		break;
	}
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_7_13_1_top preamble_3_top
#define preamble_7_13_1_bot preamble_3_bot

#define postamble_7_13_1_top postamble_3_top
#define postamble_7_13_1_bot postamble_3_bot

static struct test_stream test_7_13_1_top = { &preamble_7_13_1_top, &test_case_7_13_1_top, &postamble_7_13_1_top };
static struct test_stream test_7_13_1_bot = { &preamble_7_13_1_bot, &test_case_7_13_1_bot, &postamble_7_13_1_bot };

#define tgrp_case_7_13_2 test_group_13
#define numb_case_7_13_2 "7.13.2"
#define name_case_7_13_2 "t_snd library call -- TNOTSUPPORT error"
#define sref_case_7_13_2 "(none)"
#define desc_case_7_13_2 "\
This test case tests the TNOTSUPPORT error from the XTI library in response to\n\
the t_snd library call."

int
test_case_7_13_2_top(int child)
{
	if (do_signal(child, __TEST_T_SND) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOTSUPPORT)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_13_2_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_13_2_top preamble_2cl_top
#define preamble_7_13_2_bot preamble_2cl_bot

#define postamble_7_13_2_top postamble_2cl_top
#define postamble_7_13_2_bot postamble_2cl_bot

static struct test_stream test_7_13_2_top = { &preamble_7_13_2_top, &test_case_7_13_2_top, &postamble_7_13_2_top };
static struct test_stream test_7_13_2_bot = { &preamble_7_13_2_bot, &test_case_7_13_2_bot, &postamble_7_13_2_bot };

#define tgrp_case_7_13_3 test_group_13
#define numb_case_7_13_3 "7.13.3"
#define name_case_7_13_3 "t_snd library call -- TOUTSTATE error"
#define sref_case_7_13_3 "(none)"
#define desc_case_7_13_3 "\
This test case tests the TOUTSTATE error from the XTI library in response to\n\
the t_snd library call."

int
test_case_7_13_3_top(int child)
{
	if (do_signal(child, __TEST_T_SND) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_13_3_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_13_3_top preamble_2_top
#define preamble_7_13_3_bot preamble_2_bot

#define postamble_7_13_3_top postamble_2_top
#define postamble_7_13_3_bot postamble_2_bot

static struct test_stream test_7_13_3_top = { &preamble_7_13_3_top, &test_case_7_13_3_top, &postamble_7_13_3_top };
static struct test_stream test_7_13_3_bot = { &preamble_7_13_3_bot, &test_case_7_13_3_bot, &postamble_7_13_3_bot };

#define tgrp_case_7_14_1 test_group_13
#define numb_case_7_14_1 "7.14.1"
#define name_case_7_14_1 "t_sndv library call -- TFLOW error"
#define sref_case_7_14_1 "(none)"
#define desc_case_7_14_1 "\
This test case tests the TFLOW error from the XTI library in response to\n\
the t_sndv library call."

int
test_case_7_14_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDV) != __RESULT_SUCCESS)
		goto failure;
	state++;
	show_data = 0;
	while (do_signal(child, __TEST_T_SNDV) == __RESULT_SUCCESS) ;
	show_data = 1;
	state++;
	if (last_t_errno != TFLOW)
		goto failure;
	state++;
	test_msleep(child, LONGER_WAIT << 2);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_14_1_bot(int child)
{
	test_msleep(child, LONGER_WAIT << 1);
	for (;;) {
		state++;
		switch (wait_event(child, NORMAL_WAIT)) {
		case __TEST_DATA_REQ:
			show_data = 0;
			continue;
		case __EVENT_NO_MSG:
			show_data = 1;
			break;
		default:
			show_data = 1;
			goto failure;
		}
		break;
	}
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_7_14_1_top preamble_3_top
#define preamble_7_14_1_bot preamble_3_bot

#define postamble_7_14_1_top postamble_3_top
#define postamble_7_14_1_bot postamble_3_bot

static struct test_stream test_7_14_1_top = { &preamble_7_14_1_top, &test_case_7_14_1_top, &postamble_7_14_1_top };
static struct test_stream test_7_14_1_bot = { &preamble_7_14_1_bot, &test_case_7_14_1_bot, &postamble_7_14_1_bot };

#define tgrp_case_7_14_2 test_group_13
#define numb_case_7_14_2 "7.14.2"
#define name_case_7_14_2 "t_sndv library call -- TNOTSUPPORT error"
#define sref_case_7_14_2 "(none)"
#define desc_case_7_14_2 "\
This test case tests the TNOTSUPPORT error from the XTI library in response to\n\
the t_sndv library call."

int
test_case_7_14_2_top(int child)
{
	if (do_signal(child, __TEST_T_SNDV) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOTSUPPORT)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_14_2_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_14_2_top preamble_2cl_top
#define preamble_7_14_2_bot preamble_2cl_bot

#define postamble_7_14_2_top postamble_2cl_top
#define postamble_7_14_2_bot postamble_2cl_bot

static struct test_stream test_7_14_2_top = { &preamble_7_14_2_top, &test_case_7_14_2_top, &postamble_7_14_2_top };
static struct test_stream test_7_14_2_bot = { &preamble_7_14_2_bot, &test_case_7_14_2_bot, &postamble_7_14_2_bot };

#define tgrp_case_7_14_3 test_group_13
#define numb_case_7_14_3 "7.14.3"
#define name_case_7_14_3 "t_sndv library call -- TOUTSTATE error"
#define sref_case_7_14_3 "(none)"
#define desc_case_7_14_3 "\
This test case tests the TOUTSTATE error from the XTI library in response to\n\
the t_sndv library call."

int
test_case_7_14_3_top(int child)
{
	if (do_signal(child, __TEST_T_SNDV) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_14_3_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_14_3_top preamble_2_top
#define preamble_7_14_3_bot preamble_2_bot

#define postamble_7_14_3_top postamble_2_top
#define postamble_7_14_3_bot postamble_2_bot

static struct test_stream test_7_14_3_top = { &preamble_7_14_3_top, &test_case_7_14_3_top, &postamble_7_14_3_top };
static struct test_stream test_7_14_3_bot = { &preamble_7_14_3_bot, &test_case_7_14_3_bot, &postamble_7_14_3_bot };

#define tgrp_case_7_15_1 test_group_13
#define numb_case_7_15_1 "7.15.1"
#define name_case_7_15_1 "t_sndudata library call -- TFLOW error"
#define sref_case_7_15_1 "(none)"
#define desc_case_7_15_1 "\
This test case tests the TFLOW error from the XTI library in response to\n\
the t_sndudata library call."

int
test_case_7_15_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDUDATA) != __RESULT_SUCCESS)
		goto failure;
	state++;
	show_data = 0;
	while (do_signal(child, __TEST_T_SNDUDATA) == __RESULT_SUCCESS) ;
	show_data = 1;
	state++;
	if (last_t_errno != TFLOW)
		goto failure;
	state++;
	test_msleep(child, LONGER_WAIT << 2);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_15_1_bot(int child)
{
	test_msleep(child, LONGER_WAIT << 1);
	for (;;) {
		state++;
		switch (wait_event(child, NORMAL_WAIT)) {
		case __TEST_UNITDATA_REQ:
			show_data = 0;
			continue;
		case __EVENT_NO_MSG:
			show_data = 1;
			break;
		default:
			show_data = 1;
			goto failure;
		}
		break;
	}
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_7_15_1_top preamble_2cl_top
#define preamble_7_15_1_bot preamble_2cl_bot

#define postamble_7_15_1_top postamble_2cl_top
#define postamble_7_15_1_bot postamble_2cl_bot

static struct test_stream test_7_15_1_top = { &preamble_7_15_1_top, &test_case_7_15_1_top, &postamble_7_15_1_top };
static struct test_stream test_7_15_1_bot = { &preamble_7_15_1_bot, &test_case_7_15_1_bot, &postamble_7_15_1_bot };

#define tgrp_case_7_15_2 test_group_13
#define numb_case_7_15_2 "7.15.2"
#define name_case_7_15_2 "t_sndudata library call -- TNOTSUPPORT error"
#define sref_case_7_15_2 "(none)"
#define desc_case_7_15_2 "\
This test case tests the TNOTSUPPORT error from the XTI library in response to\n\
the t_sndudata library call."

int
test_case_7_15_2_top(int child)
{
	if (do_signal(child, __TEST_T_SNDUDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOTSUPPORT)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_15_2_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_15_2_top preamble_2_top
#define preamble_7_15_2_bot preamble_2_bot

#define postamble_7_15_2_top postamble_2_top
#define postamble_7_15_2_bot postamble_2_bot

static struct test_stream test_7_15_2_top = { &preamble_7_15_2_top, &test_case_7_15_2_top, &postamble_7_15_2_top };
static struct test_stream test_7_15_2_bot = { &preamble_7_15_2_bot, &test_case_7_15_2_bot, &postamble_7_15_2_bot };

#define tgrp_case_7_15_3 test_group_13
#define numb_case_7_15_3 "7.15.3"
#define name_case_7_15_3 "t_sndudata library call -- TOUTSTATE error"
#define sref_case_7_15_3 "(none)"
#define desc_case_7_15_3 "\
This test case tests the TOUTSTATE error from the XTI library in response to\n\
the t_sndudata library call."

int
test_case_7_15_3_top(int child)
{
	if (do_signal(child, __TEST_T_SNDUDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_15_3_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_15_3_top preamble_1cl_top
#define preamble_7_15_3_bot preamble_1cl_bot

#define postamble_7_15_3_top postamble_1cl_top
#define postamble_7_15_3_bot postamble_1cl_bot

static struct test_stream test_7_15_3_top = { &preamble_7_15_3_top, &test_case_7_15_3_top, &postamble_7_15_3_top };
static struct test_stream test_7_15_3_bot = { &preamble_7_15_3_bot, &test_case_7_15_3_bot, &postamble_7_15_3_bot };

#define tgrp_case_7_16_1 test_group_13
#define numb_case_7_16_1 "7.16.1"
#define name_case_7_16_1 "t_sndvudata library call -- TFLOW error"
#define sref_case_7_16_1 "(none)"
#define desc_case_7_16_1 "\
This test case tests the TFLOW error from the XTI library in response to\n\
the t_sndvudata library call."

int
test_case_7_16_1_top(int child)
{
	if (do_signal(child, __TEST_O_NONBLOCK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SNDVUDATA) != __RESULT_SUCCESS)
		goto failure;
	state++;
	show_data = 0;
	while (do_signal(child, __TEST_T_SNDVUDATA) == __RESULT_SUCCESS) ;
	show_data = 1;
	state++;
	if (last_t_errno != TFLOW)
		goto failure;
	state++;
	test_msleep(child, LONGER_WAIT << 2);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_16_1_bot(int child)
{
	test_msleep(child, LONGER_WAIT << 1);
	for (;;) {
		state++;
		switch (wait_event(child, NORMAL_WAIT)) {
		case __TEST_UNITDATA_REQ:
			show_data = 0;
			continue;
		case __EVENT_NO_MSG:
			show_data = 1;
			break;
		default:
			show_data = 1;
			goto failure;
		}
		break;
	}
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_7_16_1_top preamble_2cl_top
#define preamble_7_16_1_bot preamble_2cl_bot

#define postamble_7_16_1_top postamble_2cl_top
#define postamble_7_16_1_bot postamble_2cl_bot

static struct test_stream test_7_16_1_top = { &preamble_7_16_1_top, &test_case_7_16_1_top, &postamble_7_16_1_top };
static struct test_stream test_7_16_1_bot = { &preamble_7_16_1_bot, &test_case_7_16_1_bot, &postamble_7_16_1_bot };

#define tgrp_case_7_16_2 test_group_13
#define numb_case_7_16_2 "7.16.2"
#define name_case_7_16_2 "t_sndvudata library call -- TNOTSUPPORT error"
#define sref_case_7_16_2 "(none)"
#define desc_case_7_16_2 "\
This test case tests the TNOTSUPPORT error from the XTI library in response to\n\
the t_sndvudata library call."

int
test_case_7_16_2_top(int child)
{
	if (do_signal(child, __TEST_T_SNDVUDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TNOTSUPPORT)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_16_2_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_16_2_top preamble_2_top
#define preamble_7_16_2_bot preamble_2_bot

#define postamble_7_16_2_top postamble_2_top
#define postamble_7_16_2_bot postamble_2_bot

static struct test_stream test_7_16_2_top = { &preamble_7_16_2_top, &test_case_7_16_2_top, &postamble_7_16_2_top };
static struct test_stream test_7_16_2_bot = { &preamble_7_16_2_bot, &test_case_7_16_2_bot, &postamble_7_16_2_bot };

#define tgrp_case_7_16_3 test_group_13
#define numb_case_7_16_3 "7.16.3"
#define name_case_7_16_3 "t_sndvudata library call -- TOUTSTATE error"
#define sref_case_7_16_3 "(none)"
#define desc_case_7_16_3 "\
This test case tests the TOUTSTATE error from the XTI library in response to\n\
the t_sndvudata library call."

int
test_case_7_16_3_top(int child)
{
	if (do_signal(child, __TEST_T_SNDVUDATA) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TOUTSTATE)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_16_3_bot(int child)
{
	return (__RESULT_SUCCESS);
}

#define preamble_7_16_3_top preamble_1cl_top
#define preamble_7_16_3_bot preamble_1cl_bot

#define postamble_7_16_3_top postamble_1cl_top
#define postamble_7_16_3_bot postamble_1cl_bot

static struct test_stream test_7_16_3_top = { &preamble_7_16_3_top, &test_case_7_16_3_top, &postamble_7_16_3_top };
static struct test_stream test_7_16_3_bot = { &preamble_7_16_3_bot, &test_case_7_16_3_bot, &postamble_7_16_3_bot };

#define tgrp_case_7_17_1 test_group_13
#define numb_case_7_17_1 "7.17.1"
#define name_case_7_17_1 "t_sync library call -- TSTATECHNG error"
#define sref_case_7_17_1 "(none)"
#define desc_case_7_17_1 "\
This test case tests the TSTATECHNG error from the XTI library in response to\n\
the t_sync library call."

int
test_case_7_17_1_top(int child)
{
	if (do_signal(child, __TEST_PUSH) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_SYNC) == __RESULT_SUCCESS)
		goto failure;
	state++;
	if (last_t_errno != TSTATECHNG)
		goto failure;
	state++;
	if (do_signal(child, __TEST_POP) != __RESULT_SUCCESS)
		goto failure;
	state++;
	if (do_signal(child, __TEST_T_CLOSE) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_7_17_1_bot(int child)
{
	if (expect(child, INFINITE_WAIT, __TEST_CAPABILITY_REQ) != __RESULT_SUCCESS)
		goto failure;
	state++;
	last_tstate = TS_WACK_BREQ;
	test_msleep(child, SHORT_WAIT);	/* delay to avoid LiS ioctl bug */
	if (do_signal(child, __TEST_CAPABILITY_ACK) != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_7_17_1_top preamble_0
#define preamble_7_17_1_bot preamble_0

#define postamble_7_17_1_top postamble_0
#define postamble_7_17_1_bot postamble_0

static struct test_stream test_7_17_1_top = { &preamble_7_17_1_top, &test_case_7_17_1_top, &postamble_7_17_1_top };
static struct test_stream test_7_17_1_bot = { &preamble_7_17_1_bot, &test_case_7_17_1_bot, &postamble_7_17_1_bot };

#define test_group_14 "Passing data"
#define tgrp_case_8_1_1 test_group_14
#define numb_case_8_1_1 "8.1.1"
#define name_case_8_1_1 "receiving data with t_rcv -- normal data"
#define sref_case_8_1_1 "(none)"
#define desc_case_8_1_1 "\
This test case tests positive test cases for the t_rcv library call."

int
test_case_8_1_1_top(int child)
{
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (do_signal(child, __TEST_T_RCV)) {
		case __RESULT_SUCCESS:
			continue;
		case __RESULT_FAILURE:
			if (get_event(child) != __EVENT_TIMEOUT)
				goto failure;
			break;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_1_1_bot(int child)
{
	int endstate = state + 19;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate + 1; state++) {
		MORE_flag = (state < endstate) ? 1 : 0;
		test_data = "Normal test data.";
		if (do_signal(child, __TEST_DATA_IND) != __RESULT_SUCCESS) {
			MORE_flag = 0;
			goto failure;
		}
	}
	MORE_flag = 0;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_1_1_top preamble_3_top
#define preamble_8_1_1_bot preamble_3_bot

#define postamble_8_1_1_top postamble_3_top
#define postamble_8_1_1_bot postamble_3_bot

static struct test_stream test_8_1_1_top = { &preamble_8_1_1_top, &test_case_8_1_1_top, &postamble_8_1_1_top };
static struct test_stream test_8_1_1_bot = { &preamble_8_1_1_bot, &test_case_8_1_1_bot, &postamble_8_1_1_bot };

#define tgrp_case_8_1_2 test_group_14
#define numb_case_8_1_2 "8.1.2"
#define name_case_8_1_2 "receiving data with t_rcv -- expedited data"
#define sref_case_8_1_2 "(none)"
#define desc_case_8_1_2 "\
This test case tests positive test cases for the t_rcv library call."

int
test_case_8_1_2_top(int child)
{
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (do_signal(child, __TEST_T_RCV)) {
		case __RESULT_SUCCESS:
			continue;
		case __RESULT_FAILURE:
			if (get_event(child) != __EVENT_TIMEOUT)
				goto failure;
			break;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_1_2_bot(int child)
{
	int endstate = state + 19;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate + 1; state++) {
		MORE_flag = (state < endstate) ? 1 : 0;
		if (do_signal(child, __TEST_EXDATA_IND) != __RESULT_SUCCESS) {
			MORE_flag = 0;
			goto failure;
		}
	}
	MORE_flag = 0;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_1_2_top preamble_3_top
#define preamble_8_1_2_bot preamble_3_bot

#define postamble_8_1_2_top postamble_3_top
#define postamble_8_1_2_bot postamble_3_bot

static struct test_stream test_8_1_2_top = { &preamble_8_1_2_top, &test_case_8_1_2_top, &postamble_8_1_2_top };
static struct test_stream test_8_1_2_bot = { &preamble_8_1_2_bot, &test_case_8_1_2_bot, &postamble_8_1_2_bot };

#define tgrp_case_8_1_3 test_group_14
#define numb_case_8_1_3 "8.1.3"
#define name_case_8_1_3 "receiving data with t_rcv -- normal data with intermingled expedited data"
#define sref_case_8_1_3 "(none)"
#define desc_case_8_1_3 "\
This test case tests positive test cases for the t_rcv library call."

int
test_case_8_1_3_top(int child)
{
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (do_signal(child, __TEST_T_RCV)) {
		case __RESULT_SUCCESS:
			continue;
		case __RESULT_FAILURE:
			if (get_event(child) != __EVENT_TIMEOUT)
				goto failure;
			break;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_1_3_bot(int child)
{
	int begstate = state;
	int endstate = state + 19;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate + 1; state++) {
		MORE_flag = (state < endstate) ? 1 : 0;
		test_data = "Normal test data.";
		if (do_signal(child, __TEST_DATA_IND) != __RESULT_SUCCESS) {
			MORE_flag = 0;
			goto failure;
		}
		if (state == begstate + 10) {
			test_msleep(child, NORMAL_WAIT);
			MORE_flag = 1;
			if (do_signal(child, __TEST_EXDATA_IND) != __RESULT_SUCCESS) {
				MORE_flag = 0;
				goto failure;
			}
		}
		if (state == begstate + 15) {
			test_msleep(child, NORMAL_WAIT);
			MORE_flag = 0;
			if (do_signal(child, __TEST_EXDATA_IND) != __RESULT_SUCCESS) {
				MORE_flag = 0;
				goto failure;
			}
		}
	}
	MORE_flag = 0;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_1_3_top preamble_3_top
#define preamble_8_1_3_bot preamble_3_bot

#define postamble_8_1_3_top postamble_3_top
#define postamble_8_1_3_bot postamble_3_bot

static struct test_stream test_8_1_3_top = { &preamble_8_1_3_top, &test_case_8_1_3_top, &postamble_8_1_3_top };
static struct test_stream test_8_1_3_bot = { &preamble_8_1_3_bot, &test_case_8_1_3_bot, &postamble_8_1_3_bot };

#define tgrp_case_8_1_4 test_group_14
#define numb_case_8_1_4 "8.1.4"
#define name_case_8_1_4 "receiving data with t_rcv -- normal data with intermingled expedited data -- receive buffer smaller that received data"
#define sref_case_8_1_4 "(none)"
#define desc_case_8_1_4 "\
This test case tests positive test cases for the t_rcv library call."

int
test_case_8_1_4_top(int child)
{
	test_bufsize = 12;
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (do_signal(child, __TEST_T_RCV)) {
		case __RESULT_SUCCESS:
			continue;
		case __RESULT_FAILURE:
			if (get_event(child) != __EVENT_TIMEOUT) {
				test_bufsize = 256;
				goto failure;
			}
			break;
		}
		break;
	}
	state++;
	test_bufsize = 256;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_1_4_bot(int child)
{
	int begstate = state;
	int endstate = state + 19;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate + 1; state++) {
		MORE_flag = (state < endstate) ? 1 : 0;
		test_data = "Normal test data.";
		if (do_signal(child, __TEST_DATA_IND) != __RESULT_SUCCESS) {
			MORE_flag = 0;
			goto failure;
		}
		if (state == begstate + 10) {
			test_msleep(child, NORMAL_WAIT);
			MORE_flag = 1;
			if (do_signal(child, __TEST_EXDATA_IND) != __RESULT_SUCCESS) {
				MORE_flag = 0;
				goto failure;
			}
		}
		if (state == begstate + 15) {
			test_msleep(child, NORMAL_WAIT);
			MORE_flag = 0;
			if (do_signal(child, __TEST_EXDATA_IND) != __RESULT_SUCCESS) {
				MORE_flag = 0;
				goto failure;
			}
		}
	}
	MORE_flag = 0;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_1_4_top preamble_3_top
#define preamble_8_1_4_bot preamble_3_bot

#define postamble_8_1_4_top postamble_3_top
#define postamble_8_1_4_bot postamble_3_bot

static struct test_stream test_8_1_4_top = { &preamble_8_1_4_top, &test_case_8_1_4_top, &postamble_8_1_4_top };
static struct test_stream test_8_1_4_bot = { &preamble_8_1_4_bot, &test_case_8_1_4_bot, &postamble_8_1_4_bot };

#define tgrp_case_8_2_1 test_group_14
#define numb_case_8_2_1 "8.2.1"
#define name_case_8_2_1 "receiving data with t_rcvv -- normal data"
#define sref_case_8_2_1 "(none)"
#define desc_case_8_2_1 "\
This test case tests positive test cases for the t_rcvv library call."

int
test_case_8_2_1_top(int child)
{
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (do_signal(child, __TEST_T_RCVV)) {
		case __RESULT_SUCCESS:
			continue;
		case __RESULT_FAILURE:
			if (get_event(child) != __EVENT_TIMEOUT)
				goto failure;
			break;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_2_1_bot(int child)
{
	int endstate = state + 19;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate + 1; state++) {
		MORE_flag = (state < endstate) ? 1 : 0;
		test_data = "Normal test data.";
		if (do_signal(child, __TEST_DATA_IND) != __RESULT_SUCCESS) {
			MORE_flag = 0;
			goto failure;
		}
	}
	MORE_flag = 0;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_2_1_top preamble_3_top
#define preamble_8_2_1_bot preamble_3_bot

#define postamble_8_2_1_top postamble_3_top
#define postamble_8_2_1_bot postamble_3_bot

static struct test_stream test_8_2_1_top = { &preamble_8_2_1_top, &test_case_8_2_1_top, &postamble_8_2_1_top };
static struct test_stream test_8_2_1_bot = { &preamble_8_2_1_bot, &test_case_8_2_1_bot, &postamble_8_2_1_bot };

#define tgrp_case_8_2_2 test_group_14
#define numb_case_8_2_2 "8.2.2"
#define name_case_8_2_2 "receiving data with t_rcvv -- expedited data"
#define sref_case_8_2_2 "(none)"
#define desc_case_8_2_2 "\
This test case tests positive test cases for the t_rcvv library call."

int
test_case_8_2_2_top(int child)
{
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (do_signal(child, __TEST_T_RCVV)) {
		case __RESULT_SUCCESS:
			continue;
		case __RESULT_FAILURE:
			if (get_event(child) != __EVENT_TIMEOUT)
				goto failure;
			break;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_2_2_bot(int child)
{
	int endstate = state + 19;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate + 1; state++) {
		MORE_flag = (state < endstate) ? 1 : 0;
		if (do_signal(child, __TEST_EXDATA_IND) != __RESULT_SUCCESS) {
			MORE_flag = 0;
			goto failure;
		}
	}
	MORE_flag = 0;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_2_2_top preamble_3_top
#define preamble_8_2_2_bot preamble_3_bot

#define postamble_8_2_2_top postamble_3_top
#define postamble_8_2_2_bot postamble_3_bot

static struct test_stream test_8_2_2_top = { &preamble_8_2_2_top, &test_case_8_2_2_top, &postamble_8_2_2_top };
static struct test_stream test_8_2_2_bot = { &preamble_8_2_2_bot, &test_case_8_2_2_bot, &postamble_8_2_2_bot };

#define tgrp_case_8_2_3 test_group_14
#define numb_case_8_2_3 "8.2.3"
#define name_case_8_2_3 "receiving data with t_rcvv -- normal data with intermingled expedited data"
#define sref_case_8_2_3 "(none)"
#define desc_case_8_2_3 "\
This test case tests positive test cases for the t_rcvv library call."

int
test_case_8_2_3_top(int child)
{
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (do_signal(child, __TEST_T_RCVV)) {
		case __RESULT_SUCCESS:
			continue;
		case __RESULT_FAILURE:
			if (get_event(child) != __EVENT_TIMEOUT)
				goto failure;
			break;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_2_3_bot(int child)
{
	int begstate = state;
	int endstate = state + 19;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate + 1; state++) {
		MORE_flag = (state < endstate) ? 1 : 0;
		test_data = "Normal test data.";
		if (do_signal(child, __TEST_DATA_IND) != __RESULT_SUCCESS) {
			MORE_flag = 0;
			goto failure;
		}
		if (state == begstate + 10) {
			test_msleep(child, NORMAL_WAIT);
			MORE_flag = 1;
			if (do_signal(child, __TEST_EXDATA_IND) != __RESULT_SUCCESS) {
				MORE_flag = 0;
				goto failure;
			}
		}
		if (state == begstate + 15) {
			test_msleep(child, NORMAL_WAIT);
			MORE_flag = 0;
			if (do_signal(child, __TEST_EXDATA_IND) != __RESULT_SUCCESS) {
				MORE_flag = 0;
				goto failure;
			}
		}
	}
	MORE_flag = 0;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_2_3_top preamble_3_top
#define preamble_8_2_3_bot preamble_3_bot

#define postamble_8_2_3_top postamble_3_top
#define postamble_8_2_3_bot postamble_3_bot

static struct test_stream test_8_2_3_top = { &preamble_8_2_3_top, &test_case_8_2_3_top, &postamble_8_2_3_top };
static struct test_stream test_8_2_3_bot = { &preamble_8_2_3_bot, &test_case_8_2_3_bot, &postamble_8_2_3_bot };

#define tgrp_case_8_3_1 test_group_14
#define numb_case_8_3_1 "8.3.1"
#define name_case_8_3_1 "receiving data with t_rcvudata -- within TIDU sized data"
#define sref_case_8_3_1 "(none)"
#define desc_case_8_3_1 "\
This test case tests positive test cases for the t_rcvudata library call."

int
test_case_8_3_1_top(int child)
{
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (do_signal(child, __TEST_T_RCVUDATA)) {
		case __RESULT_SUCCESS:
			continue;
		case __RESULT_FAILURE:
			if (get_event(child) != __EVENT_TIMEOUT)
				goto failure;
			break;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_3_1_bot(int child)
{
	int endstate = state + 20;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate; state++) {
		test_data = "Unit test data.";
		if (do_signal(child, __TEST_UNITDATA_IND) != __RESULT_SUCCESS)
			goto failure;
	}
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_3_1_top preamble_2cl_top
#define preamble_8_3_1_bot preamble_2cl_bot

#define postamble_8_3_1_top postamble_2cl_top
#define postamble_8_3_1_bot postamble_2cl_bot

static struct test_stream test_8_3_1_top = { &preamble_8_3_1_top, &test_case_8_3_1_top, &postamble_8_3_1_top };
static struct test_stream test_8_3_1_bot = { &preamble_8_3_1_bot, &test_case_8_3_1_bot, &postamble_8_3_1_bot };

#define tgrp_case_8_3_2 test_group_14
#define numb_case_8_3_2 "8.3.2"
#define name_case_8_3_2 "receiving data with t_rcvudata -- within TIDU sized data -- receive buffer smaller than received data size"
#define sref_case_8_3_2 "(none)"
#define desc_case_8_3_2 "\
This test case tests positive test cases for the t_rcvudata library call."

int
test_case_8_3_2_top(int child)
{
	test_rcvudata.udata.maxlen = 12;
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (do_signal(child, __TEST_T_RCVUDATA)) {
		case __RESULT_SUCCESS:
			continue;
		case __RESULT_FAILURE:
			if (get_event(child) != __EVENT_TIMEOUT) {
				test_rcvudata.udata.maxlen = 256;
				goto failure;
			}
			break;
		}
		break;
	}
	state++;
	test_rcvudata.udata.maxlen = 256;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_3_2_bot(int child)
{
	int endstate = state + 20;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate; state++) {
		test_data = "Unit test data.";
		if (do_signal(child, __TEST_UNITDATA_IND) != __RESULT_SUCCESS)
			goto failure;
	}
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_3_2_top preamble_2cl_top
#define preamble_8_3_2_bot preamble_2cl_bot

#define postamble_8_3_2_top postamble_2cl_top
#define postamble_8_3_2_bot postamble_2cl_bot

static struct test_stream test_8_3_2_top = { &preamble_8_3_2_top, &test_case_8_3_2_top, &postamble_8_3_2_top };
static struct test_stream test_8_3_2_bot = { &preamble_8_3_2_bot, &test_case_8_3_2_bot, &postamble_8_3_2_bot };

#define tgrp_case_8_4_1 test_group_14
#define numb_case_8_4_1 "8.4.1"
#define name_case_8_4_1 "receiving data with t_rcvvudata -- within TIDU sized data"
#define sref_case_8_4_1 "(none)"
#define desc_case_8_4_1 "\
This test case tests positive test cases for the t_rcvvudata library call."

int
test_case_8_4_1_top(int child)
{
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (do_signal(child, __TEST_T_RCVVUDATA)) {
		case __RESULT_SUCCESS:
			continue;
		case __RESULT_FAILURE:
			if (get_event(child) != __EVENT_TIMEOUT)
				goto failure;
			break;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_4_1_bot(int child)
{
	int endstate = state + 20;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate; state++) {
		test_data = "Unit test data.";
		if (do_signal(child, __TEST_UNITDATA_IND) != __RESULT_SUCCESS)
			goto failure;
	}
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_4_1_top preamble_2cl_top
#define preamble_8_4_1_bot preamble_2cl_bot

#define postamble_8_4_1_top postamble_2cl_top
#define postamble_8_4_1_bot postamble_2cl_bot

static struct test_stream test_8_4_1_top = { &preamble_8_4_1_top, &test_case_8_4_1_top, &postamble_8_4_1_top };
static struct test_stream test_8_4_1_bot = { &preamble_8_4_1_bot, &test_case_8_4_1_bot, &postamble_8_4_1_bot };

#define tgrp_case_8_5_1 test_group_14
#define numb_case_8_5_1 "8.5.1"
#define name_case_8_5_1 "sending data with t_snd -- normal data"
#define sref_case_8_5_1 "(none)"
#define desc_case_8_5_1 "\
This test case tests positive test cases for the t_snd library call."

int
test_case_8_5_1_top(int child)
{
	int endstate = state + 19;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate + 1; state++) {
		test_sndflags = (state < endstate) ? T_MORE : 0;
		if (do_signal(child, __TEST_T_SND) != __RESULT_SUCCESS) {
			test_sndflags = 0;
			goto failure;
		}
	}
	state++;
	test_msleep(child, NORMAL_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_5_1_bot(int child)
{
	int endstate = state + 19;

	for (; state < endstate + 1; state++) {
		if (expect(child, INFINITE_WAIT, __TEST_DATA_REQ) != __RESULT_SUCCESS)
			goto failure;
	}
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_5_1_top preamble_3_top
#define preamble_8_5_1_bot preamble_3_bot

#define postamble_8_5_1_top postamble_3_top
#define postamble_8_5_1_bot postamble_3_bot

static struct test_stream test_8_5_1_top = { &preamble_8_5_1_top, &test_case_8_5_1_top, &postamble_8_5_1_top };
static struct test_stream test_8_5_1_bot = { &preamble_8_5_1_bot, &test_case_8_5_1_bot, &postamble_8_5_1_bot };

#define tgrp_case_8_5_2 test_group_14
#define numb_case_8_5_2 "8.5.2"
#define name_case_8_5_2 "sending data with t_snd -- expedited data"
#define sref_case_8_5_2 "(none)"
#define desc_case_8_5_2 "\
This test case tests positive test cases for the t_snd library call."

int
test_case_8_5_2_top(int child)
{
	int endstate = state + 19;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate + 1; state++) {
		test_sndflags = ((state < endstate) ? T_MORE : 0) | T_EXPEDITED;
		if (do_signal(child, __TEST_T_SND) != __RESULT_SUCCESS) {
			test_sndflags = T_EXPEDITED;
			goto failure;
		}
	}
	state++;
	test_msleep(child, NORMAL_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_5_2_bot(int child)
{
	int endstate = state + 19;

	for (; state < endstate + 1; state++) {
		if (expect(child, INFINITE_WAIT, __TEST_EXDATA_REQ) != __RESULT_SUCCESS)
			goto failure;
	}
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_5_2_top preamble_3_top
#define preamble_8_5_2_bot preamble_3_bot

#define postamble_8_5_2_top postamble_3_top
#define postamble_8_5_2_bot postamble_3_bot

static struct test_stream test_8_5_2_top = { &preamble_8_5_2_top, &test_case_8_5_2_top, &postamble_8_5_2_top };
static struct test_stream test_8_5_2_bot = { &preamble_8_5_2_bot, &test_case_8_5_2_bot, &postamble_8_5_2_bot };

#define tgrp_case_8_5_3 test_group_14
#define numb_case_8_5_3 "8.5.3"
#define name_case_8_5_3 "sending data with t_snd -- expedited data intermingled with normal data"
#define sref_case_8_5_3 "(none)"
#define desc_case_8_5_3 "\
This test case tests positive test cases for the t_snd library call."

int
test_case_8_5_3_top(int child)
{
	int begstate = state;
	int endstate = state + 19;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate + 1; state++) {
		test_sndflags = ((state < endstate) ? T_MORE : 0);
		if (do_signal(child, __TEST_T_SND) != __RESULT_SUCCESS) {
			test_sndflags = 0;
			goto failure;
		}
		if (state == begstate + 10) {
			test_msleep(child, NORMAL_WAIT);
			test_sndflags = T_MORE | T_EXPEDITED;
			if (do_signal(child, __TEST_T_SND) != __RESULT_SUCCESS) {
				test_sndflags = 0;
				goto failure;
			}
		}
		if (state == begstate + 15) {
			test_msleep(child, NORMAL_WAIT);
			test_sndflags = T_EXPEDITED;
			if (do_signal(child, __TEST_T_SND) != __RESULT_SUCCESS) {
				test_sndflags = 0;
				goto failure;
			}
		}
	}
	state++;
	test_msleep(child, LONGER_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_5_3_bot(int child)
{
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (get_event(child)) {
		case __TEST_DATA_REQ:
		case __TEST_EXDATA_REQ:
			continue;
		case __EVENT_TIMEOUT:
			break;
		default:
			goto failure;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_5_3_top preamble_3_top
#define preamble_8_5_3_bot preamble_3_bot

#define postamble_8_5_3_top postamble_3_top
#define postamble_8_5_3_bot postamble_3_bot

static struct test_stream test_8_5_3_top = { &preamble_8_5_3_top, &test_case_8_5_3_top, &postamble_8_5_3_top };
static struct test_stream test_8_5_3_bot = { &preamble_8_5_3_bot, &test_case_8_5_3_bot, &postamble_8_5_3_bot };

#define tgrp_case_8_5_4 test_group_14
#define numb_case_8_5_4 "8.5.4"
#define name_case_8_5_4 "sending data with t_snd -- expedited data intermingled with normal data -- data larget than TIDU size"
#define sref_case_8_5_4 "(none)"
#define desc_case_8_5_4 "\
This test case tests positive test cases for the t_snd library call."

int
test_case_8_5_4_top(int child)
{
	int begstate = state;
	int endstate = state + 19;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate + 1; state++) {
		test_sndflags = ((state < endstate) ? T_MORE : 0);
		if (do_signal(child, __TEST_T_SND) != __RESULT_SUCCESS) {
			test_sndflags = 0;
			goto failure;
		}
		if (state == begstate + 10) {
			test_msleep(child, NORMAL_WAIT);
			test_sndflags = T_MORE | T_EXPEDITED;
			if (do_signal(child, __TEST_T_SND) != __RESULT_SUCCESS) {
				test_sndflags = 0;
				goto failure;
			}
		}
		if (state == begstate + 15) {
			test_msleep(child, NORMAL_WAIT);
			test_sndflags = T_EXPEDITED;
			if (do_signal(child, __TEST_T_SND) != __RESULT_SUCCESS) {
				test_sndflags = 0;
				goto failure;
			}
		}
	}
	state++;
	test_msleep(child, LONGER_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_5_4_bot(int child)
{
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (get_event(child)) {
		case __TEST_DATA_REQ:
		case __TEST_EXDATA_REQ:
			continue;
		case __EVENT_TIMEOUT:
			break;
		default:
			goto failure;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_5_4_top preamble_3s_top
#define preamble_8_5_4_bot preamble_3s_bot

#define postamble_8_5_4_top postamble_3s_top
#define postamble_8_5_4_bot postamble_3s_bot

static struct test_stream test_8_5_4_top = { &preamble_8_5_4_top, &test_case_8_5_4_top, &postamble_8_5_4_top };
static struct test_stream test_8_5_4_bot = { &preamble_8_5_4_bot, &test_case_8_5_4_bot, &postamble_8_5_4_bot };

#define tgrp_case_8_6_1 test_group_14
#define numb_case_8_6_1 "8.6.1"
#define name_case_8_6_1 "sending data with t_sndv -- normal data"
#define sref_case_8_6_1 "(none)"
#define desc_case_8_6_1 "\
This test case tests positive test cases for the t_sndv library call."

int
test_case_8_6_1_top(int child)
{
	int endstate = state + 19;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate + 1; state++) {
		test_sndflags = (state < endstate) ? T_MORE : 0;
		if (do_signal(child, __TEST_T_SNDV) != __RESULT_SUCCESS) {
			test_sndflags = 0;
			goto failure;
		}
	}
	state++;
	test_msleep(child, LONGER_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_6_1_bot(int child)
{
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (get_event(child)) {
		case __TEST_DATA_REQ:
		case __TEST_EXDATA_REQ:
			continue;
		case __EVENT_TIMEOUT:
			break;
		default:
			goto failure;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_6_1_top preamble_3_top
#define preamble_8_6_1_bot preamble_3_bot

#define postamble_8_6_1_top postamble_3_top
#define postamble_8_6_1_bot postamble_3_bot

static struct test_stream test_8_6_1_top = { &preamble_8_6_1_top, &test_case_8_6_1_top, &postamble_8_6_1_top };
static struct test_stream test_8_6_1_bot = { &preamble_8_6_1_bot, &test_case_8_6_1_bot, &postamble_8_6_1_bot };

#define tgrp_case_8_6_2 test_group_14
#define numb_case_8_6_2 "8.6.2"
#define name_case_8_6_2 "sending data with t_sndv -- expedited data"
#define sref_case_8_6_2 "(none)"
#define desc_case_8_6_2 "\
This test case tests positive test cases for the t_sndv library call."

int
test_case_8_6_2_top(int child)
{
	int endstate = state + 19;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate + 1; state++) {
		test_sndflags = ((state < endstate) ? T_MORE : 0) | T_EXPEDITED;
		if (do_signal(child, __TEST_T_SNDV) != __RESULT_SUCCESS) {
			test_sndflags = T_EXPEDITED;
			goto failure;
		}
	}
	state++;
	test_msleep(child, LONGER_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_6_2_bot(int child)
{
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (get_event(child)) {
		case __TEST_DATA_REQ:
		case __TEST_EXDATA_REQ:
			continue;
		case __EVENT_TIMEOUT:
			break;
		default:
			goto failure;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_6_2_top preamble_3_top
#define preamble_8_6_2_bot preamble_3_bot

#define postamble_8_6_2_top postamble_3_top
#define postamble_8_6_2_bot postamble_3_bot

static struct test_stream test_8_6_2_top = { &preamble_8_6_2_top, &test_case_8_6_2_top, &postamble_8_6_2_top };
static struct test_stream test_8_6_2_bot = { &preamble_8_6_2_bot, &test_case_8_6_2_bot, &postamble_8_6_2_bot };

#define tgrp_case_8_6_3 test_group_14
#define numb_case_8_6_3 "8.6.3"
#define name_case_8_6_3 "sending data with t_sndv -- expedited data intermingled with normal data"
#define sref_case_8_6_3 "(none)"
#define desc_case_8_6_3 "\
This test case tests positive test cases for the t_sndv library call."

int
test_case_8_6_3_top(int child)
{
	int begstate = state;
	int endstate = state + 19;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate + 1; state++) {
		test_sndflags = ((state < endstate) ? T_MORE : 0);
		if (do_signal(child, __TEST_T_SNDV) != __RESULT_SUCCESS) {
			test_sndflags = 0;
			goto failure;
		}
		if (state == begstate + 10) {
			test_msleep(child, NORMAL_WAIT);
			test_sndflags = T_MORE | T_EXPEDITED;
			if (do_signal(child, __TEST_T_SNDV) != __RESULT_SUCCESS) {
				test_sndflags = 0;
				goto failure;
			}
		}
		if (state == begstate + 15) {
			test_msleep(child, NORMAL_WAIT);
			test_sndflags = T_EXPEDITED;
			if (do_signal(child, __TEST_T_SNDV) != __RESULT_SUCCESS) {
				test_sndflags = 0;
				goto failure;
			}
		}
	}
	state++;
	test_msleep(child, LONGER_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_6_3_bot(int child)
{
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (get_event(child)) {
		case __TEST_DATA_REQ:
		case __TEST_EXDATA_REQ:
			continue;
		case __EVENT_TIMEOUT:
			break;
		default:
			goto failure;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_6_3_top preamble_3_top
#define preamble_8_6_3_bot preamble_3_bot

#define postamble_8_6_3_top postamble_3_top
#define postamble_8_6_3_bot postamble_3_bot

static struct test_stream test_8_6_3_top = { &preamble_8_6_3_top, &test_case_8_6_3_top, &postamble_8_6_3_top };
static struct test_stream test_8_6_3_bot = { &preamble_8_6_3_bot, &test_case_8_6_3_bot, &postamble_8_6_3_bot };

#define tgrp_case_8_7_1 test_group_14
#define numb_case_8_7_1 "8.7.1"
#define name_case_8_7_1 "sending data with t_sndudata -- within TIDU sized data"
#define sref_case_8_7_1 "(none)"
#define desc_case_8_7_1 "\
This test case tests positive test cases for the t_sndudata library call."

int
test_case_8_7_1_top(int child)
{
	int endstate = state + 20;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate; state++) {
		if (do_signal(child, __TEST_T_SNDUDATA) != __RESULT_SUCCESS)
			goto failure;
	}
	state++;
	test_msleep(child, LONGER_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_7_1_bot(int child)
{
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (get_event(child)) {
		case __TEST_UNITDATA_REQ:
			continue;
		case __EVENT_TIMEOUT:
			break;
		default:
			goto failure;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_7_1_top preamble_2cl_top
#define preamble_8_7_1_bot preamble_2cl_bot

#define postamble_8_7_1_top postamble_2cl_top
#define postamble_8_7_1_bot postamble_2cl_bot

static struct test_stream test_8_7_1_top = { &preamble_8_7_1_top, &test_case_8_7_1_top, &postamble_8_7_1_top };
static struct test_stream test_8_7_1_bot = { &preamble_8_7_1_bot, &test_case_8_7_1_bot, &postamble_8_7_1_bot };

#define tgrp_case_8_8_1 test_group_14
#define numb_case_8_8_1 "8.8.1"
#define name_case_8_8_1 "sending data with t_sndvudata -- within TIDU sized data"
#define sref_case_8_8_1 "(none)"
#define desc_case_8_8_1 "\
This test case tests positive test cases for the t_sndvudata library call."

int
test_case_8_8_1_top(int child)
{
	int endstate = state + 20;

	test_msleep(child, NORMAL_WAIT);
	for (; state < endstate; state++) {
		if (do_signal(child, __TEST_T_SNDVUDATA) != __RESULT_SUCCESS)
			goto failure;
	}
	state++;
	test_msleep(child, LONGER_WAIT);
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

int
test_case_8_8_1_bot(int child)
{
	for (start_tt(LONG_WAIT);; start_tt(LONG_WAIT), state++) {
		switch (get_event(child)) {
		case __TEST_UNITDATA_REQ:
			continue;
		case __EVENT_TIMEOUT:
			break;
		default:
			goto failure;
		}
		break;
	}
	state++;
	if (stop_tt() != __RESULT_SUCCESS)
		goto failure;
	state++;
	return (__RESULT_SUCCESS);
      failure:
	return (__RESULT_FAILURE);
}

#define preamble_8_8_1_top preamble_2cl_top
#define preamble_8_8_1_bot preamble_2cl_bot

#define postamble_8_8_1_top postamble_2cl_top
#define postamble_8_8_1_bot postamble_2cl_bot

static struct test_stream test_8_8_1_top = { &preamble_8_8_1_top, &test_case_8_8_1_top, &postamble_8_8_1_top };
static struct test_stream test_8_8_1_bot = { &preamble_8_8_1_bot, &test_case_8_8_1_bot, &postamble_8_8_1_bot };

/*
 *  -------------------------------------------------------------------------
 *
 *  Test case child scheduler
 *
 *  -------------------------------------------------------------------------
 */
int
run_stream(int child, struct test_stream *stream)
{
	int result = __RESULT_SCRIPT_ERROR;
	int pre_result = __RESULT_SCRIPT_ERROR;
	int post_result = __RESULT_SCRIPT_ERROR;

	print_preamble(child);
	state = 100;
	if (stream->preamble && (pre_result = stream->preamble(child)) != __RESULT_SUCCESS) {
		switch (pre_result) {
		case __RESULT_NOTAPPL:
			print_notapplicable(child);
			result = __RESULT_NOTAPPL;
			break;
		case __RESULT_SKIPPED:
			print_skipped(child);
			result = __RESULT_SKIPPED;
			break;
		default:
			print_inconclusive(child);
			result = __RESULT_INCONCLUSIVE;
			break;
		}
	} else {
		print_test(child);
		state = 200;
		switch (stream->testcase(child)) {
		default:
		case __RESULT_INCONCLUSIVE:
			print_inconclusive(child);
			result = __RESULT_INCONCLUSIVE;
			break;
		case __RESULT_NOTAPPL:
			print_notapplicable(child);
			result = __RESULT_NOTAPPL;
			break;
		case __RESULT_SKIPPED:
			print_skipped(child);
			result = __RESULT_SKIPPED;
			break;
		case __RESULT_FAILURE:
			print_failed(child);
			result = __RESULT_FAILURE;
			break;
		case __RESULT_SCRIPT_ERROR:
			print_script_error(child);
			result = __RESULT_SCRIPT_ERROR;
			break;
		case __RESULT_SUCCESS:
			print_passed(child);
			result = __RESULT_SUCCESS;
			break;
		}
		print_postamble(child);
		state = 300;
		if (stream->postamble && (post_result = stream->postamble(child)) != __RESULT_SUCCESS) {
			switch (post_result) {
			case __RESULT_NOTAPPL:
				print_notapplicable(child);
				result = __RESULT_NOTAPPL;
				break;
			case __RESULT_SKIPPED:
				print_skipped(child);
				result = __RESULT_SKIPPED;
				break;
			default:
				print_inconclusive(child);
				if (result == __RESULT_SUCCESS)
					result = __RESULT_INCONCLUSIVE;
				break;
			}
		}
	}
	print_test_end(child);
	exit(result);
}

/*
 *  Fork multiple children to do the actual testing.
 *  The top child (child[0]) is the process running above the tested module, the
 *  bot child (child[1]) is the process running below (at other end of pipe) the
 *  tested module.
 */

int
test_run(struct test_stream *stream[])
{
	int children = 0;
	pid_t this_child, child[3] = { 0, };
	int this_status, status[3] = { 0, };

	if (start_tt(TEST_DURATION) != __RESULT_SUCCESS)
		goto inconclusive;
	if (stream[2]) {
		switch ((child[2] = fork())) {
		case 00:	/* we are the child */
			exit(run_stream(2, stream[2]));	/* execute stream[2] state machine */
		case -1:	/* error */
			if (child[0])
				kill(child[0], SIGKILL);	/* toast stream[0] child */
			if (child[1])
				kill(child[1], SIGKILL);	/* toast stream[1] child */
			return __RESULT_FAILURE;
		default:	/* we are the parent */
			children++;
			break;
		}
	} else
		status[2] = __RESULT_SUCCESS;
	if (stream[1]) {
		switch ((child[1] = fork())) {
		case 00:	/* we are the child */
			exit(run_stream(1, stream[1]));	/* execute stream[1] state machine */
		case -1:	/* error */
			if (child[0])
				kill(child[0], SIGKILL);	/* toast stream[0] child */
			return __RESULT_FAILURE;
		default:	/* we are the parent */
			children++;
			break;
		}
	} else
		status[1] = __RESULT_SUCCESS;
	if (stream[0]) {
		switch ((child[0] = fork())) {
		case 00:	/* we are the child */
			exit(run_stream(0, stream[0]));	/* execute stream[0] state machine */
		case -1:	/* error */
			return __RESULT_FAILURE;
		default:	/* we are the parent */
			children++;
			break;
		}
	} else
		status[0] = __RESULT_SUCCESS;
	for (; children > 0; children--) {
	      waitagain:
		if ((this_child = wait(&this_status)) > 0) {
			if (WIFEXITED(this_status)) {
				if (this_child == child[0]) {
					child[0] = 0;
					if ((status[0] = WEXITSTATUS(this_status)) != __RESULT_SUCCESS) {
						if (child[1])
							kill(child[1], SIGKILL);
						if (child[2])
							kill(child[2], SIGKILL);
					}
				}
				if (this_child == child[1]) {
					child[1] = 0;
					if ((status[1] = WEXITSTATUS(this_status)) != __RESULT_SUCCESS) {
						if (child[0])
							kill(child[0], SIGKILL);
						if (child[2])
							kill(child[2], SIGKILL);
					}
				}
				if (this_child == child[2]) {
					child[2] = 0;
					if ((status[2] = WEXITSTATUS(this_status)) != __RESULT_SUCCESS) {
						if (child[0])
							kill(child[0], SIGKILL);
						if (child[1])
							kill(child[1], SIGKILL);
					}
				}
			} else if (WIFSIGNALED(this_status)) {
				int signal = WTERMSIG(this_status);

				if (this_child == child[0]) {
					print_terminated(0, signal);
					if (child[1])
						kill(child[1], SIGKILL);
					if (child[2])
						kill(child[2], SIGKILL);
					status[0] = (signal == SIGKILL) ? __RESULT_INCONCLUSIVE : __RESULT_FAILURE;
					child[0] = 0;
				}
				if (this_child == child[1]) {
					print_terminated(1, signal);
					if (child[0])
						kill(child[0], SIGKILL);
					if (child[2])
						kill(child[2], SIGKILL);
					status[1] = (signal == SIGKILL) ? __RESULT_INCONCLUSIVE : __RESULT_FAILURE;
					child[1] = 0;
				}
				if (this_child == child[2]) {
					print_terminated(2, signal);
					if (child[0])
						kill(child[0], SIGKILL);
					if (child[1])
						kill(child[1], SIGKILL);
					status[2] = (signal == SIGKILL) ? __RESULT_INCONCLUSIVE : __RESULT_FAILURE;
					child[2] = 0;
				}
			} else if (WIFSTOPPED(this_status)) {
				int signal = WSTOPSIG(this_status);

				if (this_child == child[0]) {
					print_stopped(0, signal);
					if (child[0])
						kill(child[0], SIGKILL);
					if (child[1])
						kill(child[1], SIGKILL);
					if (child[2])
						kill(child[2], SIGKILL);
					status[0] = __RESULT_FAILURE;
					child[0] = 0;
				}
				if (this_child == child[1]) {
					print_stopped(1, signal);
					if (child[0])
						kill(child[0], SIGKILL);
					if (child[1])
						kill(child[1], SIGKILL);
					if (child[2])
						kill(child[2], SIGKILL);
					status[1] = __RESULT_FAILURE;
					child[1] = 0;
				}
				if (this_child == child[2]) {
					print_stopped(2, signal);
					if (child[0])
						kill(child[0], SIGKILL);
					if (child[1])
						kill(child[1], SIGKILL);
					if (child[2])
						kill(child[2], SIGKILL);
					status[2] = __RESULT_FAILURE;
					child[2] = 0;
				}
			}
		} else {
			if (timer_timeout) {
				timer_timeout = 0;
				print_timeout(3);
			}
			if (child[0])
				kill(child[0], SIGKILL);
			if (child[1])
				kill(child[1], SIGKILL);
			if (child[2])
				kill(child[2], SIGKILL);
			goto waitagain;
		}
	}
	if (stop_tt() != __RESULT_SUCCESS)
		goto inconclusive;
	if (status[0] == __RESULT_NOTAPPL || status[1] == __RESULT_NOTAPPL || status[2] == __RESULT_NOTAPPL)
		return (__RESULT_NOTAPPL);
	if (status[0] == __RESULT_SKIPPED || status[1] == __RESULT_SKIPPED || status[2] == __RESULT_SKIPPED)
		return (__RESULT_SKIPPED);
	if (status[0] == __RESULT_FAILURE || status[1] == __RESULT_FAILURE || status[2] == __RESULT_FAILURE)
		return (__RESULT_FAILURE);
	if (status[0] == __RESULT_SUCCESS && status[1] == __RESULT_SUCCESS && status[2] == __RESULT_SUCCESS)
		return (__RESULT_SUCCESS);
      inconclusive:
	return (__RESULT_INCONCLUSIVE);
}

/*
 *  -------------------------------------------------------------------------
 *
 *  Test case lists
 *
 *  -------------------------------------------------------------------------
 */

struct test_case {
	const char *numb;		/* test case number */
	const char *tgrp;		/* test case group */
	const char *name;		/* test case name */
	const char *desc;		/* test case description */
	const char *sref;		/* test case standards section reference */
	struct test_stream *stream[3];	/* test streams */
	int (*start) (int);		/* start function */
	int (*stop) (int);		/* stop function */
	int run;			/* whether to run this test */
	int result;			/* results of test */
} tests[] = {
	{
		numb_case_0_1, tgrp_case_0_1, name_case_0_1, desc_case_0_1, sref_case_0_1, {
	&test_0_1_conn, &test_0_1_resp, &test_0_1_list}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_1_1, tgrp_case_1_1, name_case_1_1, desc_case_1_1, sref_case_1_1, {
	&test_1_1_top, &test_1_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_1_2_1, tgrp_case_1_2_1, name_case_1_2_1, desc_case_1_2_1, sref_case_1_2_1, {
	&test_1_2_1_top, &test_1_2_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_1_2_2, tgrp_case_1_2_2, name_case_1_2_2, desc_case_1_2_2, sref_case_1_2_2, {
	&test_1_2_2_top, &test_1_2_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_1_2_3, tgrp_case_1_2_3, name_case_1_2_3, desc_case_1_2_3, sref_case_1_2_3, {
	&test_1_2_3_top, &test_1_2_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_1_1, tgrp_case_2_1_1, name_case_2_1_1, desc_case_2_1_1, sref_case_2_1_1, {
	&test_2_1_1_top, &test_2_1_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_1_2, tgrp_case_2_1_2, name_case_2_1_2, desc_case_2_1_2, sref_case_2_1_2, {
	&test_2_1_2_top, &test_2_1_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_1_3, tgrp_case_2_1_3, name_case_2_1_3, desc_case_2_1_3, sref_case_2_1_3, {
	&test_2_1_3_top, &test_2_1_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_1_4, tgrp_case_2_1_4, name_case_2_1_4, desc_case_2_1_4, sref_case_2_1_4, {
	&test_2_1_4_top, &test_2_1_4_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_1_5, tgrp_case_2_1_5, name_case_2_1_5, desc_case_2_1_5, sref_case_2_1_5, {
	&test_2_1_5_top, &test_2_1_5_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_1_6, tgrp_case_2_1_6, name_case_2_1_6, desc_case_2_1_6, sref_case_2_1_6, {
	&test_2_1_6_top, &test_2_1_6_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_1_7, tgrp_case_2_1_7, name_case_2_1_7, desc_case_2_1_7, sref_case_2_1_7, {
	&test_2_1_7_top, &test_2_1_7_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_1_8, tgrp_case_2_1_8, name_case_2_1_8, desc_case_2_1_8, sref_case_2_1_8, {
	&test_2_1_8_top, &test_2_1_8_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_2, tgrp_case_2_2, name_case_2_2, desc_case_2_2, sref_case_2_2, {
	&test_2_2_top, &test_2_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_3_1, tgrp_case_2_3_1, name_case_2_3_1, desc_case_2_3_1, sref_case_2_3_1, {
	&test_2_3_1_top, &test_2_3_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_3_2, tgrp_case_2_3_2, name_case_2_3_2, desc_case_2_3_2, sref_case_2_3_2, {
	&test_2_3_2_top, &test_2_3_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_3_3, tgrp_case_2_3_3, name_case_2_3_3, desc_case_2_3_3, sref_case_2_3_3, {
	&test_2_3_3_top, &test_2_3_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_3_4, tgrp_case_2_3_4, name_case_2_3_4, desc_case_2_3_4, sref_case_2_3_4, {
	&test_2_3_4_top, &test_2_3_4_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_3_5, tgrp_case_2_3_5, name_case_2_3_5, desc_case_2_3_5, sref_case_2_3_5, {
	&test_2_3_5_top, &test_2_3_5_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_3_6, tgrp_case_2_3_6, name_case_2_3_6, desc_case_2_3_6, sref_case_2_3_6, {
	&test_2_3_6_top, &test_2_3_6_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_3_7, tgrp_case_2_3_7, name_case_2_3_7, desc_case_2_3_7, sref_case_2_3_7, {
	&test_2_3_7_top, &test_2_3_7_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_3_8, tgrp_case_2_3_8, name_case_2_3_8, desc_case_2_3_8, sref_case_2_3_8, {
	&test_2_3_8_top, &test_2_3_8_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_3_9, tgrp_case_2_3_9, name_case_2_3_9, desc_case_2_3_9, sref_case_2_3_9, {
	&test_2_3_9_top, &test_2_3_9_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_4_1, tgrp_case_2_4_1, name_case_2_4_1, desc_case_2_4_1, sref_case_2_4_1, {
	&test_2_4_1_top, &test_2_4_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_2_5_1, tgrp_case_2_5_1, name_case_2_5_1, desc_case_2_5_1, sref_case_2_5_1, {
	&test_2_5_1_top, &test_2_5_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_3_1_1, tgrp_case_3_1_1, name_case_3_1_1, desc_case_3_1_1, sref_case_3_1_1, {
	&test_3_1_1_top, &test_3_1_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_3_2_1, tgrp_case_3_2_1, name_case_3_2_1, desc_case_3_2_1, sref_case_3_2_1, {
	&test_3_2_1_top, &test_3_2_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_3_3_1, tgrp_case_3_3_1, name_case_3_3_1, desc_case_3_3_1, sref_case_3_3_1, {
	&test_3_3_1_top, &test_3_3_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_3_4_1, tgrp_case_3_4_1, name_case_3_4_1, desc_case_3_4_1, sref_case_3_4_1, {
	&test_3_4_1_top, &test_3_4_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_3_4_2, tgrp_case_3_4_2, name_case_3_4_2, desc_case_3_4_2, sref_case_3_4_2, {
	&test_3_4_2_top, &test_3_4_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_3_5_1, tgrp_case_3_5_1, name_case_3_5_1, desc_case_3_5_1, sref_case_3_5_1, {
	&test_3_5_1_top, &test_3_5_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_3_5_2, tgrp_case_3_5_2, name_case_3_5_2, desc_case_3_5_2, sref_case_3_5_2, {
	&test_3_5_2_top, &test_3_5_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_3_6_1, tgrp_case_3_6_1, name_case_3_6_1, desc_case_3_6_1, sref_case_3_6_1, {
	&test_3_6_1_top, &test_3_6_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_3_7_1, tgrp_case_3_7_1, name_case_3_7_1, desc_case_3_7_1, sref_case_3_7_1, {
	&test_3_7_1_top, &test_3_7_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_1_1, tgrp_case_4_1_1, name_case_4_1_1, desc_case_4_1_1, sref_case_4_1_1, {
	&test_4_1_1_top, &test_4_1_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_1_2, tgrp_case_4_1_2, name_case_4_1_2, desc_case_4_1_2, sref_case_4_1_2, {
	&test_4_1_2_top, &test_4_1_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_2_1, tgrp_case_4_2_1, name_case_4_2_1, desc_case_4_2_1, sref_case_4_2_1, {
	&test_4_2_1_top, &test_4_2_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_2_2, tgrp_case_4_2_2, name_case_4_2_2, desc_case_4_2_2, sref_case_4_2_2, {
	&test_4_2_2_top, &test_4_2_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_3_1, tgrp_case_4_3_1, name_case_4_3_1, desc_case_4_3_1, sref_case_4_3_1, {
	&test_4_3_1_top, &test_4_3_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_4_1, tgrp_case_4_4_1, name_case_4_4_1, desc_case_4_4_1, sref_case_4_4_1, {
	&test_4_4_1_top, &test_4_4_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_4_2, tgrp_case_4_4_2, name_case_4_4_2, desc_case_4_4_2, sref_case_4_4_2, {
	&test_4_4_2_top, &test_4_4_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_4_3, tgrp_case_4_4_3, name_case_4_4_3, desc_case_4_4_3, sref_case_4_4_3, {
	&test_4_4_3_top, &test_4_4_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_4_4, tgrp_case_4_4_4, name_case_4_4_4, desc_case_4_4_4, sref_case_4_4_4, {
	&test_4_4_4_top, &test_4_4_4_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_4_5, tgrp_case_4_4_5, name_case_4_4_5, desc_case_4_4_5, sref_case_4_4_5, {
	&test_4_4_5_top, &test_4_4_5_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_4_6, tgrp_case_4_4_6, name_case_4_4_6, desc_case_4_4_6, sref_case_4_4_6, {
	&test_4_4_6_top, &test_4_4_6_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_4_7, tgrp_case_4_4_7, name_case_4_4_7, desc_case_4_4_7, sref_case_4_4_7, {
	&test_4_4_7_top, &test_4_4_7_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_4_8, tgrp_case_4_4_8, name_case_4_4_8, desc_case_4_4_8, sref_case_4_4_8, {
	&test_4_4_8_top, &test_4_4_8_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_5_1, tgrp_case_4_5_1, name_case_4_5_1, desc_case_4_5_1, sref_case_4_5_1, {
	&test_4_5_1_top, &test_4_5_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_6_1, tgrp_case_4_6_1, name_case_4_6_1, desc_case_4_6_1, sref_case_4_6_1, {
	&test_4_6_1_top, &test_4_6_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_6_2, tgrp_case_4_6_2, name_case_4_6_2, desc_case_4_6_2, sref_case_4_6_2, {
	&test_4_6_2_top, &test_4_6_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_6_3, tgrp_case_4_6_3, name_case_4_6_3, desc_case_4_6_3, sref_case_4_6_3, {
	&test_4_6_3_top, &test_4_6_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_6_4, tgrp_case_4_6_4, name_case_4_6_4, desc_case_4_6_4, sref_case_4_6_4, {
	&test_4_6_4_top, &test_4_6_4_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_7_1, tgrp_case_4_7_1, name_case_4_7_1, desc_case_4_7_1, sref_case_4_7_1, {
	&test_4_7_1_top, &test_4_7_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_7_2, tgrp_case_4_7_2, name_case_4_7_2, desc_case_4_7_2, sref_case_4_7_2, {
	&test_4_7_2_top, &test_4_7_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_7_3, tgrp_case_4_7_3, name_case_4_7_3, desc_case_4_7_3, sref_case_4_7_3, {
	&test_4_7_3_top, &test_4_7_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_7_4, tgrp_case_4_7_4, name_case_4_7_4, desc_case_4_7_4, sref_case_4_7_4, {
	&test_4_7_4_top, &test_4_7_4_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_8_1, tgrp_case_4_8_1, name_case_4_8_1, desc_case_4_8_1, sref_case_4_8_1, {
	&test_4_8_1_top, &test_4_8_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_8_2, tgrp_case_4_8_2, name_case_4_8_2, desc_case_4_8_2, sref_case_4_8_2, {
	&test_4_8_2_top, &test_4_8_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_8_3, tgrp_case_4_8_3, name_case_4_8_3, desc_case_4_8_3, sref_case_4_8_3, {
	&test_4_8_3_top, &test_4_8_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_4_9_1, tgrp_case_4_9_1, name_case_4_9_1, desc_case_4_9_1, sref_case_4_9_1, {
	&test_4_9_1_top, &test_4_9_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_1, tgrp_case_5_1_1, name_case_5_1_1, desc_case_5_1_1, sref_case_5_1_1, {
	&test_5_1_1_top, &test_5_1_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_2, tgrp_case_5_1_2, name_case_5_1_2, desc_case_5_1_2, sref_case_5_1_2, {
	&test_5_1_2_top, &test_5_1_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_3, tgrp_case_5_1_3, name_case_5_1_3, desc_case_5_1_3, sref_case_5_1_3, {
	&test_5_1_3_top, &test_5_1_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_4, tgrp_case_5_1_4, name_case_5_1_4, desc_case_5_1_4, sref_case_5_1_4, {
	&test_5_1_4_top, &test_5_1_4_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_5, tgrp_case_5_1_5, name_case_5_1_5, desc_case_5_1_5, sref_case_5_1_5, {
	&test_5_1_5_top, &test_5_1_5_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_6, tgrp_case_5_1_6, name_case_5_1_6, desc_case_5_1_6, sref_case_5_1_6, {
	&test_5_1_6_top, &test_5_1_6_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_7, tgrp_case_5_1_7, name_case_5_1_7, desc_case_5_1_7, sref_case_5_1_7, {
	&test_5_1_7_top, &test_5_1_7_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_8, tgrp_case_5_1_8, name_case_5_1_8, desc_case_5_1_8, sref_case_5_1_8, {
	&test_5_1_8_top, &test_5_1_8_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_9, tgrp_case_5_1_9, name_case_5_1_9, desc_case_5_1_9, sref_case_5_1_9, {
	&test_5_1_9_top, &test_5_1_9_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_10, tgrp_case_5_1_10, name_case_5_1_10, desc_case_5_1_10, sref_case_5_1_10, {
	&test_5_1_10_top, &test_5_1_10_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_11, tgrp_case_5_1_11, name_case_5_1_11, desc_case_5_1_11, sref_case_5_1_11, {
	&test_5_1_11_top, &test_5_1_11_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_12, tgrp_case_5_1_12, name_case_5_1_12, desc_case_5_1_12, sref_case_5_1_12, {
	&test_5_1_12_top, &test_5_1_12_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_13, tgrp_case_5_1_13, name_case_5_1_13, desc_case_5_1_13, sref_case_5_1_13, {
	&test_5_1_13_top, &test_5_1_13_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_14, tgrp_case_5_1_14, name_case_5_1_14, desc_case_5_1_14, sref_case_5_1_14, {
	&test_5_1_14_top, &test_5_1_14_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_15, tgrp_case_5_1_15, name_case_5_1_15, desc_case_5_1_15, sref_case_5_1_15, {
	&test_5_1_15_top, &test_5_1_15_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_16, tgrp_case_5_1_16, name_case_5_1_16, desc_case_5_1_16, sref_case_5_1_16, {
	&test_5_1_16_top, &test_5_1_16_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_17, tgrp_case_5_1_17, name_case_5_1_17, desc_case_5_1_17, sref_case_5_1_17, {
	&test_5_1_17_top, &test_5_1_17_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_18, tgrp_case_5_1_18, name_case_5_1_18, desc_case_5_1_18, sref_case_5_1_18, {
	&test_5_1_18_top, &test_5_1_18_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_19, tgrp_case_5_1_19, name_case_5_1_19, desc_case_5_1_19, sref_case_5_1_19, {
	&test_5_1_19_top, &test_5_1_19_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_20, tgrp_case_5_1_20, name_case_5_1_20, desc_case_5_1_20, sref_case_5_1_20, {
	&test_5_1_20_top, &test_5_1_20_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_21, tgrp_case_5_1_21, name_case_5_1_21, desc_case_5_1_21, sref_case_5_1_21, {
	&test_5_1_21_top, &test_5_1_21_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_22, tgrp_case_5_1_22, name_case_5_1_22, desc_case_5_1_22, sref_case_5_1_22, {
	&test_5_1_22_top, &test_5_1_22_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_23, tgrp_case_5_1_23, name_case_5_1_23, desc_case_5_1_23, sref_case_5_1_23, {
	&test_5_1_23_top, &test_5_1_23_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_24, tgrp_case_5_1_24, name_case_5_1_24, desc_case_5_1_24, sref_case_5_1_24, {
	&test_5_1_24_top, &test_5_1_24_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_25, tgrp_case_5_1_25, name_case_5_1_25, desc_case_5_1_25, sref_case_5_1_25, {
	&test_5_1_25_top, &test_5_1_25_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_26, tgrp_case_5_1_26, name_case_5_1_26, desc_case_5_1_26, sref_case_5_1_26, {
	&test_5_1_26_top, &test_5_1_26_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_27, tgrp_case_5_1_27, name_case_5_1_27, desc_case_5_1_27, sref_case_5_1_27, {
	&test_5_1_27_top, &test_5_1_27_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_1_28, tgrp_case_5_1_28, name_case_5_1_28, desc_case_5_1_28, sref_case_5_1_28, {
	&test_5_1_28_top, &test_5_1_28_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_1, tgrp_case_5_2_1, name_case_5_2_1, desc_case_5_2_1, sref_case_5_2_1, {
	&test_5_2_1_top, &test_5_2_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_2, tgrp_case_5_2_2, name_case_5_2_2, desc_case_5_2_2, sref_case_5_2_2, {
	&test_5_2_2_top, &test_5_2_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_3, tgrp_case_5_2_3, name_case_5_2_3, desc_case_5_2_3, sref_case_5_2_3, {
	&test_5_2_3_top, &test_5_2_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_4, tgrp_case_5_2_4, name_case_5_2_4, desc_case_5_2_4, sref_case_5_2_4, {
	&test_5_2_4_top, &test_5_2_4_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_5, tgrp_case_5_2_5, name_case_5_2_5, desc_case_5_2_5, sref_case_5_2_5, {
	&test_5_2_5_top, &test_5_2_5_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_6, tgrp_case_5_2_6, name_case_5_2_6, desc_case_5_2_6, sref_case_5_2_6, {
	&test_5_2_6_top, &test_5_2_6_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_7, tgrp_case_5_2_7, name_case_5_2_7, desc_case_5_2_7, sref_case_5_2_7, {
	&test_5_2_7_top, &test_5_2_7_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_8, tgrp_case_5_2_8, name_case_5_2_8, desc_case_5_2_8, sref_case_5_2_8, {
	&test_5_2_8_top, &test_5_2_8_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_9, tgrp_case_5_2_9, name_case_5_2_9, desc_case_5_2_9, sref_case_5_2_9, {
	&test_5_2_9_top, &test_5_2_9_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_10, tgrp_case_5_2_10, name_case_5_2_10, desc_case_5_2_10, sref_case_5_2_10, {
	&test_5_2_10_top, &test_5_2_10_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_11, tgrp_case_5_2_11, name_case_5_2_11, desc_case_5_2_11, sref_case_5_2_11, {
	&test_5_2_11_top, &test_5_2_11_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_12, tgrp_case_5_2_12, name_case_5_2_12, desc_case_5_2_12, sref_case_5_2_12, {
	&test_5_2_12_top, &test_5_2_12_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_13, tgrp_case_5_2_13, name_case_5_2_13, desc_case_5_2_13, sref_case_5_2_13, {
	&test_5_2_13_top, &test_5_2_13_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_14, tgrp_case_5_2_14, name_case_5_2_14, desc_case_5_2_14, sref_case_5_2_14, {
	&test_5_2_14_top, &test_5_2_14_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_15, tgrp_case_5_2_15, name_case_5_2_15, desc_case_5_2_15, sref_case_5_2_15, {
	&test_5_2_15_top, &test_5_2_15_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_16, tgrp_case_5_2_16, name_case_5_2_16, desc_case_5_2_16, sref_case_5_2_16, {
	&test_5_2_16_top, &test_5_2_16_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_17, tgrp_case_5_2_17, name_case_5_2_17, desc_case_5_2_17, sref_case_5_2_17, {
	&test_5_2_17_top, &test_5_2_17_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_18, tgrp_case_5_2_18, name_case_5_2_18, desc_case_5_2_18, sref_case_5_2_18, {
	&test_5_2_18_top, &test_5_2_18_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_19, tgrp_case_5_2_19, name_case_5_2_19, desc_case_5_2_19, sref_case_5_2_19, {
	&test_5_2_19_top, &test_5_2_19_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_20, tgrp_case_5_2_20, name_case_5_2_20, desc_case_5_2_20, sref_case_5_2_20, {
	&test_5_2_20_top, &test_5_2_20_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_21, tgrp_case_5_2_21, name_case_5_2_21, desc_case_5_2_21, sref_case_5_2_21, {
	&test_5_2_21_top, &test_5_2_21_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_22, tgrp_case_5_2_22, name_case_5_2_22, desc_case_5_2_22, sref_case_5_2_22, {
	&test_5_2_22_top, &test_5_2_22_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_23, tgrp_case_5_2_23, name_case_5_2_23, desc_case_5_2_23, sref_case_5_2_23, {
	&test_5_2_23_top, &test_5_2_23_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_24, tgrp_case_5_2_24, name_case_5_2_24, desc_case_5_2_24, sref_case_5_2_24, {
	&test_5_2_24_top, &test_5_2_24_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_25, tgrp_case_5_2_25, name_case_5_2_25, desc_case_5_2_25, sref_case_5_2_25, {
	&test_5_2_25_top, &test_5_2_25_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_26, tgrp_case_5_2_26, name_case_5_2_26, desc_case_5_2_26, sref_case_5_2_26, {
	&test_5_2_26_top, &test_5_2_26_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_27, tgrp_case_5_2_27, name_case_5_2_27, desc_case_5_2_27, sref_case_5_2_27, {
	&test_5_2_27_top, &test_5_2_27_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_2_28, tgrp_case_5_2_28, name_case_5_2_28, desc_case_5_2_28, sref_case_5_2_28, {
	&test_5_2_28_top, &test_5_2_28_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_1, tgrp_case_5_3_1, name_case_5_3_1, desc_case_5_3_1, sref_case_5_3_1, {
	&test_5_3_1_top, &test_5_3_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_2, tgrp_case_5_3_2, name_case_5_3_2, desc_case_5_3_2, sref_case_5_3_2, {
	&test_5_3_2_top, &test_5_3_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_3, tgrp_case_5_3_3, name_case_5_3_3, desc_case_5_3_3, sref_case_5_3_3, {
	&test_5_3_3_top, &test_5_3_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_4, tgrp_case_5_3_4, name_case_5_3_4, desc_case_5_3_4, sref_case_5_3_4, {
	&test_5_3_4_top, &test_5_3_4_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_5, tgrp_case_5_3_5, name_case_5_3_5, desc_case_5_3_5, sref_case_5_3_5, {
	&test_5_3_5_top, &test_5_3_5_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_6, tgrp_case_5_3_6, name_case_5_3_6, desc_case_5_3_6, sref_case_5_3_6, {
	&test_5_3_6_top, &test_5_3_6_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_7, tgrp_case_5_3_7, name_case_5_3_7, desc_case_5_3_7, sref_case_5_3_7, {
	&test_5_3_7_top, &test_5_3_7_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_8, tgrp_case_5_3_8, name_case_5_3_8, desc_case_5_3_8, sref_case_5_3_8, {
	&test_5_3_8_top, &test_5_3_8_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_9, tgrp_case_5_3_9, name_case_5_3_9, desc_case_5_3_9, sref_case_5_3_9, {
	&test_5_3_9_top, &test_5_3_9_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_10, tgrp_case_5_3_10, name_case_5_3_10, desc_case_5_3_10, sref_case_5_3_10, {
	&test_5_3_10_top, &test_5_3_10_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_11, tgrp_case_5_3_11, name_case_5_3_11, desc_case_5_3_11, sref_case_5_3_11, {
	&test_5_3_11_top, &test_5_3_11_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_12, tgrp_case_5_3_12, name_case_5_3_12, desc_case_5_3_12, sref_case_5_3_12, {
	&test_5_3_12_top, &test_5_3_12_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_13, tgrp_case_5_3_13, name_case_5_3_13, desc_case_5_3_13, sref_case_5_3_13, {
	&test_5_3_13_top, &test_5_3_13_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_14, tgrp_case_5_3_14, name_case_5_3_14, desc_case_5_3_14, sref_case_5_3_14, {
	&test_5_3_14_top, &test_5_3_14_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_15, tgrp_case_5_3_15, name_case_5_3_15, desc_case_5_3_15, sref_case_5_3_15, {
	&test_5_3_15_top, &test_5_3_15_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_16, tgrp_case_5_3_16, name_case_5_3_16, desc_case_5_3_16, sref_case_5_3_16, {
	&test_5_3_16_top, &test_5_3_16_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_17, tgrp_case_5_3_17, name_case_5_3_17, desc_case_5_3_17, sref_case_5_3_17, {
	&test_5_3_17_top, &test_5_3_17_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_18, tgrp_case_5_3_18, name_case_5_3_18, desc_case_5_3_18, sref_case_5_3_18, {
	&test_5_3_18_top, &test_5_3_18_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_19, tgrp_case_5_3_19, name_case_5_3_19, desc_case_5_3_19, sref_case_5_3_19, {
	&test_5_3_19_top, &test_5_3_19_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_20, tgrp_case_5_3_20, name_case_5_3_20, desc_case_5_3_20, sref_case_5_3_20, {
	&test_5_3_20_top, &test_5_3_20_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_21, tgrp_case_5_3_21, name_case_5_3_21, desc_case_5_3_21, sref_case_5_3_21, {
	&test_5_3_21_top, &test_5_3_21_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_22, tgrp_case_5_3_22, name_case_5_3_22, desc_case_5_3_22, sref_case_5_3_22, {
	&test_5_3_22_top, &test_5_3_22_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_23, tgrp_case_5_3_23, name_case_5_3_23, desc_case_5_3_23, sref_case_5_3_23, {
	&test_5_3_23_top, &test_5_3_23_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_24, tgrp_case_5_3_24, name_case_5_3_24, desc_case_5_3_24, sref_case_5_3_24, {
	&test_5_3_24_top, &test_5_3_24_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_25, tgrp_case_5_3_25, name_case_5_3_25, desc_case_5_3_25, sref_case_5_3_25, {
	&test_5_3_25_top, &test_5_3_25_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_26, tgrp_case_5_3_26, name_case_5_3_26, desc_case_5_3_26, sref_case_5_3_26, {
	&test_5_3_26_top, &test_5_3_26_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_27, tgrp_case_5_3_27, name_case_5_3_27, desc_case_5_3_27, sref_case_5_3_27, {
	&test_5_3_27_top, &test_5_3_27_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_5_3_28, tgrp_case_5_3_28, name_case_5_3_28, desc_case_5_3_28, sref_case_5_3_28, {
	&test_5_3_28_top, &test_5_3_28_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_1_1, tgrp_case_6_1_1, name_case_6_1_1, desc_case_6_1_1, sref_case_6_1_1, {
	&test_6_1_1_top, &test_6_1_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_1_2, tgrp_case_6_1_2, name_case_6_1_2, desc_case_6_1_2, sref_case_6_1_2, {
	&test_6_1_2_top, &test_6_1_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_1_3, tgrp_case_6_1_3, name_case_6_1_3, desc_case_6_1_3, sref_case_6_1_3, {
	&test_6_1_3_top, &test_6_1_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_1_4, tgrp_case_6_1_4, name_case_6_1_4, desc_case_6_1_4, sref_case_6_1_4, {
	&test_6_1_4_top, &test_6_1_4_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_1_5, tgrp_case_6_1_5, name_case_6_1_5, desc_case_6_1_5, sref_case_6_1_5, {
	&test_6_1_5_top, &test_6_1_5_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_1_6, tgrp_case_6_1_6, name_case_6_1_6, desc_case_6_1_6, sref_case_6_1_6, {
	&test_6_1_6_top, &test_6_1_6_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_1_7, tgrp_case_6_1_7, name_case_6_1_7, desc_case_6_1_7, sref_case_6_1_7, {
	&test_6_1_7_top, &test_6_1_7_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_1_8, tgrp_case_6_1_8, name_case_6_1_8, desc_case_6_1_8, sref_case_6_1_8, {
	&test_6_1_8_top, &test_6_1_8_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_1_9, tgrp_case_6_1_9, name_case_6_1_9, desc_case_6_1_9, sref_case_6_1_9, {
	&test_6_1_9_top, &test_6_1_9_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_1_10, tgrp_case_6_1_10, name_case_6_1_10, desc_case_6_1_10, sref_case_6_1_10, {
	&test_6_1_10_top, &test_6_1_10_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_1_11, tgrp_case_6_1_11, name_case_6_1_11, desc_case_6_1_11, sref_case_6_1_11, {
	&test_6_1_11_top, &test_6_1_11_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_1_12, tgrp_case_6_1_12, name_case_6_1_12, desc_case_6_1_12, sref_case_6_1_12, {
	&test_6_1_12_top, &test_6_1_12_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_2_1, tgrp_case_6_2_1, name_case_6_2_1, desc_case_6_2_1, sref_case_6_2_1, {
	&test_6_2_1_top, &test_6_2_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_2_2, tgrp_case_6_2_2, name_case_6_2_2, desc_case_6_2_2, sref_case_6_2_2, {
	&test_6_2_2_top, &test_6_2_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_2_3, tgrp_case_6_2_3, name_case_6_2_3, desc_case_6_2_3, sref_case_6_2_3, {
	&test_6_2_3_top, &test_6_2_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_2_4, tgrp_case_6_2_4, name_case_6_2_4, desc_case_6_2_4, sref_case_6_2_4, {
	&test_6_2_4_top, &test_6_2_4_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_2_5, tgrp_case_6_2_5, name_case_6_2_5, desc_case_6_2_5, sref_case_6_2_5, {
	&test_6_2_5_top, &test_6_2_5_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_2_6, tgrp_case_6_2_6, name_case_6_2_6, desc_case_6_2_6, sref_case_6_2_6, {
	&test_6_2_6_top, &test_6_2_6_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_3_1, tgrp_case_6_3_1, name_case_6_3_1, desc_case_6_3_1, sref_case_6_3_1, {
	&test_6_3_1_top, &test_6_3_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_3_2, tgrp_case_6_3_2, name_case_6_3_2, desc_case_6_3_2, sref_case_6_3_2, {
	&test_6_3_2_top, &test_6_3_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_3_3, tgrp_case_6_3_3, name_case_6_3_3, desc_case_6_3_3, sref_case_6_3_3, {
	&test_6_3_3_top, &test_6_3_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_3_4, tgrp_case_6_3_4, name_case_6_3_4, desc_case_6_3_4, sref_case_6_3_4, {
	&test_6_3_4_top, &test_6_3_4_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_3_5, tgrp_case_6_3_5, name_case_6_3_5, desc_case_6_3_5, sref_case_6_3_5, {
	&test_6_3_5_top, &test_6_3_5_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_3_6, tgrp_case_6_3_6, name_case_6_3_6, desc_case_6_3_6, sref_case_6_3_6, {
	&test_6_3_6_top, &test_6_3_6_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_3_7, tgrp_case_6_3_7, name_case_6_3_7, desc_case_6_3_7, sref_case_6_3_7, {
	&test_6_3_7_top, &test_6_3_7_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_3_8, tgrp_case_6_3_8, name_case_6_3_8, desc_case_6_3_8, sref_case_6_3_8, {
	&test_6_3_8_top, &test_6_3_8_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_4_1, tgrp_case_6_4_1, name_case_6_4_1, desc_case_6_4_1, sref_case_6_4_1, {
	&test_6_4_1_top, &test_6_4_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_4_2, tgrp_case_6_4_2, name_case_6_4_2, desc_case_6_4_2, sref_case_6_4_2, {
	&test_6_4_2_top, &test_6_4_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_4_3, tgrp_case_6_4_3, name_case_6_4_3, desc_case_6_4_3, sref_case_6_4_3, {
	&test_6_4_3_top, &test_6_4_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_4_4, tgrp_case_6_4_4, name_case_6_4_4, desc_case_6_4_4, sref_case_6_4_4, {
	&test_6_4_4_top, &test_6_4_4_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_4_5, tgrp_case_6_4_5, name_case_6_4_5, desc_case_6_4_5, sref_case_6_4_5, {
	&test_6_4_5_top, &test_6_4_5_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_4_6, tgrp_case_6_4_6, name_case_6_4_6, desc_case_6_4_6, sref_case_6_4_6, {
	&test_6_4_6_top, &test_6_4_6_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_5_1, tgrp_case_6_5_1, name_case_6_5_1, desc_case_6_5_1, sref_case_6_5_1, {
	&test_6_5_1_top, &test_6_5_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_5_2, tgrp_case_6_5_2, name_case_6_5_2, desc_case_6_5_2, sref_case_6_5_2, {
	&test_6_5_2_top, &test_6_5_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_5_3, tgrp_case_6_5_3, name_case_6_5_3, desc_case_6_5_3, sref_case_6_5_3, {
	&test_6_5_3_top, &test_6_5_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_5_4, tgrp_case_6_5_4, name_case_6_5_4, desc_case_6_5_4, sref_case_6_5_4, {
	&test_6_5_4_top, &test_6_5_4_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_5_5, tgrp_case_6_5_5, name_case_6_5_5, desc_case_6_5_5, sref_case_6_5_5, {
	&test_6_5_5_top, &test_6_5_5_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_6_1, tgrp_case_6_6_1, name_case_6_6_1, desc_case_6_6_1, sref_case_6_6_1, {
	&test_6_6_1_top, &test_6_6_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_6_6_2, tgrp_case_6_6_2, name_case_6_6_2, desc_case_6_6_2, sref_case_6_6_2, {
	&test_6_6_2_top, &test_6_6_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_1_1, tgrp_case_7_1_1, name_case_7_1_1, desc_case_7_1_1, sref_case_7_1_1, {
	&test_7_1_1_top, &test_7_1_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_1_2, tgrp_case_7_1_2, name_case_7_1_2, desc_case_7_1_2, sref_case_7_1_2, {
	&test_7_1_2_top, &test_7_1_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_1_3, tgrp_case_7_1_3, name_case_7_1_3, desc_case_7_1_3, sref_case_7_1_3, {
	&test_7_1_3_top, &test_7_1_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_2_1, tgrp_case_7_2_1, name_case_7_2_1, desc_case_7_2_1, sref_case_7_2_1, {
	&test_7_2_1_top, &test_7_2_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_3_1, tgrp_case_7_3_1, name_case_7_3_1, desc_case_7_3_1, sref_case_7_3_1, {
	&test_7_3_1_top, &test_7_3_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_3_2, tgrp_case_7_3_2, name_case_7_3_2, desc_case_7_3_2, sref_case_7_3_2, {
	&test_7_3_2_top, &test_7_3_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_3_3, tgrp_case_7_3_3, name_case_7_3_3, desc_case_7_3_3, sref_case_7_3_3, {
	&test_7_3_3_top, &test_7_3_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_3_4, tgrp_case_7_3_4, name_case_7_3_4, desc_case_7_3_4, sref_case_7_3_4, {
	&test_7_3_4_top, &test_7_3_4_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_3_5, tgrp_case_7_3_5, name_case_7_3_5, desc_case_7_3_5, sref_case_7_3_5, {
	&test_7_3_5_top, &test_7_3_5_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_4_1, tgrp_case_7_4_1, name_case_7_4_1, desc_case_7_4_1, sref_case_7_4_1, {
	&test_7_4_1_top, &test_7_4_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_4_2, tgrp_case_7_4_2, name_case_7_4_2, desc_case_7_4_2, sref_case_7_4_2, {
	&test_7_4_2_top, &test_7_4_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_4_3, tgrp_case_7_4_3, name_case_7_4_3, desc_case_7_4_3, sref_case_7_4_3, {
	&test_7_4_3_top, &test_7_4_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_5_1, tgrp_case_7_5_1, name_case_7_5_1, desc_case_7_5_1, sref_case_7_5_1, {
	&test_7_5_1_top, &test_7_5_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_5_2, tgrp_case_7_5_2, name_case_7_5_2, desc_case_7_5_2, sref_case_7_5_2, {
	&test_7_5_2_top, &test_7_5_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_5_3, tgrp_case_7_5_3, name_case_7_5_3, desc_case_7_5_3, sref_case_7_5_3, {
	&test_7_5_3_top, &test_7_5_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_6_1, tgrp_case_7_6_1, name_case_7_6_1, desc_case_7_6_1, sref_case_7_6_1, {
	&test_7_6_1_top, &test_7_6_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_6_2, tgrp_case_7_6_2, name_case_7_6_2, desc_case_7_6_2, sref_case_7_6_2, {
	&test_7_6_2_top, &test_7_6_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_6_3, tgrp_case_7_6_3, name_case_7_6_3, desc_case_7_6_3, sref_case_7_6_3, {
	&test_7_6_3_top, &test_7_6_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_7_1, tgrp_case_7_7_1, name_case_7_7_1, desc_case_7_7_1, sref_case_7_7_1, {
	&test_7_7_1_top, &test_7_7_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_7_2, tgrp_case_7_7_2, name_case_7_7_2, desc_case_7_7_2, sref_case_7_7_2, {
	&test_7_7_2_top, &test_7_7_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_7_3, tgrp_case_7_7_3, name_case_7_7_3, desc_case_7_7_3, sref_case_7_7_3, {
	&test_7_7_3_top, &test_7_7_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_8_1, tgrp_case_7_8_1, name_case_7_8_1, desc_case_7_8_1, sref_case_7_8_1, {
	&test_7_8_1_top, &test_7_8_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_8_2, tgrp_case_7_8_2, name_case_7_8_2, desc_case_7_8_2, sref_case_7_8_2, {
	&test_7_8_2_top, &test_7_8_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_8_3, tgrp_case_7_8_3, name_case_7_8_3, desc_case_7_8_3, sref_case_7_8_3, {
	&test_7_8_3_top, &test_7_8_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_9_1, tgrp_case_7_9_1, name_case_7_9_1, desc_case_7_9_1, sref_case_7_9_1, {
	&test_7_9_1_top, &test_7_9_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_9_2, tgrp_case_7_9_2, name_case_7_9_2, desc_case_7_9_2, sref_case_7_9_2, {
	&test_7_9_2_top, &test_7_9_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_9_3, tgrp_case_7_9_3, name_case_7_9_3, desc_case_7_9_3, sref_case_7_9_3, {
	&test_7_9_3_top, &test_7_9_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_10_1, tgrp_case_7_10_1, name_case_7_10_1, desc_case_7_10_1, sref_case_7_10_1, {
	&test_7_10_1_top, &test_7_10_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_10_2, tgrp_case_7_10_2, name_case_7_10_2, desc_case_7_10_2, sref_case_7_10_2, {
	&test_7_10_2_top, &test_7_10_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_10_3, tgrp_case_7_10_3, name_case_7_10_3, desc_case_7_10_3, sref_case_7_10_3, {
	&test_7_10_3_top, &test_7_10_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_11_1, tgrp_case_7_11_1, name_case_7_11_1, desc_case_7_11_1, sref_case_7_11_1, {
	&test_7_11_1_top, &test_7_11_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_11_2, tgrp_case_7_11_2, name_case_7_11_2, desc_case_7_11_2, sref_case_7_11_2, {
	&test_7_11_2_top, &test_7_11_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_11_3, tgrp_case_7_11_3, name_case_7_11_3, desc_case_7_11_3, sref_case_7_11_3, {
	&test_7_11_3_top, &test_7_11_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_12_1, tgrp_case_7_12_1, name_case_7_12_1, desc_case_7_12_1, sref_case_7_12_1, {
	&test_7_12_1_top, &test_7_12_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_12_2, tgrp_case_7_12_2, name_case_7_12_2, desc_case_7_12_2, sref_case_7_12_2, {
	&test_7_12_2_top, &test_7_12_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_12_3, tgrp_case_7_12_3, name_case_7_12_3, desc_case_7_12_3, sref_case_7_12_3, {
	&test_7_12_3_top, &test_7_12_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_13_1, tgrp_case_7_13_1, name_case_7_13_1, desc_case_7_13_1, sref_case_7_13_1, {
	&test_7_13_1_top, &test_7_13_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_13_2, tgrp_case_7_13_2, name_case_7_13_2, desc_case_7_13_2, sref_case_7_13_2, {
	&test_7_13_2_top, &test_7_13_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_13_3, tgrp_case_7_13_3, name_case_7_13_3, desc_case_7_13_3, sref_case_7_13_3, {
	&test_7_13_3_top, &test_7_13_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_14_1, tgrp_case_7_14_1, name_case_7_14_1, desc_case_7_14_1, sref_case_7_14_1, {
	&test_7_14_1_top, &test_7_14_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_14_2, tgrp_case_7_14_2, name_case_7_14_2, desc_case_7_14_2, sref_case_7_14_2, {
	&test_7_14_2_top, &test_7_14_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_14_3, tgrp_case_7_14_3, name_case_7_14_3, desc_case_7_14_3, sref_case_7_14_3, {
	&test_7_14_3_top, &test_7_14_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_15_1, tgrp_case_7_15_1, name_case_7_15_1, desc_case_7_15_1, sref_case_7_15_1, {
	&test_7_15_1_top, &test_7_15_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_15_2, tgrp_case_7_15_2, name_case_7_15_2, desc_case_7_15_2, sref_case_7_15_2, {
	&test_7_15_2_top, &test_7_15_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_15_3, tgrp_case_7_15_3, name_case_7_15_3, desc_case_7_15_3, sref_case_7_15_3, {
	&test_7_15_3_top, &test_7_15_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_16_1, tgrp_case_7_16_1, name_case_7_16_1, desc_case_7_16_1, sref_case_7_16_1, {
	&test_7_16_1_top, &test_7_16_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_16_2, tgrp_case_7_16_2, name_case_7_16_2, desc_case_7_16_2, sref_case_7_16_2, {
	&test_7_16_2_top, &test_7_16_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_16_3, tgrp_case_7_16_3, name_case_7_16_3, desc_case_7_16_3, sref_case_7_16_3, {
	&test_7_16_3_top, &test_7_16_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_7_17_1, tgrp_case_7_17_1, name_case_7_17_1, desc_case_7_17_1, sref_case_7_17_1, {
	&test_7_17_1_top, &test_7_17_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_1_1, tgrp_case_8_1_1, name_case_8_1_1, desc_case_8_1_1, sref_case_8_1_1, {
	&test_8_1_1_top, &test_8_1_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_1_2, tgrp_case_8_1_2, name_case_8_1_2, desc_case_8_1_2, sref_case_8_1_2, {
	&test_8_1_2_top, &test_8_1_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_1_3, tgrp_case_8_1_3, name_case_8_1_3, desc_case_8_1_3, sref_case_8_1_3, {
	&test_8_1_3_top, &test_8_1_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_1_4, tgrp_case_8_1_4, name_case_8_1_4, desc_case_8_1_4, sref_case_8_1_4, {
	&test_8_1_4_top, &test_8_1_4_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_2_1, tgrp_case_8_2_1, name_case_8_2_1, desc_case_8_2_1, sref_case_8_2_1, {
	&test_8_2_1_top, &test_8_2_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_2_2, tgrp_case_8_2_2, name_case_8_2_2, desc_case_8_2_2, sref_case_8_2_2, {
	&test_8_2_2_top, &test_8_2_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_2_3, tgrp_case_8_2_3, name_case_8_2_3, desc_case_8_2_3, sref_case_8_2_3, {
	&test_8_2_3_top, &test_8_2_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_3_1, tgrp_case_8_3_1, name_case_8_3_1, desc_case_8_3_1, sref_case_8_3_1, {
	&test_8_3_1_top, &test_8_3_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_3_2, tgrp_case_8_3_2, name_case_8_3_2, desc_case_8_3_2, sref_case_8_3_2, {
	&test_8_3_2_top, &test_8_3_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_4_1, tgrp_case_8_4_1, name_case_8_4_1, desc_case_8_4_1, sref_case_8_4_1, {
	&test_8_4_1_top, &test_8_4_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_5_1, tgrp_case_8_5_1, name_case_8_5_1, desc_case_8_5_1, sref_case_8_5_1, {
	&test_8_5_1_top, &test_8_5_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_5_2, tgrp_case_8_5_2, name_case_8_5_2, desc_case_8_5_2, sref_case_8_5_2, {
	&test_8_5_2_top, &test_8_5_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_5_3, tgrp_case_8_5_3, name_case_8_5_3, desc_case_8_5_3, sref_case_8_5_3, {
	&test_8_5_3_top, &test_8_5_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_5_4, tgrp_case_8_5_4, name_case_8_5_4, desc_case_8_5_4, sref_case_8_5_4, {
	&test_8_5_4_top, &test_8_5_4_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_6_1, tgrp_case_8_6_1, name_case_8_6_1, desc_case_8_6_1, sref_case_8_6_1, {
	&test_8_6_1_top, &test_8_6_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_6_2, tgrp_case_8_6_2, name_case_8_6_2, desc_case_8_6_2, sref_case_8_6_2, {
	&test_8_6_2_top, &test_8_6_2_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_6_3, tgrp_case_8_6_3, name_case_8_6_3, desc_case_8_6_3, sref_case_8_6_3, {
	&test_8_6_3_top, &test_8_6_3_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_7_1, tgrp_case_8_7_1, name_case_8_7_1, desc_case_8_7_1, sref_case_8_7_1, {
	&test_8_7_1_top, &test_8_7_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
		numb_case_8_8_1, tgrp_case_8_8_1, name_case_8_8_1, desc_case_8_8_1, sref_case_8_8_1, {
	&test_8_8_1_top, &test_8_8_1_bot, NULL}, &begin_tests, &end_tests, 0, 0}, {
	NULL,}
};

static int summary = 0;

void
print_header(void)
{
	if (verbose <= 0)
		return;
	dummy = lockf(fileno(stdout), F_LOCK, 0);
	fprintf(stdout, "\n%s - %s - %s - Conformance Test Suite\n", lstdname, lpkgname, shortname);
	fflush(stdout);
	dummy = lockf(fileno(stdout), F_ULOCK, 0);
}

int
do_tests(int num_tests)
{
	int i;
	int result = __RESULT_INCONCLUSIVE;
	int notapplicable = 0;
	int inconclusive = 0;
	int successes = 0;
	int failures = 0;
	int skipped = 0;
	int notselected = 0;
	int aborted = 0;

	print_header();
	show = 0;
	if (verbose > 0) {
		dummy = lockf(fileno(stdout), F_LOCK, 0);
		fprintf(stdout, "\nUsing device %s\n\n", devname);
		fflush(stdout);
		dummy = lockf(fileno(stdout), F_ULOCK, 0);
	}
	if (num_tests == 1 || begin_tests(0) == __RESULT_SUCCESS) {
		if (num_tests != 1)
			end_tests(0);
		show = 1;
		for (i = 0; i < (sizeof(tests) / sizeof(struct test_case)) && tests[i].numb; i++) {
			if (!tests[i].run) {
				tests[i].result = __RESULT_INCONCLUSIVE;
				notselected++;
				continue;
			}
			if (aborted) {
				tests[i].result = __RESULT_INCONCLUSIVE;
				inconclusive++;
				continue;
			}
			if (verbose > 0) {
				dummy = lockf(fileno(stdout), F_LOCK, 0);
				if (verbose > 1)
					fprintf(stdout, "\nTest Group: %s", tests[i].tgrp);
				fprintf(stdout, "\nTest Case %s-%s/%s: %s\n", sstdname, shortname, tests[i].numb, tests[i].name);
				if (verbose > 1)
					fprintf(stdout, "Test Reference: %s\n", tests[i].sref);
				if (verbose > 1)
					fprintf(stdout, "%s\n", tests[i].desc);
				fprintf(stdout, "\n");
				fflush(stdout);
				dummy = lockf(fileno(stdout), F_ULOCK, 0);
			}
			if ((result = tests[i].result) == 0) {
				if ((result = (*tests[i].start) (i)) != __RESULT_SUCCESS)
					goto inconclusive;
				result = test_run(tests[i].stream);
				(*tests[i].stop) (i);
			} else {
				switch (result) {
				case __RESULT_SUCCESS:
					print_passed(3);
					break;
				case __RESULT_FAILURE:
					print_failed(3);
					break;
				case __RESULT_NOTAPPL:
					print_notapplicable(3);
					break;
				case __RESULT_SKIPPED:
					print_skipped(3);
					break;
				default:
				case __RESULT_INCONCLUSIVE:
					print_inconclusive(3);
					break;
				}
			}
			switch (result) {
			case __RESULT_SUCCESS:
				successes++;
				if (verbose > 0) {
					dummy = lockf(fileno(stdout), F_LOCK, 0);
					fprintf(stdout, "\n");
					fprintf(stdout, "*********\n");
					fprintf(stdout, "********* Test Case SUCCESSFUL\n");
					fprintf(stdout, "*********\n\n");
					fflush(stdout);
					dummy = lockf(fileno(stdout), F_ULOCK, 0);
				}
				break;
			case __RESULT_FAILURE:
				failures++;
				if (verbose > 0) {
					dummy = lockf(fileno(stdout), F_LOCK, 0);
					fprintf(stdout, "\n");
					fprintf(stdout, "XXXXXXXXX\n");
					fprintf(stdout, "XXXXXXXXX Test Case FAILED\n");
					fprintf(stdout, "XXXXXXXXX\n\n");
					fflush(stdout);
					dummy = lockf(fileno(stdout), F_ULOCK, 0);
				}
				break;
			case __RESULT_NOTAPPL:
				notapplicable++;
				if (verbose > 0) {
					dummy = lockf(fileno(stdout), F_LOCK, 0);
					fprintf(stdout, "\n");
					fprintf(stdout, "XXXXXXXXX\n");
					fprintf(stdout, "XXXXXXXXX Test Case NOT APPLICABLE\n");
					fprintf(stdout, "XXXXXXXXX\n\n");
					fflush(stdout);
					dummy = lockf(fileno(stdout), F_ULOCK, 0);
				}
				break;
			case __RESULT_SKIPPED:
				skipped++;
				if (verbose > 0) {
					dummy = lockf(fileno(stdout), F_LOCK, 0);
					fprintf(stdout, "\n");
					fprintf(stdout, "XXXXXXXXX\n");
					fprintf(stdout, "XXXXXXXXX Test Case SKIPPED\n");
					fprintf(stdout, "XXXXXXXXX\n\n");
					fflush(stdout);
					dummy = lockf(fileno(stdout), F_ULOCK, 0);
				}
				break;
			default:
			case __RESULT_INCONCLUSIVE:
			      inconclusive:
				inconclusive++;
				if (verbose > 0) {
					dummy = lockf(fileno(stdout), F_LOCK, 0);
					fprintf(stdout, "\n");
					fprintf(stdout, "?????????\n");
					fprintf(stdout, "????????? Test Case INCONCLUSIVE\n");
					fprintf(stdout, "?????????\n\n");
					fflush(stdout);
					dummy = lockf(fileno(stdout), F_ULOCK, 0);
				}
				break;
			}
			tests[i].result = result;
			if (exit_on_failure && (result == __RESULT_FAILURE || result == __RESULT_INCONCLUSIVE))
				aborted = 1;
		}
		if (summary && verbose) {
			dummy = lockf(fileno(stdout), F_LOCK, 0);
			fprintf(stdout, "\n");
			fflush(stdout);
			dummy = lockf(fileno(stdout), F_ULOCK, 0);
			for (i = 0; i < (sizeof(tests) / sizeof(struct test_case)) && tests[i].numb; i++) {
				if (tests[i].run) {
					dummy = lockf(fileno(stdout), F_LOCK, 0);
					fprintf(stdout, "Test Case %s-%s/%-10s ", sstdname, shortname, tests[i].numb);
					fflush(stdout);
					dummy = lockf(fileno(stdout), F_ULOCK, 0);
					switch (tests[i].result) {
					case __RESULT_SUCCESS:
						dummy = lockf(fileno(stdout), F_LOCK, 0);
						fprintf(stdout, "SUCCESS\n");
						fflush(stdout);
						dummy = lockf(fileno(stdout), F_ULOCK, 0);
						break;
					case __RESULT_FAILURE:
						dummy = lockf(fileno(stdout), F_LOCK, 0);
						fprintf(stdout, "FAILURE\n");
						fflush(stdout);
						dummy = lockf(fileno(stdout), F_ULOCK, 0);
						break;
					case __RESULT_NOTAPPL:
						dummy = lockf(fileno(stdout), F_LOCK, 0);
						fprintf(stdout, "NOT APPLICABLE\n");
						fflush(stdout);
						dummy = lockf(fileno(stdout), F_ULOCK, 0);
						break;
					case __RESULT_SKIPPED:
						dummy = lockf(fileno(stdout), F_LOCK, 0);
						fprintf(stdout, "SKIPPED\n");
						fflush(stdout);
						dummy = lockf(fileno(stdout), F_ULOCK, 0);
						break;
					default:
					case __RESULT_INCONCLUSIVE:
						dummy = lockf(fileno(stdout), F_LOCK, 0);
						fprintf(stdout, "INCONCLUSIVE\n");
						fflush(stdout);
						dummy = lockf(fileno(stdout), F_ULOCK, 0);
						break;
					}
				}
			}
		}
		if (verbose > 0 && num_tests > 1) {
			dummy = lockf(fileno(stdout), F_LOCK, 0);
			fprintf(stdout, "\n");
			fprintf(stdout, "========= %3d successes     \n", successes);
			fprintf(stdout, "========= %3d failures      \n", failures);
			fprintf(stdout, "========= %3d inconclusive  \n", inconclusive);
			fprintf(stdout, "========= %3d not applicable\n", notapplicable);
			fprintf(stdout, "========= %3d skipped       \n", skipped);
			fprintf(stdout, "========= %3d not selected  \n", notselected);
			fprintf(stdout, "============================\n");
			fprintf(stdout, "========= %3d total         \n", successes + failures + inconclusive + notapplicable + skipped + notselected);
			if (!(aborted + failures))
				fprintf(stdout, "\nDone.\n\n");
			fflush(stdout);
			dummy = lockf(fileno(stdout), F_ULOCK, 0);
		}
		if (aborted) {
			dummy = lockf(fileno(stderr), F_LOCK, 0);
			if (verbose > 0)
				fprintf(stderr, "\n");
			fprintf(stderr, "Test Suite aborted due to failure.\n");
			if (verbose > 0)
				fprintf(stderr, "\n");
			fflush(stderr);
			dummy = lockf(fileno(stderr), F_ULOCK, 0);
		} else if (failures) {
			dummy = lockf(fileno(stderr), F_LOCK, 0);
			if (verbose > 0)
				fprintf(stderr, "\n");
			fprintf(stderr, "Test Suite failed.\n");
			if (verbose > 0)
				fprintf(stderr, "\n");
			fflush(stderr);
			dummy = lockf(fileno(stderr), F_ULOCK, 0);
		}
		if (num_tests == 1) {
			if (successes)
				return (0);
			if (failures)
				return (1);
			if (inconclusive)
				return (1);
			if (notapplicable)
				return (0);
			if (skipped)
				return (77);
		}
		return (aborted);
	} else {
		end_tests(0);
		show = 1;
		dummy = lockf(fileno(stderr), F_LOCK, 0);
		fprintf(stderr, "Test Suite setup failed!\n");
		fflush(stderr);
		dummy = lockf(fileno(stderr), F_ULOCK, 0);
		return (2);
	}
}

void
copying(int argc, char *argv[])
{
	if (!verbose)
		return;
	print_header();
	fprintf(stdout, "\
\n\
Copyright (c) 2001-2006  OpenSS7 Corporation <http://www.openss7.com/>\n\
Copyright (c) 1997-2001  Brian F. G. Bidulock <bidulock@openss7.org>\n\
\n\
All Rights Reserved.\n\
\n\
Unauthorized distribution or duplication is prohibited.\n\
\n\
This software and related documentation is protected by copyright and distribut-\n\
ed under licenses restricting its use,  copying, distribution and decompilation.\n\
No part of this software or related documentation may  be reproduced in any form\n\
by any means without the prior  written  authorization of the  copyright holder,\n\
and licensors, if any.\n\
\n\
The recipient of this document,  by its retention and use, warrants that the re-\n\
cipient  will protect this  information and  keep it confidential,  and will not\n\
disclose the information contained  in this document without the written permis-\n\
sion of its owner.\n\
\n\
The author reserves the right to revise  this software and documentation for any\n\
reason,  including but not limited to, conformity with standards  promulgated by\n\
various agencies, utilization of advances in the state of the technical arts, or\n\
the reflection of changes  in the design of any techniques, or procedures embod-\n\
ied, described, or  referred to herein.   The author  is under no  obligation to\n\
provide any feature listed herein.\n\
\n\
As an exception to the above,  this software may be  distributed  under the  GNU\n\
General Public License (GPL) Version 2,  so long as the  software is distributed\n\
with, and only used for the testing of, OpenSS7 modules, drivers, and libraries.\n\
\n\
U.S. GOVERNMENT RESTRICTED RIGHTS.  If you are licensing this Software on behalf\n\
of the  U.S. Government  (\"Government\"),  the following provisions apply to you.\n\
If the Software is  supplied by the Department of Defense (\"DoD\"), it is classi-\n\
fied as  \"Commercial Computer Software\"  under paragraph 252.227-7014 of the DoD\n\
Supplement  to the  Federal Acquisition Regulations  (\"DFARS\") (or any successor\n\
regulations) and the  Government  is acquiring  only the license rights  granted\n\
herein (the license  rights customarily  provided to non-Government  users).  If\n\
the Software is supplied to any unit or agency of the Government other than DoD,\n\
it is classified as  \"Restricted Computer Software\" and the  Government's rights\n\
in the  Software are defined in  paragraph 52.227-19 of the Federal  Acquisition\n\
Regulations  (\"FAR\") (or any successor regulations) or, in the cases of NASA, in\n\
paragraph  18.52.227-86 of the  NASA Supplement  to the  FAR (or  any  successor\n\
regulations).\n\
\n\
");
}

void
version(int argc, char *argv[])
{
	if (!verbose)
		return;
	fprintf(stdout, "\
\n\
%1$s:\n\
    %2$s\n\
    Copyright (c) 1997-2006  OpenSS7 Corporation.  All Rights Reserved.\n\
\n\
    Distributed by OpenSS7 Corporation under GPL Version 2,\n\
    incorporated here by reference.\n\
\n\
    See `%1$s --copying' for copying permission.\n\
\n\
", argv[0], ident);
}

void
usage(int argc, char *argv[])
{
	if (!verbose)
		return;
	fprintf(stderr, "\
Usage:\n\
    %1$s [options]\n\
    %1$s {-h, --help}\n\
    %1$s {-V, --version}\n\
    %1$s {-C, --copying}\n\
", argv[0]);
}

void
help(int argc, char *argv[])
{
	if (!verbose)
		return;
	fprintf(stdout, "\
\n\
Usage:\n\
    %1$s [options]\n\
    %1$s {-h, --help}\n\
    %1$s {-V, --version}\n\
    %1$s {-C, --copying}\n\
Arguments:\n\
    (none)\n\
Options:\n\
    -d, --device DEVICE\n\
        device name to open [default: %2$s].\n\
    -e, --exit\n\
        exit on the first failed or inconclusive test case.\n\
    -l, --list [RANGE]\n\
        list test case names within a range [default: all] and exit.\n\
    -f, --fast [SCALE]\n\
        increase speed of tests by scaling timers [default: 50]\n\
    -s, --summary\n\
        print a test case summary at end of testing [default: off]\n\
    -o, --onetest [TESTCASE]\n\
        run a single test case.\n\
    -t, --tests [RANGE]\n\
        run a range of test cases.\n\
    -m, --messages\n\
        display messages. [default: off]\n\
    -q, --quiet\n\
        suppress normal output (equivalent to --verbose=0)\n\
    -v, --verbose [LEVEL]\n\
        increase verbosity or set to LEVEL [default: 1]\n\
        this option may be repeated.\n\
    -h, --help, -?, --?\n\
        print this usage message and exit\n\
    -V, --version\n\
        print version and exit\n\
    -C, --copying\n\
        print copying permission and exit\n\
\n\
", argv[0], devname);
}

int
main(int argc, char *argv[])
{
	size_t l, n;
	int range = 0;
	struct test_case *t;
	int tests_to_run = 0;

	for (t = tests; t->numb; t++) {
		if (!t->result) {
			t->run = 1;
			tests_to_run++;
		}
	}
	for (;;) {
		int c, val;

#if defined _GNU_SOURCE
		int option_index = 0;
		/* *INDENT-OFF* */
		static struct option long_options[] = {
			{"device",	required_argument,	NULL, 'd'},
			{"exit",	no_argument,		NULL, 'e'},
			{"list",	optional_argument,	NULL, 'l'},
			{"fast",	optional_argument,	NULL, 'f'},
			{"summary",	no_argument,		NULL, 's'},
			{"onetest",	required_argument,	NULL, 'o'},
			{"tests",	required_argument,	NULL, 't'},
			{"messages",	no_argument,		NULL, 'm'},
			{"quiet",	no_argument,		NULL, 'q'},
			{"verbose",	optional_argument,	NULL, 'v'},
			{"help",	no_argument,		NULL, 'h'},
			{"version",	no_argument,		NULL, 'V'},
			{"copying",	no_argument,		NULL, 'C'},
			{"?",		no_argument,		NULL, 'h'},
			{NULL,		0,			NULL,  0 }
		};
		/* *INDENT-ON* */

		c = getopt_long(argc, argv, "d:el::f::so:t:mqvhVC?", long_options, &option_index);
#else				/* defined _GNU_SOURCE */
		c = getopt(argc, argv, "d:el::f::so:t:mqvhVC?");
#endif				/* defined _GNU_SOURCE */
		if (c == -1)
			break;
		switch (c) {
		case 'd':
			if (optarg) {
				snprintf(devname, sizeof(devname), "%s", optarg);
				break;
			}
			goto bad_option;
		case 'e':
			exit_on_failure = 1;
			break;
		case 'l':
			if (optarg) {
				l = strnlen(optarg, 16);
				fprintf(stdout, "\n");
				for (n = 0, t = tests; t->numb; t++)
					if (!strncmp(t->numb, optarg, l)) {
						if (verbose > 2)
							fprintf(stdout, "Test Group: %s\n", t->tgrp);
						fprintf(stdout, "Test Case %s-%s/%s: %s\n", sstdname, shortname, t->numb, t->name);
						if (verbose > 2)
							fprintf(stdout, "Test Reference: %s\n", t->sref);
						if (verbose > 1)
							fprintf(stdout, "%s\n\n", t->desc);
						fflush(stdout);
						n++;
					}
				if (!n) {
					fprintf(stderr, "WARNING: specification `%s' matched no test\n", optarg);
					fflush(stderr);
					goto bad_option;
				}
				if (verbose <= 1)
					fprintf(stdout, "\n");
				fflush(stdout);
				exit(0);
			} else {
				fprintf(stdout, "\n");
				for (t = tests; t->numb; t++) {
					if (verbose > 2)
						fprintf(stdout, "Test Group: %s\n", t->tgrp);
					fprintf(stdout, "Test Case %s-%s/%s: %s\n", sstdname, shortname, t->numb, t->name);
					if (verbose > 2)
						fprintf(stdout, "Test Reference: %s\n", t->sref);
					if (verbose > 1)
						fprintf(stdout, "%s\n\n", t->desc);
					fflush(stdout);
				}
				if (verbose <= 1)
					fprintf(stdout, "\n");
				fflush(stdout);
				exit(0);
			}
			break;
		case 'f':
			if (optarg)
				timer_scale = atoi(optarg);
			else
				timer_scale = 50;
			fprintf(stderr, "WARNING: timers are scaled by a factor of %ld\n", timer_scale);
			break;
		case 's':
			summary = 1;
			break;
		case 'o':
			if (optarg) {
				if (!range) {
					for (t = tests; t->numb; t++)
						t->run = 0;
					tests_to_run = 0;
				}
				range = 1;
				for (n = 0, t = tests; t->numb; t++)
					if (!strncmp(t->numb, optarg, 16)) {
						// if (!t->result) {
						t->run = 1;
						n++;
						tests_to_run++;
						// }
					}
				if (!n) {
					fprintf(stderr, "WARNING: specification `%s' matched no test\n", optarg);
					fflush(stderr);
					goto bad_option;
				}
				break;
			}
			goto bad_option;
		case 'q':
			verbose = 0;
			break;
		case 'v':
			if (optarg == NULL) {
				verbose++;
				break;
			}
			if ((val = strtol(optarg, NULL, 0)) < 0)
				goto bad_option;
			verbose = val;
			break;
		case 't':
			l = strnlen(optarg, 16);
			if (!range) {
				for (t = tests; t->numb; t++)
					t->run = 0;
				tests_to_run = 0;
			}
			range = 1;
			for (n = 0, t = tests; t->numb; t++)
				if (!strncmp(t->numb, optarg, l)) {
					// if (!t->result) {
					t->run = 1;
					n++;
					tests_to_run++;
					// }
				}
			if (!n) {
				fprintf(stderr, "WARNING: specification `%s' matched no test\n", optarg);
				fflush(stderr);
				goto bad_option;
			}
			break;
		case 'm':
			show_msg = 1;
			break;
		case 'H':	/* -H */
		case 'h':	/* -h, --help */
			help(argc, argv);
			exit(0);
		case 'V':
			version(argc, argv);
			exit(0);
		case 'C':
			copying(argc, argv);
			exit(0);
		case '?':
		default:
		      bad_option:
			optind--;
		      bad_nonopt:
			if (optind < argc && verbose) {
				fprintf(stderr, "%s: illegal syntax -- ", argv[0]);
				while (optind < argc)
					fprintf(stderr, "%s ", argv[optind++]);
				fprintf(stderr, "\n");
				fflush(stderr);
			}
			goto bad_usage;
		      bad_usage:
			usage(argc, argv);
			exit(2);
		}
	}
	/* 
	 * dont' ignore non-option arguments
	 */
	if (optind < argc)
		goto bad_nonopt;
	switch (tests_to_run) {
	case 0:
		if (verbose > 0) {
			fprintf(stderr, "%s: error: no tests to run\n", argv[0]);
			fflush(stderr);
		}
		exit(2);
	case 1:
		break;
	default:
		copying(argc, argv);
	}
	exit(do_tests(tests_to_run));
}
