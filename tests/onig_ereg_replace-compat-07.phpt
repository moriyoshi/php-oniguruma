--TEST--
onig_ereg_replace() compat test 7
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_ereg_replace') or die("skip onig_ereg_replace() is not available in this build");
?>
--FILE--
<?php
/* (counterpart: ext/standard/tests/reg/010.phpt) */
  $a="abc122222222223";
  echo onig_ereg_replace("1(2*)3","\\1def\\1",$a);
?>
--EXPECT--
abc2222222222def2222222222
