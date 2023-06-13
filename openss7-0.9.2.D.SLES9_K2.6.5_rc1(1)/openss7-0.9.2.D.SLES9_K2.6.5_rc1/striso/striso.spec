# vim: ft=spec sw=4 et tw=78 com=b\:#,fb\:- fo+=tcqlorn nocin nosi
# =============================================================================
# 
# @(#) striso.spec.in,v OpenSS7-0_9_2_D_rc2(0.9.2.2) 2006/05/08 03:33:31
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
# Last Modified 2006/05/08 03:33:31 by brian
#
# =============================================================================

# default is to build kernel modules and tools
# conditional builds of binary rpms relies on the sneaky spec trick that
# rpmbuild will not build a binary rpm when it has no %files section
%global BUILD_modules 1
%global BUILD_devel   1
%global BUILD_tools   1
%global BUILD_libs    0
%global BUILD_noarch  1
%global BUILD_arch    1
%global BUILD_source  1
%global BUILD_test    1
%global BUILD_devs    1
%global MAKE_devs     0
%global BUILD_lis     0
%global BUILD_lfs     0

%ifnarch noarch
%global BUILD_lfs     1
%endif

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
%global majbase     180
%global makedev     devices.lst
%global epoch       0
%global base        striso
%global title       OpenSS7 ISO Stack
%global stitle      ISO

%global modules     streams-x224
#global preloads    
#global libraries   
%global tools       strss7_mknod
%global infofiles   %{name}
#global initfiles

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
%{?_without_lis:     %{expand: %%global BUILD_lis     0}}
%{?_without_lis:     %{expand: %%global BUILD_lfs     1}}
%{?_without_lfs:     %{expand: %%global BUILD_lfs     0}}
%{?_without_lfs:     %{expand: %%global BUILD_lis     1}}

%{?_with_devfs:      %{expand: %%global NEED_devs     0}}
%{?_with_modules:    %{expand: %%global BUILD_modules 1}}
%{?_with_devel:      %{expand: %%global BUILD_devel   1}}
%{?_with_tools:      %{expand: %%global BUILD_tools   1}}
%{?_with_indep:      %{expand: %%global BUILD_noarch  1}}
%{?_with_arch:       %{expand: %%global BUILD_arch    1}}
%{?_with_public:     %{expand: %%global BUILD_source  1}}
%{?_with_lis:        %{expand: %%global BUILD_lis     1}}
%{?_with_lis:        %{expand: %%global BUILD_lfs     0}}
%{?_with_lfs:        %{expand: %%global BUILD_lfs     1}}
%{?_with_lfs:        %{expand: %%global BUILD_lis     0}}

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

%global _STREAMS SVR 4.2
%global _streams STREAMS
%global _ksubdir streams
%global _kmodext streams
%if %{BUILD_lfs}
%global _STREAMS LfS
%global _streams streams
%global _ksubdir streams
%global _kmodext streams
%global _conflict LiS
%global _strvers %{?extrarelease2}
%{expand: %%global _with_lfs --with-lfs}
%endif
%if %{BUILD_lis}
%global _STREAMS LiS
%global _streams LiS
%global _ksubdir lis
%global _kmodext lis
%global _conflict streams
%global _strvers 0:2.18.1%{?extrarelease2}
%{expand: %%global _with_lis --with-lis}
%endif

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

%global descripline %{title} provides SS7/ISDN/VoIP/SIGTRAN protocols for %{_STREAMS} STREAMS.

