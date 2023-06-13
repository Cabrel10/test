/*****************************************************************************

 @(#) isastream.c,v OpenSS7-0_9_2_D_rc2(0.9.2.8) 2005/12/08 00:59:58

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

 Last Modified 2005/12/08 00:59:58 by brian

 *****************************************************************************/

#ident "@(#) isastream.c,v OpenSS7-0_9_2_D_rc2(0.9.2.8) 2005/12/08 00:59:58"

static char const ident[] =
    "isastream.c,v OpenSS7-0_9_2_D_rc2(0.9.2.8) 2005/12/08 00:59:58";

#define _XOPEN_SOURCE 600
#define _REENTRANT
#define _THREAD_SAFE

#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stropts.h>

/**
 * @ingroup libLiS
 * @brief test a stream.
 * @param fd a file descriptor to test.
 *
 * isastream() cannot contain a thread cancellation point (SUS/XOPEN/POSIX).
 * Because isastream() consists of a single system call, asynchronous thread
 * cancellation protection is not required.
 */
int
isastream(int fd)
{
	if (ioctl(fd, I_ISASTREAM) == -1) {
		if (errno == EBADF)
			return (-1);
		return (0);
	}
	return (1);
}
