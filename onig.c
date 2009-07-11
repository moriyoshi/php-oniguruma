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
   | Author: Tsukada Takuya <tsukada@fminn.nagano.nagano.jp>              |
   |         Rui Hirokawa <hirokawa@php.net>                              |
   |         Moriyoshi Koizumi <moriyoshi@php.net>                        |
   +----------------------------------------------------------------------+
 */

/* $Id: onig.c,v 1.224.2.22.2.25.2.55 2009/05/27 13:42:17 tony2001 Exp $ */

/* {{{ includes */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/php_smart_str.h"
#include "php_ini.h"

#include "php_onig.h"
/* }}} */

#if HAVE_PHP_ONIG

#define PHP_ONIG_MAXCACHE 50

ZEND_DECLARE_MODULE_GLOBALS(onig);
static PHP_GINIT_FUNCTION(onig);
static PHP_GSHUTDOWN_FUNCTION(onig);
static PHP_MINIT_FUNCTION(onig);
static PHP_MSHUTDOWN_FUNCTION(onig);
static PHP_RINIT_FUNCTION(onig);
static PHP_RSHUTDOWN_FUNCTION(onig);
static PHP_MINFO_FUNCTION(onig);

static PHP_INI_MH(OnUpdateOnigEncoding);

static PHP_FUNCTION(onig_regex_encoding);
static PHP_FUNCTION(onig_ereg);
static PHP_FUNCTION(onig_eregi);
static PHP_FUNCTION(onig_ereg_replace);
static PHP_FUNCTION(onig_eregi_replace);
static PHP_FUNCTION(onig_split);
static PHP_FUNCTION(onig_ereg_match);
static PHP_FUNCTION(onig_regex_set_options);

static void php_onig_regex_free_cache(php_onig_regex_t **pre);
static OnigEncoding _php_onig_name2mbctype(const char *pname);

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(arginfo_onig_regex_encoding, 0, 0, 0)
	ZEND_ARG_INFO(0, encoding)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_onig_ereg, 0, 0, 2)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, string)
	ZEND_ARG_INFO(1, registers)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_onig_eregi, 0, 0, 2)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, string)
	ZEND_ARG_INFO(1, registers)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_onig_ereg_replace, 0, 0, 3)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, replacement)
	ZEND_ARG_INFO(0, string)
	ZEND_ARG_INFO(0, option)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_onig_eregi_replace, 0, 0, 3)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, replacement)
	ZEND_ARG_INFO(0, string)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_onig_split, 0, 0, 2)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, string)
	ZEND_ARG_INFO(0, limit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_onig_ereg_match, 0, 0, 2)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, string)
	ZEND_ARG_INFO(0, option)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_onig_ereg_search, 0, 0, 0)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, option)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_onig_ereg_search_pos, 0, 0, 0)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, option)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_onig_ereg_search_regs, 0, 0, 0)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, option)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_onig_ereg_search_init, 0, 0, 1)
	ZEND_ARG_INFO(0, string)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, option)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_onig_ereg_search_getregs, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_onig_ereg_search_getpos, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_onig_ereg_search_setpos, 0, 0, 1)
	ZEND_ARG_INFO(0, position)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_onig_regex_set_options, 0, 0, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ zend_function_entry onig_functions[] */
const zend_function_entry onig_functions[] = {
	PHP_FE(onig_regex_encoding,		arginfo_onig_regex_encoding)
	PHP_FE(onig_regex_set_options,	arginfo_onig_regex_set_options)
	PHP_FE(onig_ereg,				arginfo_onig_ereg)
	PHP_FE(onig_eregi,				arginfo_onig_eregi)
	PHP_FE(onig_ereg_replace,		arginfo_onig_ereg_replace)
	PHP_FE(onig_eregi_replace,		arginfo_onig_eregi_replace)
	PHP_FE(onig_split,				arginfo_onig_split)
	PHP_FE(onig_ereg_match,			arginfo_onig_ereg_match)
	{ NULL, NULL, NULL }
};
/* }}} */

