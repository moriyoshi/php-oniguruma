--TEST--
Test onig_regex_encoding() function : usage variations - Pass different data types as $encoding arg
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_regex_encoding') or die("skip onig_regex_encoding() is not available in this build");
?>
--FILE--
<?php
/* Prototype  : string onig_regex_encoding([string $encoding])
 * Description: Returns the current encoding for regex as a string. 
 * Source code: ext/onig/php_mbregex.c
 */

/*
 * Pass different data types as $encoding argument to onig_regex_encoding() to test behaviour
 * Where possible, 'UTF-8' has been entered as a string value
 */

echo "*** Testing onig_regex_encoding() : usage variations ***\n";
//get an unset variable
$unset_var = 10;
unset ($unset_var);

// get a class
class classA
{
  public function __toString() {
    return "UTF-8";
  }
}

// heredoc string
$heredoc = <<<EOT
UTF-8
EOT;

// get a resource variable
$fp = fopen(__FILE__, "r");

// unexpected values to be passed to $encoding argument
$inputs = array(

       // int data
/*1*/  0,
       1,
       12345,
       -2345,

       // float data
/*5*/  10.5,
       -10.5,
       12.3456789000e10,
       12.3456789000E-10,
       .5,

       // null data
/*10*/ NULL,
       null,

       // boolean data
/*12*/ true,
       false,
       TRUE,
       FALSE,
       
       // empty data
/*16*/ "",
       '',

       // string data
/*18*/ "UTF-8",
       'UTF-8',
       $heredoc,
       
       // object data
/*21*/ new classA(),

       // undefined data
/*22*/ @$undefined_var,

       // unset data
/*23*/ @$unset_var,

       // resource variable
/*24*/ $fp
);

// loop through each element of $inputs to check the behavior of onig_regex_encoding()
$iterator = 1;
foreach($inputs as $input) {
  echo "\n-- Iteration $iterator --\n";
  var_dump( onig_regex_encoding($input) );
  $iterator++;
};

fclose($fp);

echo "Done";
?>
--EXPECTF--
*** Testing onig_regex_encoding() : usage variations ***

-- Iteration 1 --

Warning: onig_regex_encoding(): Unknown encoding "0" in %s on line %d
NULL

-- Iteration 2 --

Warning: onig_regex_encoding(): Unknown encoding "1" in %s on line %d
NULL

-- Iteration 3 --

Warning: onig_regex_encoding(): Unknown encoding "12345" in %s on line %d
NULL

-- Iteration 4 --

Warning: onig_regex_encoding(): Unknown encoding "-2345" in %s on line %d
NULL

-- Iteration 5 --

Warning: onig_regex_encoding(): Unknown encoding "10.5" in %s on line %d
NULL

-- Iteration 6 --

Warning: onig_regex_encoding(): Unknown encoding "-10.5" in %s on line %d
NULL

-- Iteration 7 --

Warning: onig_regex_encoding(): Unknown encoding "123456789000" in %s on line %d
NULL

-- Iteration 8 --

Warning: onig_regex_encoding(): Unknown encoding "1.23456789E-9" in %s on line %d
NULL

-- Iteration 9 --

Warning: onig_regex_encoding(): Unknown encoding "0.5" in %s on line %d
NULL

-- Iteration 10 --

Warning: onig_regex_encoding(): Unknown encoding "" in %s on line %d
NULL

-- Iteration 11 --

Warning: onig_regex_encoding(): Unknown encoding "" in %s on line %d
NULL

-- Iteration 12 --

Warning: onig_regex_encoding(): Unknown encoding "1" in %s on line %d
NULL

-- Iteration 13 --

Warning: onig_regex_encoding(): Unknown encoding "" in %s on line %d
NULL

-- Iteration 14 --

Warning: onig_regex_encoding(): Unknown encoding "1" in %s on line %d
NULL

-- Iteration 15 --

Warning: onig_regex_encoding(): Unknown encoding "" in %s on line %d
NULL

-- Iteration 16 --

Warning: onig_regex_encoding(): Unknown encoding "" in %s on line %d
NULL

-- Iteration 17 --

Warning: onig_regex_encoding(): Unknown encoding "" in %s on line %d
NULL

-- Iteration 18 --
bool(true)

-- Iteration 19 --
bool(true)

-- Iteration 20 --
bool(true)

-- Iteration 21 --
bool(true)

-- Iteration 22 --

Warning: onig_regex_encoding(): Unknown encoding "" in %s on line %d
NULL

-- Iteration 23 --

Warning: onig_regex_encoding(): Unknown encoding "" in %s on line %d
NULL

-- Iteration 24 --

Warning: onig_regex_encoding() expects parameter 1 to be string, resource given in %s on line %d
NULL
Done
