--TEST--
Test onig_ereg() function : error conditions - pass incorrect number of arguments
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_ereg') or die("skip onig_ereg() is not available in this build");
?>
--FILE--
<?php
/* Prototype  : int onig_ereg(string $pattern, string $string [, array $registers])
 * Description: Regular expression match for multibyte string 
 * Source code: ext/onig/php_mbregex.c
 */

/*
 * Test behaviour of onig_ereg() when passed an incorrcect number of arguments
 */

echo "*** Testing onig_ereg() : error conditions ***\n";


//Test onig_ereg with one more than the expected number of arguments
echo "\n-- Testing onig_ereg() function with more than expected no. of arguments --\n";
$pattern = b'string_val';
$string = b'string_val';
$registers = array(1, 2);
$extra_arg = 10;
var_dump( onig_ereg($pattern, $string, $registers, $extra_arg) );

// Testing onig_ereg with one less than the expected number of arguments
echo "\n-- Testing onig_ereg() function with less than expected no. of arguments --\n";
$pattern = b'string_val';
var_dump( onig_ereg($pattern) );

echo "Done";
?>
--EXPECTF--
*** Testing onig_ereg() : error conditions ***

-- Testing onig_ereg() function with more than expected no. of arguments --

Warning: onig_ereg() expects at most 3 parameters, 4 given in %s on line %d
NULL

-- Testing onig_ereg() function with less than expected no. of arguments --

Warning: onig_ereg() expects at least 2 parameters, 1 given in %s on line %d
NULL
Done
