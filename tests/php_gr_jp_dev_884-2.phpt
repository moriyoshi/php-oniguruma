--TEST--
php-dev@php.gr.jp #884 (2)
--SKIPIF--
<?php
extension_loaded('onig') or die('skip onig not available');
function_exists('onig_ereg_replace') or die("skip onig_ereg_replace() is not available in this build");
?>
--FILE--
<?php
var_dump(onig_ereg_replace("C?$", "Z", "ABC"));
var_dump(ereg_replace("C?$", "Z", "ABC"));
var_dump(onig_ereg_replace("C*$", "Z", "ABC"));
var_dump(ereg_replace("C*$", "Z", "ABC"));
?>
--EXPECTF--
string(4) "ABZZ"

Deprecated: Function ereg_replace() is deprecated in %s on line %d
string(4) "ABZZ"
string(4) "ABZZ"

Deprecated: Function ereg_replace() is deprecated in %s on line %d
string(4) "ABZZ"
