# vim: ft=spec sw=4 et tw=78 com=b\:#,fb\:- fo+=tcqlorn nocin nosi
# =============================================================================
# 
# @(#) LiS.spec.in,v OpenSS7-0_9_2_D_rc2(1.1.6.59) 2006/07/07 21:08:35
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
# Last Modified 2006/07/07 21:08:35 by brian
#
# =============================================================================

# default is to build kernel modules and tools
# conditional builds of binary rpms relies on the sneaky spec trick that
# rpmbuild will not build a binary rpm when it has no %files section
%global BUILD_modules 1
%global BUILD_devel   1
%global BUILD_tools   1
%global BUILD_libs    1
%global BUILD_noarch  1
%global BUILD_arch    1
%global BUILD_source  1
%global BUILD_test    1
%global BUILD_devs    1
%global MAKE_devs     0

# there are three ways to pass kernel version to the build process:
#    global _kversion in /usr/lib/rpm/macros, /etc/rpm/macros or ~/.rpmmacros
#    global _kversion on the rpmbuild command line
#    global PACKAGE_KVERSION environment variable
#    just let it default to `uname -r`
#{!?_kversion: #{expand: ##global _kversion #(env | grep PACKAGE_KVERSION | sed -e 's|.*=||')}}
%{!?_kversion: %{expand: %%global _kversion %(uname -r)}}

%global kernel_version %{_kversion}
%global kernel_source %(echo %{_kversion} | sed -e 's|BOOT$||;s|smp$||;s|bigmem$||;s|debug$||;s|enterprise$||;s|hugemem$||;s|secure$||;s|-i686-up-4GB$||;s|-p3-smp-64GB$||;s|-64GB-SMP|.SuSE|;s|-default||')
%global kernel_number %(echo %{_kversion} | sed -e 's|-.*$||')
%global kernel_release %(echo %{_kversion} | sed -e 's|-|.|g')
%global kernel_fedora %(echo %{_kversion} | sed -e 's|.*FC4.*$|1|;s|[^1].*$|0|')
%global kernel_others %(echo %{_kversion} | sed -e 's|.*FC4.*$|0|;s|[^0].*$|1|')
%global core_name   core-%{_kversion}
%global info_name   info-%{_kversion}
%global majbase     230
%global makedev     devices.lst
%global epoch       0
%global base        lis
%global title       Linux STREAMS (LiS)
%global stitle      LiS
%global virtual     STREAMS

%global modules     streams streams-pipemod streams-sad streams-liskmod streams-connld streams-mtdrv
%global preloads    streams
%global libraries   LiS pLiS
%global tools       ldltest strtst thrtst timetst fattach fdetach ldlconfig oc pafd psfd rwf streams strmakenodes mkfifo lis_test lis_test2
%global infofiles   %{name}
%global initfiles   streams

%{expand: %%global without_ko_modules %(echo %{_kversion} | sed -e 's|^2\.[56]\..*$|0|;s|^2\.4\..*$|1|')}
%{expand: %%global have_libraries   %{?libraries:1}%{!?libraries:0}}
%{expand: %%global have_preloads    %{?preloads: 1}%{!?preloads: 0}}
%{expand: %%global have_initscripts %{?initfiles:1}%{!?initfiles:0}}

%if %{without_ko_modules}
# assume devs for older kernels
%global NEED_devs     1
%global NEED_init     1
%else
# assume no devs for newer kernels
%global NEED_devs     0
%global NEED_init     0
%endif

%{!?_configdir:      %{expand: %%global _configdir    %%{_sysconfdir}/sysconfig}}
%{!?_modutildir:     %{expand: %%global _modutildir   %%{_sysconfdir}/modutils}}
%{!?_kmoduledir:     %{expand: %%global _kmoduledir   /lib/modules/%%{kernel_version}}}

# because we only need to build the tools binaries once for each architecture,
# but need to build the modules binaries once for each architecture and kernel
# version, we accept the --with[out] options "modules" and "tools"
%{?_without_modules: %{expand: %%global BUILD_modules 0}}
%{?_without_devel:   %{expand: %%global BUILD_devel   0}}
%{?_without_tools:   %{expand: %%global BUILD_tools   0}}
%{?_without_indep:   %{expand: %%global BUILD_noarch  0}}
%{?_without_arch:    %{expand: %%global BUILD_arch    0}}
%{?_without_public:  %{expand: %%global BUILD_source  0}}
%{?_with_devfs:      %{expand: %%global NEED_devs     0}}
%{?_with_modules:    %{expand: %%global BUILD_modules 1}}
%{?_with_devel:      %{expand: %%global BUILD_devel   1}}
%{?_with_tools:      %{expand: %%global BUILD_tools   1}}
%{?_with_indep:      %{expand: %%global BUILD_noarch  1}}
%{?_with_arch:       %{expand: %%global BUILD_arch    1}}
%{?_with_public:     %{expand: %%global BUILD_source  1}}

%define oldrpm %(if (rpm --version|grep 'RPM version 4' >/dev/null 2>&1) ; then echo 0 ; else echo 1; fi)
%define newrpm %(if (rpm --version|grep 'RPM version 4' >/dev/null 2>&1) ; then echo 1 ; else echo 0; fi)

%if %{BUILD_arch}
%global BUILD_source  0
%endif

%if %{NEED_devs}
%{expand: %%global BUILD_devs    %newrpm}
%{expand: %%global MAKE_devs     %oldrpm}
%else
%global BUILD_devs    0
%global MAKE_devs     0
%endif

%if %{BUILD_tools}
%else
%global BUILD_libs    0
%global BUILD_devs    0
%global MAKE_devs     0
%global BUILD_source  0
%endif

%if %{BUILD_devel}
%else
%global BUILD_source  0
%endif

%global _ksubdir lis
%global _kmodext lis
%global _conflict streams

%if %{BUILD_modules}
%global FILES_modules       1
%global FILES_ko_modules    1
%else
%global FILES_modules       0
%global FILES_ko_modules    0
%global kernel_fedora       0
%global kernel_others       0
%endif

%if %{BUILD_tools}
%global FILES_libs          1
%global FILES_dev           1
%global FILES_devlibs       1
%global FILES_tools         1
%global FILES_makedev       1
%global FILES_builddev      1
%global FILES_test          %{BUILD_test}
%global FILES_doc           1
%global FILES_doclibs       1
%global FILES_source        %{BUILD_source}
%global FILES_init          1
%global FILES_ko_init       1
%global FILES_noinit        1
%global FILES_ko_noinit     1
%else
%global FILES_libs          0
%global FILES_dev           0
%global FILES_devlibs       0
%global FILES_tools         0
%global FILES_makedev       0
%global FILES_builddev      0
%global FILES_test          0
%global FILES_doc           0
%global FILES_doclibs       0
%global FILES_source        0
%global FILES_init          0
%global FILES_ko_init       0
%global FILES_noinit        0
%global FILES_ko_noinit     0
%endif

%if %{BUILD_devel}
%else
%global FILES_doc           0
%global FILES_doclibs       0
%global FILES_dev           0
%global FILES_devlibs       0
%global FILES_source        0
%endif

%if %{BUILD_arch}
%else
%global FILES_modules       0
%global FILES_ko_modules    0
%global FILES_libs          0
%global FILES_dev           0
%global FILES_devlibs       0
%global FILES_tools         0
%endif

%if %{BUILD_noarch}
%else
%global FILES_makedev       0
%global FILES_builddev      0
%global FILES_test          0
%global FILES_doc           0
%global FILES_doclibs       0
%global FILES_source        0
%global FILES_ko_init       0
%global FILES_init          0
%global FILES_ko_noinit     0
%global FILES_noinit        0
%endif

%if %{BUILD_libs}
%global FILES_dev           0
%global FILES_doc           0
%else
%global FILES_libs          0
%global FILES_devlibs       0
%global FILES_doclibs       0
%endif

%if %{without_ko_modules}
%global FILES_ko_modules    0
%global FILES_ko_init       0
%global FILES_ko_noinit     0
%else
%global FILES_modules       0
%global FILES_init          0
%global FILES_noinit        0
%global FILES_makedev       0
%global FILES_builddev      0
%endif

%if %{BUILD_devs}
%global FILES_makedev       0
%else
%global FILES_builddev      0
%endif

%if %{have_initscripts}
%global FILES_ko_noinit     0
%global FILES_noinit        0
%global NEED_init           1
%else
%global FILES_ko_init       0
%global FILES_init          0
%endif


# we have some more options that control configure for the build
%{?disturl:%{!?url:%{expand: %%global url %{disturl}}}}
%{?extrarelease:  %{expand: %%global fullrelease 1%%{extrarelease}}}
%{!?extrarelease: %{expand: %%global fullrelease 1}}
%{?extrarelease2:  %{expand: %%global fullrelease2 1%%{extrarelease2}}}
%{!?extrarelease2: %{expand: %%global fullrelease2 1}}

# a macro to [build] require one package of the same version as another (possibly virtual) package
# use the arguments [Build]Requires: package1 [>]= package2
%define requires_prov() %(LANG_ALL="C" rpm -q --provides %4 --qf "%1 %2 %3 %{EPOCH}:%{VERSION}-%{RELEASE}\\n" | tail -1 | sed -e 's|.*is not.*$|%1 %2|')
%define version_of_or() %(LANG_ALL="C" rpm -q --provides %1 --qf "%{EPOCH}:%{VERSION}-%{RELEASE}\\n" | tail -1 | sed -e 's|.*is not.*$|%2|')

%global descripline %{title} is a (poor) implementation of SVR 4 %{virtual} for Linux.

Summary:        %{title} (SVR 4 %{virtual})
Name:           LiS
Epoch:          0
Version:        2.18.4.rc2
Release:        1
License:        GPL
Group:          System Environment/Kernel
%{?url:URL:            %{url}}
%{?_xpm_icon:Icon:           %{_xpm_icon}}
%{?_gif_icon:Icon:           %{_gif_icon}}
ExclusiveArch:  %{ix86} k6 x86_64 s390 s390x ppc ppciseries ppcpseries ppc64 arm armv4l sh mips mipsel noarch
ExclusiveOs:    Linux
#Source:         ftp://ftp.gcom.com/pub/linux/src/%{name}/%{name}-%{version}.tgz
#Source:         %{name}-%{version}.tar.bz2
Source:         http://www.openss7.org/tarballs/%{name}-%{version}.tar.bz2
%{!?buildroot:BuildRoot:      %{_tmppath}/%{name}-%{version}-%{fullrelease}-root}
# TODO: FC4 wants kernel-devel not kernel-source
# older rpm can't handle nested ifs
%if %{kernel_fedora}
BuildRequires:  kernel-devel = %{kernel_source}
%endif
%if %{kernel_others}
BuildRequires:  kernel-source = %{kernel_source}
%endif
%if %{BUILD_modules}
BuildRequires:  modutils >= 2.4.12
%endif

%define newgcc %(if test `gcc -v 2>&1 | grep 'gcc version' | sed -e 's|gcc version ||;s| .*$||'` != '2.95.3' ; then echo 1 ; else echo 0 ; fi)

