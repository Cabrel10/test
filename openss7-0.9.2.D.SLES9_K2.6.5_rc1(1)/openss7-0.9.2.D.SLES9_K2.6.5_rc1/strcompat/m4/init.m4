# vim: ft=config sw=4 noet nocin nosi com=b\:#,b\:dnl,b\:***,b\:@%\:@ fo+=tcqlorn
# =============================================================================
# BEGINNING OF SEPARATE COPYRIGHT MATERIAL
# =============================================================================
# 
# @(#) init.m4,v OpenSS7-0_9_2_D_rc2(0.9.2.13) 2006/03/14 09:04:10
#
# -----------------------------------------------------------------------------
#
# Copyright (c) 2001-2006  OpenSS7 Corporation <http://www.openss7.com/>
# Copyright (c) 1997-2000  Brian F. G. Bidulock <bidulock@openss7.org>
#
# All Rights Reserved.
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc., 675 Mass
# Ave, Cambridge, MA 02139, USA.
#
# -----------------------------------------------------------------------------
#
# U.S. GOVERNMENT RESTRICTED RIGHTS.  If you are licensing this Software on
# behalf of the U.S. Government ("Government"), the following provisions apply
# to you.  If the Software is supplied by the Department of Defense ("DoD"), it
# is classified as "Commercial Computer Software" under paragraph 252.227-7014
# of the DoD Supplement to the Federal Acquisition Regulations ("DFARS") (or any
# successor regulations) and the Government is acquiring only the license rights
# granted herein (the license rights customarily provided to non-Government
# users).  If the Software is supplied to any unit or agency of the Government
# other than DoD, it is classified as "Restricted Computer Software" and the
# Government's rights in the Software are defined in paragraph 52.227-19 of the
# Federal Acquisition Regulations ("FAR") (or any successor regulations) or, in
# the cases of NASA, in paragraph 18.52.227-86 of the NASA Supplement to the FAR
# (or any successor regulations).
#
# -----------------------------------------------------------------------------
#
# Commercial licensing and support of this software is available from OpenSS7
# Corporation at a fee.  See http://www.openss7.com/
#
# -----------------------------------------------------------------------------
#
# Last Modified 2006/03/14 09:04:10 by brian
#
# =============================================================================

# =============================================================================
# _INIT_SCRIPTS
# -----------------------------------------------------------------------------
AC_DEFUN([_INIT_SCRIPTS], [dnl
    _INIT_SCRIPTS_OPTIONS
    _INIT_SCRIPTS_SETUP
    _INIT_SCRIPTS_OUTPUT
])# _INIT_SCRIPTS
# =============================================================================

# =============================================================================
# _INIT_SCRIPTS_OPTIONS
# -----------------------------------------------------------------------------
AC_DEFUN([_INIT_SCRIPTS_OPTIONS], [dnl
    AC_ARG_ENABLE([initscripts],
	AS_HELP_STRING([--disable-initscripts],
	    [disable installation of init scripts. @<:@default=enabled@:>@]),
	[enable_initscripts="$enableval"],
	[enable_initscripts='yes'])
])# _INIT_SCRIPTS_OPTIONS
# =============================================================================

