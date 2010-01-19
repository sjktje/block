#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([block], [0.0.1b], [sjk@ankeborg.nu])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
#http://www.gnu.org/software/autoconf/manual/html_node/Particular-Programs.html#Particular-Programs
# -- sjk
#AC_PROG_INSTALL

# Checks for libraries.
have_libsjk=no
AC_CHECK_LIB([sjk], [perrorf], [have_libsjk=yes])

if test "x${have_libsjk}" = xno; then
    echo "-------------------------------------------"
    echo " libsjk is required to build this piece of "
    echo " software. Please install libsjk before    "
    echo " installing block.                         "
    echo "-------------------------------------------"
    (exit 1); exit 1;
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h unistd.h libsjk.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
