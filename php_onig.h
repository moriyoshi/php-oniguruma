/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2009 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Moriyoshi Koizumi <moriyoshi@php.net>                        |
   +----------------------------------------------------------------------+
 */

/* $Id$ */

#ifndef _PHP_ONIG_H
#define _PHP_ONIG_H

#ifdef COMPILE_DL_ONIG
#undef HAVE_PHP_ONIG
#define HAVE_PHP_ONIG 1
#endif

#if HAVE_PHP_ONIG

#define re_pattern_buffer           php_onig_re_pattern_buffer
#define regex_t                     php_onig_regex_t
#define re_registers                php_onig_re_registers
#include <oniguruma.h>
#undef UChar

#ifdef PHP_WIN32
#	undef PHP_ONIG_API
#	ifdef PHP_ONIG_EXPORTS
#		define PHP_ONIG_API __declspec(dllexport)
#	elif defined(COMPILE_DL_ONIG)
#		define PHP_ONIG_API __declspec(dllimport)
#	else
#		define PHP_ONIG_API /* nothing special */
#	endif
#elif defined(__GNUC__) && __GNUC__ >= 4
#	undef PHP_ONIG_API
#	define PHP_ONIG_API __attribute__ ((visibility("default")))
#else
#	undef PHP_ONIG_API
#	define PHP_ONIG_API /* nothing special */
#endif

extern zend_module_entry onig_module_entry;
#define onig_module_ptr &onig_module_entry

ZEND_BEGIN_MODULE_GLOBALS(onig)
    OnigEncoding current_mbctype;
    HashTable ht_rc;
    OnigOptionType default_options;
    OnigSyntaxType *default_syntax;
ZEND_END_MODULE_GLOBALS(onig)

#ifdef ZTS
#define ONIG(v) TSRMG(onig_globals_id, zend_onig_globals *, v)
#else
#define ONIG(v) (onig_globals.v)
#endif

#else	/* HAVE_MBSTRING */

#define onig_module_ptr NULL

#endif	/* HAVE_MBSTRING */

#define phpext_onig_ptr onig_module_ptr

#endif		/* _PHP_ONIG_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