%ifarch s390 s390x
BuildRequires:  gcc >= 2.95.3
%else
%ifarch %all_ppc
BuildRequires:  gcc >= 2.96-75
%else
%if %newgcc
BuildRequires:  gcc >= 2.96-98
%else
BuildRequires:  gcc = 2.95.3
%endif
%endif
%endif
%if %newgcc
BuildRequires:  gcc >= 2.96
%else
BuildRequires:  gcc = 2.95.3
%endif

# these are some needs of the configure script
# to manipulate manual pages:
BuildRequires:  /usr/bin/groff
BuildRequires:  /usr/bin/soelim
BuildRequires:  /usr/bin/refer
BuildRequires:  /usr/bin/tbl
BuildRequires:  /usr/bin/pic
BuildRequires:  /usr/bin/gzip

%if %{without_ko_modules}
# to generate kernel symbols:
BuildRequires:  /sbin/genksyms
%endif

# to build documentation
#BuildRequires: transfig >= 3.2
#BuildRequires: ImageMagick
#BuildRequires: tetex >= 1.0
#BuildRequires: ghostscript >= 6.51
# to install documentation
#BuildRequires: texinfo >= 4.0
# find the rest automagically
Autoreq:        true

# ---------------------------------------------------------------------------
%description
%{descripline}
The %{name} package includes kernel modules, STREAMS drivers,
modules, libraries, utilities, test programs, daemons, and
development environment for the development and execution of
System V Release 4 %{virtual} modules and drivers developed for
Linux.  This distribution is currently applicable to 2.4 and 2.6
kernels and is validated for ix86, x86_64, ppc and ppc64
architectures, but should build and install well for other
architectures.  THE %{name} PACKAGE VALIDATES POORLY ON MOST
ARCHITECTURES AND DISTRIBUTIONS AND IS ESSENTIALLY UNUSABLE ON
SMP.  THIS PACKAGE IS DEPRECATED.  YOU SHOULD BE USING THE
%{_conflict} PACKAGE INSTEAD.  This SRPM builds the following
binary packages:
- %{name}-%{core_name} -- core kernel modules.
- %{name}-%{info_name} -- core kernel module symbol versions.
- %{name}-lib -- run-time libraries.
- %{name}-devel -- headers, libs and docs.
- %{name}-util -- admin utilities.
- %{name}-dev -- devices.
- %{name}-test -- test utilities.
- %{name}-doc -- manpages, manuals and docs.
- %{name}-source -- source package.
- %{name}-init -- init scripts and config files.

%prep
# ---------------------------------------------------------------------------

%setup -q -n %{name}-%{version}

%build
# ---------------------------------------------------------------------------

preferred_cflags="%{optflags}"

%ifarch %ix86
preferred_cflags="$preferred_cflags -D__USE_STRING_INLINES -fstrict-aliasing"
%endif

%ifarch sparc
preferred_cflags="$preferred_cflags -fcall-used-g7"
%endif

%ifarch sparcv9
preferred_cflags="$preferred_cflags -fcall-used-g7"
%endif

%ifarch sparc64
preferred_cflags="$preferred_cflags -mvis -fcall-used-g7"
%endif

%ifnarch ia64 s390 s390x ppc ppciseries ppc64
if test "`gcc --version`" != '2.95.3' ; then
preferred_cflags="$preferred_cflags -freorder-blocks"
fi
%endif

%define cache_file %{_builddir}/%{_target_platform}-%{kernel_version}-config.cache
%define csite_file %{_builddir}/%{_target_platform}-config.site
%define mpost_file %{_builddir}/%{_target_platform}-%{kernel_version}-modpost.cache

%define enable()  %(echo "%1" | sed -e 's|^--with-|--enable-|')
%define disable() %(echo "%1" | sed -e 's|^--without-|--disable-|')

./configure \
    CC="${CC}" \
    CFLAGS="${CFLAGS:-$preferred_cflags}" \
    LDFLAGS="${LDFLAGS}" \
    CPPFLAGS="${CPPFLAGS}" \
    CPP="${CPP}" \
    CXX="${CXX:-$CC}" \
    CXXFLAGS="${CXXFLAGS:-$preferred_cflags}" \
    CXXCPP="${CXXCPP:-$CPP}" \
    CONFIG_SITE="%{csite_file}" \
    MODPOST_CACHE="%{mpost_file}" \
    --cache-file="%{cache_file}" \
%ifnarch noarch
    --host=%{_target_platform} \
%endif
    --prefix=%{_prefix} \
    --exec-prefix=%{_exec_prefix} \
    --bindir=%{_bindir} \
    --sbindir=%{_sbindir} \
    --sysconfdir=%{_sysconfdir} \
    --datadir=%{_datadir} \
    --includedir=%{_includedir} \
    --libdir=%{_libdir} \
    --libexecdir=%{_libexecdir} \
    --localstatedir=%{_localstatedir} \
    --sharedstatedir=%{_sharedstatedir} \
    --mandir=%{_mandir} \
    --infodir=%{_infodir} \
    --disable-maintainer-mode \
    --disable-dependency-tracking \
    --with-gnu-ld \
    --with-k-release=%{kernel_version} \
    --with-k-modules=%{_kmoduledir} \
    --with-base-major=%{majbase} \
    %{?_with_arch:               '%enable %_with_arch'} \
    %{?_without_arch:            '%disable %_without_arch'} \
    %{?_with_indep:              '%enable %_with_indep'} \
    %{?_without_indep:           '%disable %_without_indep'} \
    %{?_with_checks:             '%enable %_with_checks'} \
    %{?_without_checks:          '%disable %_without_checks'} \
    %{?_with_autotest:           '%enable %_with_autotest'} \
    %{?_without_autotest:        '%disable %_without_autotest'} \
    %{?_with_cooked_manpages:    '%_with_cooked_manpages'} \
    %{?_without_cooked_manpages: '%_without_cooked_manpages'} \
    %{?_with_public:             '%enable %_with_public'} \
    %{?_without_public:          '%disable %_without_public'} \
    %{?_with_k_optimize:         '%_with_k_optimize'} \
    %{?_with_k_debug:            '%enable %_with_k_debug'} \
    %{?_without_k_debug:         '%disable %_without_k_debug'} \
    %{?_with_k_test:             '%enable %_with_k_test'} \
    %{?_without_k_test:          '%disable %_without_k_test'} \
    %{?_with_k_safe:             '%enable %_with_k_safe'} \
    %{?_without_k_safe:          '%disable %_without_k_safe'} \
    %{?_with_k_inline:           '%enable %_with_k_inline'} \
    %{?_without_k_inline:        '%disable %_without_k_inline'} \
    %{?_with_devfs:              '%enable %_with_devfs'} \
    %{?_without_devfs:           '%disable %_without_devfs'} \
    %{?_with_devel:              '%enable %_with_devel'} \
    %{?_without_devel:           '%disable %_without_devel'} \
    %{?_with_tools:              '%enable %_with_tools'} \
    %{?_without_tools:           '%disable %_without_tools'} \
    %{?_with_modules:            '%enable %_with_modules'} \
    %{?_without_modules:         '%disable %_without_modules'} \
    %{?_with_solaris_cmn_err:    '%{_with_solaris_cmn_err}'} \
    %{?_without_solaris_cmn_err: '%{_without_solaris_cmn_err}'} \
    %{?_with_solaris_consts:     '%{_with_solaris_consts}'} \
    %{?_without_solaris_consts:  '%{_without_solaris_consts}'} \
    %{?_with_lis_regparms:       '%{_with_lis_regparms}'} \
    %{?_without_lis_regparms:    '%{_without_lis_regparms}'} \
    %{?_with_k_modversions:      '%enable %_with_k_modversions'} \
    %{?_without_k_modversions:   '%disable %_without_k_modversions'} \
    %{?_with_k_cache:            '%enable %_with_k_cache'} \
    %{?_without_k_cache:         '%disable %_without_k_cache'} \
    %{?_with_k_timers:           '%enable %_with_k_timers'} \
    %{?_without_k_timers:        '%disable %_without_k_timers'} \
    %{?_with_broken_cpu_flags:   '%enable %_with_broken_cpu_flags'} \
    %{?_without_broken_cpu_flags:'%disable %_without_broken_cpu_flags'} \
    %{?_with_atomic_stats:       '%enable %_with_atomic_stats'} \
    %{?_without_atomic_stats:    '%disable %_without_atomic_stats'} \
    %{?_with_lis_development:    '%enable %_with_lis_development'} \
    %{?_without_lis_development: '%disable %_without_lis_development'} \
    || { rm -f %{cache_file} ; exit 1 ; }

make
make check

%install
# ---------------------------------------------------------------------------

# in case it was not cleaned
[ -n "%{buildroot}" -a "%{buildroot}" != / ] && rm -rf %{buildroot}

# if we are going to create debug packages, we need to install unstripped
# binaries; otherwise use libtoolized install-strip to strip binaries, modules,
# shared and static libraries.
make DESTDIR="%{buildroot}" DOCDIR="%{_docdir}" \
    %{!?_enable_debug_packages: install-strip} \
    %{?_enable_debug_packages: install}

# copy out our devices list
# cp -pf %{mybuilddir}/%{makedev} .

# get the installed info directory out of the build root
[ -e "%{buildroot}/%{_infodir}/dir" ] && rm -f "%{buildroot}/%{_infodir}/dir"

function package_source() {
# remove include2 symbolic link
    rm -fr include2
# punch rpm release to Custom
    echo '.Custom.1'  > .rpmextra
# create configured source directory for source package
    mkdir -p %{buildroot}%{_usrsrc}/%{name}-%{version}-%{fullrelease}
# tar and move the sources for source release
    tar cf - . | tar xf - -C %{buildroot}%{_usrsrc}/%{name}-%{version}-%{fullrelease}
# clean out binaries and auxilliary files
    ( cd %{buildroot}%{_usrsrc}/%{name}-%{version}-%{fullrelease} ; make mostlyclean )
# create symbolic link
    ln -sf %{name}-%{version}-%{fullrelease} %{buildroot}%{_usrsrc}/%{name}
}

%if %{BUILD_source}
package_source
%endif

# stupid mandrake spec-helper will compress our macros files otherwise
#export EXCLUDE_FROM_COMPRESS="%{base}.refs %{base}.macros"

# stupid suse check compresses everything (including our macro files)
#{?suse_check: %{expand: %%define suse_check echo "SuSE Check skipped"}}

%clean
# ---------------------------------------------------------------------------

[ -n "%{buildroot}" -a "%{buildroot}" != / ] && rm -rf %{buildroot}

# ===========================================================================
%package %{core_name}

