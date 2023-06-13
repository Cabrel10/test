/*****************************************************************************

 @(#) strlog.h,v 0.9.2.12 2005/12/17 08:39:20 brian Exp

 -----------------------------------------------------------------------------

 Copyright (C) 2001-2005  OpenSS7 Corporation <http://www.openss7.com>

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

 Last Modified 2005/12/17 08:39:20 by brian

 *****************************************************************************/

#ifndef __SYS_STRLOG_H__
#define __SYS_STRLOG_H__

#ident "@(#) strlog.h,v OpenSS7-0_9_2_D_rc2(0.9.2.12) 2005/12/17 08:39:20"

#define SL_ERROR    0x0001
#define SL_TRACE    0x0002
#define SL_NOTIFY   0x0004
#define SL_CONSOLE  0x0008
#define SL_FATAL    0x0010
#define SL_WARN	    0x0020
#define SL_NOTE	    0x0040
#define SL_NOPUTBUF 0x0080	/* uw7 src compatibility (does nothing) */

#define LOGMSGSZ    1024	/* max format string length */
#define NLOGARGS    3		/* max number of arguments (really unlimited) */

#define LOGCTL		(('L')<<8)
#define I_ERRLOG	(LOGCTL | 1)	/* error logger */
#define I_TRCLOG	(LOGCTL | 2)	/* trace logger */
#define I_CONSLOG	(LOGCTL | 3)	/* console logger */

extern int strlog(short mid, short sid, char level, unsigned short flags, char *fmt, ...)
    __attribute__ ((format(printf, 5, 6)));
extern int vstrlog(short mid, short sid, char level, unsigned short flag, char *fmt, va_list args);

typedef int (*vstrlog_t) (short, short, char, unsigned short, char *, va_list);
extern vstrlog_t register_strlog(vstrlog_t newlog);

struct trace_ids {
	short ti_mid;
	short ti_sid;
	char ti_level;
	short ti_flags;			/* not for Solaris */
};

struct log_ctl {
	short mid;
	short sid;
	char level;
	short flags;
	long ltime;			/* clock32_t or clock_t under Solaris */
	long ttime;			/* time32_t or time_t under Solaris */
	int seq_no;
	int pri;			/* priority = (facility|level) except HPUX */
};

#endif				/* __SYS_STRLOG_H__ */
