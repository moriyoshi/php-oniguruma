--TEST--
onig_ereg_replace() compat test 6
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_ereg_replace') or die("skip onig_ereg_replace() is not available in this build");
?>
--FILE--
<?php
/* (counterpart: ext/standard/tests/reg/008.phpt) */
  echo onig_ereg_replace("([a-z]*)([-=+|]*)([0-9]+)","\\3 \\1 \\2\n","abc+-|=123");
?>
--EXPECT--
123 abc +-|=