Summary:        %{title} core kernel modules.
Group:          System Environment/Kernel
Prefix:         %{_kmoduledir}
Requires:       %(LANG_ALL="C" rpm -q --whatprovides %{_kmoduledir}/kernel --qf "%{NAME} = %{VERSION}-%{RELEASE}\\n")
%if %{NEED_init}
Requires:       %{name}-init = %{epoch}:%{version}-%{fullrelease2}
%endif
%if %{NEED_devs}
Requires:       %{name}-dev = %{epoch}:%{version}-%{fullrelease2}
%endif
Conflicts:      %{_conflict}-%{core_name}
#Prereq:         /sbin/depmod
#Prereq:         /sbin/lsmod
#Prereq:         /sbin/insmod
#Prereq:         /sbin/modprobe
# SuSE puts lsof in /usr/bin
Prereq:         lsof
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{virtual}-%{core_name} = %{epoch}:%{version}-%{fullrelease}
Provides:       %{virtual}-core = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-%{core_name} = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-core = %{epoch}:%{version}-%{fullrelease}
Provides:       %{virtual}-%{core_name} = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{virtual}-core = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-%{core_name} = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-core = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description %{core_name}
%{descripline}
The %{name}-%{core_name} binary package contains the kernel
modules that provide the Linux kernel SVR 4 %{virtual} facility.
This includes assorted basic %{virtual} drivers and modules such
as pipes, fifos, echo, etc.  This binary package also includes
modprobe configuration files for the associated kernel modules.
This package is heavily tied to the kernel for which it was
compiled.  This package applies to kernel version
%{kernel_version}.  If you cannot find a binary package that
matches your kernel, rebuild from the source rpm.  THE %{name}
PACKAGE VALIDATES POORLY ON MOST ARCHITECTURES AND DISTRIBUTIONS
AND IS ESSENTIALLY UNUSABLE ON SMP.  THIS PACKAGE IS DEPRECATED.
YOU SHOULD BE USING THE %{_conflict} PACKAGE INSTEAD.

