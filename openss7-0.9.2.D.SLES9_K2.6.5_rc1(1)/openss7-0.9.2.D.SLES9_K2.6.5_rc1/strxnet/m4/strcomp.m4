# vim: ft=config sw=4 noet nocin nosi com=b\:#,b\:dnl,b\:***,b\:@%\:@ fo+=tcqlorn
# =============================================================================
# BEGINNING OF SEPARATE COPYRIGHT MATERIAL
# =============================================================================
# 
# @(#) strcomp.m4,v OpenSS7-0_9_2_D_rc2(0.9.2.14) 2006/03/14 09:20:47
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
# Last Modified 2006/03/14 09:20:47 by brian
#
# -----------------------------------------------------------------------------
#
# strcomp.m4,v
# Revision 0.9.2.14  2006/03/14 09:20:47  brian
# - typo
#
# Revision 0.9.2.13  2006/03/14 09:04:11  brian
# - syntax consistency, advanced search
#
# =============================================================================

# -----------------------------------------------------------------------------
# This file provides some common macros for finding a STREAMS compatiblityu
# release and necessary include directories and other configuration for
# compiling kernel modules to run with the STREAMS compatibility package.
# -----------------------------------------------------------------------------
# Interesting enough, there is no need to have strcompat loaded on the build
# machine to compile modules.  Only the proper header files are required.
# -----------------------------------------------------------------------------

# =============================================================================
# _STRCOMP
# -----------------------------------------------------------------------------
# Check for the existence of COMPAT header file, particularly sys/os7/compat.h.
# COMPAT header files are required for building the ABI interface for STREAMS.
# Without COMPAT header files, the ABI interface for STREAMS will not be built.
# -----------------------------------------------------------------------------
AC_DEFUN([_STRCOMP], [dnl
    AC_REQUIRE([_LINUX_STREAMS])
    _STRCOMP_OPTIONS
    _STRCOMP_SETUP
dnl
dnl Skip kernel checks if not configuring for the kernel (i.e. no _LINUX_KERNEL)
dnl as we do for netperf.
dnl
    m4_ifdef([_LINUX_KERNEL], [_STRCOMP_KERNEL])
    _STRCOMP_USER
    _STRCOMP_OUTPUT
    AC_SUBST([STRCOMP_CPPFLAGS])dnl
    AC_SUBST([STRCOMP_MODFLAGS])dnl
    AC_SUBST([STRCOMP_LDADD])dnl
    AC_SUBST([STRCOMP_MODMAP])dnl
    AC_SUBST([STRCOMP_SYMVER])dnl
    AC_SUBST([STRCOMP_MANPATH])dnl
    AC_SUBST([STRCOMP_VERSION])dnl
])# _STRCOMP
# =============================================================================

# =============================================================================
# _STRCOMP_OPTIONS
# -----------------------------------------------------------------------------
# allow the user to specify the header file location
# -----------------------------------------------------------------------------
AC_DEFUN([_STRCOMP_OPTIONS], [dnl
    AC_ARG_WITH([compat],
		AS_HELP_STRING([--with-compat=HEADERS],
			       [specify the STREAMS compatibility header file directory.
				@<:@default=INCLUDEDIR/strcompat@:>@]),
		[with_compat="$withval"],
		[with_compat=''])
])# _STRCOMP_OPTIONS
# =============================================================================

# =============================================================================
# _STRCOMP_SETUP
# -----------------------------------------------------------------------------
AC_DEFUN([_STRCOMP_SETUP], [dnl
    _STRCOMP_CHECK_HEADERS
    for strcomp_include in $strcomp_cv_includes ; do
	STRCOMP_CPPFLAGS="${STRCOMP_CPPFLAGS}${STRCOMP_CPPFLAGS:+ }-I${strcomp_include}"
    done
    if test :"${strcomp_cv_config:-no}" != :no ; then
	STRCOMP_CPPFLAGS="${STRCOMP_CPPFLAGS}${STRCOMP_CPPFLAGS:+ }-include ${strcomp_cv_config}"
    fi
    if test :"${strcomp_cv_modversions:-no}" != :no ; then
	STRCOMP_MODFLAGS="${STRCOMP_MODFLAGS}${STRCOMP_MODFLAGS:+ }-include ${strcomp_cv_modversions}"
    fi
])# _STRCOMP_SETUP
# =============================================================================

