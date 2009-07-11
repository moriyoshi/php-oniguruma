--TEST--
onig_split() compat test 1
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_split') or die("skip onig_split() is not available in this build");
?>
--FILE--
<?php
/* (counterpart: ext/standard/tests/reg/009.phpt) */
	$a=onig_split("[[:space:]]","this is	a
test");
	echo count($a) . "\n";
	for ($i = 0; $i < count($a); $i++) {
          echo $a[$i] . "\n";
        }
?>
--EXPECT--
4
this
is
a
test
