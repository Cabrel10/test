# vim: ft=config sw=4 noet nocin nosi com=b\:#,b\:dnl,b\:***,b\:@%\:@ fo+=tcqlorn
# =============================================================================
# BEGINNING OF SEPARATE COPYRIGHT MATERIAL
# =============================================================================
# 
# @(#) openss7.m4,v OpenSS7-0_9_2_D_rc2(0.9.2.32) 2006/03/25 12:55:08
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
# Last Modified 2006/03/25 12:55:08 by brian
#
# -----------------------------------------------------------------------------
#
# openss7.m4,v
# Revision 0.9.2.32  2006/03/25 12:55:08  brian
# - got enable_static backwards
#
# Revision 0.9.2.31  2006/03/20 12:12:18  brian
# - don't build libtool static libraries no devel
#
# Revision 0.9.2.30  2006/03/20 11:51:09  brian
# - added check for --disable-devel
#
# Revision 0.9.2.29  2006/03/11 09:49:51  brian
# - a bit better checking
#
# =============================================================================

# =============================================================================
# _OPENSS7_PACKAGE([SHORT-TITLE], [LONG-TITLE])
# -----------------------------------------------------------------------------
AC_DEFUN([_OPENSS7_PACKAGE], [dnl
    PACKAGE_TITLE='$2'
    AC_SUBST([PACKAGE_TITLE])dnl
    m4_ifndef([AC_PACKAGE_TITLE], [m4_define([AC_PACKAGE_TITLE], [$2])])
    PACKAGE_SHORTTITLE='$1'
    AC_SUBST([PACKAGE_SHORTTITLE])dnl
    m4_ifndef([AC_PACKAGE_SHORTTITLE], [m4_define([AC_PACKAGE_SHORTTITLE], [$1])])
    upper='ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    lower='abcdefghijklmnopqrstuvwxyz'
    PACKAGE_LCNAME=`echo "$PACKAGE_NAME" | sed -e y/$upper/$lower/`
    AC_SUBST([PACKAGE_LCNAME])dnl
    m4_ifndef([AC_PACKAGE_LCNAME], [m4_define([AC_PACKAGE_LCNAME], [m4_tolower(AC_PACKAGE_NAME)])])
    PACKAGE_UCNAME=`echo "$PACKAGE_NAME" | sed -e y/$lower/$upper/`
    AC_SUBST([PACKAGE_UCNAME])dnl
    m4_ifndef([AC_PACKAGE_UCNAME], [m4_define([AC_PACKAGE_UCNAME], [m4_toupper(AC_PACKAGE_NAME)])])
    unset upper lower
    PACKAGE_DATE=`date -I`
    AC_SUBST([PACKAGE_DATE])dnl
    AC_DEFINE_UNQUOTED([PACKAGE_DATE], ["$PACKAGE_DATE"], [The package release date.])
    PKGINCL="include/sys/${PACKAGE_NAME}"
    AC_SUBST([PKGINCL])dnl
    _OPENSS7_OPTIONS
    _OPENSS7_CACHE
    _OPENSS7_DEBUG
    AC_SUBST([cross_compiling])dnl
# =============================================================================
# 
# Copyright (c) 2001-2006  OpenSS7 Corporation <http://www.openss7.com/>
# Copyright (c) 1997-2000  Brian F. G. Bidulock <bidulock@openss7.org>
# 
# =============================================================================
# ENDING OF SEPARATE COPYRIGHT MATERIAL
# =============================================================================
])# _OPENSS7_PACKAGE
# =============================================================================

# =============================================================================
# _OPENSS7_DIRCHANGE
# -----------------------------------------------------------------------------
AC_DEFUN([_OPENSS7_DIRCHANGE], [dnl
    ac_default_prefix='/usr'
dnl
dnl Find the real root of the install tree
dnl
    if test :"$prefix" = :NONE ; then
	newprefix="$ac_default_prefix"
    else
	newprefix="$prefix"
    fi
    rootdir=`echo $newprefix | sed -e 's|/local$||;s|/usr$||'`
dnl
dnl Need to adjust directories if default
dnl
    if test :"$sysconfdir" = :'${prefix}/etc' ; then sysconfdir='${rootdir}/etc' ; fi
    if test :"$localstatedir" = :'${prefix}/var' ; then localstatdir='${rootdir}/var' ; fi
    if test :"${newprefix#$rootdir}" = : ; then
	if test :"$infodir" = :'${prefix}/info' ; then infodir='${prefix}/usr/share/info' ; fi
	if test :"$mandir" = :'${prefix}/man' ; then mandir='${prefix}/usr/share/man' ; fi
    fi
    if test :"${newprefix#$rootdir}" = :/usr ; then
	if test :"$infodir" = :'${prefix}/info' ; then infodir='${prefix}/share/info' ; fi
	if test :"$mandir" = :'${prefix}/man' ; then mandir='${prefix}/share/man' ; fi
    fi
    AC_SUBST([rootdir])
dnl
dnl Need to check this before libtool gets done
dnl
    AC_MSG_CHECKING([for development environment])
    AC_ARG_ENABLE([devel],
	AS_HELP_STRING([--disable-devel],
	    [disable development environment.  @<:@default=enabled@:>@]),
	[dnl
	    if test :"${USE_MAINTAINER_MODE:-no}" != :no
	    then
		enable_devel='yes'
	    else
		enable_devel="$enableval"
	    fi
	], [enable_devel='yes'])
    AC_MSG_RESULT([$enable_devel])
    AM_CONDITIONAL([DEVELOPMENT], [test :"${enable_devel:-yes}" = :yes])dnl
dnl
dnl Don't build libtool static libraries if development environment not
dnl specified
dnl
    if test :"${enable_devel:-yes}" != :yes
    then
	enable_static='no'
    fi
])# _OPENSS7_DIRCHANGE
# =============================================================================

