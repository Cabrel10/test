# vim: ft=config sw=4 noet nocin nosi com=b\:#,b\:dnl,b\:***,b\:@%\:@ fo+=tcqlorn
# =============================================================================
# BEGINNING OF SEPARATE COPYRIGHT MATERIAL
# =============================================================================
# 
# @(#) xns.m4,v OpenSS7-0_9_2_D_rc2(0.9.2.30) 2006/03/14 09:20:47
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
# xns.m4,v
# Revision 0.9.2.30  2006/03/14 09:20:47  brian
# - typo
#
# Revision 0.9.2.29  2006/03/14 09:04:11  brian
# - syntax consistency, advanced search
#
# =============================================================================

# -----------------------------------------------------------------------------
# This file provides some common macros for finding a STREAMS XNS
# release and necessary include directories and other configuration for
# compiling kernel modules to run with the STREAMS XNS package.
# -----------------------------------------------------------------------------
# Interesting enough, there is no need to have strxns loaded on the build
# machine to compile modules.  Only the proper header files are required.
# -----------------------------------------------------------------------------

# =============================================================================
# _XNS
# -----------------------------------------------------------------------------
# Check for the existence of XNS header files, particularly sys/npi.h.
# XNS headers files are required for building the NPI interface for XNS.
# Without XNS header files, the NPI interface to XNS will not be built.
# -----------------------------------------------------------------------------
AC_DEFUN([_XNS], [dnl
    AC_REQUIRE([_LINUX_STREAMS])dnl
    _XNS_OPTIONS
    _XNS_SETUP
dnl
dnl Skip kernel checks if not configuring for the kernel (i.e. no _LINUX_KERNEL)
dnl as we do for netperf.
dnl
    m4_ifdef([_LINUX_KERNEL], [_XNS_KERNEL])
    _XNS_USER
    _XNS_OUTPUT
    AC_SUBST([XNS_CPPFLAGS])dnl
    AC_SUBST([XNS_MODFLAGS])dnl
    AC_SUBST([XNS_LDADD])dnl
    AC_SUBST([XNS_MODMAP])dnl
    AC_SUBST([XNS_SYMVER])dnl
    AC_SUBST([XNS_MANPATH])dnl
    AC_SUBST([XNS_VERSION])dnl
])# _XNS
# =============================================================================

# =============================================================================
# _XNS_OPTIONS
# -----------------------------------------------------------------------------
# allow the user to specify the header file location
# -----------------------------------------------------------------------------
AC_DEFUN([_XNS_OPTIONS], [dnl
    AC_ARG_WITH([xns],
		AS_HELP_STRING([--with-xns=HEADERS],
			       [specify the XNS header file directory.
				@<:@default=$INCLUDEDIR/strxns@:>@]),
		[with_xns="$withval"],
		[with_xns=''])
])# _XNS_OPTIONS
# =============================================================================

# =============================================================================
# _XNS_SETUP
# -----------------------------------------------------------------------------
AC_DEFUN([_XNS_SETUP], [dnl
    _XNS_CHECK_HEADERS
    for xns_include in $xns_cv_includes ; do
	XNS_CPPFLAGS="${XNS_CPPFLAGS}${XNS_CPPFLAGS:+ }-I${xns_include}"
    done
    if test :"${xns_cv_config:-no}" != :no ; then
	XNS_CPPFLAGS="${XNS_CPPFLAGS}${XNS_CPPFLAGS:+ }-include ${xns_cv_config}"
    fi
    if test :"${xns_cv_modversions:=no}" != :no ; then
	XNS_MODFLAGS="${XNS_MODFLAGS}${XNS_MODFLAGS:+ }-include ${xns_cv_modversions}"
    fi
])# _XNS_SETUP
# =============================================================================

