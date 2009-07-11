--TEST--
onig_ereg_replace() compat test 12
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_ereg_replace') or die("skip onig_ereg_replace() is not available in this build");
?>
--FILE--
<?php
/* (counterpart: ext/standard/tests/reg/015.phpt) */
  echo onig_ereg_replace("^","z","abc123");
?>
--EXPECT--
zabc123