/* {{{ zend_module_entry onig_module_entry */
zend_module_entry onig_module_entry = {
	STANDARD_MODULE_HEADER,
	"onig",
	onig_functions,
	PHP_MINIT(onig),
	PHP_MSHUTDOWN(onig),
	PHP_RINIT(onig),
	PHP_RSHUTDOWN(onig),
	PHP_MINFO(onig),
	NO_VERSION_YET,
	PHP_MODULE_GLOBALS(onig),
	PHP_GINIT(onig),
	PHP_GSHUTDOWN(onig),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_ONIG
ZEND_GET_MODULE(onig)
#endif

/* {{{ php.ini directive registration */
PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY("onig.internal_encoding",
						"UTF-8",
						PHP_INI_ALL, OnUpdateOnigEncoding,
						current_mbctype,
						zend_onig_globals, onig_globals)
PHP_INI_END()
/* }}} */

/* {{{ module global initialize handler */
static PHP_GINIT_FUNCTION(onig)
{
	onig_globals->current_mbctype = ONIG_ENCODING_UTF8;
	onig_globals->default_options = ONIG_OPTION_MULTILINE | ONIG_OPTION_SINGLELINE;
	onig_globals->default_syntax = ONIG_SYNTAX_RUBY;
	zend_hash_init(&(onig_globals->ht_rc), 0, NULL, (void (*)(void *)) php_onig_regex_free_cache, 1);
}
/* }}} */

/* {{{ PHP_GSHUTDOWN_FUNCTION */
static PHP_GSHUTDOWN_FUNCTION(onig)
{
	zend_hash_destroy(&onig_globals->ht_rc);
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION(onig) */
static PHP_MINIT_FUNCTION(onig)
{
	REGISTER_INI_ENTRIES();
	onig_init();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION(onig) */
static PHP_MSHUTDOWN_FUNCTION(onig)
{
	UNREGISTER_INI_ENTRIES();
	onig_end();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION(onig) */
static PHP_RINIT_FUNCTION(onig)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION(onig) */
static PHP_RSHUTDOWN_FUNCTION(onig)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION(onig) */
static PHP_MINFO_FUNCTION(onig)
{
	char buf[32];

	php_info_print_table_start();
	php_info_print_table_row(2, "Oniguruma support", "enabled");
	snprintf(buf, sizeof(buf), "%d.%d.%d",
			ONIGURUMA_VERSION_MAJOR,
			ONIGURUMA_VERSION_MINOR,
			ONIGURUMA_VERSION_TEENY);
	php_info_print_table_row(2, "Version", buf);
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ static PHP_INI_MH(OnUpdateOnigEncoding) */
static PHP_INI_MH(OnUpdateOnigEncoding)
{
#ifndef ZTS
	char *base = (char *) mh_arg2;
#else
	char *base = (char *) ts_resource(*((int *) mh_arg2));
#endif
	OnigEncoding *p = (OnigEncoding *)(base + (size_t) mh_arg1);
	OnigEncoding _new_value = _php_onig_name2mbctype(new_value);

	if (_new_value == ONIG_ENCODING_UNDEF) {
		return FAILURE;	
	}

	*p = _new_value;

	return SUCCESS;
}
/* }}} */

/* {{{ encoding name map */
typedef struct _php_onig_enc_name_map_t {
	const char *names;
	OnigEncoding code;
} php_onig_enc_name_map_t;

php_onig_enc_name_map_t enc_name_map[] = {
#ifdef ONIG_ENCODING_EUC_JP
	{
		"EUC-JP\0EUCJP\0X-EUC-JP\0UJIS\0EUCJP\0EUCJP-WIN\0",
		ONIG_ENCODING_EUC_JP
	},
#endif
#ifdef ONIG_ENCODING_UTF8
	{
		"UTF-8\0UTF8\0",
		ONIG_ENCODING_UTF8
	},
#endif
#ifdef ONIG_ENCODING_UTF16_BE
	{
		"UTF-16\0UTF-16BE\0",
		ONIG_ENCODING_UTF16_BE
	},
#endif
#ifdef ONIG_ENCODING_UTF16_LE
	{
		"UTF-16LE\0",
		ONIG_ENCODING_UTF16_LE
	},
#endif
#ifdef ONIG_ENCODING_UTF32_BE
	{
		"UCS-4\0UTF-32\0UTF-32BE\0",
		ONIG_ENCODING_UTF32_BE
	},
#endif
#ifdef ONIG_ENCODING_UTF32_LE
	{
		"UCS-4LE\0UTF-32LE\0",
		ONIG_ENCODING_UTF32_LE
	},
#endif
#ifdef ONIG_ENCODING_SJIS
	{
		"SJIS\0CP932\0MS932\0SHIFT_JIS\0SJIS-WIN\0WINDOWS-31J\0",
		ONIG_ENCODING_SJIS
	},
#endif
#ifdef ONIG_ENCODING_BIG5
	{
		"BIG5\0BIG-5\0BIGFIVE\0CN-BIG5\0BIG-FIVE\0",
		ONIG_ENCODING_BIG5
	},
#endif
#ifdef ONIG_ENCODING_EUC_CN
	{
		"EUC-CN\0EUCCN\0EUC_CN\0GB-2312\0GB2312\0",
		ONIG_ENCODING_EUC_CN
	},
#endif
#ifdef ONIG_ENCODING_EUC_TW
	{
		"EUC-TW\0EUCTW\0EUC_TW\0",
		ONIG_ENCODING_EUC_TW
	},
#endif
#ifdef ONIG_ENCODING_EUC_KR
	{
		"EUC-KR\0EUCKR\0EUC_KR\0",
		ONIG_ENCODING_EUC_KR
	},
#endif
#if defined(ONIG_ENCODING_KOI8) && !PHP_ONIG_BAD_KOI8_ENTRY
	{
		"KOI8\0KOI-8\0",
		ONIG_ENCODING_KOI8
	},
#endif
#ifdef ONIG_ENCODING_KOI8_R
	{
		"KOI8R\0KOI8-R\0KOI-8R\0",
		ONIG_ENCODING_KOI8_R
	},
#endif
#ifdef ONIG_ENCODING_ISO_8859_1
	{
		"ISO-8859-1\0ISO8859-1\0ISO_8859_1\0ISO8859_1\0",
		ONIG_ENCODING_ISO_8859_1
	},
#endif
#ifdef ONIG_ENCODING_ISO_8859_2
	{
		"ISO-8859-2\0ISO8859-2\0ISO_8859_2\0ISO8859_2\0",
		ONIG_ENCODING_ISO_8859_2
	},
#endif
#ifdef ONIG_ENCODING_ISO_8859_3
	{
		"ISO-8859-3\0ISO8859-3\0ISO_8859_3\0ISO8859_3\0",
		ONIG_ENCODING_ISO_8859_3
	},
#endif
#ifdef ONIG_ENCODING_ISO_8859_4
	{
		"ISO-8859-4\0ISO8859-4\0ISO_8859_4\0ISO8859_4\0",
		ONIG_ENCODING_ISO_8859_4
	},
#endif
#ifdef ONIG_ENCODING_ISO_8859_5
	{
		"ISO-8859-5\0ISO8859-5\0ISO_8859_5\0ISO8859_5\0",
		ONIG_ENCODING_ISO_8859_5
	},
#endif
#ifdef ONIG_ENCODING_ISO_8859_6
	{
		"ISO-8859-6\0ISO8859-6\0ISO_8859_6\0ISO8859_6\0",
		ONIG_ENCODING_ISO_8859_6
	},
#endif
#ifdef ONIG_ENCODING_ISO_8859_7
	{
		"ISO-8859-7\0ISO8859-7\0ISO_8859_7\0ISO8859_7\0",
		ONIG_ENCODING_ISO_8859_7
	},
#endif
#ifdef ONIG_ENCODING_ISO_8859_8
	{
		"ISO-8859-8\0ISO8859-8\0ISO_8859_8\0ISO8859_8\0",
		ONIG_ENCODING_ISO_8859_8
	},
#endif
#ifdef ONIG_ENCODING_ISO_8859_9
	{
		"ISO-8859-9\0ISO8859-9\0ISO_8859_9\0ISO8859_9\0",
		ONIG_ENCODING_ISO_8859_9
	},
#endif
#ifdef ONIG_ENCODING_ISO_8859_10
	{
		"ISO-8859-10\0ISO8859-10\0ISO_8859_10\0ISO8859_10\0",
		ONIG_ENCODING_ISO_8859_10
	},
#endif
#ifdef ONIG_ENCODING_ISO_8859_11
	{
		"ISO-8859-11\0ISO8859-11\0ISO_8859_11\0ISO8859_11\0",
		ONIG_ENCODING_ISO_8859_11
	},
#endif
#ifdef ONIG_ENCODING_ISO_8859_13
	{
		"ISO-8859-13\0ISO8859-13\0ISO_8859_13\0ISO8859_13\0",
		ONIG_ENCODING_ISO_8859_13
	},
#endif
#ifdef ONIG_ENCODING_ISO_8859_14
	{
		"ISO-8859-14\0ISO8859-14\0ISO_8859_14\0ISO8859_14\0",
		ONIG_ENCODING_ISO_8859_14
	},
#endif
#ifdef ONIG_ENCODING_ISO_8859_15
	{
		"ISO-8859-15\0ISO8859-15\0ISO_8859_15\0ISO8859_15\0",
		ONIG_ENCODING_ISO_8859_15
	},
#endif
#ifdef ONIG_ENCODING_ISO_8859_16
	{
		"ISO-8859-16\0ISO8859-16\0ISO_8859_16\0ISO8859_16\0",
		ONIG_ENCODING_ISO_8859_16
	},
#endif
#ifdef ONIG_ENCODING_ASCII
	{
		"ASCII\0US-ASCII\0US_ASCII\0ISO646\0",
		ONIG_ENCODING_ASCII
	},
#endif
	{ NULL, ONIG_ENCODING_UNDEF }
};
/* }}} */

/* {{{ php_onig_name2mbctype */
static OnigEncoding _php_onig_name2mbctype(const char *pname)
{
	const char *p;
	php_onig_enc_name_map_t *mapping;

	if (pname == NULL) {
		return ONIG_ENCODING_UNDEF;
	}

	for (mapping = enc_name_map; mapping->names != NULL; mapping++) {
		for (p = mapping->names; *p != '\0'; p += (strlen(p) + 1)) {
			if (strcasecmp(p, pname) == 0) {
				return mapping->code;
			}
		}
	}

	return ONIG_ENCODING_UNDEF;
}
/* }}} */

/* {{{ php_onig_mbctype2name */
static const char *_php_onig_mbctype2name(OnigEncoding mbctype)
{
	php_onig_enc_name_map_t *mapping;

	for (mapping = enc_name_map; mapping->names != NULL; mapping++) {
		if (mapping->code == mbctype) {
			return mapping->names;
		}
	}

	return NULL;
}
/* }}} */

/*
 * regex cache
 */
/* {{{ php_onig_compile_pattern */
static php_onig_regex_t *php_onig_compile_pattern(const char *pattern, int patlen, OnigOptionType options, OnigEncoding enc, OnigSyntaxType *syntax TSRMLS_DC)
{
	int err_code = 0;
	int found = 0;
	php_onig_regex_t *retval = NULL, **rc = NULL;
	OnigErrorInfo err_info;
	OnigUChar err_str[ONIG_MAX_ERROR_MESSAGE_LEN];

	found = zend_hash_find(&ONIG(ht_rc), (char *)pattern, patlen+1, (void **) &rc);
	if (found == FAILURE || (*rc)->options != options || (*rc)->enc != enc || (*rc)->syntax != syntax) {
		if ((err_code = onig_new(&retval, (OnigUChar *)pattern, (OnigUChar *)(pattern + patlen), options, enc, syntax, &err_info)) != ONIG_NORMAL) {
			onig_error_code_to_str(err_str, err_code, err_info);
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "mbregex compile err: %s", err_str);
			retval = NULL;
			goto out;
		}
		zend_hash_update(&ONIG(ht_rc), (char *) pattern, patlen + 1, (void *) &retval, sizeof(retval), NULL);
	} else if (found == SUCCESS) {
		retval = *rc;
	}
out:
	return retval; 
}
/* }}} */

/* {{{ _php_onig_get_option_string */
static size_t _php_onig_get_option_string(char *str, size_t len, OnigOptionType option, OnigSyntaxType *syntax)
{
	size_t len_left = len;
	size_t len_req = 0;
	char *p = str;
	char c;

	if ((option & ONIG_OPTION_IGNORECASE) != 0) {
		if (len_left > 0) {
			--len_left;
			*(p++) = 'i';
		}
		++len_req;	
	}

	if ((option & ONIG_OPTION_EXTEND) != 0) {
		if (len_left > 0) {
			--len_left;
			*(p++) = 'x';
		}
		++len_req;	
	}

	if ((option & (ONIG_OPTION_MULTILINE | ONIG_OPTION_SINGLELINE)) ==
			(ONIG_OPTION_MULTILINE | ONIG_OPTION_SINGLELINE)) {
		if (len_left > 0) {
			--len_left;
			*(p++) = 'p';
		}
		++len_req;	
	} else {
		if ((option & ONIG_OPTION_MULTILINE) != 0) {
			if (len_left > 0) {
				--len_left;
				*(p++) = 'm';
			}
			++len_req;	
		}

		if ((option & ONIG_OPTION_SINGLELINE) != 0) {
			if (len_left > 0) {
				--len_left;
				*(p++) = 's';
			}
			++len_req;	
		}
	}	
	if ((option & ONIG_OPTION_FIND_LONGEST) != 0) {
		if (len_left > 0) {
			--len_left;
			*(p++) = 'l';
		}
		++len_req;	
	}
	if ((option & ONIG_OPTION_FIND_NOT_EMPTY) != 0) {
		if (len_left > 0) {
			--len_left;
			*(p++) = 'n';
		}
		++len_req;	
	}

	c = 0;

	if (syntax == ONIG_SYNTAX_JAVA) {
		c = 'j';
	} else if (syntax == ONIG_SYNTAX_GNU_REGEX) {
		c = 'u';
	} else if (syntax == ONIG_SYNTAX_GREP) {
		c = 'g';
	} else if (syntax == ONIG_SYNTAX_EMACS) {
		c = 'c';
	} else if (syntax == ONIG_SYNTAX_RUBY) {
		c = 'r';
	} else if (syntax == ONIG_SYNTAX_PERL) {
		c = 'z';
	} else if (syntax == ONIG_SYNTAX_POSIX_BASIC) {
		c = 'b';
	} else if (syntax == ONIG_SYNTAX_POSIX_EXTENDED) {
		c = 'd';
	}

	if (c != 0) {
		if (len_left > 0) {
			--len_left;
			*(p++) = c;
		}
		++len_req;
	}


	if (len_left > 0) {
		--len_left;
		*(p++) = '\0';
	}
	++len_req;	
	if (len < len_req) {
		return len_req;
	}

	return 0;
}
/* }}} */

/* {{{ _php_onig_init_options */
static void
_php_onig_init_options(const char *parg, int narg, OnigOptionType *option, OnigSyntaxType **syntax, int *eval) 
{
	int n;
	char c;
	int optm = 0; 

	*syntax = ONIG_SYNTAX_RUBY;

	if (parg != NULL) {
		n = 0;
		while(n < narg) {
			c = parg[n++];
			switch (c) {
				case 'i':
					optm |= ONIG_OPTION_IGNORECASE;
					break;
				case 'x':
					optm |= ONIG_OPTION_EXTEND;
					break;
				case 'm':
					optm |= ONIG_OPTION_MULTILINE;
					break;
				case 's':
					optm |= ONIG_OPTION_SINGLELINE;
					break;
				case 'p':
					optm |= ONIG_OPTION_MULTILINE | ONIG_OPTION_SINGLELINE;
					break;
				case 'l':
					optm |= ONIG_OPTION_FIND_LONGEST;
					break;
				case 'n':
					optm |= ONIG_OPTION_FIND_NOT_EMPTY;
					break;
				case 'j':
					*syntax = ONIG_SYNTAX_JAVA;
					break;
				case 'u':
					*syntax = ONIG_SYNTAX_GNU_REGEX;
					break;
				case 'g':
					*syntax = ONIG_SYNTAX_GREP;
					break;
				case 'c':
					*syntax = ONIG_SYNTAX_EMACS;
					break;
				case 'r':
					*syntax = ONIG_SYNTAX_RUBY;
					break;
				case 'z':
					*syntax = ONIG_SYNTAX_PERL;
					break;
				case 'b':
					*syntax = ONIG_SYNTAX_POSIX_BASIC;
					break;
				case 'd':
					*syntax = ONIG_SYNTAX_POSIX_EXTENDED;
					break;
				case 'e':
					if (eval != NULL) *eval = 1; 
					break;
				default:
					break;
			}
		}
		if (option != NULL) *option|=optm; 
	}
}
/* }}} */

/* {{{ static void php_onig_free_cache() */
static void php_onig_regex_free_cache(php_onig_regex_t **pre) 
{
	onig_free(*pre);
}
/* }}} */

/* {{{ proto string onig_regex_encoding([string encoding])
   Returns the current encoding for regex as a string. */
PHP_FUNCTION(onig_regex_encoding)
{
	size_t argc = ZEND_NUM_ARGS();
	char *encoding;
	int encoding_len;
	OnigEncoding mbctype;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &encoding, &encoding_len) == FAILURE) {
		return;
	}

	if (argc == 0) {
		const char *retval = _php_onig_mbctype2name(ONIG(current_mbctype));

		if (retval == NULL) {
			return;
		}

		RETURN_STRING((char *)retval, 1);
	} else if (argc == 1) {
		mbctype = _php_onig_name2mbctype(encoding);

		if (mbctype == ONIG_ENCODING_UNDEF) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unknown encoding \"%s\"", encoding);
			return;
		}

		ONIG(current_mbctype) = mbctype;
		RETURN_TRUE;
	}
}
/* }}} */

