--TEST--
onig_ereg_replace() compat test 8
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_ereg_replace') or die("skip onig_ereg_replace() is not available in this build");
?>
--FILE--
<?php
/* (counterpart: ext/standard/tests/reg/011.phpt) */
  $a="abc123";
  echo onig_ereg_replace("123","def\\0ghi",$a);
?>
--EXPECT--
abcdef123ghi