%pre %{core_name}
# ---------------------------------------------------------------------------
arg="$1"
function remove_depmods() {
    local t
    [ $# -gt 0 ] || return 0
    echo "$*"
    for t in $@ ; do
        prereq=prereq_`echo $t | sed -r -e 's|^.*/||;s|\.(k)?o(\.gz)?(:)?$||;s|[^a-zA-Z0-9_]|_|g'`
        eval "remove_depmods \$$prereq"
    done
}
if [ $arg = 1 ] ; then : # fresh install
    # This is a fresh install of this version of kernel module.  Make sure that there are no non-rpm
    # installed kernel modules hanging around for this kernel version.
    list="%{modules}" ; modl_mods=
    if [ -d %{_kmoduledir} ] ; then
	for m in `find %{_kmoduledir} \( -name 'streams*.o' -o -name 'streams*.ko' -o -name 'streams*.o.gz' -o -name 'streams*.ko.gz' \) 2>/dev/null` ; do
		b=`echo $m | sed -r -e 's|^.*/||;s|\.(k)?o(\.gz)?||'`
		case " $list " in (*" $b "*) ;; (*) continue ;; esac
		echo "$m"
		modl_mods="${modl_mods:+$modl_mods }$m"
	done
        if [ -n "$modl_mods" -a -f %{_kmoduledir}/modules.dep ] ; then
            # First we can build a dependency tree from every dependency expression that can be
            # found in the modules.dep file.
            xtrace=`shopt -p -o | grep xtrace` ; set +x
            dep= ; while read line ; do
                case $line in
                    (*\\)   line="`echo $line | sed -e 's|\\$||'`"
                            dep="${dep:+$dep }$line" ; continue ;;
                    (*)	    dep="${dep:+$dep }$line" ;;
                esac
                [ -n "$dep" ] || continue;
                set dummy $dep
                if [ "${3+set}" = "set" ] ; then
                    t=`echo $2 | sed -e 's|:$||'`
                    target=target_`echo $t | sed -r -e 's|^.*/||;s|\.(k)?o(\.gz)?(:)?$||;s|[^a-zA-Z0-9_]|_|g'`
                    shift 2
                    for p in $@ ; do
                        prereq=prereq_`echo $p | sed -r -e 's|^.*/||;s|\.(k)?o(\.gz)?(:)?$||;s|[^a-zA-Z0-9_]|_|g'`
                        eval "$prereq=\"\${$prereq:+\$$prereq }$t\""
                        eval "$target=\"\${$target:+\$$target }$p\""
                    done
                fi
                dep=
            done < %{_kmoduledir}/modules.dep
            eval "$xtrace"
        fi
    fi
    # remove conflicting and dependent modules
    if [ -n "$modl_mods" ] ; then
        rm -f -- $modl_mods
        for m in $list ; do
            prereq=prereq_`echo $m | sed -r -e 's|^.*/||;s|\.(k)?o(\.gz)?(:)?$||;s|[^a-zA-Z0-9_]|_|g'`
            eval "modl_deps=\"\`remove_depmods \$$prereq\`\""
            [ -n "$modl_deps" ] || continue
            rm -f -- $modl_deps
        done
    fi
fi
if [ $arg = 1 -o $arg = 2 ] ; then : # fresh install or upgrade
    if [ ":%{kernel_version}" = ":`uname -r`" ] ; then
        # If we are upgrading and the upgrade is for the running kernel, or we are installing new on
        # the running kernel, we have to worry about previous loaded modules for the old version or
        # a non-rpm version.
        mods="%{modules}" ; for m in $mods ; do
            if ( lsmod | grep -q "^$m" ) ; then
                # Module running, kick the stack.
                modprobe -r `lsmod | grep "^$m" | cut -f1 '-d '`
                if ( lsmod | grep -q "^$m" ) ; then
                    fdetach -a 2>/dev/null
                    modprobe -r `lsmod | grep "^$m" | cut -f1 '-d '`
                    if ( lsmod | grep -q "^$m" ) ; then
                        lsof | while read -a tokens; do
                            if [ :"$tokens[4]" = :"CHR" ]; then
                                major=`echo "$tokens[5]" | sed -e 's|,.*||'`
                                if [ $major -eq %{majbase} ]; then
                                    eval "kill -HUP $tokens[1]"
                                fi
                            fi
                        done
                        /sbin/sleep 5 # wait for processes to die and close streams
                        modprobe -r `lsmod | grep "^$m" | cut -f1 '-d '`
                        if ( lsmod | grep -q "^$m" ) ; then
                            echo "Cannot install or upgrade with $m module running." >&2
                            exit 1
                        fi
                    fi
                fi
            fi
        done
    fi
fi

%post %{core_name}
# ---------------------------------------------------------------------------
if [ $1 = 1 -o $1 = 2 ] ; then : # fresh install or upgrade
    if [ ":%{kernel_version}" = ":`uname -r`" ] ; then
        # This is a running kernel, do a depmod on the modules directory for the running
        # kernel.
        depmod -ae || :
%if %{have_initscripts}
        # Running kernels do need init scripts run.
        list='%{initfiles}' ; for i in $list ; do
            if [ -f %_initrddir/$i ] ; then
                /sbin/service $i start 2>&1 || %_initrddir/$i start 2>&1 || :
            fi
        done
%endif
    elif [ -r /boot/System.map-%{kernel_version} ] ; then
        # Non-running kernels a depmod will be performed on boot by depmod anyway, but
        # try one now.
        UNAME_MACHINE=%{_target_cpu} \
        depmod -ae -F /boot/System.map-%{kernel_version} %{kernel_version} || :
        # Non-running kernels do not need init scripts run.
    fi
fi

%preun %{core_name}
# ---------------------------------------------------------------------------
if [ $1 = 0 -o $1 = 1 ] ; then : # final removal or upgrade
    if [ ":%{kernel_version}" = ":`uname -r`" ] ; then
        # We are about to uninstall for good on a running kernel then we have to worry about which
        # modules are running
%if %{have_initscripts}
        # First lets try shutting down using any init script first in reverse order
        list='%{initfiles}' ; for i in $list ; do
            reverse="$i${reverse:+ $reverse}"
        done
        for i in $reverse ; do
            if [ -f %_initrddir/$i ] ; then
                /sbin/service $i stop 2>&1 || %_initrddir/$i stop 2>&1 || :
            fi
        done
%endif
        mods="%{modules}" ; for m in $mods ; do
            if ( lsmod | grep -q "^$m" ) ; then
                # modules running, kick the stack
                modprobe -r `lsmod | grep "^$m" | cut -f1 '-d '`
                if ( lsmod | grep -q "^$m" ) ; then
                    fdetach -a
                    modprobe -r `lsmod | grep "^$m" | cut -f1 '-d '`
                    if ( lsmod | grep -q "^$m" ) ; then
                        lsof | while read -a tokens; do
                            if [ :"$tokens[4]" = :"CHR" ]; then
                                major=`echo "$tokens[5]" | sed -e 's|,.*||'`
                                if [ $major -eq %{majbase} ]; then
                                    eval "kill -KILL $tokens[1]"
                                fi
                            fi
                        done
                        /sbin/sleep 5 # wait for processes to die and close streams
                        modprobe -r `lsmod | grep "^$m" | cut -f1 '-d '`
                        if ( lsmod | grep -q "^$m" ) ; then
                            echo "Cannot uninstall with $m module running." >&2
                            exit 1
                        fi
                    fi
                fi
            fi
        done
    fi
fi

%postun %{core_name}
# ---------------------------------------------------------------------------
if [ $1 = 0 -o $1 = 1 ] ; then : # final removal or upgrade
    if [ :"%{kernel_version}" = :"`uname -r`" ] ; then
        # This is a running kernel, do a depmod on the modules directory for.
        depmod -ae || :
    elif [ -r /boot/System.map-%{kernel_version} ] ; then
        # Non-running kernels a depmod will be performed on boot but depmod anyway.
        UNAME_MACHINE=%{_target_cpu} \
        depmod -ae -F /boot/System.map-%{kernel_version} %{kernel_version} || :
    fi
    if [ :"%{kernel_version}" = :"`uname -r`" ] ; then
        # One more kick at killing any loaded modules on uninstall, last stage of the upgrade on a
        # running kernel for upgrade.
        reboot='no'
        mods="%{modules}" ; for m in $mods ; do
            if ( lsmod | grep -q "^$m" ) ; then
                # Modules running, kick the stack.
                modprobe -r `lsmod | grep "^$m" | cut -f1 '-d '`
                if ( lsmod | grep -q "^$m" ) ; then
                    fdetach -a
                    modprobe -r `lsmod | grep "^$m" | cut -f1 '-d '`
                    if ( lsmod | grep -q "^$m" ) ; then
                        insmod -ar "$m"
                        lsof | while read -a tokens; do
                            if [ :"$tokens[4]" = :"CHR" ]; then
                                major=`echo "$tokens[5]" | sed -e 's|,.*||'`
                                if [ $major -eq %{majbase} ]; then
                                    if [ $1 = 0 ] ; then
                                        # Extreme prejudice for removal.
                                        eval "kill -KILL $tokens[1]"
                                    else
                                        # Try to get them to reload for upgrade.
                                        eval "kill -HUP $tokens[1]"
                                    fi
                                fi
                            fi
                        done
                        /sbin/sleep 5 # wait for processes to die and close streams
                        modprobe -r `lsmod | grep "^$m" | cut -f1 '-d '`
                        if ( lsmod | grep -q "^$m" ) ; then
                            echo "Error: modules may be still loaded" >&2
                            lsmod | grep "^$m" >&2
                            reboot='yes'
                        fi
                    fi
                fi
            fi
        done
        if [ "$reboot" = 'yes' ]; then
            echo "you may have to reboot" >&2
            exit 1
        fi
    fi
fi

%if %{FILES_modules}
%files %{core_name}
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%dir %{_kmoduledir}/%{base}
%attr(644,root,root) %{_kmoduledir}/modules.%{base}
%{_kmoduledir}/%{base}/*
%endif

%if %{FILES_ko_modules}
%files %{core_name}
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%dir %{_kmoduledir}/%{base}
%{_kmoduledir}/%{base}/*
%endif

# ===========================================================================
%package %{info_name}

Summary:        %{title} core kernel module symbol versions.
Group:          System Environment/Kernel
Prefix:         %{_includedir}/%{name}/%{kernel_version}
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{virtual}-%{info_name} = %{epoch}:%{version}-%{fullrelease}
Provides:       %{virtual}-info = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-%{info_name} = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-info = %{epoch}:%{version}-%{fullrelease}
Provides:       %{virtual}-%{info_name} = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{virtual}-info = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-%{info_name} = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-info = %{epoch}:%{version}-%{fullrelease2}
Obsoletes:      %{name}-core < %{epoch}:%{version}-%{fullrelease}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description %{info_name}
%{descripline}
The %{name}-%{info_name} binary package contains the kernel
modules symbol information for development of additional
%{virtual} kernel modules against the core kernel modules
included in the %{name}-core binary package.  This package is
heavily tied to the core kernel modules and kernel for which it
was compiled.  This package applies to core kernel modules
%{name}-%{core_name} for kernel version %{_kversion}.  THE
%{name} PACKAGE VALIDATES POORLY ON MOST ARCHITECTURES AND
DISTRIBUTIONS AND IS ESSENTIALLY UNUSABLE ON SMP.  THIS PACKAGE
IS DEPRECATED.  YOU SHOULD BE USING THE %{_conflict} PACKAGE
INSTEAD.

%if %{FILES_modules}
%files %{info_name}
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%dir %{_includedir}/%{name}/%{kernel_version}
%{_includedir}/%{name}/%{kernel_version}/*
%endif

# don't include package for FILES_ko_modules

# ===========================================================================
%package lib

Summary:        %{title} run-time libraries.
Group:          System Environment/Libraries
Prefix:         %{_libdir}
Prefix:         %{_datadir}/locale
Obsoletes:      %{name}-libs
#Prereq:         /sbin/ldconfig
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{virtual}-lib = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-lib = %{epoch}:%{version}-%{fullrelease}
Provides:       %{virtual}-lib = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-lib = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description lib
%{descripline}
The %{name}-lib binary package contains the run-time (shared
object) libraries necessary to run applications programs and
utilities developed for %{title}.  Also included are the libtool
.la files describing the shared object libraries.  THE %{name}
PACKAGE VALIDATES POORLY ON MOST ARCHITECTURES AND DISTRIBUTIONS
AND IS ESSENTIALLY UNUSABLE ON SMP.  THIS PACKAGE IS DEPRECATED.
YOU SHOULD BE USING THE %{_conflict} PACKAGE INSTEAD.

%if %{have_libraries}
%pre lib
# ---------------------------------------------------------------------------
if [ $1 = 1 ] ; then : # fresh install
    # See if there are any libraries hanging around from a non-rpm install.  If there are, just
    # delete them.
    libs="%{libraries}" ; for lib in $libs ; do
        old_libs="`( ldconfig -p | grep '\<lib'$lib'\>' | cut '-d ' -f4 ) 2>/dev/null`"
        if [ -n "$old_libs" ] ; then
            # Old libraries hanging around, delete them.
            echo "warning: Deleting old non-rpm libraries $old_libs" >&2
            rm -f $old_libs
        fi
    done
fi
%endif

%post lib
# ---------------------------------------------------------------------------
if [ $1 = 1 -o $1 = 2 ] ; then : # fresh install or upgrade
    # After a fresh install or upgrade, do a load config.
    /sbin/ldconfig
fi

%postun lib
# ---------------------------------------------------------------------------
if [ $1 = 0 -o $1 = 1 ] ; then
    # Whether complete removal or upgrade, do a load config.
    /sbin/ldconfig
fi

%if %{FILES_libs}
%files lib
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%{_libdir}/*.so*
%{_libdir}/*.la
#%{_datadir}/locale/*
%endif

# ===========================================================================
%package devel

Summary:        %{title} headers, libs and docs.
Group:          Development/System
Prefix:         %{_libdir}
Prefix:         %{_includedir}/%{name}
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{virtual}-devel = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-devel = %{epoch}:%{version}-%{fullrelease}
Provides:       %{virtual}-devel = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-devel = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description devel
%{descripline}
The %{name}-devel binary package contains library archives for
static compilation, and headers files to develop %{title}
modules and drivers.  This also includes the header files and
static libraries required to compile %{title} applications
programs.  THE %{name} PACKAGE VALIDATES POORLY ON MOST
ARCHITECTURES AND DISTRIBUTIONS AND IS ESSENTIALLY UNUSABLE ON
SMP.  THIS PACKAGE IS DEPRECATED.  YOU SHOULD BE USING THE
%{_conflict} PACKAGE INSTEAD.

%if %{FILES_devlibs}
%files devel
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%{_libdir}/*.a
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/*
%endif

%if %{FILES_dev}
%files devel
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/*
%endif

# ===========================================================================
%package util

Summary:        %{title} admin utilities.
Group:          System Environment/Base
Prefix:         %{_bindir}
Prefix:         %{_sbindir}
Prefix:         %{_libexecdir}
#Prefix:         %{_sysconfdir}
Requires:       %{name}-lib = %{epoch}:%{version}-%{fullrelease2}
Obsoletes:      %{name}-utils
Conflicts:      %{_conflict}-util
Conflicts:      %{_conflict}-utils
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{virtual}-util = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-util = %{epoch}:%{version}-%{fullrelease}
Provides:       %{virtual}-util = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-util = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description util
%{descripline}
The %{name}-util binary package provides administrative and
configuration test utilities and commands associated with the
%{title} package.  Note that these utilities are needed for
running the validation test suites contained in the %{name}-test
binary package.  THE %{name} PACKAGE VALIDATES POORLY ON MOST
ARCHITECTURES AND DISTRIBUTIONS AND IS ESSENTIALLY UNUSABLE ON
SMP.  THIS PACKAGE IS DEPRECATED.  YOU SHOULD BE USING THE
%{_conflict} PACKAGE INSTEAD.

%pre util
# ---------------------------------------------------------------------------
if [ $1 = 1 ]; then : # fresh install
    # This is a fresh install, look for old non-rpm tools.
    tools="%{tools}" ; for tool in $tools ; do
        location="`which $tool 2>/dev/null`"
        while [ -n "$location" -a -x "$location" ] ; do
            eval "rm -f $location"
            location="`which $tool 2>/dev/null`"
            while [ -n "$location" -a -x "$location" ] ; do
                eval "rm -f $location"
                location="`which $tool 2>/dev/null`"
            done
        done
    done
    # Old non-rpm distro puts some man pages in the wrong place, clean them out.
    if [ -f /usr/man/man3/fattach.3 ] ; then
        rm -f /usr/man/man3/fattach.3 2>/dev/null
        rm -f /usr/man/man3/detach.3 2>/dev/null
        rm -f /usr/man/man3/isastream.3 2>/dev/null
        rmdir -p /usr/man/man3 2>/dev/null || :
        rm -f /usr/man/man8/fattach.8 2>/dev/null
        rm -f /usr/man/man8/fdetach.8 2>/dev/null
        rmdir -p /usr/man/man8 2>/dev/null || :
        rm -f /usr/man/man9/connld.9 2>/dev/null
        rm -f /usr/man/man9/fifo.9 2>/dev/null
        rm -f /usr/man/man9/pipemod.9 2>/dev/null
        rmdir -p /usr/man/man9 2>/dev/null || :
        rmdir -p /usr/man 2>/dev/null || :
    fi
fi

%if %{FILES_tools}
%files util
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%{_bindir}/*
%{_sbindir}/*
%{_libexecdir}/%{name}/*test*
%{_libexecdir}/%{name}/*tst
#{_sysconfdir}/lis.conf
%endif

# ===========================================================================
%package dev

Summary:        %{title} devices.
Group:          System Environment/Base
Conflicts:      %{_conflict}-dev
Conflicts:      %{_conflict}-devs
Requires:       dev >= 3.3
%if %{MAKE_devs}
Requires:       %{name}-util = %{epoch}:%{version}-%{fullrelease2}
%endif
Obsoletes:      %{name}-devs
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{virtual}-dev = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-dev = %{epoch}:%{version}-%{fullrelease}
Provides:       %{virtual}-dev = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-dev = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description dev
%{descripline}
The %{name}-dev binary package contains the device definitions
necessary to run applications programs and utilities developed
for %{title}.  On older systems, this binary creates the devices
themselvs.  On newer systems, this binary package includes the
executable for creating devices from init scripts.  The package
is not required for devfs based systems.  THE %{name} PACKAGE
VALIDATES POORLY ON MOST ARCHITECTURES AND DISTRIBUTIONS AND IS
ESSENTIALLY UNUSABLE ON SMP.  THIS PACKAGE IS DEPRECATED.  YOU
SHOULD BE USING THE %{_conflict} PACKAGE INSTEAD.

%if %{MAKE_devs}
%post dev
if [ $1 = 1 ] ; then : # fresh install
    # Older rpm versions < 4.0 (such as those used by SuSE) are not capable of doing rootless builds
    # because they have no %%dev macro, so in that case we just run the makenodes program that was
    # generated in the build to create devices.
    /usr/sbin/strmakenodes
fi

%preun dev
if [ $1 = 0 ] ; then : # final removal
    # Older rpm versions < 4.0 (such as those used by SuSE) are not capable of doing rootless builds
    # because they have no %%dev macro, so in that case we just run the makenodes program that was
    # generated in the build to create devices.
    /usr/sbin/strmakenodes --remove
fi
%endif

%if %{FILES_makedev}
%files dev
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%endif

%if %{FILES_builddev}
%files dev -f %{makedev}
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%endif

# ===========================================================================
%package test

Summary:        %{title} test utilities.
Group:          System Environment/Base
Prefix:         %{_libexecdir}/%{name}
Requires:       %{name}-util = %{epoch}:%{version}-%{fullrelease2}
Obsoletes:      %{name}-tests
Conflicts:      %{_conflict}-test
Conflicts:      %{_conflict}-tests
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{virtual}-test = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-test = %{epoch}:%{version}-%{fullrelease}
Provides:       %{virtual}-test = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-test = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description test
%{descripline}
The %{name}-test binary package provides the autotest validation
testsuites used for validating and troubleshooting the %{title}
package.  It also includes the send-pr script for generating and
submitting problem reports to bugs@openss7.org.
THE %{name} PACKAGE VALIDATES POORLY ON MOST ARCHITECTURES AND
DISTRIBUTIONS AND IS ESSENTIALLY UNUSABLE ON SMP.  THIS PACKAGE
IS DEPRECATED.  YOU SHOULD BE USING THE %{_conflict} PACKAGE
INSTEAD.  DO NOT REPORT BUGS.

%if %{FILES_test}
%files test
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%{_libexecdir}/%{name}/atlocal
%{_libexecdir}/%{name}/testsuite*
%{_libexecdir}/%{name}/send-pr
%{_libexecdir}/%{name}/send-pr.config
%endif

# ===========================================================================
%package doc

Summary:        %{title} manpages, manuals and docs.
Group:          Documentation
Prefix:         %{_docdir}
Prefix:         %{_infodir}
Prefix:         %{_mandir}
Conflicts:      texinfo < 3.11
Conflicts:      %{_conflict}-doc
Conflicts:      %{_conflict}-docs
Obsoletes:      %{name}-docs
#Prereq:         /sbin/install-info
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{virtual}-doc = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-doc = %{epoch}:%{version}-%{fullrelease}
Provides:       %{virtual}-doc = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-doc = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description doc
%{descripline}
The %{name}-doc binary package contains text, html, info and pdf
(texinfo) documentation and manuals for the development of
%{title} applications, modules and drivers, as well as the
installation and reference manual.  Also included are extensive
manual pages for the entire package, including manual pages for
commands, administrative utilities, user interfaces, and kernel
functions.  THE %{name} PACKAGE VALIDATES POORLY ON MOST
ARCHITECTURES AND DISTRIBUTIONS AND IS ESSENTIALLY UNUSABLE ON
SMP.  THIS PACKAGE IS DEPRECATED.  YOU SHOULD BE USING THE
%{_conflict} PACKAGE INSTEAD.

%post doc
# ---------------------------------------------------------------------------
if [ $1 = 1 ] ; then : # fresh install
    if [ -x /sbin/install-info ] ; then
        # Older rpm versions do not compress info files.
        for n in %{infofiles} ; do
            if [ -f %{_infodir}/$n.info ] ; then
                /sbin/install-info --info-dir=%{_infodir} %{_infodir}/$n.info
            elif [ -f %{_infodir}/$n.info.gz ] ; then
                /sbin/install-info --info-dir=%{_infodir} %{_infodir}/$n.info.gz
            elif [ -f %{_infodir}/$n.info.bz2 ] ; then
                /sbin/install-info --info-dir=%{_infodir} %{_infodir}/$n.info.bz2
            fi
        done
    fi
    if [ -x /usr/sbin/makewhatis ] ; then
        LANGUAGE= LANG= /usr/sbin/makewhatis -v -u %{_mandir} 2>&1 || :
    fi
    [ ! -f %{_mandir}/%{base}.refs.gz    ] || gunzip  %{_mandir}/%{base}.refs.gz
    [ ! -f %{_mandir}/%{base}.refs.bz2   ] || bunzip2 %{_mandir}/%{base}.refs.bz2
    [ ! -f %{_mandir}/%{base}.macros.gz  ] || gunzip  %{_mandir}/%{base}.macros.gz
    [ ! -f %{_mandir}/%{base}.macros.bz2 ] || bunzip2 %{_mandir}/%{base}.macros.bz2
fi

%preun doc
# ---------------------------------------------------------------------------
if [ $1 = 0 -o $1 = 1 ] ; then : # final removal or upgrade
    if [ -x /sbin/install-info ] ; then
        # Older rpm versions do not compress info files.
        for n in %{infofiles} ; do
            if [ -f %{_infodir}/$n.info ] ; then
                /sbin/install-info --delete --info-dir=%{_infodir} %{_infodir}/$n.info
            elif [ -f %{_infodir}/$n.info.gz ] ; then
                /sbin/install-info --delete --info-dir=%{_infodir} %{_infodir}/$n.info.gz
            elif [ -f %{_infodir}/$n.info.bz2 ] ; then
                /sbin/install-info --delete --info-dir=%{_infodir} %{_infodir}/$n.info.bz2
            fi
        done
    fi
    rm -f -- %{_mandir}/%{base}.refs
    rm -f -- %{_mandir}/%{base}.macros
fi

%postun doc
# ---------------------------------------------------------------------------
if [ $1 = 0 -o $1 = 1 ] ; then : # final removal or upgrade
    if [ -x /usr/sbin/makewhatis ] ; then
        LANGUAGE= LANG= /usr/sbin/makewhatis -v -u %{_mandir} 2>&1 || :
    fi
fi

%if %{FILES_doclibs}
%files doc
# ---------------------------------------------------------------------------
%defattr(-,root,root)
#doc ABOUT-NLS
%doc AUTHORS ChangeLog COPYING INSTALL NEWS README THANKS manual
%doc LSM %{name}-%{version}.lsm
%doc README-make
%{_infodir}/*
%{_mandir}/%{base}.refs*
%{_mandir}/%{base}.macros*
%{_mandir}/man*/*
%endif

%if %{FILES_doc}
%files doc
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%doc AUTHORS ChangeLog COPYING INSTALL NEWS README THANKS manual
%doc LSM %{name}-%{version}.lsm
%{_infodir}/*
%{_mandir}/%{base}.refs*
%{_mandir}/%{base}.macros*
%{_mandir}/man*/*
%endif

# ===========================================================================
%package source

Summary:        %{title} source package.
Group:          Development/System
Prefix:         %{_usrsrc}
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{virtual}-source = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-source = %{epoch}:%{version}-%{fullrelease}
Provides:       %{virtual}-source = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-source = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description source
%{descripline}
The %{name}-source binary package contains the source code
necessary for building the %{name} release.  It also includes
the autoconf configuration utilities necessary to create and
distribute this rpm.  To develop on the package, it would be
better to use the tarball or source rpm release.  THE %{name}
PACKAGE VALIDATES POORLY ON MOST ARCHITECTURES AND DISTRIBUTIONS
AND IS ESSENTIALLY UNUSABLE ON SMP.  THIS PACKAGE IS DEPRECATED.
YOU SHOULD BE USING THE %{_conflict} PACKAGE INSTEAD.

%if %{FILES_source}
%files source
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%dir %{_usrsrc}/%{name}-%{version}-%{fullrelease}
%{_usrsrc}/%{name}
%{_usrsrc}/%{name}-%{version}-%{fullrelease}/*
%{_usrsrc}/%{name}-%{version}-%{fullrelease}/.[^.]*
%endif

# ===========================================================================
%package init

Summary:        %{title} init scripts and config files
Group:          System Environment/Base
Prefix:         %{_configdir}
Prefix:         %{_initrddir}
Prefix:         %{_sysconfdir}
%if %{without_ko_modules}
Prefix:         %{_modutildir}
%endif
Requires:       %{name}-util = %{epoch}:%{version}-%{fullrelease2}
Obsoletes:      %{name}-inits
Conflicts:      %{_conflict}-init
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{virtual}-init = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-init = %{epoch}:%{version}-%{fullrelease}
Provides:       %{virtual}-init = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-init = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description init
%{descripline}
The %{name}-init binary package provides the Sys V init scripts
and configuration files associated with the %{title} package.
These init scripts load kernel modules, create devices, and
apply system configuration customizations at boot.  THE %{name}
PACKAGE VALIDATES POORLY ON MOST ARCHITECTURES AND DISTRIBUTIONS
AND IS ESSENTIALLY UNUSABLE ON SMP.  THIS PACKAGE IS DEPRECATED.
YOU SHOULD BE USING THE %{_conflict} PACKAGE INSTEAD.

%pre init
# ---------------------------------------------------------------------------
arg="$1"
function remove_depmods() {
    local t
    [ $# -gt 0 ] || return 0
    echo "$*"
    for t in $@ ; do
        prereq=prereq_`echo $t | sed -r -e 's|^.*/||;s|\.(k)?o(\.gz)?(:)?$||;s|[^a-zA-Z0-9_]|_|g'`
        eval "remove_depmods \$$prereq"
    done
}
# This is an initial installation of the init subpackage.  We need to check for non-rpm
# installations on the host and remove any entries that might later conflict with our entries.  We
# treat every fresh installation like an upgrade from non-deb/rpm install.
if [ $arg = 1 ] ; then : # fresh install
    # When performing a fresh install we want to go looking for non-rpm installations of LiS or
    # STREAMS or supplementary packages and perform what would otherwise have been the preun or
    # postun actions.
    list="%{modules}" ; modl_mods=
    for dir in %{_kmoduledir}/../* ; do
        [ -d $dir ] || continue
	for m in `find $dir \( -name 'streams*.o' -o -name 'streams*.ko' -o -name 'streams*.o.gz' -o -name 'streams*.ko.gz' \) 2>/dev/null` ; do
		b=`echo $m | sed -r -e 's|^.*/||;s|\.(k)?o(\.gz)?||'`
		case " $list " in (*" $b "*) ;; (*) continue ;; esac
		echo "$m"
		modl_mods="${modl_mods:+$modl_mods }$m"
	done
        if [ -n "$modl_mods" -a -f $dir/modules.dep ] ; then
            # First we can build a dependency tree from every dependency expression that can be
            # found in the modules.dep file.
            xtrace=`shopt -p -o | grep xtrace` ; set +x
            dep= ; while read line ; do
                case $line in
                    (*\\)   line="`echo $line | sed -e 's|\\$||'`"
                            dep="${dep:+$dep }$line" ; continue ;;
                    (*)	    dep="${dep:+$dep }$line" ;;
                esac
                [ -n "$dep" ] || continue;
                set dummy $dep
                if [ "${3+set}" = "set" ] ; then
                    t=`echo $2 | sed -e 's|:$||'`
                    target=target_`echo $t | sed -r -e 's|^.*/||;s|\.(k)?o(\.gz)?(:)?$||;s|[^a-zA-Z0-9_]|_|g'`
                    shift 2
                    for p in $@ ; do
                        prereq=prereq_`echo $p | sed -r -e 's|^.*/||;s|\.(k)?o(\.gz)?(:)?$||;s|[^a-zA-Z0-9_]|_|g'`
                        eval "$prereq=\"\${$prereq:+\$$prereq }$t\""
                        eval "$target=\"\${$target:+\$$target }$p\""
                    done
                fi
                dep=
            done < $dir/modules.dep
            eval "$xtrace"
        fi
        conf_old=%_sysconfdir/modules.conf
        if [ -f $conf_old ] ; then
            conf_new=%_tmppath/modules.conf.new.$$
            conf_tmp=%_tmppath/modules.conf.tmp.$$
            cp -f -- $conf_old $conf_new
            # Unfortunately davem starting copying our install procedure, so we need to root out all
            # kinds of things from the modules.conf file, all prune statements, any if include
            # statements
            for suffix in %name LiS lis streams STREAMS strcompat strutil strxns strxnet strinet strsctp strss7 ; do
                # This the old style /BEGIN LiS/,/END LiS/ enclosure in the modules.conf file.
                if ( grep -q -- "BEGIN $suffix" $conf_new && grep -q -- "END $suffix" $conf_new ) 2>/dev/null ; then
                    sed -e '\|BEGIN '$suffix'|,\|END '$suffix'|d'            $conf_new > $conf_tmp
                    mv -f -- $conf_tmp $conf_new
                fi
                # This is my newer technique with a prune and an if statement in the modules.conf
                # file that was also used by davem in LiS-2.18.0.
                if [ -f $dir/modules.$suffix ] ; then
                    rm -f -- $dir/modules.$suffix
                    sed -e '\|^prune[[:space:]].*modules.'$suffix'|d'        $conf_new > $conf_tmp
                    sed -e '\|^if[[:space:]].*modules.'$suffix'|,\|^endif|d' $conf_tmp > $conf_new
                    sed -e '\|^include[[:space:]].*modules.'$suffix'|d'      $conf_new > $conf_tmp
                    mv -f -- $conf_tmp $conf_new
                fi
                # This is my newer latest technique with only an include statement in modules.conf
                # and a modules.conf fragment in the debian update-modules directory _modutildir
                # that includes the prune and if statements.  That works both for RH-style and
                # Debian-style modules.conf.
                if [ -f %_modutildir/$suffix ] ; then
                    rm -f -- %_modutildir/$suffix
                    sed -e '\|^include[[:space:]].*%_modutildir/'$suffix'|d' $conf_new > $conf_tmp
                    mv -f -- $conf_tmp $conf_new
                fi
            done
            chmod --reference=$conf_old -- $conf_new
            cp -f -b --suffix=.rpmsave -- $conf_new $conf_old
            rm -f -- $conf_new
        fi
    done