/* {{{ _php_onig_ereg_exec */
static void _php_onig_ereg_exec(INTERNAL_FUNCTION_PARAMETERS, int icase)
{
	zval **arg_pattern, *array;
	char *string;
	int string_len;
	php_onig_regex_t *re;
	OnigRegion *regs = NULL;
	int i, match_len, beg, end;
	OnigOptionType options;
	char *str;

	array = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Zs|z", &arg_pattern, &string, &string_len, &array) == FAILURE) {
		return;
	}

	options = ONIG(default_options);
	if (icase) {
		options |= ONIG_OPTION_IGNORECASE;
	}

	/* compile the regular expression from the supplied regex */
	if (Z_TYPE_PP(arg_pattern) != IS_STRING) {
		/* we convert numbers to integers and treat them as a string */
		if (Z_TYPE_PP(arg_pattern) == IS_DOUBLE) {
			convert_to_long_ex(arg_pattern);	/* get rid of decimal places */
		}
		convert_to_string_ex(arg_pattern);
		/* don't bother doing an extended regex with just a number */
	}

	if (!Z_STRVAL_PP(arg_pattern) || Z_STRLEN_PP(arg_pattern) == 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "empty pattern");
		RETVAL_FALSE;
		goto out;
	}

	re = php_onig_compile_pattern(Z_STRVAL_PP(arg_pattern), Z_STRLEN_PP(arg_pattern), options, ONIG(current_mbctype), ONIG(default_syntax) TSRMLS_CC);
	if (re == NULL) {
		RETVAL_FALSE;
		goto out;
	}

	regs = onig_region_new();

	/* actually execute the regular expression */
	if (onig_search(re, (OnigUChar *)string, (OnigUChar *)(string + string_len), (OnigUChar *)string, (OnigUChar *)(string + string_len), regs, 0) < 0) {
		RETVAL_FALSE;
		goto out;
	}

	match_len = 1;
	str = string;
	if (array != NULL) {
		match_len = regs->end[0] - regs->beg[0];
		zval_dtor(array);
		array_init(array);
		for (i = 0; i < regs->num_regs; i++) {
			beg = regs->beg[i];
			end = regs->end[i];
			if (beg >= 0 && beg < end && end <= string_len) {
				add_index_stringl(array, i, (char *)&str[beg], end - beg, 1);
			} else {
				add_index_bool(array, i, 0);
			}
		}
	}

	if (match_len == 0) {
		match_len = 1;
	}
	RETVAL_LONG(match_len);
