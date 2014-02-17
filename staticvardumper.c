/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_staticvardumper.h"

int dump_statics(zend_function *function, zval *return_value TSRMLS_DC)
{
	if (function->type == ZEND_USER_FUNCTION) {
		ulong    hashIndex    = 0;
		char*    hashKey      = NULL;
		int      hashKeyType  = 0;
		zval     *statics;

		MAKE_STD_ZVAL(statics);
		array_init(statics);

		zend_hash_internal_pointer_reset(function->op_array.static_variables);
		while ((hashKeyType = zend_hash_get_current_key(function->op_array.static_variables, &hashKey, &hashIndex, 0)) != HASH_KEY_NON_EXISTANT) {
			if (hashKeyType == HASH_KEY_IS_STRING) {
				zval *value;
				zend_hash_get_current_data(function->op_array.static_variables, (void **)&value);
				add_assoc_zval(statics, hashKey, value);
			}
			zend_hash_move_forward(function->op_array.static_variables);
		}

		add_assoc_zval(return_value, function->op_array.function_name, statics);
	}
	return 0;
}

/* {{{ proto string dump_static_vars()
   Return an array with all static variables */
PHP_FUNCTION(dump_static_vars)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	array_init(return_value);
	zend_hash_apply_with_argument(EG(function_table), (apply_func_arg_t) dump_statics, return_value TSRMLS_CC);
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(staticvardumper)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "staticvardumper support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ staticvardumper_functions[]
 *
 * Every user visible function must have an entry in staticvardumper_functions[].
 */
const zend_function_entry staticvardumper_functions[] = {
	PHP_FE(dump_static_vars,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in staticvardumper_functions[] */
};
/* }}} */

/* {{{ staticvardumper_module_entry
 */
zend_module_entry staticvardumper_module_entry = {
	STANDARD_MODULE_HEADER,
	"staticvardumper",
	staticvardumper_functions,
	NULL,
	NULL,
	NULL,
	NULL,
	PHP_MINFO(staticvardumper),
	PHP_STATICVARDUMPER_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_STATICVARDUMPER
ZEND_GET_MODULE(staticvardumper)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
