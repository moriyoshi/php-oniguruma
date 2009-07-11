--TEST--
php-dev@php.gr.jp #884 (1)
--SKIPIF--
<?php
extension_loaded('onig') or die('skip onig not available');
function_exists('onig_ereg_replace') or die("skip onig_ereg_replace() is not available in this build");
?>
--FILE--
<?php
set_time_limit(2);
var_dump(ereg_replace(".*", "b", "a"));
var_dump(preg_replace("/.*/", "b", "a"));
var_dump(onig_ereg_replace(".*", "b", "a"));
?>
--EXPECTF--
Deprecated: Function ereg_replace() is deprecated in %s on line %d
string(2) "bb"
string(2) "bb"
string(2) "bb"