out:
	if (regs != NULL) {
		onig_region_free(regs, 1);
	}
}
/* }}} */

/* {{{ proto int onig_ereg(string pattern, string string [, array registers])
   Regular expression match for multibyte string */
PHP_FUNCTION(onig_ereg)
{
	_php_onig_ereg_exec(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}
/* }}} */

/* {{{ proto int onig_eregi(string pattern, string string [, array registers])
   Case-insensitive regular expression match for multibyte string */
PHP_FUNCTION(onig_eregi)
{
	_php_onig_ereg_exec(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}
/* }}} */

/* {{{ _php_onig_ereg_replace_exec */
static void _php_onig_ereg_replace_exec(INTERNAL_FUNCTION_PARAMETERS, OnigOptionType options)
{
	zval **arg_pattern_zval;

	char *arg_pattern;
	int arg_pattern_len;

	char *replace;
	int replace_len;

	char *string;
	int string_len;

	char *p;
	php_onig_regex_t *re;
	OnigSyntaxType *syntax;
	OnigRegion *regs = NULL;
	smart_str out_buf = { 0 };
	smart_str eval_buf = { 0 };
	smart_str *pbuf;
	int i, err, eval, n;
	OnigUChar *pos;
	OnigUChar *string_lim;
	char *description = NULL;
	char pat_buf[2];

	eval = 0;
	{
		char *option_str = NULL;
		int option_str_len = 0;

		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Zss|s",
									&arg_pattern_zval,
									&replace, &replace_len,
									&string, &string_len,
									&option_str, &option_str_len) == FAILURE) {
			return;
		}

		if (option_str != NULL) {
			_php_onig_init_options(option_str, option_str_len, &options, &syntax, &eval);
		} else {
			options |= ONIG(default_options);
			syntax = ONIG(default_syntax);
		}
	}
	if (Z_TYPE_PP(arg_pattern_zval) == IS_STRING) {
		arg_pattern = Z_STRVAL_PP(arg_pattern_zval);
		arg_pattern_len = Z_STRLEN_PP(arg_pattern_zval);
	} else {
		/* FIXME: this code is not multibyte aware! */
		convert_to_long_ex(arg_pattern_zval);
		pat_buf[0] = (char)Z_LVAL_PP(arg_pattern_zval);	
		pat_buf[1] = '\0';

		arg_pattern = pat_buf;
		arg_pattern_len = 1;	
	}
	/* create regex pattern buffer */
	re = php_onig_compile_pattern(arg_pattern, arg_pattern_len, options, ONIG(current_mbctype), syntax TSRMLS_CC);
	if (re == NULL) {
		RETURN_FALSE;
	}

	if (eval) {
		pbuf = &eval_buf;
		description = zend_make_compiled_string_description("mbregex replace" TSRMLS_CC);
	} else {
		pbuf = &out_buf;
		description = NULL;
	}

	/* do the actual work */
	err = 0;
	pos = (OnigUChar *)string;
	string_lim = (OnigUChar*)(string + string_len);
	regs = onig_region_new();
	while (err >= 0) {
		err = onig_search(re, (OnigUChar *)string, (OnigUChar *)string_lim, pos, (OnigUChar *)string_lim, regs, 0);
		if (err <= -2) {
			OnigUChar err_str[ONIG_MAX_ERROR_MESSAGE_LEN];
			onig_error_code_to_str(err_str, err);
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "mbregex search failure in php_mbereg_replace_exec(): %s", err_str);
			break;
		}
		if (err >= 0) {
#if moriyoshi_0
			if (regs->beg[0] == regs->end[0]) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "Empty regular expression");
				break;
			}
