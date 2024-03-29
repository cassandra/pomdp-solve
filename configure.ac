#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([pomdp-solve],[5.5],[arc@cassandra.org])
AC_CONFIG_SRCDIR([src/main.c])
AM_INIT_AUTOMAKE

# Allow the defined vars to come from a file instead of a slew of '-D'
# compiler flags.
AC_CONFIG_HEADERS([config.h])

## Option for using compile -g flag for debugging symbols
##
AM_CONDITIONAL(DEBUG, false)
AC_ARG_ENABLE(debug,
        [  --enable-debug      enable debugging symbols],
        [
                if test "x$enableval" = "xyes" ; then
                        AM_CONDITIONAL(DEBUG, true)
                fi
        ]
)

## Option for using compile -pg flag for profiling
##
AM_CONDITIONAL(PROFILE, false)
AC_ARG_ENABLE(profile,
        [  --enable-profile      enable debugging symbols and profiling],
        [
                if test "x$enableval" = "xyes" ; then
                        AM_CONDITIONAL(PROFILE, true)
                fi
        ]
)

## Option for using dmalloc memory leak checking library
##
AM_CONDITIONAL(DMALLOC, false)
AC_ARG_ENABLE(dmalloc,
        [  --enable-dmalloc      enable using dmalloc memory checker],
        [
                if test "x$enableval" = "xyes" ; then
                        AM_CONDITIONAL(DMALLOC, true)
                fi
        ]
)

# Checks for programs.
AC_PROG_YACC
AC_PROG_CC
AC_PROG_LEX([yywrap])
AC_PROG_RANLIB

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lcplex':
AC_CHECK_LIB([cplex], [main])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])

# Checks for header files.
AC_CHECK_HEADERS([float.h limits.h stddef.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC

AC_FUNC_STRTOD
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([bzero floor gettimeofday memset pow sqrt strstr])

AC_CONFIG_FILES([Makefile                   \
           src/Makefile               \
           src/mdp/Makefile           \
           src/lp-solve/Makefile           \
           src/laspack/Makefile
           testing/Makefile])
AC_OUTPUT