Summary:        OpenSS7 %{_STREAMS} STREAMS %{stitle} for Linux
Name:           striso
Epoch:          0
Version:        0.9.2.1.rc2
Release:        1
License:        GPL
Group:          System Environment/Kernel
%{?url:URL:            %{url}}
%{?_xpm_icon:Icon:           %{_xpm_icon}}
%{?_gif_icon:Icon:           %{_gif_icon}}
ExclusiveArch:  %{ix86} k6 x86_64 s390 s390x ppc ppciseries ppcpseries ppc64 arm armv4l sh mips mipsel noarch
ExclusiveOs:    Linux
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
BuildRequires: strcompat-devel = %version_of_or strcompat-devel %{?extrarelease2}
%if %{without_ko_modules}
BuildRequires: strcompat-%{info_name} = %version_of_or strcompat-devel %{?extrarelease2}
%else
BuildRequires: strcompat-%{core_name} = %version_of_or strcompat-devel %{?extrarelease2}
%endif
BuildRequires:    strxns-devel >=     0:0.9.2.4.rc2-1%{?extrarelease2}
BuildRequires:   strxnet-devel >=     0:0.9.2.9.rc2-1%{?extrarelease2}
BuildRequires:   strinet-devel >=    %{?extrarelease2}
BuildRequires:   strsctp-devel >=    %{?extrarelease2}

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
The %{name} package includes kernel modules, %{_STREAMS} STREAMS
drivers, modules, libraries, utilities, test programs, daemons,
and development environment for the development and execution of
Signalling System No. 7 (SS7), Signalling Transport (SIGTRAN),
Integrated Service Digital Network (ISDN), Voice over IP (VoIP),
Media Gateway Control (MGC), Media Gateway (MG), Switching
protocol stack components and applications for the %{_STREAMS}
STREAMS environment.  This distribution is currently applicable
to 2.4 and 2.6 kernels and is validated for ix86, x86_64, ppc
and ppc64 architectures, but should build and install well for
other architectures.  This SRPM builds the following binary
packages:
- %{name}-%{_streams}-%{core_name} -- core kernel modules.
- %{name}-%{_streams}-%{info_name} -- core kernel module symbol
  versions.
- %{name}-%{_streams}-lib -- run-time libraries.
- %{name}-%{_streams}-util -- admin utilities.
- %{name}-devel -- headers, libs and docs.
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
    %{?_with_lis:                '%_with_lis'} \
    %{?_with_lfs:                '%_with_lfs'} \
    %{?_with_sctp_slow_verification:        '%enable %_with_sctp_slow_verification'} \
    %{?_without_sctp_slow_verification:     '%disable %_without_sctp_slow_verification'} \
    %{?_with_sctp_throttle_heartbeats:      '%enable %_with_sctp_throttle_heartbeats'} \
    %{?_without_sctp_throttle_heartbeats:   '%disable %_without_sctp_throttle_heartbeats'} \
    %{?_with_sctp_discard_ootb:             '%enable %_with_sctp_discard_ootb'} \
    %{?_without_sctp_discard_ootb:          '%disable %_without_sctp_discard_ootb'} \
    %{?_with_sctp_extended_ip_support:      '%enable %_with_sctp_extended_ip_support'} \
    %{?_without_sctp_extended_ip_support:   '%disable %_without_sctp_extended_ip_support'} \
    %{?_with_sctp_hmac_sha1:                '%enable %_with_sctp_hmac_sha1'} \
    %{?_without_sctp_hmac_sha1:             '%disable %_without_sctp_hmac_sha1'} \
    %{?_with_sctp_hmac_md5:                 '%enable %_with_sctp_hmac_md5'} \
    %{?_without_sctp_hmac_md5:              '%disable %_without_sctp_hmac_md5'} \
    %{?_with_sctp_adler32:                  '%enable %_with_sctp_adler32'} \
    %{?_without_sctp_adler32:               '%disable %_without_sctp_adler32'} \
    %{?_with_sctp_crc32c:                   '%enable %_with_sctp_crc32c'} \
    %{?_without_sctp_crc32c:                '%disable %_without_sctp_crc32c'} \
    %{?_with_sctp_throttle_passiveopens:    '%enable %_with_sctp_throttle_passiveopens'} \
    %{?_without_sctp_throttle_passiveopens: '%disable %_without_sctp_throttle_passiveopens'} \
    %{?_with_sctp_ecn:                      '%enable %_with_sctp_ecn'} \
    %{?_without_sctp_ecn:                   '%disable %_without_sctp_ecn'} \
    %{?_with_sctp_lifetimes:                '%enable %_with_sctp_lifetimes'} \
    %{?_without_sctp_lifetimes:             '%disable %_without_sctp_lifetimes'} \
    %{?_with_sctp_add_ip:                   '%enable %_with_sctp_add_ip'} \
    %{?_without_sctp_add_ip:                '%disable %_without_sctp_add_ip'} \
    %{?_with_sctp_adaptation_layer_info:    '%enable %_with_sctp_adaptation_layer_info'} \
    %{?_without_sctp_adaptation_layer_info: '%disable %_without_sctp_adaptation_layer_info'} \
    %{?_with_sctp_partial_reliability:      '%enable %_with_sctp_partial_reliability'} \
    %{?_without_sctp_partial_reliability:   '%disable %_without_sctp_partial_reliability'} \
    %{?_with_sctp_debug:                    '%enable %_with_sctp_debug'} \
    %{?_without_sctp_debug:                 '%disable %_without_sctp_debug'} \
    %{?_with_sctp_error_generator:          '%enable %_with_sctp_error_generator'} \
    %{?_without_sctp_error_generator:       '%disable %_without_sctp_error_generator'} \
    %{?_with_sctp:                          '%_with_sctp'} \
    %{?_without_sctp:                       '%_without_sctp'} \
    %{?_with_sctp2:                         '%_with_sctp2'} \
    %{?_without_sctp2:                      '%_without_sctp2'} \
    %{?_with_xns:                           '%_with_xns'} \
    %{?_without_xns:                        '%_without_xns'} \
    %{?_with_tli:                           '%_with_tli'} \
    %{?_without_tli:                        '%_without_tli'} \
    %{?_with_inet:                          '%_with_inet'} \
    %{?_without_inet:                       '%_without_inet'} \
    %{?_with_xnet:                          '%_with_xnet'} \
    %{?_without_xnet:                       '%_without_xnet'} \
    %{?_with_sock:                          '%_with_sock'} \
    %{?_without_sock:                       '%_without_sock'} \
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
%package %{_streams}-%{core_name}