#endif
			/* copy the part of the string before the match */
			smart_str_appendl(&out_buf, pos, (size_t)((OnigUChar *)(string + regs->beg[0]) - pos));
			/* copy replacement and backrefs */
			i = 0;
			p = replace;
			while (i < replace_len) {
				int fwd = 1; /* FIXME */
				n = -1;
				if ((replace_len - i) >= 2 && fwd == 1 &&
					p[0] == '\\' && p[1] >= '0' && p[1] <= '9') {
					n = p[1] - '0';
				}
				if (n >= 0 && n < regs->num_regs) {
					if (regs->beg[n] >= 0 && regs->beg[n] < regs->end[n] && regs->end[n] <= string_len) {
						smart_str_appendl(pbuf, string + regs->beg[n], regs->end[n] - regs->beg[n]);
					}
					p += 2;
					i += 2;
				} else {
					smart_str_appendl(pbuf, p, fwd);
					p += fwd;
					i += fwd;
				}
			}
			if (eval) {
				zval v;
				/* null terminate buffer */
				smart_str_0(&eval_buf);
				/* do eval */
				if (zend_eval_stringl(eval_buf.c, eval_buf.len, &v, description TSRMLS_CC) == FAILURE) {
					efree(description);
					php_error_docref(NULL TSRMLS_CC,E_ERROR, "Failed evaluating code: %s%s", PHP_EOL, eval_buf.c);
					/* zend_error() does not return in this case */
				}

				/* result of eval */
				convert_to_string(&v);
				smart_str_appendl(&out_buf, Z_STRVAL(v), Z_STRLEN(v));
				/* Clean up */
				eval_buf.len = 0;
				zval_dtor(&v);
			}
			n = regs->end[0];
			if ((pos - (OnigUChar *)string) < n) {
				pos = (OnigUChar *)string + n;
			} else {
				if (pos < string_lim) {
					smart_str_appendl(&out_buf, pos, 1); 
				}
				pos++;
			}
		} else { /* nomatch */
			/* stick that last bit of string on our output */
			if (string_lim - pos > 0) {
				smart_str_appendl(&out_buf, pos, string_lim - pos);
			}
		}
		onig_region_free(regs, 0);
	}

	if (description) {
		efree(description);
	}
	if (regs != NULL) {
		onig_region_free(regs, 1);
	}
	smart_str_free(&eval_buf);

	if (err <= -2) {
		smart_str_free(&out_buf);	
		RETVAL_FALSE;
	} else {
		smart_str_appendc(&out_buf, '\0');
		RETVAL_STRINGL((char *)out_buf.c, out_buf.len - 1, 0);
	}
}
/* }}} */

