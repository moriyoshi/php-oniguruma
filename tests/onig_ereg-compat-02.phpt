--TEST--
onig_ereg() compat test 2
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_ereg') or die("skip onig_ereg() is not available in this build");
?>
--FILE--
<?php
/* (counterpart: ext/standard/tests/reg/005.phpt) */
  $a="This is a nice and simple string";
  echo onig_ereg(".*(is).*(is).*",$a,$registers);
  echo "\n";
  echo $registers[0];
  echo "\n";
  echo $registers[1];
  echo "\n";
  echo $registers[2];
  echo "\n";
?>
--EXPECT--
32
This is a nice and simple string
is
is
