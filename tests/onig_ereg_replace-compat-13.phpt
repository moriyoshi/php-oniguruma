--TEST--
onig_ereg_replace() compat test 13
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_ereg_replace') or die("skip onig_ereg_replace() is not available in this build");
?>
--FILE--
<?php
/* (counterpart: ext/standard/tests/reg/016.phpt) */
  echo onig_ereg_replace('\?',"abc","?123?");
?>
--EXPECT--
abc123abc