# =============================================================================
# _STRCOMP_CHECK_HEADERS
# -----------------------------------------------------------------------------
AC_DEFUN([_STRCOMP_CHECK_HEADERS], [dnl
    # Test for the existence of Linux STREAMS Compatibility header files.  The
    # package normally requires compatibility header files to compile.
    AC_CACHE_CHECK([for compat include directory], [strcomp_cv_includes], [dnl
	strcomp_what="sys/os7/compat.h"
	if test :"${with_compat:-no}" != :no -a :"${with_compat:-no}" != :yes ; then
	    # First thing to do is to take user specified director(ies)
	    AC_MSG_RESULT([(searching $with_compat)])
	    for strcomp_dir in $with_compat ; do
		if test -d "$strcomp_dir" ; then
		    AC_MSG_CHECKING([for compat include directory... $strcomp_dir])
		    if test -r "$strcomp_dir/$strcomp_what" ; then
			strcomp_cv_cinludes="$with_compat"
			AC_MSG_RESULT([yes])
			break
		    fi
		    AC_MSG_RESULT([no])
		fi
	    done
	    if test :"${strcomp_cv_includes:-no}" = :no ; then
		AC_MSG_WARN([
*** 
*** You have specified include directories using:
***
***	    --with-compat="$with_compat"
***
*** however, $strcomp_what does not exist in any of the specified
*** directories.  Configure will continue to search other known
*** directories.
*** ])
	    fi
	    AC_MSG_CHECKING([for compat include directory])
	fi
	if test :"${strcomp_cv_includes:-no}" = :no ; then
	    # The next place to look is under the master source and build
	    # directory, if any.
	    AC_MSG_RESULT([(searching $master_srcdir $master_builddir)])
	    strcomp_search_path="
		${master_srcdir:+$master_srcdir/strcompat/src/include}
		${master_builddir:+$master_builddir/strcompat/src/include}"
	    for strcomp_dir in $strcomp_search_path ; do
		if test -d "$strcomp_dir" ; then
		    AC_MSG_CHECKING([for compat include directory... $strcomp_dir])
		    if test -r "$strcomp_dir/$strcomp_what" ; then
			strcomp_cv_includes="$strcomp_search_path"
			strcomp_cv_ldadd= # "$master_builddir/strcompat/libcompat.la"
			strcomp_cv_modmap="$master_builddir/strcompat/Modules.map"
			strcomp_cv_symver="$master_builddir/strcompat/Module.symvers"
			strcomp_cv_manpath="$master_builddir/strcompat/doc/man"
			AC_MSG_RESULT([yes])
			break
		    fi
		    AC_MSG_RESULT([no])
		fi
	    done
	    AC_MSG_CHECKING([for compat include directory])
	fi
	if test :"${strcomp_cv_includes:-no}" = :no ; then
	    # The next place to look now is for a peer package being built under
	    # the same top directory, and then the higher level directory.
	    strcomp_here=`pwd`
	    AC_MSG_RESULT([(searching from $strcomp_here)])
	    for strcomp_dir in \
		$srcdir/strcompat*/src/include \
		$srcdir/../strcompat*/src/include \
		../_build/$srcdir/../../strcompat*/src/include \
		../_build/$srcdir/../../../strcompat*/src/include
	    do
		if test -d "$strcomp_dir" ; then
		    strcomp_bld=`echo $strcomp_dir | sed -e "s|^$srcdir/|$strcomp_here/|;"'s|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
		    strcomp_dir=`(cd $strcomp_dir; pwd)`
		    AC_MSG_CHECKING([for compat include directory... $strcomp_dir])
		    if test -r "$strcomp_dir/$strcomp_what" ; then
			strcomp_cv_includes="$strcomp_dir $strcomp_bld"
			strcomp_cv_ldadd= # `echo "$strcomp_bld/../../libcompat.la" | sed -e 's|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
			strcomp_cv_modmap=`echo "$strcomp_bld/../../Modules.map" | sed -e 's|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
			strcomp_cv_symver=`echo "$strcomp_bld/../../Module.symvers" |sed -e 's|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
			strcomp_cv_manpath=`echo "$strcomp_bld/../../doc/man" |sed -e 's|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
			AC_MSG_RESULT([yes])
			break
		    fi
		    AC_MSG_RESULT([no])
		fi
	    done
	    AC_MSG_CHECKING([for compat include directory])
	fi
	if test :"${strcomp_cv_includes:-no}" = :no ; then
	    # Compat header files are normally found in the strcompat package now.
	    # They used to be part of the compatibility add-on package and even older
	    # versions are part of the LfS release packages.
	    case "$strcomp_cv_package" in
		(LiS)
		    eval "strcomp_search_path=\"
			${DESTDIR}${includedir}/strcompat
			${DESTDIR}${rootdir}${oldincludedir}/strcompat
			${DESTDIR}${rootdir}/usr/include/strcompat
			${DESTDIR}${rootdir}/usr/local/include/strcompat
			${DESTDIR}${rootdir}/usr/src/strcompat/src/include
			${DESTDIR}${includedir}/LiS
			${DESTDIR}${rootdir}${oldincludedir}/LiS
			${DESTDIR}${rootdir}/usr/include/LiS
			${DESTDIR}${rootdir}/usr/local/include/LiS
			${DESTDIR}${rootdir}/usr/src/LiS/include
			${DESTDIR}${oldincludedir}/strcompat
			${DESTDIR}/usr/include/strcompat
			${DESTDIR}/usr/local/include/strcompat
			${DESTDIR}/usr/src/strcompat/src/include
			${DESTDIR}${oldincludedir}/LiS
			${DESTDIR}/usr/include/LiS
			${DESTDIR}/usr/local/include/LiS
			${DESTDIR}/usr/src/LiS/include\""
		    ;;
		(LfS)
		    eval "strcomp_search_path=\"
			${DESTDIR}${includedir}/strcompat
			${DESTDIR}${rootdir}${oldincludedir}/strcompat
			${DESTDIR}${rootdir}/usr/include/strcompat
			${DESTDIR}${rootdir}/usr/local/include/strcompat
			${DESTDIR}${rootdir}/usr/src/strcompat/src/include
			${DESTDIR}${includedir}/streams
			${DESTDIR}${rootdir}${oldincludedir}/streams
			${DESTDIR}${rootdir}/usr/include/streams
			${DESTDIR}${rootdir}/usr/local/include/streams
			${DESTDIR}${rootdir}/usr/src/streams/include
			${DESTDIR}${oldincludedir}/strcompat
			${DESTDIR}/usr/include/strcompat
			${DESTDIR}/usr/local/include/strcompat
			${DESTDIR}/usr/src/strcompat/src/include
			${DESTDIR}${oldincludedir}/streams
			${DESTDIR}/usr/include/streams
			${DESTDIR}/usr/local/include/streams
			${DESTDIR}/usr/src/streams/include\""
		    ;;
	    esac
	    strcomp_search_path=`echo "$strcomp_search_path" | sed -e 's|\<NONE\>||g;s|//|/|g'`
	    strcomp_cv_includes=
	    AC_MSG_RESULT([(searching)])
	    for strcomp_dir in $strcomp_search_path ; do
		if test -d "$strcomp_dir" ; then
		    AC_MSG_CHECKING([for compat include directory... $strcomp_dir])
		    if test -r "$strcomp_dir/$strcomp_what" ; then
			strcomp_cv_includes="$strcomp_dir"
			strcomp_cv_ldadd= # '-lcompat'
			strcomp_cv_modmap=
			strcomp_cv_symver=
			strcomp_cv_manpath=
			AC_MSG_RESULT([yes])
			break
		    fi
		    AC_MSG_RESULT([no])
		fi
	    done
	    AC_MSG_CHECKING([for compat include directory])
	fi
    ])
    AC_CACHE_CHECK([for compat ldadd],[strcomp_cv_ldadd],[dnl
	for strcomp_dir in $strcomp_cv_includes ; do
	    if test -f "$strcomp_dir/../../libcompat.la" ; then
		strcomp_cv_ldadd=`echo "$strcomp_dir/../../libcompat.la" | sed -e 's|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
		break
	    fi
	done
	if test -z "$strcomp_cv_ldadd" ; then
	    strcomp_cv_ldadd= # '-lcompat'
	fi
    ])
    AC_CACHE_CHECK([for compat modmap],[strcomp_cv_modmap],[dnl
	for strcomp_dir in $strcomp_cv_includes ; do
	    if test -f $strcomp_dir/../../Modules.map ; then
		strcomp_cv_modmap=`echo "$strcomp_dir/../../Modules.map" | sed -e 's|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
		break
	    fi
	done
    ])
    AC_CACHE_CHECK([for compat symver],[strcomp_cv_symver],[dnl
	for strcomp_dir in $strcomp_cv_includes ; do
	    if test -f $strcomp_dir/../../Module.symvers ; then
		strcomp_cv_symver=`echo "$strcomp_dir/../../Module.symvers" | sed -e 's|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
		break
	    fi
	done
    ])
    AC_CACHE_CHECK([for compat manpath],[strcomp_cv_manpath],[dnl
	for strcomp_dir in $strcomp_cv_includes ; do
	    if test -d $strcomp_dir/../../doc/man; then
		strcomp_cv_manpath=`echo "$strcomp_dir/../../doc/man" | sed -e 's|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
		break
	    fi
	done
    ])
    if test :"${strcomp_cv_includes:-no}" = :no ; then
	AC_MSG_ERROR([
*** 
*** Configure could not find the STREAMS compat include directories.  If
*** you wish to use the STREAMS compat package you will need to specify
*** the location of the STREAMS compat (strcompat) include directories with
*** the --with-compat=@<:@DIRECTORY@<:@ DIRECTORY@:>@@:>@ option to
*** ./configure and try again.
***
*** Perhaps you just forgot to load the STREAMS compat package?  The
*** STREAMS strcompat package is available from The OpenSS7 Project
*** download page at http://www.openss7.org/ and comes in a tarball
*** named something like "strcompat-0.9.2.3.tar.gz".
*** ])
    fi
    AC_CACHE_CHECK([for strcompat version], [strcomp_cv_version], [dnl
	strcomp_what="sys/strcompat/version.h"
	strcomp_file=
	if test -n "$strcomp_cv_includes" ; then
	    for strcomp_dir in $strcomp_cv_includes ; do
		# old place for version
		if test -f "$strcomp_dir/$strcomp_what" ; then
		    strcomp_file="$strcomp_dir/$strcomp_what"
		    break
		fi
		# new place for version
		if test -n "$linux_cv_k_release" ; then
dnl		    if linux_cv_k_release is not defined (no _LINUX_KERNEL) then
dnl		    this will just not be set
		    if test -f "$strcomp_dir/$linux_cv_k_release/$target_cpu/$strcomp_what" ; then
			strcomp_file="$strcomp_dir/$linux_cv_k_release/$target_cpu/$strcomp_what"
			break
		    fi
		fi
	    done
	fi
	if test :${strcomp_file:-no} != :no ; then
	    strcomp_cv_version=`grep '#define.*\<STRCOMPAT_VERSION\>' $strcomp_file 2>/dev/null | sed -e 's|^[^"]*"||;s|".*$||'`
	fi
    ])
    strcomp_what="sys/config.h"
    AC_CACHE_CHECK([for strcompat $strcomp_what], [strcomp_cv_config], [dnl
	strcomp_cv_config=
	if test -n "$strcomp_cv_includes" ; then
	    for strcomp_dir in $strcomp_cv_includes ; do
		# old place for config
		if test -f "$strcomp_dir/$strcomp_what" ; then
		    strcomp_cv_config="$strcomp_dir/$strcomp_what"
		    break
		fi
		# new place for config
		if test -n "$linux_cv_k_release" ; then
dnl		    if linux_cv_k_release is not defined (no _LINUX_KERNEL) then
dnl		    this will just not be set
		    if test -f "$strcomp_dir/$linux_cv_k_release/$target_cpu/$strcomp_what" ; then
			strcomp_cv_config="$strcomp_dir/$linux_cv_k_release/$target_cpu/$strcomp_what"
			break
		    fi
		fi
	    done
	fi
    ])
    strcomp_what="sys/strcompat/modversions.h"
    AC_CACHE_CHECK([for strcompat $strcomp_what], [strcomp_cv_modversions], [dnl
	strcomp_cv_modversions=
dnl	if linux_cv_k_ko_modules is not defined (no _LINUX_KERNEL) then we
dnl	assume normal objects
	if test :"${linux_cv_k_ko_modules:-no}" = :no ; then
	    if test -n "$strcomp_cv_includes" ; then
		for strcomp_dir in $strcomp_cv_includes ; do
		    # old place for modversions
		    if test -f "$strcomp_dir/$strcomp_what" ; then
			strcomp_cv_modversions="$strcomp_dir/$strcomp_what"
			break
		    fi
		    # new place for modversions
		    if test -n "$linux_cv_k_release" ; then
dnl			if linux_cv_k_release is not defined (no _LINUX_KERNEL)
dnl			then this will just not be set
			if test -f "$strcomp_dir/$linux_cv_k_release/$target_cpu/$strcomp_what" ; then
			    strcomp_cv_includes="$strcomp_dir/$linux_cv_k_release/$target_cpu $strcomp_cv_includes"
			    strcomp_cv_modversions="$strcomp_dir/$linux_cv_k_release/$target_cpu/$strcomp_what"
			    break
			fi
		    fi
		done
	    fi
	fi
    ])
    AC_MSG_CHECKING([for compat added configure arguments])
dnl Older rpms (particularly those used by SuSE) are too stupid to handle --with
dnl and --without rpmopt syntax, so convert to the equivalent --define syntax.
dnl Also, I don't know that even rpm 4.2 handles --with xxx=yyy properly, so we
dnl use defines.
    if test -z "$with_compat" ; then
	if test :"${strcomp_cv_includes:-no}" = :no ; then :
	    PACKAGE_RPMOPTIONS="${PACKAGE_RPMOPTIONS}${PACKAGE_RPMOPTIONS:+ }--define \"_with_compat --with-compat\""
	    PACKAGE_DEBOPTIONS="${PACKAGE_DEBOPTIONS}${PACKAGE_DEBOPTIONS:+ }'--with-compat'"
	    AC_MSG_RESULT([--with-compat])
	else
	    PACKAGE_RPMOPTIONS="${PACKAGE_RPMOPTIONS}${PACKAGE_RPMOPTIONS:+ }--define \"_without_compat --without-compat\""
	    PACKAGE_DEBOPTIONS="${PACKAGE_DEBOPTIONS}${PACKAGE_DEBOPTIONS:+ }'--without-compat'"
	    AC_MSG_RESULT([--without-compat])
	fi
    else
	AC_MSG_RESULT([--with-compat="$with_compat"])
    fi
])# _STRCOMP_CHECK_HEADERS
# =============================================================================

# =============================================================================
# _STRCOMP_KERNEL
# -----------------------------------------------------------------------------
# Need to know about irqreturn_t for sys/os7/compat.h STREAMS compatibility file.
# -----------------------------------------------------------------------------
AC_DEFUN([_STRCOMP_KERNEL], [dnl
    _LINUX_CHECK_TYPES([irqreturn_t], [:], [:], [
#include <linux/compiler.h>
#include <linux/config.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>	/* for irqreturn_t */ 
    ])
])# _STRCOMP_KERNEL
# =============================================================================

# =============================================================================
# _STRCOMP_OUTPUT
# -----------------------------------------------------------------------------
AC_DEFUN([_STRCOMP_OUTPUT], [dnl
    _STRCOMP_DEFINES
])# _STRCOMP_OUTPUT
# =============================================================================

# =============================================================================
# _STRCOMP_DEFINES
# -----------------------------------------------------------------------------
AC_DEFUN([_STRCOMP_DEFINES], [dnl
    if test :"${strcomp_cv_modversions:-no}" != :no ; then
	AC_DEFINE_UNQUOTED([HAVE_SYS_STRCOMP_MODVERSIONS_H], [1], [Define when
	    the STREAMS compatibiltiy release supports module versions such as
	    the OpenSS7 autoconf releases.])
    fi
    STRCOMP_CPPFLAGS="${STRCOMP_CPPFLAGS:+ ${STRCOMP_CPPFLAGS}}"
    STRCOMP_LDADD="$strcomp_cv_ldadd"
    STRCOMP_MODMAP="$strcomp_cv_modmap"
    STRCOMP_SYMVER="$strcomp_cv_symver"
    STRCOMP_MANPATH="$strcomp_cv_manpath"
    STRCOMP_VERSION="$strcomp_cv_version"
    MODPOST_INPUTS="${MODPOST_INPUTS}${STRCOMP_SYMVER:+${MODPOST_INPUTS:+ }${STRCOMP_SYMVER}}"
    AC_DEFINE_UNQUOTED([_XOPEN_SOURCE], [600], [dnl
	Define for SuSv3.  This is necessary for LiS and LfS and strcompat for
	that matter.
    ])
])# _STRCOMP_DEFINES
# =============================================================================

# =============================================================================
# _STRCOMP_USER
# -----------------------------------------------------------------------------
AC_DEFUN([_STRCOMP_USER], [dnl
])# _STRCOMP_USER
# =============================================================================

# =============================================================================
# _STRCOMP_
# -----------------------------------------------------------------------------
AC_DEFUN([_STRCOMP_], [dnl
])# _STRCOMP_
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
