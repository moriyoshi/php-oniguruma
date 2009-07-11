--TEST--
onig_ereg() compat test 1
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_ereg') or die("skip onig_ereg() is not available in this build");
?>
--FILE--
<?php
/* (counterpart: ext/standard/tests/reg/004.phpt) */
  $a="This is a nice and simple string";
  if (onig_ereg(".*nice and simple.*",$a)) {
    echo "ok\n";
  }
  if (!onig_ereg(".*doesn't exist.*",$a)) {
    echo "ok\n";
  }
?>
--EXPECT--
ok
ok