# =============================================================================
# _XNS_CHECK_HEADERS
# -----------------------------------------------------------------------------
AC_DEFUN([_XNS_CHECK_HEADERS], [dnl
    # Test for the existence of Linux Fast-STREAMS XNS header files.  The
    # package normally requires XNS header files to compile.
    AC_CACHE_CHECK([for xns include directory], [xns_cv_includes], [dnl
	xns_what="sys/npi.h"
	if test :"${with_xns:-no}" != :no -a :"${with_xns:-no}" != :yes ; then
	    # First thing to do is to take user specified director(ies)
	    AC_MSG_RESULT([(searching $with_xns)])
	    for xns_dir in $with_xns ; do
		if test -d "$xns_dir" ; then
		    AC_MSG_CHECKING([for xns include directory... $xns_dir])
		    if test -r "$xns_dir/$xns_what" ; then
			xns_cv_includes="$with_xns"
			AC_MSG_RESULT([yes])
			break
		    fi
		    AC_MSG_RESULT([no])
		fi
	    done
	    if test :"${xns_cv_includes:-no}" = :no ; then
		AC_MSG_WARN([
*** 
*** You have specified include directories using:
***
***	    --with-xns="$with_xns"
***
*** however, $xns_what does not exist in any of the specified
*** directories.  Configure will continue to search other known
*** directories.
*** ])
	    fi
	    AC_MSG_CHECKING([for xns include directory])
	fi
	if test :"${xns_cv_includes:-no}" = :no ; then
	    # The next place to look is under the master source and build
	    # directory, if any.
	    AC_MSG_RESULT([(searching $master_srcdir $master_builddir)])
	    xns_search_path="
		${master_srcdir:+$master_srcdir/strxns/src/include}
		${master_builddir:+$master_builddir/strxns/src/include}"
	    for xns_dir in $xns_search_path ; do
		if test -d "$xns_dir" ; then
		    AC_MSG_CHECKING([for xns include directory... $xns_dir])
		    if test -r "$xns_dir/$xns_what" ; then
			xns_cv_includes="$xns_search_path"
			xns_cv_ldadd= # "$master_builddir/strxns/libxns.la"
			xns_cv_modmap= # "$master_builddir/strxns/Modules.map"
			xns_cv_symver= # "$master_builddir/strxns/Module.symvers"
			xns_cv_manpath="$master_builddir/strxns/doc/man"
			AC_MSG_RESULT([yes])
			break
		    fi
		    AC_MSG_RESULT([no])
		fi
	    done
	    AC_MSG_CHECKING([for xns include directory])
	fi
	if test :"${xns_cv_includes:-no}" = :no ; then
	    # The next place to look now is for a peer package being built under
	    # the same top directory, and then the higher level directory.
	    xns_here=`pwd`
	    AC_MSG_RESULT([(searching from $xns_here)])
	    for xns_dir in \
		$srcdir/strxns*/src/include \
		$srcdir/../strxns*/src/include \
		../_build/$srcdir/../../strxns*/src/include \
		../_build/$srcdir/../../../strxns*/src/include
	    do
		if test -d "$xns_dir" ; then
		    xns_bld=`echo $xns_dir | sed -e "s|^$srcdir/|$xns_here/|;"'s|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
		    xns_dir=`(cd $xns_dir; pwd)`
		    AC_MSG_CHECKING([for xns include directory... $xns_dir])
		    if test -r "$xns_dir/$xns_what" ; then
			xns_cv_includes="$xns_dir $xns_bld"
			xns_cv_ldadd= # `echo "$xns_bld/../../libxns.la" |sed -e 's|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
			xns_cv_modmap= # `echo "$xns_bld/../../Modules.map" |sed -e 's|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
			xns_cv_symver= # `echo "$xns_bld/../../Module.symvers" |sed -e 's|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
			xns_cv_manpath=`echo "$xns_bld/../../doc/man" |sed -e 's|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
			AC_MSG_RESULT([yes])
			break
		    fi
		    AC_MSG_RESULT([no])
		fi
	    done
	    AC_MSG_CHECKING([for xns include directory])
	fi
	if test :"${xns_cv_includes:-no}" = :no ; then
	    # XNS header files are normaly found in the strxns package now.
	    # They used to be part of the INET add-on package and even older
	    # versions are part of the LiS release packages.
	    case "$streams_cv_package" in
		(LiS)
		    eval "xns_search_path=\"
			${DESTDIR}${includedir}/strxns
			${DESTDIR}${rootdir}${oldincludedir}/strxns
			${DESTDIR}${rootdir}/usr/include/strxns
			${DESTDIR}${rootdir}/usr/local/include/strxns
			${DESTDIR}${rootdir}/usr/src/strxns/src/include
			${DESTDIR}${includedir}/strinet
			${DESTDIR}${rootdir}${oldincludedir}/strinet
			${DESTDIR}${rootdir}/usr/include/strinet
			${DESTDIR}${rootdir}/usr/local/include/strinet
			${DESTDIR}${rootdir}/usr/src/strinet/src/include
			${DESTDIR}${includedir}/strxnet
			${DESTDIR}${rootdir}${oldincludedir}/strxnet
			${DESTDIR}${rootdir}/usr/include/strxnet
			${DESTDIR}${rootdir}/usr/local/include/strxnet
			${DESTDIR}${rootdir}/usr/src/strxnet/src/include
			${DESTDIR}${includedir}/LiS/xti
			${DESTDIR}${rootdir}${oldincludedir}/LiS/xti
			${DESTDIR}${rootdir}/usr/include/LiS/xti
			${DESTDIR}${rootdir}/usr/local/include/LiS/xti
			${DESTDIR}${rootdir}/usr/src/LiS/include/xti
			${DESTDIR}${includedir}/xti
			${DESTDIR}${rootdir}${oldincludedir}/xti
			${DESTDIR}${rootdir}/usr/include/xti
			${DESTDIR}${rootdir}/usr/local/include/xti
			${DESTDIR}${includedir}/LiS
			${DESTDIR}${rootdir}${oldincludedir}/LiS
			${DESTDIR}${rootdir}/usr/include/LiS
			${DESTDIR}${rootdir}/usr/local/include/LiS
			${DESTDIR}${rootdir}/usr/src/LiS/include
			${DESTDIR}${oldincludedir}/strxns
			${DESTDIR}/usr/include/strxns
			${DESTDIR}/usr/local/include/strxns
			${DESTDIR}/usr/src/strxns/src/include
			${DESTDIR}${oldincludedir}/strinet
			${DESTDIR}/usr/include/strinet
			${DESTDIR}/usr/local/include/strinet
			${DESTDIR}/usr/src/strinet/src/include
			${DESTDIR}${oldincludedir}/strxnet
			${DESTDIR}/usr/include/strxnet
			${DESTDIR}/usr/local/include/strxnet
			${DESTDIR}/usr/src/strxnet/src/include
			${DESTDIR}${oldincludedir}/LiS/xti
			${DESTDIR}/usr/include/LiS/xti
			${DESTDIR}/usr/local/include/LiS/xti
			${DESTDIR}/usr/src/LiS/include/xti
			${DESTDIR}${oldincludedir}/xti
			${DESTDIR}/usr/include/xti
			${DESTDIR}/usr/local/include/xti
			${DESTDIR}${oldincludedir}/LiS
			${DESTDIR}/usr/include/LiS
			${DESTDIR}/usr/local/include/LiS
			${DESTDIR}/usr/src/LiS/include\""
		    ;;
		(LfS)
		    eval "xns_search_path=\"
			${DESTDIR}${includedir}/strxns
			${DESTDIR}${rootdir}${oldincludedir}/strxns
			${DESTDIR}${rootdir}/usr/include/strxns
			${DESTDIR}${rootdir}/usr/local/include/strxns
			${DESTDIR}${rootdir}/usr/src/strxns/src/include
			${DESTDIR}${includedir}/strinet
			${DESTDIR}${rootdir}${oldincludedir}/strinet
			${DESTDIR}${rootdir}/usr/include/strinet
			${DESTDIR}${rootdir}/usr/local/include/strinet
			${DESTDIR}${rootdir}/usr/src/strinet/src/include
			${DESTDIR}${includedir}/strxnet
			${DESTDIR}${rootdir}${oldincludedir}/strxnet
			${DESTDIR}${rootdir}/usr/include/strxnet
			${DESTDIR}${rootdir}/usr/local/include/strxnet
			${DESTDIR}${rootdir}/usr/src/strxnet/src/include
			${DESTDIR}${includedir}/streams
			${DESTDIR}${rootdir}${oldincludedir}/streams
			${DESTDIR}${rootdir}/usr/include/streams
			${DESTDIR}${rootdir}/usr/local/include/streams
			${DESTDIR}${rootdir}/usr/src/streams/include
			${DESTDIR}${oldincludedir}/strxns
			${DESTDIR}/usr/include/strxns
			${DESTDIR}/usr/local/include/strxns
			${DESTDIR}/usr/src/strxns/src/include
			${DESTDIR}${oldincludedir}/strinet
			${DESTDIR}/usr/include/strinet
			${DESTDIR}/usr/local/include/strinet
			${DESTDIR}/usr/src/strinet/src/include
			${DESTDIR}${oldincludedir}/strxnet
			${DESTDIR}/usr/include/strxnet
			${DESTDIR}/usr/local/include/strxnet
			${DESTDIR}/usr/src/strxnet/src/include
			${DESTDIR}${oldincludedir}/streams
			${DESTDIR}/usr/include/streams
			${DESTDIR}/usr/local/include/streams
			${DESTDIR}/usr/src/streams/include\""
		    ;;
	    esac
	    xns_search_path=`echo "$xns_search_path" | sed -e 's|\<NONE\>||g;s|//|/|g'`
	    xns_cv_includes=
	    AC_MSG_RESULT([(searching)])
	    for xns_dir in $xns_search_path ; do
		if test -d "$xns_dir" ; then
		    AC_MSG_CHECKING([for xns include directory... $xns_dir])
		    if test -r "$xns_dir/$xns_what" ; then
			xns_cv_includes="$xns_dir"
			xns_cv_ldadd= # '-lxns'
			xns_cv_modmap=
			xns_cv_symver=
			xns_cv_manpath=
			AC_MSG_RESULT([yes])
			break
		    fi
		    AC_MSG_RESULT([no])
		fi
	    done
	    AC_MSG_CHECKING([for xns include directory])
	fi
    ])
    AC_CACHE_CHECK([for xns ldadd],[xns_cv_ldadd],[dnl
	for xns_dir in $xns_cv_includes ; do
	    if test -f "$xns_dir/../../libxns.la" ; then
		xns_cv_ldadd=`echo "$xns_dir/../../libxns.la" | sed -e 's|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
		break
	    fi
	done
	if test -z "$xns_cv_ldadd" ; then
	    xns_cv_ldadd= # '-lxns'
	fi
    ])
    AC_CACHE_CHECK([for xns modmap],[xns_cv_modmap],[dnl
	for xns_dir in $xns_cv_includes ; do
	    if test -f "$xns_dir/../../Modules.map" ; then
		xns_cv_modmap=`echo "$xns_dir/../../Modules.map" | sed -e 's|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
		break
	    fi
	done
    ])
    AC_CACHE_CHECK([for xns symver],[xns_cv_symver],[dnl
	for xns_dir in $xns_cv_includes ; do
	    if test -f "$xns_dir/../../Module.symvers" ; then
		xns_cv_symver=`echo "$xns_dir/../../Module.symvers" | sed -e 's|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
		break
	    fi
	done
    ])
    AC_CACHE_CHECK([for xns manpath],[xns_cv_manpath],[dnl
	for xns_dir in $xns_cv_includes ; do
	    if test -d "$xns_dir/../../doc/man" ; then
		xns_cv_manpath=`echo "$xns_dir/../../doc/man" | sed -e 's|/[[^/]][[^/]]*/\.\./|/|g;s|/[[^/]][[^/]]*/\.\./|/|g;s|/\./|/|g;s|//|/|g'`
		break
	    fi
	done
    ])
    if test :"${xns_cv_includes:-no}" = :no ; then :
	AC_MSG_ERROR([
*** 
*** Configure could not find the STREAMS XNS include directories.  If
*** you wish to use the STREAMS XNS package you will need to specify
*** the location of the STREAMS XNS (strxns) include directories with
*** the --with-xns=@<:@DIRECTORY@<:@ DIRECTORY@:>@@:>@ option to
*** ./configure and try again.
***
*** Perhaps you just forgot to load the STREAMS XNS package?  The
*** STREAMS strxns package is available from The OpenSS7 Project
*** download page at http://www.openss7.org/ and comes in a tarball
*** named something like "strxns-0.9.2.3.tar.gz".
*** ])
    fi
    AC_CACHE_CHECK([for xns version], [xns_cv_version], [dnl
	xns_what="sys/strxns/version.h"
	xns_file=
	if test -n "$xns_cv_includes" ; then
	    for xns_dir in $xns_cv_includes ; do
		# old place for version
		if test -f "$xns_dir/$xns_what" ; then
		    xns_file="$xns_dir/$xns_what"
		    break
		fi
		# new place for version
		if test -n "$linux_cv_k_release" ; then
dnl		    if linux_cv_k_release is not defined (no _LINUX_KERNEL) then
dnl		    this will just not be set
		    if test -f "$xns_dir/$linux_cv_k_release/$target_cpu/$xns_what" ; then
			xns_file="$xns_dir/$linux_cv_k_release/$target_cpu/$xns_what"
			break
		    fi
		fi
	    done
	fi
	if test :${xns_file:-no} != :no ; then
	    xns_cv_version=`grep '#define.*\<STRXNS_VERSION\>' $xns_file 2>/dev/null | sed -e 's|^[^"]*"||;s|".*$||'`
	fi
    ])
    xns_what="sys/config.h"
    AC_CACHE_CHECK([for xns $xns_what], [xns_cv_config], [dnl
	xns_cv_config=
	if test -n "$xns_cv_includes" ; then
	    for xns_dir in $xns_cv_includes ; do
		# old place for config
		if test -f "$xns_dir/$xns_what" ; then
		    xns_cv_config="$xns_dir/$xns-what"
		    break
		fi
		# new place for config
		if test -n "$linux_cv_k_release" ; then
dnl		    if linux_cv_k_release is not defined (no _LINUX_KERNEL) then
dnl		    this will just not be set
		    if test -f "$xns_dir/$linux_cv_k_release/$target_cpu/$xns_what" ; then
			xns_cv_config="$xns_dir/$linux_cv_k_release/$target_cpu/$xns_what"
			break
		    fi
		fi
	    done
	fi
    ])
    xns_what="sys/strxns/modversions.h"
    AC_CACHE_CHECK([for xns $xns_what], [xns_cv_config], [dnl
	xns_cv_modversions=
dnl	if linux_cv_k_ko_modules is not defined (no _LINUX_KERNEL) then we
dnl	assume normal objects
	if test :"${linux_cv_k_ko_modules:-no}" = :no ; then
	    if test -n "$xns_cv_includes" ; then
		for xns_dir in $xns_cv_includes ; do
		    # old place for modversions
		    if test -f "$xns_dir/$xns_what" ; then
			xns_cv_modversions="$xns_dir/$xns_what"
			break
		    fi
		    # new place for modversions
		    if test -n "$linux_cv_k_release" ; then
dnl			if linux_cv_k_release is not defined (no _LINUX_KERNEL)
dnl			then this will just not be set
			if test "$xns_dir/$linux_cv_k_release/$target_cpu/$xns_what" ; then
			    xns_cv_includes="$xns_dir/$linux_cv_k_release/$target_cpu $xns_cv_includes"
			    xns_cv_modversions="$xns_dir/$linux_cv_k_release/$target_cpu/$xns_what"
			    break
			fi
		    fi
		done
	    fi
	fi
    ])
    AC_MSG_CHECKING([for xns added configure arguments])
dnl Older rpms (particularly those used by SuSE) are too stupid to handle --with
dnl and --without rpmopt syntax, so convert to the equivalent --define syntax.
dnl Also, I don't know that even rpm 4.2 handles --with xxx=yyy properly, so we
dnl use defines.
    if test -z "$with_xns" ; then
	if test :"${xns_cv_includes:-no}" = :no ; then
	    PACKAGE_RPMOPTIONS="${PACKAGE_RPMOPTIONS}${PACKAGE_RPMOPTIONS:+ }--define \"_with_xns --with-xns\""
	    PACKAGE_DEBOPTIONS="${PACKAGE_DEBOPTIONS}${PACKAGE_DEBOPTIONS:+ }'--with-xns'"
	    AC_MSG_RESULT([--with-xns])
	else
	    PACKAGE_RPMOPTIONS="${PACKAGE_RPMOPTIONS}${PACKAGE_RPMOPTIONS:+ }--define \"_without_xns --without-xns\""
	    PACKAGE_DEBOPTIONS="${PACKAGE_DEBOPTIONS}${PACKAGE_DEBOPTIONS:+ }'--without-xns'"
	    AC_MSG_RESULT([--without-xns])
	fi
    else
	AC_MSG_RESULT([--with-xns="$with_xns"])
    fi
])# _XNS_CHECK_HEADERS
# =============================================================================

