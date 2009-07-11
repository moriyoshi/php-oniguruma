--TEST--
Test onig_regex_encoding() function : basic functionality
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_regex_encoding') or die("skip onig_regex_encoding() is not available in this build");
?>
--FILE--
<?php
/* Prototype  : proto string onig_regex_encoding([string encoding])
 * Description: Returns the current encoding for regex as a string. 
 * Source code: ext/onig/php_mbregex.c 
 */

/*
 * Test Basic functionality of onig_regex_encoding
 */

echo "*** Testing onig_regex_encoding() : basic functionality ***\n";

var_dump(onig_regex_encoding());

var_dump(onig_regex_encoding('UTF-8'));

var_dump(onig_regex_encoding());


echo "Done";
?>
--EXPECTF--
*** Testing onig_regex_encoding() : basic functionality ***
string(%d) "%s"
bool(true)
string(5) "UTF-8"
Done