# =============================================================================
# _OPENSS7_CACHE
# -----------------------------------------------------------------------------
# A little trick with caches and site files.
# -----------------------------------------------------------------------------
AC_DEFUN([_OPENSS7_CACHE], [dnl
    # force a cache file to be created even if not specified
    if test "$cache_file" = '/dev/null' -o :"$cache_file" = :
    then
	cache_file='config.cache'
	if test ! -e "$cache_file"
	then
	    cat /dev/null > "$cache_file"
	fi
    fi
    # if site file not specified, use local site file
    if test :"$CONFIG_SITE" = :
    then
	CONFIG_SITE='config.site'
    fi
    AC_CONFIG_COMMANDS([siteconfig], [dnl
	if test :"$CONFIG_SITE" != : -a :"$cache_file" != :
	then
	    for config_site in $CONFIG_SITE
	    do
		if test -w "$config_site" -o ! -e "$config_site"
		then
		    if touch "$config_site" >/dev/null 2>&1
		    then
			cat "$cache_file" | egrep -v '\<(ac_cv_env_|ac_cv_host|ac_cv_target|linux_cv_|ksyms_cv_|rpm_cv_|deb_cv_|strconf_cv_|sctp_cv_|xns_cv_|lis_cv_|lfs_cv_|strcomp_cv_|streams_cv_|xti_cv_|xopen_cv_|inet_cv_|xnet_cv_|devfs_cv_|init_cv_|pkg_cv_)' > "$config_site" 2>/dev/null
		    fi
		fi
	    done
	fi], [cache_file="$cache_file" ; CONFIG_SITE="$CONFIG_SITE"])
])# _OPENSS7_CACHE
# =============================================================================

# =============================================================================
# _OPENSS7_DEBUG
# -----------------------------------------------------------------------------
AC_DEFUN([_OPENSS7_DEBUG], [dnl
    if test :"${USE_MAINTAINER_MODE:-no}" != :no
    then
	CFLAGS=`echo "$CFLAGS" | sed -e 's|-Wall||;s|  | |g'`
	CFLAGS=`echo "$CFLAGS" | sed -e 's|-Werror||;s|  | |g'`
	CFLAGS=`echo "$CFLAGS" | sed -e 's|-Wundef||;s|  | |g'`
	CFLAGS=`echo "$CFLAGS" | sed -e 's|-Wp,-D_FORTIFY_SOURCE=[0-9]*||;s|  | |g'`
	CFLAGS=`echo "$CFLAGS" | sed -e 's|^  *||;s|  *$||;s|    | |g;s|   | |g;s|  | |g'`
	CFLAGS="${CFLAGS}${CFLAGS:+ }-Wall -Wstrict-prototypes -Wno-trigraphs -Wundef -Wp,-D_FORTIFY_SOURCE=2 -Werror"
    fi
])# _OPENSS7_DEBUG
# =============================================================================

# =============================================================================
# _OPENSS7_OPTIONS
# -----------------------------------------------------------------------------
AC_DEFUN([_OPENSS7_OPTIONS], [dnl
    _OPENSS7_OPTIONS_CHECK
    _OPENSS7_OPTIONS_GPG
    _OPENSS7_OPTIONS_PKG_EPOCH
    _OPENSS7_OPTIONS_PKG_RELEASE
    _OPENSS7_OPTIONS_PKG_DISTDIR
    _OPENSS7_OPTIONS_PKG_ARCH
    _OPENSS7_OPTIONS_PKG_INDEP
])# _OPENSS7_OPTIONS
# =============================================================================

