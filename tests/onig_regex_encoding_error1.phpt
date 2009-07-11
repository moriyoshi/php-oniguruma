--TEST--
Test onig_regex_encoding() function : error conditions - pass incorrect number of args
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_regex_encoding') or die("skip onig_regex_encoding() is not available in this build");
?>
--FILE--
<?php
/* Prototype  : string onig_regex_encoding([string $encoding])
 * Description: Returns the current encoding for regex as a string. 
 * Source code: ext/onig/php_mbregex.c
 */

/*
 * Test onig_regex_encoding with one more than expected number of arguments
 */

echo "*** Testing onig_regex_encoding() : error conditions ***\n";


echo "\n-- Testing onig_regex_encoding() function with more than expected no. of arguments --\n";
$encoding = 'string_val';
$extra_arg = 10;
var_dump( onig_regex_encoding($encoding, $extra_arg) );

echo "Done";
?>
--EXPECTF--
*** Testing onig_regex_encoding() : error conditions ***

-- Testing onig_regex_encoding() function with more than expected no. of arguments --

Warning: onig_regex_encoding() expects at most 1 parameter, 2 given in %s on line %d
NULL
Done