%if %{without_ko_modules}
    prel_old=%_sysconfdir/modules
%else
    prel_old=%_sysconfdir/modprobe.preload
%endif
    if [ -f $prel_old ] ; then
        prel_tmp=%_tmppath/modprobe.preload.tmp.$$
        prel_new=%_tmppath/modprobe.preload.new.$$
        cp -f -- $prel_old $prel_new
        sed -e '\|^[[:space:]]*streams|d' $prel_new > $prel_tmp
        mv -f -- $prel_tmp $prel_new
        chmod --reference=$prel_old -- $prel_new
        cp -f -b --suffix=.rpmsave -- $prel_new $prel_old
        rm -f -- $prel_new
    fi
    # Fresh install, go looking for non-rpm initscripts and remove them.
    list='%{initfiles}' ; for i in $list strms_up strms_dn ; do
        if [ -f %_initrddir/$i ] ; then
            # Find the best working init script removal tool
            [ -x /sbin/insserv             ] && /sbin/insserv -r          $i 2>&1 && continue
            [ -x /sbin/chkconfig           ] && /sbin/chkconfig --del     $i 2>&1 && continue
            [ -x /usr/lib/lsb/remove_initd ] && /usr/lib/lsb/remove_initd $i 2>&1 && continue
        fi
    done
    # remove conflicting and dependent modules
    if [ -n "$modl_mods" ] ; then
        rm -f -- $modl_mods
        for m in $list ; do
            prereq=prereq_`echo $m | sed -r -e 's|^.*/||;s|\.(k)?o(\.gz)?(:)?$||;s|[^a-zA-Z0-9_]|_|g'`
            eval "modl_deps=\"\`remove_depmods \$$prereq\`\""
            [ -n "$modl_deps" ] || continue
            rm -f -- $modl_deps
        done
    fi
