--TEST--
Test onig_split() function : error conditions 
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_split') or die("skip onig_split() is not available in this build");
?>
--FILE--
<?php
/* Prototype  : proto array onig_split(string pattern, string string [, int limit])
 * Description: split multibyte string into array by regular expression 
 * Source code: ext/onig/php_mbregex.c
 * Alias to functions: 
 */

/*
 * test too few and too many parameters
 */

echo "*** Testing onig_split() : error conditions ***\n";


//Test onig_split with one more than the expected number of arguments
echo "\n-- Testing onig_split() function with more than expected no. of arguments --\n";
$pattern = ' ';
$string = 'a b c d e f g';
$limit = 0;
$extra_arg = 10;
var_dump( onig_split($pattern, $string, $limit, $extra_arg) );

// Testing onig_split with one less than the expected number of arguments
echo "\n-- Testing onig_split() function with less than expected no. of arguments --\n";
$pattern = 'string_val';
var_dump( onig_split($pattern) );

echo "Done";
?>
--EXPECTF--
*** Testing onig_split() : error conditions ***

-- Testing onig_split() function with more than expected no. of arguments --

Warning: onig_split() expects at most 3 parameters, 4 given in %s on line %d
NULL

-- Testing onig_split() function with less than expected no. of arguments --

Warning: onig_split() expects at least 2 parameters, 1 given in %s on line %d
NULL
Done