/* {{{ proto string onig_ereg_replace(string pattern, string replacement, string string [, string option])
   Replace regular expression for multibyte string */
PHP_FUNCTION(onig_ereg_replace)
{
	_php_onig_ereg_replace_exec(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}
/* }}} */

/* {{{ proto string onig_eregi_replace(string pattern, string replacement, string string)
   Case insensitive replace regular expression for multibyte string */
PHP_FUNCTION(onig_eregi_replace)
{
	_php_onig_ereg_replace_exec(INTERNAL_FUNCTION_PARAM_PASSTHRU, ONIG_OPTION_IGNORECASE);
}
/* }}} */

/* {{{ proto array onig_split(string pattern, string string [, int limit])
   split multibyte string into array by regular expression */
PHP_FUNCTION(onig_split)
{
	char *arg_pattern;
	int arg_pattern_len;
	php_onig_regex_t *re;
	OnigRegion *regs = NULL;
	char *string;
	OnigUChar *pos;
	int string_len;

	int n, err;
	long count = -1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|l", &arg_pattern, &arg_pattern_len, &string, &string_len, &count) == FAILURE) {
		return;
	} 

	if (count == 0) {
		count = 1;
	}

	/* create regex pattern buffer */
	if ((re = php_onig_compile_pattern(arg_pattern, arg_pattern_len, ONIG(default_options), ONIG(current_mbctype), ONIG(default_syntax) TSRMLS_CC)) == NULL) {
		RETURN_FALSE;
	}

	array_init(return_value);

	pos = (OnigUChar *)string;
	err = 0;
	regs = onig_region_new();
	/* churn through str, generating array entries as we go */
	while ((--count != 0) &&
		   (err = onig_search(re, (OnigUChar *)string, (OnigUChar *)(string + string_len), pos, (OnigUChar *)(string + string_len), regs, 0)) >= 0) {
		if (regs->beg[0] == regs->end[0]) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Empty regular expression");
			break;
		}

		/* add it to the array */
		if (regs->beg[0] < string_len && regs->beg[0] >= (pos - (OnigUChar *)string)) {
			add_next_index_stringl(return_value, (char *)pos, ((OnigUChar *)(string + regs->beg[0]) - pos), 1);
		} else {
			err = -2;
			break;
		}
		/* point at our new starting point */
		n = regs->end[0];
		if ((pos - (OnigUChar *)string) < n) {
			pos = (OnigUChar *)string + n;
		}
		if (count < 0) {
			count = 0;
		}
		onig_region_free(regs, 0);
	}

	onig_region_free(regs, 1);

	/* see if we encountered an error */
	if (err <= -2) {
		OnigUChar err_str[ONIG_MAX_ERROR_MESSAGE_LEN];
		onig_error_code_to_str(err_str, err);
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "mbregex search failure in mbsplit(): %s", err_str);
		zval_dtor(return_value);
		RETURN_FALSE;
	}

	/* otherwise we just have one last element to add to the array */
	n = ((OnigUChar *)(string + string_len) - pos);
	if (n > 0) {
		add_next_index_stringl(return_value, (char *)pos, n, 1);
	} else {
		add_next_index_stringl(return_value, "", 0, 1);
	}
}
/* }}} */