fi
if [ $arg = 2 ] ; then : # upgrade
    # Because we are doing an upgrade, the preun and postun scriplets for the old rpm are going to
    # be called.  Hopefully the old rpm can handle removal correctly.
fi

%post init
# ---------------------------------------------------------------------------
if [ $1 = 1 ] ; then : # fresh install
%if %{without_ko_modules}
    # At the end of a fresh install we need to add our lines to the modules.conf file if it exists,
    # and it is necessary to use a modconf file (2.4 and pre 2.5.48 kernel), an even if the line is
    # already there (in which case we remove the existing line).
    conf_old=%_sysconfdir/modules.conf
    if [ -f $conf_old ] ; then
        conf_tmp=%_tmppath/modules.conf.tmp.$$
        conf_new=%_tmppath/modules.conf.new.$$
        cp -f -- $conf_old $conf_new
        ( sed -e '\|^include[[:space:]].*%_modutildir/%base|d' $conf_new
          echo "include %_modutildir/%base" ) > $conf_tmp
        mv -f -- $conf_tmp $conf_new
        chmod --reference=$conf_old $conf_new
        cp -f -b --suffix=.rpmsave -- $conf_new $conf_old
        rm -f -- $conf_new
    fi
%endif
%if %{without_ko_modules}
    prel_old=%_sysconfdir/modules
%else
    prel_old=%_sysconfdir/modprobe.preload
%endif
%if %{have_preloads}
    # At the end of a fresh install we need to add our preloads to the modules or modprobe.preload
    # file, if it exists, and is necessary, and even if the lines are already there (in which case
    # we remove the existing line.
    if [ -f $prel_old ] ; then
        prel_tmp=%_tmppath/modprobe.preload.tmp.$$
        prel_new=%_tmppath/modprobe.preload.new.$$
        cp -f -- $prel_old $prel_new
        mods='%{preloads}' ; for m in $mods ; do
            ( sed -e '\|^[[:space:]]*'"$m"'[[:space:]]*$|d' $prel_new
              echo "$m" ) > $prel_tmp
            mv -f -- $prel_tmp $prel_new
        done
        chmod --reference=$prel_old -- $prel_new
        cp -f -b --suffix=.rpmsave -- $prel_new $prel_old
        rm -f -- $prel_new
    fi
%endif
fi
if [ $1 = 2 ] ; then : # upgrade
%if %{without_ko_modules}
    # At the end of an upgrade we place our new lines in the modules.conf file.
    conf_old=%_sysconfdir/modules.conf
    if [ -f $conf_old ] ; then
        conf_tmp=%_tmppath/modules.conf.tmp.$$
        conf_new=%_tmppath/modules.conf.new.$$
        cp -f -- $conf_old $conf_new
        ( cat $conf_new
          echo "include %_modutildir/%base" ) > $conf_tmp
        mv -f -- $conf_tmp $conf_new
        chmod --reference=$conf_old $conf_new
        cp -f -b --suffix=.rpmsave -- $conf_new $conf_old
        rm -f -- $conf_new
    fi
%endif
%if %{without_ko_modules}
    prel_old=%_sysconfdir/modules
%else
    prel_old=%_sysconfdir/modprobe.preload
%endif
%if %{have_preloads}
    # At the end of an upgrade we place just our preloads in the modules or modprobe.preload file.
    if [ -f $prel_old ] ; then
        prel_tmp=%_tmppath/modprobe.preload.tmp.$$
        prel_new=%_tmppath/modprobe.preload.new.$$
        cp -f -- $prel_old $prel_new
        mods='%{preloads}' ; for m in $mods ; do
            ( cat $repl_new ; echo "$m" ) > $prel_tmp
            mv -f -- $prel_tmp $prel_new
        done
        chmod --reference=$prel_old -- $prel_new
        cp -f -b --suffix=.rpmsave -- $prel_new $prel_old
        rm -f -- $prel_new
    fi
%endif
%if %{have_initscripts}
    # This is the end of a fresh install, add init scripts if we can
    list='%{initfiles}' ; for i in $list ; do
        # Find the best working init script installation tool
        [ -x /sbin/insserv              ] && /sbin/insserv              $i 2>&1 && continue
        [ -x /sbin/chkconfig            ] && /sbin/chkconfig --add      $i 2>&1 && continue
        [ -x /usr/lib/lsb/install_initd ] && /usr/lib/lsb/install_initd $i 2>&1 && continue
    done
%endif
fi

%preun init
# ---------------------------------------------------------------------------
if [ $1 = 0 ] ; then : # final removal
%if %{without_ko_modules}
    # When we are going away altogether we want to remove any lines whatsoever from the modules.conf
    # file.
    conf_old=%_sysconfdir/modules.conf
    if [ -f $conf_old ] ; then
        conf_tmp=%_tmppath/modules.conf.tmp.$$
        conf_new=%_tmppath/modules.conf.new.$$
        cp -f -- $conf_old $conf_new
        sed -e '\|^include[[:space:]].*%_modutildir/%base|d' $conf_new > $conf_tmp
        mv -f -- $conf_tmp $conf_new
        chmod --reference=$conf_old $conf_new
        cp -f -b --suffix=.rpmsave -- $conf_new $conf_old
        rm -f -- $conf_new
    fi
%endif
%if %{without_ko_modules}
    prel_old=%_sysconfdir/modules
%else
    prel_old=%_sysconfdir/modprobe.preload
%endif
%if %{have_preloads}
    # When we are going away altogether we want to remove any of our preloads whatsoever from the
    # modules or modprobe.preload file.
    if [ -f $prel_old ] ; then
        prel_tmp=%_tmppath/modprobe.preload.tmp.$$
        prel_new=%_tmppath/modprobe.preload.new.$$
        cp -f -- $prel_old $prel_new
        mods='%{preloads}' ; for m in $mods ; do
            sed -e '\|^[[:space:]]*'"$m"'[[:space:]]*$|d' $repl_new > $prel_tmp
            mv -f -- $prel_tmp $prel_new
        done
        chmod --reference=$prel_old -- $prel_new
        cp -f -b --suffix=.rpmsave -- $prel_new $prel_old
        rm -f -- $prel_new
    fi
%endif
%if %{have_initscripts}
    # Uninstalling for good, remove init scripts if we can
    list='%{initfiles}' ; for i in $list ; do
        if [ -f %_initrddir/$i ] ; then
            # rpm is not too good about the order of removal
            /sbin/service $i stop 2>&1 || %_initrddir/$i stop 2>&1 || :
            # Find the best working init script removal tool
            [ -x /sbin/insserv             ] && /sbin/insserv -r          $i 2>&1 && continue
            [ -x /sbin/chkconfig           ] && /sbin/chkconfig --del     $i 2>&1 && continue
            [ -x /usr/lib/lsb/remove_initd ] && /usr/lib/lsb/remove_initd $i 2>&1 && continue
        fi
    done
%endif
fi
if [ $1 = 1 ] ; then : # upgrade
%if %{without_ko_modules}
    # At the end of the upgrade (when _this_package_ is upgraded from) we remove our lines from the
    # modules.conf file that we placed in them in the post scriptlet.  We only adjust our line and
    # leave any lines added by the new package post scriptlet.
    conf_old=%_sysconfdir/modules.conf
    if [ -f $conf_old ] ; then
        conf_tmp=%_tmppath/modules.conf.tmp.$$
        conf_new=%_tmppath/modules.conf.new.$$
        cp -f -- $conf_old $conf_new
        # This groovy sed script just pulls out the first matching line and leaves the rest
        # untouched.
        sed -e '
