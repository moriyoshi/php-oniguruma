--TEST--
php-users@php.gr.jp #10830 
--SKIPIF--
<?php
extension_loaded('onig') or die('skip onig not available');
function_exists('onig_ereg') or die("skip onig_ereg() is not available in this build");
?>
--FILE--
<?php
$a="aaa\n<>";

var_dump( onig_ereg("^[^><]+$",$a) );
var_dump( ereg("^[^><]+$",$a) );
?>

--EXPECTF--
bool(false)

Deprecated: Function ereg() is deprecated in %s on line %d
bool(false)
