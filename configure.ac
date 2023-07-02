AC_PREREQ([2.69])
AC_INIT([ffqueue], [1.7.60], [https://ffqueue.bruchhaus.dk/])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_CONFIG_SRCDIR([src/FFQApp.cpp])
AC_PROG_CXX
WXVER=`wx-config --version | sed -e 's/\.//g'`
AS_IF([test -z $WXVER], [ AC_MSG_ERROR(["wx-config" was not found. To correct this you must either install the development version of wxWidgets (usual package name "libwxgtk3.0-dev" or alike) OR if you have built your own version of wxWidgets you must ensure that "wx-config" is added to the PATH environment variable.]) ], [])
AS_IF([test ${WXVER::3} -lt 302], [ AC_MSG_ERROR([wxWidgets must be at least version 3.0.2]) ], [])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_CHECK_LIB([z], [compress2], [], [AC_MSG_ERROR([ZLib (libz) was not found!])])
LIBS="$LIBS `wx-config --libs` -lz"
AC_OUTPUT