# =============================================================================
# _OPENSS7_OPTIONS_CHECK
# -----------------------------------------------------------------------------
AC_DEFUN([_OPENSS7_OPTIONS_CHECK], [dnl
    AC_MSG_CHECKING([for preinstall tests on check])
    AC_ARG_ENABLE([checks],
	AS_HELP_STRING([--enable-checks],
	    [enable preinstall checks. @<:@default=auto@:>@]),
	[enable_checks="$enableval"], [dnl
	     if test :"${USE_MAINTAINER_MODE:-no}" != :no
	     then
		 enable_checks='yes'
	     else
		 enable_checks='no'
	     fi])
    AC_MSG_RESULT([$enable_checks])
    AM_CONDITIONAL([PERFORM_CHECKS], [test :"${enable_checks:-yes}" = :yes])dnl
])# _OPENSS7_OPTIONS_CHECK
# =============================================================================

# =============================================================================
# _OPENSS7_OPTIONS_GPG
# -----------------------------------------------------------------------------
AC_DEFUN([_OPENSS7_OPTIONS_GPG], [dnl
    AC_ARG_VAR([GPG], [GPG signature command])
    AC_PATH_TOOL([GPG], [gpg pgp], [], [$PATH:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin])
    if test :"${GPG:-no}" = :no ; then
	AC_MSG_WARN([Could not find gpg program in PATH.])
	GPG=/usr/bin/gpg
    fi
dnl ---------------------------------------------------------
    AC_MSG_CHECKING([for gpg user])
    AC_ARG_VAR([GNUPGUSER], [GPG user name])
    AC_ARG_WITH([gpg-user],
	AS_HELP_STRING([--with-gpg-user=USERNAME],
	    [specify the USER for signing DEBs, RPMs and tarballs.
	    @<:@default=${GNUPGUSER}@:>@]),
	[with_gpg_user="$withval"],
	[with_gpg_user="$GNUPGUSER"
	 if test :"${with_gpg_user:-no}" = :no ; then
	     if test -r .gnupguser; then d= ; else d="$srcdir/" ; fi
	     if test -r ${d}.gnupguser
	     then with_gpg_user="`cat ${d}.gnupguser`"
	     else with_gpg_user=''
	     fi
dnl          if test :"${with_gpg_user:-no}" = :no ; then
dnl              with_gpg_user="`whoami`"
dnl          fi
	 fi])
    GNUPGUSER="${with_gpg_user:-`whoami`}"
    AC_MSG_RESULT([${GNUPGUSER:-no}])
dnl ---------------------------------------------------------
    AC_MSG_CHECKING([for gpg home])
    AC_ARG_VAR([GNUPGHOME], [GPG home directory])
    AC_ARG_WITH([gpg-home],
	AS_HELP_STRING([--with-gpg-home=HOMEDIR],
	    [specify the HOME for signing DEBs, RPMs and tarballs.
	    @<:@default=${GNUPGHOME:-~/.gnupg}@:>@]),
	[with_gpg_home="$withval"],
	[with_gpg_home="$GNUPGHOME"
	 if test :"${with_gpg_home:-no}" = :no ; then
	     if test -r .gnupghome; then d= ; else d="$srcdir/" ; fi
	     if test -r ${d}.gnupghome
	     then with_gpg_home="`cat ${d}.gnupghome`"
	     else with_gpg_home=''
	     fi
dnl          if test :"${with_gpg_home:-no}" = :no ; then
dnl              with_gpg_home='~/.gnupg'
dnl          fi
	 fi])
    GNUPGHOME="${with_gpg_home:-~/.gnupg}"
    AC_MSG_RESULT([${GNUPGHOME:-no}])
])# _OPENSS7_OPTIONS_GPG
# =============================================================================

# =========================================================================
# _OPENSS7_OPTIONS_PKG_EPOCH
# -------------------------------------------------------------------------
AC_DEFUN([_OPENSS7_OPTIONS_PKG_EPOCH], [dnl
    AC_MSG_CHECKING([for pkg epoch])
    AC_ARG_WITH([pkg-epoch],
	AS_HELP_STRING([--with-pkg-epoch=EPOCH],
	    [specify the EPOCH for the package file.  @<:@default=auto@:>@]),
	[with_pkg_epoch="$withval"],
	[if test -r .pkgepoch; then d= ; else d="$srcdir/" ; fi
	 if test -r ${d}.pkgepoch
	 then with_pkg_epoch="`cat ${d}.pkgepoch`"
	 else with_pkg_epoch=1
	 fi])
    AC_MSG_RESULT([${with_pkg_epoch:-1}])
    PACKAGE_EPOCH="${with_pkg_epoch:-1}"
    AC_SUBST([PACKAGE_EPOCH])dnl
    AC_DEFINE_UNQUOTED([PACKAGE_EPOCH], [$PACKAGE_EPOCH], [The Package Epoch.
	This defaults to 1.])
])# _OPENSS7_OPTIONS_PKG_EPOCH
# =========================================================================

