--TEST--
onig_ereg_replace() compat test 10
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_ereg_replace') or die("skip onig_ereg_replace() is not available in this build");
?>
--FILE--
<?php
/* (counterpart: ext/standard/tests/reg/013.phpt) */
  $a="abc123";
  echo onig_ereg_replace("123","def\\g\\\\hi\\",$a);
?>
--EXPECT--
abcdef\g\\hi\