Summary:        OpenSS7 %{_STREAMS} STREAMS %{stitle} kernel modules.
Group:          System Environment/Kernel
Prefix:         %{_kmoduledir}
Requires:       %(LANG_ALL="C" rpm -q --whatprovides %{_kmoduledir}/kernel --qf "%{NAME} = %{VERSION}-%{RELEASE}\\n")
Requires:       strcompat-%{core_name} = %version_of_or strcompat-%{core_name} %{?extrarelease2}
%if %{NEED_init}
Requires:       %{name}-init = %{epoch}:%{version}-%{fullrelease2}
%endif
%if %{NEED_devs}
Requires:       %{name}-dev = %{epoch}:%{version}-%{fullrelease2}
%endif
%{?_conflict:Conflicts:      %{name}-%{_conflict}-%{core_name}}
#Prereq:         /sbin/depmod
#Prereq:         /sbin/lsmod
#Prereq:         /sbin/insmod
#Prereq:         /sbin/modprobe
# SuSE puts lsof in /usr/bin
Prereq:         lsof
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{name}-%{_streams}-%{core_name} = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-STREAMS-%{core_name} = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-%{_streams}-core = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-STREAMS-core = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-%{core_name} = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-core = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-%{_streams}-%{core_name} = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-STREAMS-%{core_name} = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-%{_streams}-core = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-STREAMS-core = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-%{core_name} = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-core = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description %{_streams}-%{core_name}
%{descripline}
The %{name}-%{_streams}-%{core_name} binary package contains the
kernel modules that provide the Linux kernel %{stitle}
%{_STREAMS} STREAMS drivers and modules.  This includes assorted
drivers and modules for additional protocol stacks such as
SIGTRAN.  This package is heavily tied to the kernel for which
it and the %{_STREAMS} STREAMS facility were compiled.  This
package applies to kernel version %{kernel_version} and requires
a %{_STREAMS} STREAMS package compiled for the same kernel.  If
you cannot find a binary package that matches your kernel,
rebuild from the source rpm.