\|^include[[:space:]].*%_modutildir/%base|!b
N
s|^include[[:space:]].*%_modutildir/%base.*\
||
: loop
n
b loop' $conf_new > $conf_tmp
        mv -f -- $conf_tmp $conf_new
        chmod --reference=$conf_old $conf_new
        cp -f -b --suffix=.rpmsave -- $conf_new $conf_old
        rm -f -- $conf_new
    fi
%endif
%if %{without_ko_modules}
    prel_old=%_sysconfdir/modules
%else
    prel_old=%_sysconfdir/modprobe.preload
%endif
%if %{have_preloads}
    # At the end of the upgrade (when _this_package_ is upgraded from) we remove our lines from the
    # modules or modprobe.preloads file that we placed in them in the post scriptlet.  We only
    # adjust our lines and leave any lines added by the new package post scriptlet.
    if [ -f $prel_old ] ; then
        prel_tmp=%_tmppath/modprobe.preload.tmp.$$
        prel_new=%_tmppath/modprobe.preload.new.$$
        cp -f -- $prel_old $prel_new
        mods='%{preloads}' ; for m in $mods ; do
            # This groovy sed script just pulls out the first matching line and leaves the rest
            # untouched.
            sed -e '
\|^[[:space:]].*'"$m"'[[:space:]]*$|!b
N
s|^[[:space:]].*'"$m"'[[:space:]]*\
||
: loop
n
b loop' $prel_new > $prel_tmp
            mv -f -- $prel_tmp $prel_new
        done
        chmod --reference=$prel_old -- $prel_new
        cp -f -b --suffix=.rpmsave -- $prel_new $prel_old
        rm -f -- $prel_new
    fi
%endif
fi

