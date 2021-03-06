AC_INIT([OPIC],[0.4.2])

AC_CONFIG_SRCDIR([README.md])
AC_CONFIG_AUX_DIR([build-aux])
AM_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_PROG_AR
LT_INIT
AC_CONFIG_MACRO_DIR([m4])

#AC_PROG_CC_C11
AC_PROG_CC_C99
AM_PROG_CC_C_O
# This doesn't work on travis-ci.
# But it's ok. Right now the SIMD version of robin_hood is not faster than the
# one without SIMD, so not having arch flag should be fine for us.
# AX_GCC_ARCHFLAG
AC_PROG_LIBTOOL
AC_C_INLINE
AC_HEADER_STDBOOL
AC_CHECK_HEADERS([limits.h inttypes.h])
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_MMAP

AX_CFLAGS_WARN_ALL
AX_PTHREAD

# TODO(fchern):
# check cmocka

# Checks for log4c
AC_PATH_PROG([LOG4C_CONFIG], [log4c-config])
AS_IF([test "x$LOG4C_CONFIG" != "x"], [
  log4c_CFLAGS=`$LOG4C_CONFIG --cflags`
  log4c_LIBS=`$LOG4C_CONFIG --libs`
  AC_SUBST([log4c_CFLAGS])
  AC_SUBST([log4c_LIBS])
  AC_DEFINE([HAS_LOG4C], [1], [Define to 1 if log4c is available.])
])

# ugly test to detect gcc and clang that pretends to be gcc
AS_IF([test "x$GCC" = "xyes"],
  [AS_IF([$CC --version | grep "clang"],
   [atomic_LIBS=""],
   [atomic_LIBS="-latomic"])],
  [atomic_LIBS=""])
AC_SUBST([atomic_LIBS])

PKG_CHECK_MODULES([cmocka], [cmocka >= 1.0.1])

AC_CONFIG_FILES([
  Makefile
  opic/Makefile
  opic/malloc/Makefile
  opic/hash/Makefile
  benchmark/Makefile
  benchmark/malloc/Makefile
  benchmark/robin_hood/Makefile
])

AC_CONFIG_SUBDIRS([
  benchmark/robin_hood/spookyhash-c
  benchmark/robin_hood/cityhash-c
  benchmark/robin_hood/farmhash-c
])

AC_OUTPUT