# =============================================================================
# _INIT_SCRIPTS_SETUP
# -----------------------------------------------------------------------------
AC_DEFUN([_INIT_SCRIPTS_SETUP], [dnl
dnl
dnl first find the inittab: we can always figure out the initial
dnl init script from the inittab
dnl
    AC_CACHE_CHECK([for init SysV inittab], [init_cv_inittab], [dnl
	init_cv_inittab='no'
	eval "init_search_path=\"
	    ${DESTDIR}${sysconfdir}/inittab\""
	init_search_path=`echo "$init_search_path" | sed -e 's|\<NONE\>||g;s|//|/|g'`
	for init_tmp in $init_search_path ; do
	    if test -f $init_tmp ; then
		init_cv_inittab="$init_tmp"
		break
	    fi
	done
    ])
dnl 
dnl next check for the initial init script from inittab
dnl 
    AC_CACHE_CHECK([for init SysV script], [init_cv_script], [dnl
	init_cv_script='no'
	if test :"$init_cv_inittab" != :no ; then
	    init_tmp="$(< $init_cv_inittab | grep -c1 '^si::sysinit:' | sed -e 's|^si::sysinit:||;s|[[[:space:]]].*||')"
	    init_tmp=`echo "${DESTDIR}${rootdir}$init_tmp" | sed -e 's|\<NONE\>||g;s|//|/|g'`
	    if test -f "$init_tmp" ; then
		init_cv_script="$init_tmp"
	    fi
	fi
dnl
dnl     fallback is to go looking for it in the usual places
dnl
	if test :"${init_cv_script:-no}" = :no ; then
	    AC_MSG_RESULT([searching...])
	    eval "init_search_path=\"
		${DESTDIR}${sysconfdir}/init.d/rcS
		${DESTDIR}${sysconfdir}/rc.d/rc.sysinit\""
	    init_search_path=`echo "$init_search_path" | sed -e 's|\<NONE\>||g;s|//|/|g'`
	    for init_tmp in $init_search_path ; do
		AC_MSG_CHECKING([for init SysV script... $init_tmp])
		if test -f $init_tmp ; then
		    init_cv_script="$init_tmp"
		    AC_MSG_RESULT([yes])
		    break
		fi
		AC_MSG_RESULT([no])
	    done
	    AC_MSG_CHECKING([for init SysV script])
	fi
    ])
    AC_CACHE_CHECK([for init SysV rcS.d directory], [init_cv_rcs_dir], [dnl
	AC_MSG_RESULT([searching...])
	init_cv_rcs_dir='no'
	eval "init_search_path=\"
	    ${DESTDIR}${sysconfdir}/rcS.d
	    ${sysconfdir}/rcS.d
	    /etc/rcS.d\""
	init_search_path=`echo "$init_search_path" | sed -e 's|\<NONE\>||g;s|//|/|g'`
	for init_tmp in $init_search_path ; do
	    AC_MSG_CHECKING([for init SysV rcS.d directory... $init_tmp])
	    if test -d $init_tmp ; then
		init_cv_rcs_dir="$init_tmp"
		AC_MSG_RESULT([yes])
		break
	    fi
	    AC_MSG_RESULT([no])
	done
	AC_MSG_CHECKING([for init SysV rcS.d directory])
    ])
    AC_CACHE_CHECK([for init SysV rc.d directory], [init_cv_rc_dir], [dnl
	init_cv_rc_dir='no'
	eval "init_search_path=\"
	    ${DESTDIR}${sysconfdir}/rc.d\""
	init_search_path=`echo "$init_search_path" | sed -e 's|\<NONE\>||g;s|//|/|g'`
	for init_tmp in $init_search_path ; do
	    if test -d $init_tmp ; then
		init_cv_rc_dir="$init_tmp"
		break
	    fi
	done
    ])
dnl
dnl This script needs to exist to add to /etc/modules successfully
dnl
    AC_CACHE_CHECK([for init SysV rc.modules script], [init_cv_rc_modules], [dnl
	init_cv_rc_modules='no'
	eval "init_search_path=\"
	    ${DESTDIR}${sysconfdir}/rc.d/rc.modules\""
	init_search_path=`echo "$init_search_path" | sed -e 's|\<NONE\>||g;s|//|/|g'`
	for init_tmp in $init_search_path ; do
	    if test -f $init_tmp ; then
		init_cv_rc_modules="$init_tmp"
		break
	    fi
	done
    ])
dnl
dnl This is where we are going to add preloaded modules
dnl
    AC_CACHE_CHECK([for init SysV modules file], [init_cv_modules], [dnl
	init_cv_modules='no'
	eval "init_search_path=\"
	    ${DESTDIR}${sysconfdir}/modules\""
	init_search_path=`echo "$init_search_path" | sed -e 's|\<NONE\>||g;s|//|/|g'`
	for init_tmp in $init_search_path ; do
	    if test -f $init_tmp ; then
		init_cv_modules="$init_tmp"
		break
	    fi
	done
    ])
dnl
dnl This is where we are going to have to generate symbolic links if chkconfig
dnl does not exist
dnl
    AC_CACHE_CHECK([for init SysV rcX.d directory], [init_cv_rcx_dir], [dnl
	AC_MSG_RESULT([searching...])
	init_cv_rcx_dir='no'
	eval "init_search_path=\"
	    ${DESTDIR}${sysconfdir}/rc.d/rc[[S0-6]].d
	    ${DESTDIR}${sysconfdir}/rc[[S0-6]].d\""
	init_search_path=`echo "$init_search_path" | sed -e 's|\<NONE\>||g;s|//|/|g'`
	for init_tmp in $init_search_path ; do
	    AC_MSG_CHECKING([for init SysV rcX.d directory... $init_tmp])
	    if test -d $init_tmp ; then
		init_cv_rcx_dir="$init_tmp"
		AC_MSG_RESULT([yes])
		break
	    fi
	    AC_MSG_RESULT([no])
	done
	AC_MSG_CHECKING([for init SysV rcX.d directory])
    ])
dnl
dnl I suppose we really don't care about these.
dnl
    AC_ARG_VAR([CHKCONFIG], [Chkconfig command])
    AC_PATH_TOOL([CHKCONFIG], [chkconfig], [], [$PATH:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin])
    if test "${CHKCONFIG:-no}" = :no ; then
	AC_MSG_WARN([Could not find chkconfig program in PATH.])
	CHKCONFIG=''
    fi
    AC_ARG_VAR([INSSERV], [Insserv command])
    AC_PATH_TOOL([INSSERV], [insserv], [], [$PATH:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin])
    if test "${INSSERV:-no}" = :no ; then
	AC_MSG_WARN([Could not find insserv program in PATH.])
	INSSERV=''
    fi
dnl
dnl initrddir is where we are going to put init scripts
dnl
    AC_CACHE_CHECK([for init SysV init.d directory], [init_cv_initrddir], [dnl
	AC_MSG_RESULT([searching...])
	init_cv_initrddir='no'
	eval "init_search_path=\"
	    ${DESTDIR}${sysconfdir}/rc.d/init.d
	    ${DESTDIR}${sysconfdir}/init.d\""
	for init_tmp in $init_search_path ; do
	    eval "init_dir=\"$init_tmp\""
	    init_dir=`echo "$init_dir" | sed -e 's|\<NONE\>||g;s|//|/|g'`
	    AC_MSG_CHECKING([for init SysV init.d directory... $init_dir])
	    if test -d $init_dir -a ! -L $init_dir ; then
		init_cv_initrddir="$init_tmp"
		AC_MSG_RESULT([yes])
		break
	    fi
	    AC_MSG_RESULT([no])
	done
	AC_MSG_CHECKING([for init SysV init.d directory])
    ])
dnl
dnl configdir is where we are going to put init script default files
dnl
    AC_CACHE_CHECK([for init SysV config directory], [init_cv_configdir], [dnl
	AC_MSG_RESULT([searching...])
	init_cv_configdir='no'
	eval "init_search_path=\"
	    ${DESTDIR}${sysconfdir}/sysconfig
	    ${DESTDIR}${sysconfdir}/default\""
	for init_tmp in $init_search_path ; do
	    eval "init_dir=\"$init_tmp\""
	    init_dir=`echo "$init_dir" | sed -e 's|\<NONE\>||g;s|//|/|g'`
	    AC_MSG_CHECKING([for init SysV config directory... $init_dir])
	    if test -d $init_dir ; then
		init_cv_configdir="$init_tmp"
		AC_MSG_RESULT([yes])
		break
	    fi
	    AC_MSG_RESULT([no])
	done
	AC_MSG_CHECKING([for init SysV config directory])
    ])
    AC_CACHE_CHECK([for init SysV installation], [init_cv_install], [dnl
	init_cv_install='yes'
	test :"${enable_initscripts:-yes}" = :no && init_cv_install='no'
    ])
])# _INIT_SCRIPTS_SETUP
# =============================================================================

