# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)

AC_INIT(scim-unikey, 0.3~r32, mr.lequoctuan@gmail.com)

cflags_set=${CFLAGS+set}
cppflags_set=${CPPFLAGS+set}

# Init automake stuff
AM_INIT_AUTOMAKE([dist-bzip2])
AM_CONFIG_HEADER([config.h])
AC_CONFIG_SRCDIR([src/scim_unikey_imengine.h])
AM_PROG_INSTALL_STRIP

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION(0.17)

# Init libtool
LT_INIT
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_LANG(C++)

AC_C_CONST
AC_TYPE_SIZE_T

SCIM_VERSION=1.4.0

UNIKEY_CURRENT=1
UNIKEY_REVISION=0
UNIKEY_AGE=0

AC_SUBST(UNIKEY_CURRENT)
AC_SUBST(UNIKEY_REVISION)
AC_SUBST(UNIKEY_AGE)

PKG_CHECK_MODULES(GIO, [
    gio-2.0
])
AC_SUBST(GIO)

PKG_CHECK_MODULES(SCIM,[scim >= $SCIM_VERSION])
AC_SUBST(SCIM_VERSION)

PKG_CHECK_MODULES(SCIM_GTKUTILS,[scim-gtkutils >= $SCIM_VERSION],
		  		[SCIM_HAS_GTKUTILS=yes],
				[SCIM_HAS_GTKUTILS=no])

if test "$SCIM_HAS_GTKUTILS" = "yes"; then
  SCIM_BUILD_SETUP=1
else
  SCIM_BUILD_SETUP=0
fi

AM_CONDITIONAL(SCIM_BUILD_SETUP, [test "$SCIM_HAS_GTKUTILS" = "yes"])
AC_SUBST(SCIM_BUILD_SETUP)


# Checks for header files.
AC_HEADER_STDC

# Checks for library functions.
AC_SUBST(ac_aux_dir)

# libtool option to control which symbols are exported
# right now, symbols starting with _ are not exported
LIBTOOL_EXPORT_OPTIONS='-export-symbols-regex "^[[^_]].*"'
AC_SUBST(LIBTOOL_EXPORT_OPTIONS)

# Extra args.
AC_ARG_ENABLE(debug,
	      [  --enable-debug     Turn on debugging],
	      [case "${enableval}" in
	       yes) debug=yes ;;
	       no)  debug=no ;;
	       *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
       	      esac],[debug=no])

if test "x$debug" = "xyes"; then
  test "$cflags_set" = set || CFLAGS="$CFLAGS -g"
  test "$cppflags_set" = set || CPPFLAGS="$CXXFLAGS -g"
  SCIM_DEBUG_FLAGS="-DENABLE_DEBUG=1"
else
  SCIM_DEBUG_FLAGS=""
fi
AC_SUBST(SCIM_DEBUG_FLAGS)

newprefix=`$PKG_CONFIG --variable=prefix scim`
if test "x$newprefix" != "x"; then
  prefix=$newprefix
fi

SCIM_ICONDIR=`$PKG_CONFIG --variable=icondir scim`
SCIM_MODULEDIR=`$PKG_CONFIG --variable=moduledir scim`

if test "x$SCIM_ICONDIR" = "x"; then
  SCIM_ICONDIR=${datadir}/scim/icons
fi

if test "x$SCIM_MODULEDIR" = "x"; then
  SCIM_MODULEDIR=${libdir}/scim-1.0
fi

AC_SUBST(SCIM_ICONDIR)
AC_SUBST(SCIM_MODULEDIR)

AC_CONFIG_FILES([Makefile
  src/Makefile
  ukengine/Makefile
  icon/Makefile
  po/Makefile.in
  setup/Makefile
])
AC_OUTPUT

