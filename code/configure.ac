#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#
AC_PREREQ(2.59)
AC_INIT([hs],[3.9.3],[support@abinition.com])
AM_INIT_AUTOMAKE(hs,3.9.3)
AC_CONFIG_SRCDIR([hs.c])
AM_CONFIG_HEADER(config.h:config.in)

PACKAGE=hs
HS_VERSION=3.9.3
HS_MAJOR=3
HS_MINOR=9
HS_RELEASE=3

AC_PROG_INSTALL([/usr/local/bin])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lc':
#AC_CHECK_LIB([c], [main])
# FIXME: Replace `main' with a function in `-lcrypto':
#AC_CHECK_LIB([crypto], [main])
# FIXME: Replace `main' with a function in `-lgd':
#AC_CHECK_LIB([gd], [main])
# FIXME: Replace `main' with a function in `-ljpeg':
#AC_CHECK_LIB([jpeg], [main])
# FIXME: Replace `main' with a function in `-lm':
#AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-lmysqlclient':
#AC_CHECK_LIB([mysqlclient], [main])
# FIXME: Replace `main' with a function in `-lssl':
#AC_CHECK_LIB([ssl], [main])
# FIXME: Replace `main' with a function in `-lz':
#AC_CHECK_LIB([z], [main])

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stddef.h stdlib.h string.h sys/file.h sys/ioctl.h sys/socket.h sys/systeminfo.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM
AC_TYPE_UID_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([alarm bzero dup2 floor gethostbyaddr gethostbyname gethostname gettimeofday inet_ntoa memchr memmove memset mkdir mkfifo pow putenv select setenv socket sqrt strchr strcspn strerror strrchr strspn strstr sysinfo tzset])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
