#
# Copyright (C) 2008 Iowa State University 
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.
#

AC_INIT(GamessQ, 1.2.2, MMP.Guy@gmail.com)
AC_CONFIG_SRCDIR(src/gamessq.cpp)
AC_CONFIG_AUX_DIR(config)
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([1.9 foreign])
AM_MAINTAINER_MODE

WXCONFIG=wx-config
AC_ARG_WITH(wx-config,
[[  --with-wx-config=FILE   Use the given path to wx-config when determining
                          wxWidgets configuration; defaults to "wx-config"]],
[
    if test "$withval" != "yes" -a "$withval" != ""; then
        WXCONFIG=$withval
    fi
])

dnl C-compiler checks
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_CXX

AC_MSG_CHECKING([wxWidgets version])
if wxversion=`"$WXCONFIG" --version`; then
    AC_MSG_RESULT([$wxversion])
else
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([wxWidgets is required. Try --with-wx-config.])
fi

AC_MSG_CHECKING([target system type])
if test -n "`echo ${target_os} | grep 'mingw'`" -o \
-n "`echo ${target_os} | grep 'cygwin'`"; then
AC_MSG_RESULT([windows])
AC_SUBST([target_system_type], ["windows"])

AC_CHECK_TOOL([WINDRES], [windres], [])
AC_SUBST([WINDRES])
AM_CONDITIONAL([HAS_WINDRES], [test -n "$WINDRES"])

AC_MSG_CHECKING([for unicode support in wxWidgets])
if $WXCONFIG --unicode=yes --version > /dev/null 2> /dev/null; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
	AC_MSG_ERROR([wxWidgets must be compiled with unicode support in windows])
fi

WX_CXXFLAGS="`"$WXCONFIG" --unicode=yes --cxxflags`"
AC_SUBST([WX_CXXFLAGS])
WX_LIBS="`"$WXCONFIG" --unicode=yes --libs core,base,net,adv`"
AC_SUBST([WX_LIBS])

AC_CHECK_HEADER([windows.h],[],
[AC_MSG_ERROR([could not find windows.h wich is required for GamessQ.])])
AC_CHECK_HEADER([tlhelp32.h],[],
[AC_MSG_ERROR([could not find tlhelp32.h wich is required for GamessQ.])],
[#include <windows.h>
])

else
AC_MSG_RESULT([posix])
AC_SUBST([target_system_type], ["posix"])

AM_CONDITIONAL([HAS_WINDRES], [false])

AC_SUBST([GAMESSQ_OS_FILES], ['$GAMESSQ_POSIX_FILES'])
WX_CXXFLAGS="`"$WXCONFIG" --cxxflags` -DDATA_DIR='\"${datadir}/$PACKAGE\"'"
AC_SUBST([WX_CXXFLAGS])
WX_LIBS="`"$WXCONFIG" --libs base,core,adv`"
AC_SUBST([WX_LIBS])

AC_CHECK_HEADER([signal.h],[],
[AC_MSG_ERROR([could not find signal.h wich is required for GamessQ.])])
AC_CHECK_HEADER([sys/socket.h],[],
[AC_MSG_ERROR([could not find sys/socket.h wich is required for GamessQ.])])
AC_CHECK_HEADER([poll.h],[],
[AC_MSG_ERROR([could not find poll.h wich is required for GamessQ.])])

fi

AM_CONDITIONAL([WINDOWS], [test x$target_system_type == xwindows])

if test "x${target_vendor}" = "xapple"; then
AC_CONFIG_FILES([bundle.sh], [chmod +x bundle.sh])
WX_LIBS+=" -headerpod_max_install_names"
fi

# Provide a test suite option
#     Check for boost.test

#Begin Mike Ekstrand's cleverness
if test -z "${datarootdir}"; then
    datarootdir="${datadir}"
    AC_SUBST([datarootdir])
fi

# if test -z "${docdir}"; then
#     # be compatible with old make's
#     docdir='${datarootdir}/doc/${PACKAGE}'
#     dvidir='${docdir}'
#     pdfdir='${docdir}'
#     psdir='${docdir}'
#     htmldir='${docdir}'
#     AC_SUBST([docdir])
#     AC_SUBST([dvidir])
#     AC_SUBST([pdfdir])
#     AC_SUBST([psdir])
#     AC_SUBST([htmldir])
# fi
#End cleverness

AC_SUBST(ac_aux_dir)
AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

