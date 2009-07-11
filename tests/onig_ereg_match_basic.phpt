--TEST--
Test onig_ereg_match() function : basic functionality
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_ereg_match') or die("skip onig_ereg_match() is not available in this build");
?>
--FILE--
<?php
/* Prototype  : bool onig_ereg_match(string $pattern, string $string [,string $option])
 * Description: Regular expression match for multibyte string 
 * Source code: ext/onig/php_mbregex.c
 */

/*
 * Test basic functionality of onig_ereg_match
 */

onig_regex_encoding('UTF-8');

echo "*** Testing onig_ereg_match() : basic functionality ***\n";
$string_ascii = b'abc def, 0123456789';
$string_mb = base64_decode('5pel5pys6Kqe44OG44Kt44K544OI44Gn44GZ44CCMDEyMzTvvJXvvJbvvJfvvJjvvJnjgII=');

//will return true as pattern matches from start of string
echo "\n-- ASCII string 1 --\n";
var_dump(onig_ereg_match(b'.*def', $string_ascii));

//will return false as pattern would match half way through string 
echo "\n-- ASCII string 2 --\n";
var_dump(onig_ereg_match(b'def', $string_ascii));

echo "\n-- Multibyte string 1 --\n";
$regex1 = base64_decode('5pel5pys6KqeKC4qKT9bMS05XSs=');
var_dump(onig_ereg_match($regex1, $string_mb, b'i'));

echo "\n-- Multibyte string 2 --\n";
$regex2 = base64_decode('5LiW55WM');
var_dump(onig_ereg_match($regex2, $string_mb));

echo "Done";
?>
--EXPECTF--

*** Testing onig_ereg_match() : basic functionality ***

-- ASCII string 1 --
bool(true)

-- ASCII string 2 --
bool(false)

-- Multibyte string 1 --
bool(true)

-- Multibyte string 2 --
bool(false)
Done