/* {{{ proto bool onig_ereg_match(string pattern, string string [,string option])
   Regular expression match for multibyte string */
PHP_FUNCTION(onig_ereg_match)
{
	char *arg_pattern;
	int arg_pattern_len;

	char *string;
	int string_len;

	php_onig_regex_t *re;
	OnigSyntaxType *syntax;
	OnigOptionType option = 0;
	int err;

	{
		char *option_str = NULL;
		int option_str_len = 0;

		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|s",
		                          &arg_pattern, &arg_pattern_len, &string, &string_len,
		                          &option_str, &option_str_len)==FAILURE) {
			return;
		}

		if (option_str != NULL) {
			_php_onig_init_options(option_str, option_str_len, &option, &syntax, NULL);
		} else {
			option |= ONIG(default_options);
			syntax = ONIG(default_syntax);
		}
	}

	if ((re = php_onig_compile_pattern(arg_pattern, arg_pattern_len, option, ONIG(current_mbctype), syntax TSRMLS_CC)) == NULL) {
		RETURN_FALSE;
	}

	/* match */
	err = onig_match(re, (OnigUChar *)string, (OnigUChar *)(string + string_len), (OnigUChar *)string, NULL, 0);
	if (err >= 0) {
		RETVAL_TRUE;
	} else {
		RETVAL_FALSE;
	}
}
/* }}} */