%pre %{_streams}-%{core_name}
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

%post %{_streams}-%{core_name}
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

%preun %{_streams}-%{core_name}
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

%postun %{_streams}-%{core_name}
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
%files %{_streams}-%{core_name}
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%dir %{_kmoduledir}/%{base}
%attr(644,root,root) %{_kmoduledir}/modules.%{base}
%{_kmoduledir}/%{base}/*
%endif

%if %{FILES_ko_modules}
%files %{_streams}-%{core_name}
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%dir %{_kmoduledir}/%{base}
%{_kmoduledir}/%{base}/*
%endif

# ===========================================================================
%package %{_streams}-%{info_name}

Summary:        %{title} core kernel module symbol versions.
Group:          System Environment/Kernel
Prefix:         %{_includedir}/%{name}/%{kernel_version}
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{name}-%{_streams}-%{info_name} = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-%{_streams}-info = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-%{info_name} = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-info = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-%{_streams}-%{info_name} = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-%{_streams}-info = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-%{info_name} = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-info = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description %{_streams}-%{info_name}
%{descripline}
The %{name}-%{_streams}-%{info_name} package contains the kernel
modules symbol information for development of additional kernel
modules against the core kernel modules included in the
%{name}-core binary package.  This package is heavily tied to
the core kernel modules and kernel for which it was compiled.
This package applies to core kernel modules
%{name}-%{_streams}-%{core_name} for kernel version
%{_kversion}.

%if %{FILES_modules}
%files %{_streams}-%{info_name}
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%dir %{_includedir}/%{name}/%{kernel_version}
%{_includedir}/%{name}/%{kernel_version}/*
%endif

# don't include package for FILES_ko_modules

# ===========================================================================
%package %{_streams}-lib

Summary:        OpenSS7 %{_STREAMS} STREAMS %{stitle} run-time libraries.
Group:          System Environment/Libraries
Prefix:         %{_libdir}
Prefix:         %{_datadir}/locale
%{?_strvers:Requires: %{_streams}-lib = %version_of_or %{_streams}-lib %{_strvers}}
%{!?_strvers:%requires_prov Requires: %{_streams}-lib = %{_streams}-lib}
Requires:         strxnet-lib >=     0:0.9.2.9.rc2-1%{?extrarelease2}
Obsoletes:      %{name}-%{_streams}-libs
Prereq:         /sbin/ldconfig
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{name}-%{_streams}-lib = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-STREAMS-lib = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-lib = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-%{_streams}-lib = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-STREAMS-lib = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-lib = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description %{_streams}-lib
%{descripline}
The %{name}-%{_streams}-lib package contains the run-time
(shared object) libraries necessary to run applications programs
and utilities developed for %{title}.  Also included are the
libtool .la files describing the shared object libraries.

%if %{have_libraries}
%pre %{_streams}-lib
# ---------------------------------------------------------------------------
if [ $1 = 1 ] ; then : # fresh install
    # See if there are any libraries hanging around from a non-rpm install.  If there are, just
    # delete them.
    libs="%{libraries}" ; for lib in $libs ; do
        old_libs="`( /sbin/ldconfig -p | grep '\<lib'$lib'\>' | cut '-d ' -f4 ) 2>/dev/null`"
        if [ -n "$old_libs" ] ; then
            # Old libraries hanging around, delete them.
            echo "warning: Deleting old non-rpm libraries $old_libs" >&2
            rm -f $old_libs
        fi
    done
fi
%endif

%post %{_streams}-lib
# ---------------------------------------------------------------------------
if [ $1 = 1 -o $1 = 2 ] ; then : # fresh install or upgrade
    # After a fresh install or upgrade, do a load config.
    /sbin/ldconfig
fi

%postun %{_streams}-lib
# ---------------------------------------------------------------------------
if [ $1 = 0 -o $1 = 1 ] ; then
    # Whether complete removal or upgrade, do a load config.
    /sbin/ldconfig
fi

%if %{FILES_libs}
%files %{_streams}-lib
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%{_libdir}/*.so*
%{_libdir}/*.la
%{_datadir}/locale/*
%endif

# ===========================================================================
%package devel

Summary:        OpenSS7 %{_STREAMS} STREAMS %{stitle} headers, lib and doc.
Group:          Development/System
Prefix:         %{_libdir}
Prefix:         %{_includedir}/%{name}
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{name}-devel = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-devel = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description devel
%{descripline}
The %{name}-devel package contains library archives for static
compilation, and headers files to develop OpenSS7 %{_STREAMS}
STREAMS %{stitle} applications.  This also includes the header
files and static libraries required to compile OpenSS7
%{_STREAMS} STREAMS %{stitle} applications programs.

%if %{FILES_devlibs}
%files devel
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%{_libdir}/*.a
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/*.h
%{_includedir}/%{name}/sys/*
%{_includedir}/%{name}/ss7/*
%endif

%if %{FILES_dev}
%files devel
# ---------------------------------------------------------------------------
%defattr(-,root,root)
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/*.h
%{_includedir}/%{name}/sys/*
%{_includedir}/%{name}/ss7/*
%endif

# ===========================================================================
%package %{_streams}-util

Summary:        OpenSS7 %{_STREAMS} STREAMS %{stitle} admin utilities.
Group:          System Environment/Base
Prefix:         %{_bindir}
Prefix:         %{_sbindir}
Prefix:         %{_libexecdir}
Prefix:         %{_sysconfdir}
%if %{have_libraries}
Requires:       %{name}-%{_streams}-lib = %{epoch}:%{version}-%{fullrelease2}
%endif
%{?_strvers:Requires: %{_streams}-lib = %version_of_or %{_streams}-lib %{_strvers}}
%{!?_strvers:%requires_prov Requires: %{_streams}-lib = %{_streams}-lib}
Obsoletes:      %{name}-%{_streams}-utils
Obsoletes:      %{name}-utils
%{?_conflict:Conflicts:      %{name}-%{_conflict}-util}
%{?_conflict:Conflicts:      %{name}-%{_conflict}-utils}
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{name}-%{_streams}-util = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-STREAMS-util = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-util = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-%{_streams}-util = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-STREAMS-util = %{epoch}:%{version}-%{fullrelease2}
Provides:       %{name}-util = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description %{_streams}-util
%{descripline}
The %{name}-%{_streams}-util binary package provides
administrative and configuration test utilities and commands
associated with the OpenSS7 %{_STREAMS} STREAMS %{stitle}
package.  Note that these utilities are needed for running the
validation test suites contained in the %{name}-test binary
package.

%pre %{_streams}-util
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
fi

%if %{FILES_tools}
%files %{_streams}-util
# ---------------------------------------------------------------------------
%defattr(-,root,root)
#%{_bindir}/*
%{_sbindir}/*
%{_libexecdir}/%{name}/*test*
#%{_libexecdir}/%{name}/*tst
%endif

# ===========================================================================
%package dev

Summary:        OpenSS7 %{_STREAMS} STREAMS %{stitle} devices.
Group:          System Environment/Base
Requires:       dev >= 3.3
%if %{MAKE_devs}
Requires:       %{name}-util = %{epoch}:%{version}-%{fullrelease2}
%endif
Obsoletes:      %{name}-devs
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{name}-dev = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-dev = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description dev
%{descripline}
The %{name}-dev binary package contains the device definitions
necessary to run applications programs and utilities developed
for %{title}.  On older systems, this binary creates the devices
themselvs.  On newer systems, this binary package includes the
executable for creating devices from init scripts.

The package is not required for devfs based systems.

%if %{MAKE_devs}
%post dev
if [ $1 = 1 ] ; then : # fresh install
    # Older rpm versions < 4.0 (such as those used by SuSE) are not capable of doing rootless builds
    # because they have no %%dev macro, so in that case we just run the makenodes program that was
    # generated in the build to create devices.
    /usr/sbin/striso_mknod
fi

%preun dev
if [ $1 = 0 ] ; then : # final removal
    # Older rpm versions < 4.0 (such as those used by SuSE) are not capable of doing rootless builds
    # because they have no %%dev macro, so in that case we just run the makenodes program that was
    # generated in the build to create devices.
    /usr/sbin/striso_mknod --remove
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
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{name}-test = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-test = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description test
%{descripline}
The %{name}-test binary package provides the autotest validation
testsuites used for validating and troubleshooting the %{title}
package.  It also includes the send-pr script for generating and
submitting problem reports to bugs@openss7.org.

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

Summary:        OpenSS7 %{_STREAMS} STREAMS %{stitle} manpages and doc.
Group:          Documentation
Prefix:         %{_docdir}
Prefix:         %{_infodir}
Prefix:         %{_mandir}
Conflicts:      texinfo < 3.11
Obsoletes:      %{name}-docs
#Prereq:         /sbin/install-info
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{name}-doc = %{epoch}:%{version}-%{fullrelease}
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
functions.

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
%doc ABOUT-NLS
%doc AUTHORS ChangeLog COPYING INSTALL NEWS README THANKS doc/manual
%doc doc/specs doc/projects doc/drafts
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
%doc AUTHORS ChangeLog COPYING INSTALL NEWS README THANKS doc/manual
%doc doc/specs doc/projects doc/drafts
%doc LSM %{name}-%{version}.lsm
%{_infodir}/*
%{_mandir}/%{base}.refs*
%{_mandir}/%{base}.macros*
%{_mandir}/man*/*
%endif