%if %{FILES_init}
%files init
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%{_sysconfdir}/streams.conf
%{_initrddir}/../*
%{_configdir}/*
%{_modutildir}/*
%endif

%if %{FILES_ko_init}
%files init
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%{_sysconfdir}/streams.conf
%{_initrddir}/../*
%{_configdir}/*
%endif

%if %{FILES_noinit}
%files init
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%{_modutildir}/*
%endif

%if %{FILES_ko_noinit}
%files init
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%endif

# ===========================================================================
%changelog
# ---------------------------------------------------------------------------
# LiS.spec.in,v
# Revision 1.1.6.59  2006/07/07 21:08:35  brian
# - place all header files in development subpackage
#
# Revision 1.1.6.58  2006/05/08 03:33:25  brian
# - updated headers, corrected init scrip removal
#

* Fri Mar 24 2006 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.57  2006/03/24 10:02:37  brian
- added send-pr to %package test description

* Fri Mar 24 2006 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.56  2006/03/24 00:03:27  brian
- added send-pr files to distribution

* Mon Mar 20 2006 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.55  2006/03/20 11:54:40  brian
- tied in --disable-devel flag

* Tue Mar 14 2006 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.54  2006/03/14 21:09:49  brian
- two levels of distro specific rpm extra tags

* Fri Mar 10 2006 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.53  2006/03/10 12:03:15  brian
- updated descriptions and headers

* Thu Mar  9 2006 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.52  2006/03/09 14:02:52  brian
- updating descriptions

* Mon Feb 20 2006 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.51  2006/02/20 10:06:59  brian
- changes for 64 bit distributions

* Fri Dec 30 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.50  2005/12/30 12:21:10  brian
- updating release notes

* Fri Dec 23 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.49  2005/12/23 20:28:22  brian
- use lsb initd last

* Fri Dec 23 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.48  2005/12/23 05:23:27  brian
- corrections

* Thu Dec 22 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.47  2005/12/22 15:16:16  brian
- different approach to compressed marco files

* Thu Dec 22 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.46  2005/12/22 13:51:22  brian
- tough build for RH 7.3

* Thu Dec 22 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.45  2005/12/22 10:26:04  brian
- a little overzealous when deleting from modules.conf and modutildir

* Thu Dec 22 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.44  2005/12/22 07:26:42  brian
- make command location a little more portable

* Wed Dec 21 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.43  2005/12/21 06:07:10  brian
- cleaned up versioning for RPM builds

* Mon Dec 19 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.42  2005/12/19 12:42:36  brian
- locking down for release

* Fri Dec 16 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.41  2005/12/16 12:13:49  brian
- build does not require texinfo

* Fri Dec 16 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.40  2005/12/16 09:23:59  brian
- updated to support SuSE/UL and LSB init scripts

* Wed Dec 14 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.39  2005/12/14 11:40:50  brian
- quiet rpm -ihv output

* Tue Dec 13 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.38  2005/12/13 13:42:15  brian
- corrected nested ifs unusable by rpm

* Sun Nov 13 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.37  2005/11/13 11:00:30  brian
- FC4 wants kernel-devel instead of kernel-source

* Sun Nov  6 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.36  2005/11/06 10:59:06  brian
- updates

* Wed Sep 14 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.35  2005/09/14 23:40:51  brian
- bumped release numbers and change logs

* Thu Jul  7 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.34  2005/07/07 20:28:31  brian
- changes for release

* Wed Jul  6 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.33  2005/07/06 04:14:25  brian
- added requirement for strcompat package

* Sat Jun 25 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.32  2005/06/25 07:01:47  brian
- more efficient dependency checking

* Fri Jun 24 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.31  2005/06/24 13:40:40  brian
- some install/erase script corrections

* Fri Jun 24 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.30  2005/06/24 09:00:34  brian
- init package must be installed first because it cleans
  out any non-rpm modules and other things before the first
  fresh RPM install
- this wasn't happending on 2.6 systems

* Tue May 31 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.29  2005/05/31 05:10:39  brian
- correct case on modutildir filename

* Mon May 30 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.28  2005/05/30 14:25:50  brian
- packaged standalong test suites

* Sun May 29 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.27  2005/05/29 11:28:33  brian
- better approach to mostlyclean

* Sun May 29 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.26  2005/05/29 07:49:26  brian
- updates and corrections for SuSE 9.2 build

* Sat May 28 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.25  2005/05/28 05:07:01  brian
- do not provide host if no architecture

* Fri May 27 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.24  2005/05/27 20:03:42  brian
- suppress excessive shell debugging statements

* Thu May 26 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.23  2005/05/26 09:59:32  brian
- a few changes for WBEL3 release build

* Thu May 19 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.22  2005/05/19 20:48:12  brian
- suppress shell trace in loops

* Sat May 14 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.21  2005/05/14 08:35:00  brian
- copyright header correction

* Wed May 11 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.20  2005/05/11 07:58:49  brian
- add mention of 2.6 kernel support

* Sat Apr  9 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.19  2005/04/09 09:34:55  brian
- update spec files

* Tue Apr  5 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.18  2005/04/05 01:57:04  brian
- more robust sed

* Sat Apr  2 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.17  2005/04/02 22:13:39  brian
- remove _streams macro from streams packages

* Sat Apr  2 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.16  2005/04/02 16:47:25  brian
- improvements for arch indep build

* Sat Apr  2 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.15  2005/04/02 16:24:59  brian
- more adjustments to arch indep build

* Sat Apr  2 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.14  2005/04/02 03:23:38  brian
- build noarch rpm packages

* Tue Mar 29 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.13  2005/03/29 17:16:28  brian
- Updated lsms and added LSM to distribution.

* Mon Mar 28 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.12  2005/03/28 07:42:28  brian
- better debian build

* Sat Mar 26 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.11  2005/03/26 03:11:09  brian
- Reworking maintainance scripts

* Thu Mar 24 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.10  2005/03/24 05:18:53  brian
- always include sysconfdir in util package

* Thu Mar 24 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.9  2005/03/24 01:38:16  brian
- better debian build

* Tue Mar 22 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.8  2005/03/22 19:44:13  brian
- do not need symbolic link in prep anymore

* Thu Mar 17 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.7  2005/03/17 11:22:33  brian
- Updated change logs.

* Wed Mar 16 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.6  2005/03/16 11:58:23  brian
- enhance devfs support

* Wed Mar 16 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.5  2005/03/16 10:48:37  brian
- straighten out devfs and get source urls correct

* Wed Mar 16 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.4  2005/03/16 05:46:55  brian
- correction to chkconfig arguments

* Tue Mar 15 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.3  2005/03/15 10:41:49  brian
- Updated changelog and a few inconsistencies.

* Mon Mar 14 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.2  2005/03/14 01:22:12  brian
- Added init scripts.

* Wed Mar  9 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.6.1  2005/03/09 23:13:43  brian
- First stab at autoconf'ed 2.18.0.  Results of merge.

* Wed Mar  9 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.81  2005/03/09 12:14:21  brian
- Corrected problematic conditionals.

* Wed Mar  9 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.80  2005/03/09 08:00:22  brian
- Updated revision numbering.

* Wed Mar  9 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.79  2005/03/09 03:40:46  brian
- Major revision number changes.

* Tue Mar  8 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.78  2005/03/08 10:52:57  brian
- Correction to modules and tools in spec files.

* Mon Mar  7 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.77  2005/03/07 06:09:26  brian
- More spec changes for 2.6.

* Fri Feb 25 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.76  2005/02/25 10:56:39  brian
- Don't specify build architecture.

* Tue Feb 22 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.75  2005/02/22 08:32:02  brian
- spec changes for updated makefile fragments


* Sat Feb 19 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.74  2005/02/19 12:06:51  brian
- rpm-epoch to pkg-epoch, rpm-release to pkg-release
- proper quotation of configure options

* Tue Feb 15 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.73  2005/02/15 14:15:18  brian
- Remove link before linking.

* Thu Feb 10 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.72  2005/02/10 12:56:25  brian
- Minor adjustments for compressed modules.

* Mon Feb  7 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.71  2005/02/07 22:23:01  brian
- Updated changelog.

* Thu Feb  3 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.70  2005/02/03 12:34:52  brian
- Spec file grand rework.

* Wed Feb  2 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.69  2005/02/02 10:43:01  brian
- Build in builddir again.

* Tue Feb  1 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.68  2005/02/01 19:01:24  brian
- Working up master build.

* Sat Jan 29 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.67  2005/01/29 11:22:39  brian
- Have master build working well.

* Fri Jan 21 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.66  2005/01/21 08:39:16  brian
- Work around mandrake spec-helper for uncooked manpages.

* Thu Jan 20 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.65  2005/01/20 00:51:52  brian
- Go back to quick no dependencies compile.

* Wed Jan 19 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.64  2005/01/19 07:38:22  brian
- Added checks and autotest options to configure.

* Tue Jan 18 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.63  2005/01/18 20:23:43  brian
- Also reference makedev file one down.

* Tue Jan 18 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.62  2005/01/18 13:31:09  brian
- Support multiple builds on same NFS mount.

* Tue Jan 18 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.61  2005/01/18 10:42:25  brian
- Rationalize to new separated build approach.

* Sat Jan 15 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.60  2005/01/15 19:42:55  brian
- Added kernel module checks.

* Thu Jan 13 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.59  2005/01/13 12:18:07  brian
- *** empty log message ***

* Mon Jan 10 2005 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.58  2005/01/10 18:59:07  brian
- mandrake places libexec files in /usr/lib, correct globbing
- updated change logs

* Thu Dec 29 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.57  2004/12/30 21:22:18  brian
- Mandrake compresses info files with bzip2.

* Thu Dec 29 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.56  2004/12/30 08:05:02  brian
- Fix globbing on source package.

* Wed Dec 29 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.55  2004/12/29 21:34:50  brian
- Need lower case base name.

* Wed Dec 29 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.54  2004/12/29 21:32:18  brian
- Straighten out cache and site files.

* Wed Dec 29 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.53  2004/12/29 07:23:10  brian
- Added missing config_site macro definition.

* Tue Dec 21 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.52  2004/12/21 22:22:11  brian
- Added automatic site configuration to rpm build.

* Sun Dec 19 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.51  2004/12/19 15:11:42  brian
- Working up better spec files.

* Sun Dec 19 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.50  2004/12/19 08:49:48  brian
- Updating build process.

* Sat Dec 18 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.49  2004/12/18 14:22:53  brian
- Cache configure results.

* Fri Dec 17 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.48  2004/12/17 13:31:16  brian
- Cleared up build depdendencies, added info subpackage to LiS and streams.

* Fri Dec 17 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.47  2004/12/17 04:25:39  brian
- Missing hugemem.

* Fri Dec 17 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.46  2004/12/17 04:02:31  brian
- Improving spec files.

* Wed Dec 15 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.45  2004/12/15 15:25:33  brian
- avoid undefined debug macro

* Wed Dec 15 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.44  2004/12/15 14:59:06  brian
- Update RPM build to build multiple distribution binary RPMs from a single SRPM.

* Mon Dec 13 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.43  2004/12/13 10:52:02  brian
- Removed vendor specific headers. (Define these in your .rpmmacros file.)
- Added both .xpm and .gif icons into binary packages.
- Fixed extrainfo empty macro body warning once and for all.

* Wed Nov 24 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.42  2004/11/24 14:30:45  brian
- Changes for multiple rpm distro builds.

* Wed Nov 24 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.41  2004/11/24 11:27:50  brian
- Changes to accomodate %_vendor macro for multiple distribution rpm builds.

* Sat Nov  6 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.40  2004/11/09 11:49:41  brian
- Changelog updates.

* Sat Nov  6 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.39  2004/11/06 09:16:48  brian
- Changes to fully support symbol versioning.

* Wed Oct 13 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.38  2004/10/13 01:49:05  brian
- Updates spec changelogs.

* Tue Aug 31 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.37  2004/08/31 04:49:03  brian
- Avoid empty macro body error on extrainfo.

* Mon Aug 23 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.36  2004/08/23 11:46:54  brian
- Minor corrections and updates.

* Sun Aug 22 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.35  2004/08/22 05:07:16  brian
- Converted to common file operation.

* Tue Aug 17 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.34  2004/08/17 11:44:40  brian
- Upgraded spec files.

* Fri Aug 13 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.33  2004/08/13 07:08:40  brian
- Preparation for release.

* Sat Aug  7 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.32  2004/08/07 11:00:42  brian
- More rational provides structure.

* Sun Jul 11 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.31  2004/07/11 08:46:30  brian
- Corrected symbolic link again.

* Mon Jun 28 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.30  2004/06/28 19:40:21  brian
- Do not package up debug files on recent rpmbuild.

* Mon Jun 28 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.29  2004/06/28 11:33:10  brian
- Support binary release.

* Mon Jun 28 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.28  2004/06/28 09:01:17  brian
- Updated spec files.

* Sun Jun 27 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.27  2004/06/27 10:15:26  brian
- A little factoring.

* Sat Jun 26 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.26  2004/06/27 04:22:09  brian
- Change modules subdirectory and extension to 'lis'.

* Sat Jun 26 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.25  2004/06/27 03:05:23  brian
- Removed XTI/TLI and Linux Networking.
- updated changelog

* Sat Jun 26 2004 Brian Bidulock <bidulock@openss7.org>
- removed references to XTI/TLI
- removed tli, inet, and xnet options
- removed libxnet manual
- skip locale files (was only for libxnet)
- updated changelog

* Mon Jun 21 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.24  2004/06/21 18:08:35  brian
- RPM 4.2.1 and up set default epoch to 0.

* Sun Jun 20 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.23  2004/06/20 20:22:16  brian
- FC1 rpm 4.2.1 can't handle internal Requires.

* Sun Jun  6 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.22  2004/06/06 03:32:50  brian
- Updated changelog.

* Thu Jun  3 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.21  2004/06/03 20:13:24  brian
- Incorporated HP patches from Nicolas THOMAS.

* Mon May 24 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.20  2004/05/24 20:49:09  brian
- Don't copy sources if source package not built.

* Mon May 24 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.19  2004/05/24 12:48:56  brian
- Updated build process.

* Sun May 23 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.18  2004/05/23 08:40:53  brian
- Corrected info install.

* Thu May 20 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.17  2004/05/20 09:00:58  brian
- Place DESTDIR definition after make.

* Mon May 17 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.16  2004/05/17 19:07:37  brian
- Added ability to build tools or modules separately.

* Mon May 17 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.15  2004/05/17 08:27:34  brian
- Need locale directory for libLiS.

* Sun May 16 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.14  2004/05/16 19:16:32  brian
- Updated info install.

* Thu May 13 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.13  2004/05/13 09:06:22  brian
- made tli modules, inet driver and xnet library optional
- added HTML output to texinfo build
- passes distcheck

* Sun Apr 25 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.12  2004/04/25 07:50:35  brian
- Updated change log.

* Sun Apr 25 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.11  2004/04/25 07:49:42  brian
- Added streams to conflicts list.
- Check for existence of devices list before copying.
- Change devs subpackage to dev, libs subpackage to lib, utils subpackage to util.
- Install prebuild PDF versions of the manuals in the doc directory.
- Updated change log.

* Fri Apr 23 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.10  2004/04/23 08:30:43  brian
- added propagation of debugging flags
- make dependencies, devices list and pdfs separately (just in case)
- parameterized devices list
- copy devices list instead of regenerating it
- updated change logs

* Tue Apr 13 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.9  2004/04/13 21:47:33  brian
- Updated change logs.

* Wed Apr  7 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.8  2004/04/07 22:22:40  brian
- Updating build system.

* Mon Apr  5 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.7  2004/04/05 02:10:57  brian
- Cooked manpages and other general rearrangements.

* Thu Mar  4 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.6  2004/03/04 10:44:42  brian
- corrected info (un)install for older rpm versions

* Sun Feb 29 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.5  2004/02/29 10:23:51  brian
- Corrected info (un)install for older rpm versions.

* Fri Jan 23 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.4  2004/01/23 12:14:32  brian
- Corrected option misspelling.

* Tue Jan 20 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.3  2004/01/20 00:03:01  brian
- Fixed conflict with openss7 release headers and man pages.
- Added CVS ignore files.

* Sat Jan 17 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.29  2004/01/17 10:11:37  brian
- Removed man pages and headers that conflict with openss7 release.

* Tue Jan 13 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.2  2004/01/13 16:11:31  brian
- Improvements to buid process.  Common m4 files for kernel and rpm.

* Mon Jan 12 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.4.1  2004/01/12 23:32:35  brian
- Updated LiS-2.16.18 gcom release to autoconf.

* Mon Jan 12 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.28  2004/01/12 22:48:35  brian
- Spelling correction.

* Fri Jan  9 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.27  2004/01/09 06:52:08  brian
- Do no build devs package when without tools specified.

* Wed Jan  7 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.26  2004/01/07 11:34:09  brian
- Updated copyright dates.

* Wed Jan  7 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.25  2004/01/07 10:19:24  brian
- Added installation of locale files.

* Sun Jan  4 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.24  2004/01/04 23:28:45  brian
- corrected removal of double directory references
- Removed double reference to include directories
  and files.

* Sun Jan  4 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.23  2004/01/04 11:29:01  brian
- Placed icons back in packages for rpm 4.0.4.

* Sun Jan  4 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.22  2004/01/04 00:52:57  brian
- Let rpm do the stripping (for debug info in rpm 4.2)
- Remove duplicate Icon entries.

* Thu Jan  1 2004 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.21  2004/01/01 22:59:26  brian
- Some corrections for RedHat 9/gcc3 compile.

* Mon Dec 29 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.20  2003/12/29 05:38:11  brian
- tirdwr module passing initial test cases.

* Sun Dec 28 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.19  2003/12/28 10:50:00  brian
- corrected typing errors
- performed depmod on install against non-running kernel
- corrected iBCS device conflicts

* Tue Dec 23 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.18  2003/12/23 11:26:10  brian
- Finalizing addition of INET streams and libxnet.

* Tue Dec 23 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.17  2003/12/23 11:21:18  brian
- Added directory for /dev/inet devices.

* Tue Dec 23 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.16  2003/12/23 04:08:54  brian
- Fixed removal of modules.conf line additions.

* Wed Dec 17 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.15  2003/12/17 00:23:16  brian
- Added XTI/TLI package into release.

* Tue Dec 16 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.14  2003/12/16 16:20:52  brian
- Added icons to binary packages.
- Modified modprobe removal to get entire stack.

* Tue Dec 16 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.13
- More corrections to module loading and versioning.

* Tue Dec 16 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.12
- Corrections to modules.conf permissions and handling
- Corrections to pre and post handling on core modules

* Mon Dec 15 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.11 
- Added texinfo docs and logos.

* Sat Dec 13 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.10
- Corrections to icon bundling.

* Sat Dec 13 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.9
- removed a lot of redundant files and added devices to
  spec file

* Sat Dec 13 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.8
- final minor changes

* Fri Dec 12 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.7
- added makedevices or LIS_MKNOD script for adding and
  removing devices

* Thu Dec 11 2003 Brian Bidulock <bidulock@openss7.org>
- converted to use libtool to strip libraries, binaries
  and modules

* Thu Dec 11 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.6
- updated spec for post and pre, started on strmknods
  script
- Completed pre/post scripts and added devices with
  modules

* Thu Dec 11 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.5
- Added manual pages and [M]akefile.am's

* Thu Dec 11 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.4
- Many improvements to RPM build process
- autoreconf doesn't like makefile.am so changed back to
  Makefile.am

* Wed Dec 10 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.3
- Removed old makefiles.

* Wed Dec 10 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.2
- Minor changes to targets.

* Wed Dec 10 2003 Brian Bidulock <bidulock@openss7.org>
- Revision 1.1.2.1
- Start of autoconf changes.

* Wed Nov 26 2003 Brian Bidulock <bidulock@openss7.org>
- Began this RPM build for LiS.

# vim: ft=spec sw=4 et tw=78 com=b\:#,fb\:- fo+=tcqlorn nocin nosi
