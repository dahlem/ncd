# Copyright (C) 2015 Dominik Dahlem <Dominik.Dahlem@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AC_PREREQ(2.61)
AC_INIT([Normalised Compression Distance],
        [0.5],
        [Dominik Dahlem Dominik.Dahlem@gmail.com], 
        [ncd])

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])

# place where extra autoconf macros are kept
AC_CONFIG_MACRO_DIR([m4])

AC_LANG([C++])

AM_INIT_AUTOMAKE

AC_PROG_CXX
define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

# Checks for header files.
# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_LONG_LONG
AC_TYPE_SIZE_T
AC_CXX_BOOL
AC_CXX_CONST_CAST
AC_CXX_EXPLICIT
AC_CXX_EXCEPTIONS
AC_CXX_TEMPLATES
AC_CXX_HAVE_STD
AC_CXX_NAMESPACES
AC_CXX_HAVE_NUMERIC_LIMITS
AC_CXX_HAVE_STL

# Checks for library functions.
AC_HEADER_STDC

AC_CHECK_HEADERS([zlib.h],)
AC_CHECK_LIB([z], [compress2],, [unset HAVE_ZLIB])

# Boost library check
AX_BOOST_BASE([1.48])

AX_BOOST_PROGRAM_OPTIONS
AX_BOOST_SYSTEM
AX_BOOST_FILESYSTEM

# Check for debug
#  AC_DEFINE(BOOST_ENABLE_ASSERT_HANDLER, 1, [enable boost asserts])
AC_ARG_ENABLE(debug, AS_HELP_STRING(
		     [--enable-debug], 
		     [enable debug @<:@default=no@:>@]), 
		     [DEBUGGING=$enableval], [DEBUGGING=no])
if test "x$DEBUGGING" = xno; then
  AC_DEFINE(NDEBUG, 1, [define debugging, if disabled])
  AC_DEFINE(BOOST_DISABLE_ASSERTS, 1, [disable boost asserts, if enabled])
fi

# Check for OpenMP
AC_ARG_ENABLE(openmp, AS_HELP_STRING(
		   [--enable-openmp], 
		   [enable OpenMP @<:@default=no@:>@]), 
		   [USE_OPENMP=$enableval], [USE_OPENMP=no])
if test "x$USE_OPENMP" = xyes; then
   AX_OPENMP
fi

CFLAGS="-Wno-long-long -g -O2 $OPENMP_CXXFLAGS"
CPPFLAGS="$CFLAGS $CPPFLAGS"
CXXFLAGS="$CFLAGS $CXXFLAGS"



AC_CONFIG_FILES([
   Makefile
   m4/Makefile
   src/Makefile
   src/main/Makefile
])


AC_OUTPUT

for top_builddir in . .. ../.. $ac_auxdir $ac_auxdir/..; do
  test -f $top_builddir/configure && break
done

AC_MSG_RESULT([

$PACKAGE $VERSION: Automatic configuration OK.

Type 'make' to compile $PACKAGE.

])
