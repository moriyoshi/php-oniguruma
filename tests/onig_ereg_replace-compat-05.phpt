--TEST--
onig_ereg_replace() compat test 5
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_ereg_replace') or die("skip onig_ereg_replace() is not available in this build");
?>
--FILE--
<?php
/* (counterpart: ext/standard/tests/reg/007.phpt) */
  $a="abcd";
  $b=onig_ereg_replace("abcd","",$a);
  echo "strlen(\$b)=".strlen($b);
?>
--EXPECT--
strlen($b)=0
