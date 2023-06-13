/*****************************************************************************

 @(#) stropts32.h,v 0.9.2.3 2006/03/03 10:57:11 brian Exp

 -----------------------------------------------------------------------------

 Copyright (c) 2001-2006  OpenSS7 Corporation <http://www.openss7.com/>

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

 Last Modified 2006/03/03 10:57:11 by brian

 -----------------------------------------------------------------------------

 stropts32.h,v
 Revision 0.9.2.3  2006/03/03 10:57:11  brian
 - 32-bit compatibility support, updates for release

 Revision 0.9.2.2  2006/02/20 10:59:20  brian
 - updated copyright headers on changed files

 Revision 0.9.2.1  2006/02/20 09:36:14  brian
 - added 32 bit compatibility headers

 *****************************************************************************/

#ifndef __SYS_STREAMS_STROPTS32_H__
#define __SYS_STREAMS_STROPTS32_H__

#ident "@(#) stropts32.h,v OpenSS7-0_9_2_D_rc2(0.9.2.3) Copyright (c) 2001-2006 OpenSS7 Corporation."

struct bandinfo32 {
	unsigned char bi_pri;
	int32_t bi_flag;
};

struct strbuf32 {
	int32_t maxlen;
	int32_t len;
	uint32_t buf;
};

struct strpeek32 {
	struct strbuf32 ctlbuf;
	struct strbuf32 databuf;
	uint32_t flags;
};

struct strfdinsert32 {
	struct strbuf32 ctlbuf;		/* ctrl part for putmsg(2) */
	struct strbuf32 databuf;	/* data part for putmsg(2) */
	int32_t flags;			/* flags for putmsg(2) */
	int32_t fildes;			/* file descriptor to insert */
	int32_t offset;			/* offset within control part for insertion */
};

struct strioctl32 {
	int32_t ic_cmd;			/* command to perform */
	int32_t ic_timout;		/* ioctl timeout period */
	int32_t ic_len;			/* size of data buffer */
	uint32_t ic_dp;			/* addr of data buffer */
};

struct strrecvfd32 {
	int32_t fd;			/* file descriptor */
	uid_t uid;			/* user id */
	gid_t gid;			/* group id */
	char fill[8];			/* UnixWare/Solaris compatibility */
};

struct str_mlist32 {
	char l_name[FMNAMESZ + 1];
};

struct str_list32 {
	int32_t sl_nmods;
	uint32_t sl_modlist;
};

struct strsigset32 {
	pid_t ss_pid;			/* XXX */
	int32_t ss_events;
};

/* for ioctl emulation of getmsg() getpmsg() putmsg() putpmsg() matches Mac OT */
struct strpmsg32 {
	struct strbuf32 ctlbuf;
	struct strbuf32 databuf;
	int32_t band;
	int32_t flags;			/* actually long for Mac OT */
};

#endif				/* __SYS_STREAMS_STROPTS32_H__ */
