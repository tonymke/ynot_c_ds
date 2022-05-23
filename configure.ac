AC_INIT([ynot_c_ds],[0.1],[tony@tony-lechner.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_MACRO_DIRS([m4])

PKG_CHECK_MODULES([CHECK], [check >= 0.15.0])

AC_PROG_CC
AM_PROG_AR # for mingw
LT_INIT

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
	src/Makefile
	tests/Makefile
])
AC_OUTPUT

