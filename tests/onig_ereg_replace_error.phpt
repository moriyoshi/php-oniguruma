--TEST--
Test onig_ereg_replace() function : error conditions 
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_ereg_replace') or die("skip onig_ereg_replace() is not available in this build");
?>
--FILE--
<?php
/* Prototype  : proto string onig_ereg_replace(string pattern, string replacement, string string [, string option])
 * Description: Replace regular expression for multibyte string 
 * Source code: ext/onig/php_mbregex.c
 * Alias to functions: 
 */

echo "*** Testing onig_ereg_replace() : error conditions ***\n";

//Test onig_ereg_replace with one more than the expected number of arguments
echo "\n-- Testing onig_ereg_replace() function with more than expected no. of arguments --\n";
$pattern = b'[a-k]';
$replacement = b'1';
$string = b'string_val';
$option = '';
$extra_arg = 10;
var_dump( onig_ereg_replace($pattern, $replacement, $string, $option, $extra_arg) );

// Testing onig_ereg_replace with one less than the expected number of arguments
echo "\n-- Testing onig_ereg_replace() function with less than expected no. of arguments --\n";
$pattern = b'string_val';
$replacement = b'string_val';
var_dump( onig_ereg_replace($pattern, $replacement) );

echo "Done";
?>
--EXPECTF--
*** Testing onig_ereg_replace() : error conditions ***

-- Testing onig_ereg_replace() function with more than expected no. of arguments --

Warning: onig_ereg_replace() expects at most 4 parameters, 5 given in %s on line %d
NULL

-- Testing onig_ereg_replace() function with less than expected no. of arguments --

Warning: onig_ereg_replace() expects at least 3 parameters, 2 given in %s on line %d
NULL
Done