/* {{{ php_onig_set_options */
static void _php_onig_set_options(OnigOptionType options, OnigSyntaxType *syntax, OnigOptionType *prev_options, OnigSyntaxType **prev_syntax TSRMLS_DC) 
{
	if (prev_options != NULL) {
		*prev_options = ONIG(default_options);
	}
	if (prev_syntax != NULL) {
		*prev_syntax = ONIG(default_syntax);
	}
	ONIG(default_options) = options;
	ONIG(default_syntax) = syntax;
}
/* }}} */

/* {{{ proto string onig_regex_set_options([string options])
   Set or get the default options for mbregex functions */
PHP_FUNCTION(onig_regex_set_options)
{
	OnigOptionType opt;
	OnigSyntaxType *syntax;
	char *string = NULL;
	int string_len;
	char buf[16];

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s",
	                          &string, &string_len) == FAILURE) {
		return;
	}
	if (string != NULL) {
		opt = 0;
		syntax = NULL;
		_php_onig_init_options(string, string_len, &opt, &syntax, NULL);
		_php_onig_set_options(opt, syntax, NULL, NULL TSRMLS_CC);
	} else {
		opt = ONIG(default_options);
		syntax = ONIG(default_syntax);
	}
	_php_onig_get_option_string(buf, sizeof(buf), opt, syntax);

	RETVAL_STRING(buf, 1);
}
/* }}} */

#endif	/* HAVE_ONIG */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: fdm=marker
 * vim: noet sw=4 ts=4
 */