# =========================================================================
# _OPENSS7_OPTIONS_PKG_RELEASE
# -------------------------------------------------------------------------
AC_DEFUN([_OPENSS7_OPTIONS_PKG_RELEASE], [dnl
    AC_MSG_CHECKING([for pkg release])
    AC_ARG_WITH([pkg-release],
	AS_HELP_STRING([--with-pkg-release=RELEASE],
	    [specify the RELEASE for the package files.  @<:@default=auto@:>@]),
	[with_pkg_release="$withval"],
	[if test -r .pkgrelease ; then d= ; else d="$srcdir/" ; fi
	 if test -r ${d}.pkgrelease
	 then with_pkg_release="`cat ${d}.pkgrelease`"
	 else with_pkg_release=1
	 fi])
    AC_MSG_RESULT([${with_pkg_release:-1}])
    PACKAGE_RELEASE="${with_pkg_release:-1}"
    AC_SUBST([PACKAGE_RELEASE])dnl
    AC_DEFINE_UNQUOTED([PACKAGE_RELEASE], ["$PACKAGE_RELEASE"], [The Package
	Release. This defaults to Custom.])
])# _OPENSS7_OPTIONS_PKG_RELEASE
# =========================================================================

# =============================================================================
# _OPENSS7_OPTIONS_PKG_DISTDIR
# -----------------------------------------------------------------------------
AC_DEFUN([_OPENSS7_OPTIONS_PKG_DISTDIR], [dnl
    pkg_tmp=`(cd . ; pwd)`
    AC_ARG_WITH([pkg-distdir],
	AS_HELP_STRING([--with-pkg-distdir=DIR],
	    [specify the package distribution directory. @<:@default=.@:>@]),
	[with_pkg_distdir="$withval"],
	[with_pkg_distdir="$pkg_tmp"])
    AC_CACHE_CHECK([for pkg distdir], [pkg_cv_distdir], [dnl
	case :"${with_pkg_distdir:-default}" in
	    (:no|:NO)
		pkg_cv_distdir="$pkg_tmp"
		;;
	    (:yes|:YES|:default|:DEFAULT)
		pkg_cv_distdir="/usr/src"
		;;
	    (*)
		pkg_cv_distdir="$with_pkg_distdir"
		;;
	esac
    ])
    PACKAGE_DISTDIR="$pkg_cv_distdir"
    AC_SUBST([PACKAGE_DISTDIR])dnl
])# _OPENSS7_OPTIONS_PKG_DISTDIR
# =============================================================================

# =============================================================================
# _OPENSS7_OPTIONS_PKG_ARCH
# -----------------------------------------------------------------------------
# Debian (and rpm for that matter) can build architecture dependent or
# architecture independent packages.  This option specifies whether architecture
# dependent packages are to be built and installed.
# -----------------------------------------------------------------------------
AC_DEFUN([_OPENSS7_OPTIONS_PKG_ARCH], [dnl
    AC_MSG_CHECKING([for deb build/install of arch packages])
    AC_ARG_ENABLE([arch],
	AS_HELP_STRING([--enable-arch],
	    [build and install arch packages.  @<:@default=yes@:>@]),
	[enable_arch="$enableval"],
	[enable_arch='yes'])
    AC_MSG_RESULT([${enable_arch:-yes}])
    AM_CONDITIONAL([PKG_BUILD_ARCH], [test :"${enable_arch:-yes}" = :yes])dnl
	
])# _OPENSS7_OPTIONS_PKG_ARCH
# =============================================================================

# =============================================================================
# _OPENSS7_OPTIONS_PKG_INDEP
# -----------------------------------------------------------------------------
# Debian (and rpm for that matter) can build architecture dependent or
# architecture independent packages.  This option specifies whether architecture
# independent packages are to be built and installed.
# -----------------------------------------------------------------------------
AC_DEFUN([_OPENSS7_OPTIONS_PKG_INDEP], [dnl
    AC_MSG_CHECKING([for deb build/install of indep packages])
    AC_ARG_ENABLE([indep],
	AS_HELP_STRING([--enable-indep],
	    [build and install indep packages.  @<:@default=yes@:>@]),
	[enable_indep="$enableval"],
	[enable_indep='yes'])
    AC_MSG_RESULT([${enable_indep:-yes}])
    AM_CONDITIONAL([PKG_BUILD_INDEP], [test :"${enable_indep:-yes}" = :yes])dnl
])# _OPENSS7_OPTIONS_PKG_INDEP
# =============================================================================

# =============================================================================
# _OPENSS7
# -----------------------------------------------------------------------------
AC_DEFUN([_OPENSS7], [dnl
])# _OPENSS7
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