# =============================================================================
# _XNS_KERNEL
# -----------------------------------------------------------------------------
AC_DEFUN([_XNS_KERNEL], [dnl
])# _XNS_KERNEL
# =============================================================================

# =============================================================================
# _XNS_OUTPUT
# -----------------------------------------------------------------------------
AC_DEFUN([_XNS_OUTPUT], [dnl
    _XNS_DEFINES
])# _XNS_OUTPUT
# =============================================================================

# =============================================================================
# _XNS_DEFINES
# -----------------------------------------------------------------------------
AC_DEFUN([_XNS_DEFINES], [dnl
    if test :"${xns_cv_modversions:-no}" != :no ; then
	AC_DEFINE_UNQUOTED([HAVE_SYS_XNS_MODVERSIONS_H], [1], [Define when
	    the STREAMS XNS release supports module versions such as
	    the OpenSS7 autoconf releases.])
    fi
    XNS_CPPFLAGS="${XNS_CPPFLAGS:+ }${XNS_CPPFLAGS}"
    XNS_LDADD="$xns_cv_ldadd"
    XNS_MODMAP="$xns_cv_modmap"
    XNS_SYMVER="$xns_cv_symver"
    XNS_MANPATH="$xns_cv_manpath"
    XNS_VERSION="$xns_cv_version"
    MODPOST_INPUT="${MODPOST_INPUTS}${XNS_SYMVER:+${MODPOST_INPUTS:+ }${XNS_SYMVER}}"
    AC_DEFINE_UNQUOTED([_XOPEN_SOURCE], [600], [dnl
	Define for SuSv3.  This is necessary for LiS and LfS and strxns for
	that matter.
    ])
])# _XNS_DEFINES
# =============================================================================

# =============================================================================
# _XNS_USER
# -----------------------------------------------------------------------------
AC_DEFUN([_XNS_USER], [dnl
])# _XNS_USER
# =============================================================================

# =============================================================================
# _XNS_
# -----------------------------------------------------------------------------
AC_DEFUN([_XNS_], [dnl
])# _XNS_
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