# ===========================================================================
%package source

Summary:        OpenSS7 %{_STREAMS} STREAMS %{stitle} source package.
Group:          Development/System
Prefix:         %{_usrsrc}
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{name}-source = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-source = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description source
%{descripline}
The %{name}-source binary package contains the source code
necessary for building the %{name} release.  It also includes
the autoconf configuration utilities necessary to create and
distribute this rpm.  To develop on the package, it would be
better to use the tarball or source rpm release.

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
Requires:       strcompat-init = %version_of_or strcompat-init %{?extrarelease2}
Requires:       %{name}-util = %{epoch}:%{version}-%{fullrelease2}
Obsoletes:      %{name}-inits
Autoreq:        true
%{?_xpm:Icon: %_xpm}
%{?_gif:Icon: %_gif}
Provides:       %{name}-init = %{epoch}:%{version}-%{fullrelease}
Provides:       %{name}-init = %{epoch}:%{version}-%{fullrelease2}
Release:        %{fullrelease}

# ---------------------------------------------------------------------------
%description init
%{descripline}
The %{name}-init binary package provides the Sys V init scripts
and configuration files associated with the %{title} package.
These init scripts load kernel modules, create devices, and
apply system configuration customizations at boot.

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
            for suffix in %name ; do
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
    list='%{initfiles}' ; for i in $list ; do
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
#{_sysconfdir}/%{name}.conf
%{_initrddir}/../*
%{_configdir}/*
%{_modutildir}/*
%endif

%if %{FILES_ko_init}
%files init
# ---------------------------------------------------------------------------
%defattr(-,root,root)
#{_sysconfdir}/%{name}.conf
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
# striso.spec.in,v
# Revision 0.9.2.2  2006/05/08 03:33:31  brian
# - updated headers, corrected init scrip removal
#

* Fri Apr  7 2006 Brian Bidulock <bidulock@openss7.org>
- Revision 0.9.2.1  2006/04/07 22:01:25  brian
- initial branch

* Fri Apr  7 2006 Brian Bidulock <bidulock@openss7.org>
- Revision 0.9  2006/04/07 21:56:37  brian
- initial addition of striso files

* Fri Apr  7 2006 Brian Bidulock <bidulock@openss7.org>
- Began this RPM build for OpenSS7 ISO.
- Borrowed heavily from the OpenSS7 SS7 build.

# vim: ft=spec sw=4 et tw=78 com=b\:#,fb\:- fo+=tcqlorn nocin nosi