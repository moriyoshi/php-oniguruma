--TEST--
onig_ereg_replace() compat test 4
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_ereg_replace') or die("skip onig_ereg_replace() is not available in this build");
?>
--FILE--
<?php
/* (counterpart: ext/standard/tests/reg/006.phpt) */
  $a="This is a nice and simple string";
  echo onig_ereg_replace("^This","That",$a);
?>
--EXPECT--
That is a nice and simple string
