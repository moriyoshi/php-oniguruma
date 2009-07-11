dnl
dnl $Id: config.m4,v 1.58.2.4.2.11.2.8 2009/04/20 15:39:48 jani Exp $
dnl

PHP_ARG_WITH(onig, [for oniguruma],
[  --with-onig[=DIR]       Enable oniguruma extension DIR is the oniguruma install prefix.], yes, yes)

if test "$PHP_ONIG" != "no"; then  
  AC_DEFINE([HAVE_PHP_ONIG],1,[whether to enable oniguruma support])

  if test "$PHP_ONIG" != "yes"; then
    if test ! -f "$PHP_ONIG/include/oniguruma.h"; then
      AC_MSG_ERROR([oniguruma.h not found in $PHP_ONIG/include])
    fi
    PHP_ADD_INCLUDE([$PHP_ONIG/include])

    PHP_CHECK_LIBRARY(onig, onig_init, [
      PHP_ADD_LIBRARY_WITH_PATH(onig, $PHP_ONIG/$PHP_LIBDIR, ONIG_SHARED_LIBADD)
    ],[
      AC_MSG_ERROR([Problem with oniguruma. Please check config.log for more information.])
    ], [
      -L$PHP_ONIG/$PHP_LIBDIR
    ])
  else
    PHP_CHECK_LIBRARY(onig, onig_init, [
      PHP_ADD_LIBRARY(onig,, ONIG_SHARED_LIBADD)
    ],[
      AC_MSG_ERROR([Problem with oniguruma. Please check config.log for more information.])
    ])
  fi

  save_old_LDFLAGS=$LDFLAGS
  PHP_EVAL_LIBLINE([$ONIG_SHARED_LIBADD], LDFLAGS)
  AC_MSG_CHECKING([if oniguruma has an invalid entry for KOI8 encoding])
  AC_TRY_LINK([
#include <oniguruma.h>
  ], [
return (int)(ONIG_ENCODING_KOI8 + 1);
  ], [
    AC_MSG_RESULT([no])
  ], [
    AC_MSG_RESULT([yes])
    AC_DEFINE([PHP_ONIG_BAD_KOI8_ENTRY], [1], [define to 1 if oniguruma has an invalid entry for KOI8 encoding])
  ])
  LDFLAGS=$save_old_LDFLAGS

  PHP_NEW_EXTENSION(onig, $PHP_ONIG_SOURCES, $ext_shared,,-DONIG_ESCAPE_UCHAR_COLLISION=1 -DUChar=OnigUChar)
  PHP_SUBST(ONIG_SHARED_LIBADD)

  if test "$ext_shared" = "no"; then
    PHP_ADD_SOURCES(PHP_EXT_DIR(onig), [onig.c])
    out="php_config.h"
  else
    PHP_ADD_SOURCES_X(PHP_EXT_DIR(onig), [onig.c],,shared_objects_onig,yes)
    if test -f "$ext_builddir/config.h.in"; then
      out="$abs_builddir/config.h"
    else
      out="php_config.h"
    fi
  fi
  
  PHP_INSTALL_HEADERS([ext/onig], [php_onig.h])
fi

# vim600: sts=2 sw=2 et
