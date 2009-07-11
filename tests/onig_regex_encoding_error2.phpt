--TEST--
Test onig_regex_encoding() function : error conditions - Pass an unknown encoding
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
 * Pass onig_regex_encoding an unknown type of encoding
 */

echo "*** Testing onig_regex_encoding() : error conditions ***\n";

var_dump(onig_regex_encoding('unknown'));


echo "Done";
?>
--EXPECTF--
*** Testing onig_regex_encoding() : error conditions ***

Warning: onig_regex_encoding(): Unknown encoding "unknown" in %s on line %d
NULL
Done