# =============================================================================
# _INIT_SCRIPTS_OUTPUT
# -----------------------------------------------------------------------------
AC_DEFUN([_INIT_SCRIPTS_OUTPUT], [dnl
    AM_CONDITIONAL([INSTALL_INITSCRIPTS], [test :"${init_cv_install:-yes}" = :yes])dnl
    AM_CONDITIONAL([WITH_RCSD_DIRECTORY], [test :${init_cv_rcs_dir:-no} != :no])dnl
dnl
dnl initrddir is where we are going to put init scripts relative to DESTDIR
dnl
    if test :"${init_cv_initrddir:-no}" != :no ; then
	init_tmp=`echo "${DESTDIR}" | sed -e 's|\<NONE\>||g;s|//|/|g'`
	initrddir="${init_cv_initrddir#$init_tmp}"
    else
	if test :${init_cv_rcs_dir:-no} = :no ; then
dnl         redhat style
	    initrddir='${sysconfdir}/rc.d/init.d'
	else
dnl         debian style
	    initrddir='${sysconfdir}/init.d'
	fi
    fi
    AC_SUBST([initrddir])
dnl
dnl configdir is where we are going to put init script default files relative to DESTDIR
dnl
    if test :"${init_cv_configdir:-no}" != :no ; then
	init_tmp=`echo "${DESTDIR}" | sed -e 's|\<NONE\>||g;s|//|/|g'`
	configdir="${init_cv_configdir#$init_tmp}"
    else
	if test :${init_cv_rcs_dir:-no} = :no ; then
dnl         redhat style
	    configdir='${sysconfdir}/sysconfig'
	else
dnl         debian style
	    configdir='${sysconfdir}/default'
	fi
    fi
    AC_SUBST([configdir])
])# _INIT_SCRIPTS_OUTPUT
# =============================================================================

# =============================================================================
# _INIT_
# -----------------------------------------------------------------------------
AC_DEFUN([_INIT_], [dnl
])# _INIT_
# =============================================================================

# =============================================================================
# 
# Copyright (c) 2001-2006  OpenSS7 Corporation <http://www.openss7.com/>
# Copyright (c) 1997-2000  Brian F. G. Bidulock <bidulock@openss7.org>
# 
# =============================================================================
# ENDING OF SEPARATE COPYRIGHT MATERIAL
# =============================================================================
# vim: ft=config sw=4 noet nocin nosi com=b\:#,b\:dnl,b\:***,b\:@%\:@ fo+=tcqlorn
