/*****************************************************************************

 @(#) queue.h,v 0.9.2.11 2006/04/24 05:00:46 brian Exp

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

 Last Modified 2006/04/24 05:00:46 by brian

 *****************************************************************************/

#ifndef __OS7_QUEUE_H__
#define __OS7_QUEUE_H__

#ident "@(#) queue.h,v OpenSS7-0_9_2_D_rc2(0.9.2.11) Copyright (c) 2001-2006 OpenSS7 Corporation."

#define QR_DONE		0
#define QR_ABSORBED	1
#define QR_TRIMMED	2
#define QR_LOOP		3
#define QR_PASSALONG	4
#define QR_PASSFLOW	5
#define QR_DISABLE	6
#define QR_STRIP	7
#define QR_RETRY	8

extern int streamscall ss7_w_flush(queue_t *q, mblk_t *mp);
extern int streamscall ss7_r_flush(queue_t *q, mblk_t *mp);
extern int streamscall ss7_putq(queue_t *q, mblk_t *mp, int streamscall (*proc) (queue_t *, mblk_t *));
extern int streamscall ss7_srvq(queue_t *q, int streamscall (*proc) (queue_t *, mblk_t *),
				void streamscall (*wakeup) (queue_t *));
extern int streamscall ss7_oput(queue_t *q, mblk_t *mp);
extern int streamscall ss7_osrv(queue_t *q);
extern int streamscall ss7_iput(queue_t *q, mblk_t *mp);
extern int streamscall ss7_isrv(queue_t *q);

#endif				/* __OS7_QUEUE_H__ */
