--TEST--
onig_ereg() and invalid arguments
--SKIPIF--
<?php if (!function_exists("onig_ereg")) print "skip"; ?>
--FILE--
<?php

$a = array(
	array(1,2,3),
	array("", "", ""),
	array(array(), 1, ""),
	array(1, array(), ""),
	array(1, "", array()),
	);

foreach ($a as $args) {
	var_dump(onig_ereg($args[0], $args[1], $args[2]));
	var_dump($args);
}
?>
===DONE===
--EXPECTF--	
bool(false)
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
array(3) {
  [0]=>
  string(0) ""
  [1]=>
  string(0) ""
  [2]=>
  string(0) ""
}

Notice: Array to string conversion in %s on line %d
bool(false)
array(3) {
  [0]=>
  array(0) {
  }
  [1]=>
  int(1)
  [2]=>
  string(0) ""
}

Warning: onig_ereg() expects parameter 2 to be string, array given in %s on line %d
NULL
array(3) {
  [0]=>
  int(1)
  [1]=>
  array(0) {
  }
  [2]=>
  string(0) ""
}
bool(false)
array(3) {
  [0]=>
  int(1)
  [1]=>
  string(0) ""
  [2]=>
  array(0) {
  }
}
===DONE===
