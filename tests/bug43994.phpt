--TEST--
Test onig_ereg() function : onig_ereg 'successfully' matching incorrectly
--SKIPIF--
<?php
extension_loaded('onig') or die('skip');
function_exists('onig_ereg') or die("skip onig_ereg() is not available in this build");
?>
--FILE--
<?php
/* Prototype  : int onig_ereg(string $pattern, string $string [, array $registers])
 * Description: Regular expression match for multibyte string 
 * Source code: ext/onig/php_mbregex.c
 */

/*
 * onig_ereg 'successfully' matching incorrectly: 
 * Bug now seems to be fixed - error message is now generated when an 'empty'
 * pattern is supplied to onig_ereg. Similar error message to ereg().
 */

$unset_var = 10;
unset ($unset_var);
$inputs = array(NULL, null, false, FALSE, "", '', @$undefined_var,
@$unset_var);

$iterator = 1;
foreach($inputs as $input) {
	if(@is_array($onig_regs)){
		$onig_regs = '';
	}
	echo "\n-- Iteration $iterator --\n";
	echo "Without \$regs arg:\n";
	var_dump( onig_ereg($input, 'hello, world') );
	echo "With \$regs arg:\n";
	var_dump(onig_ereg($input, 'hello, world', $onig_regs));
	var_dump($onig_regs);
	$iterator++;
};
?>

--EXPECTF---- 

-- Iteration 1 --
Without $regs arg:

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
With $regs arg:

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
NULL

-- Iteration 2 --
Without $regs arg:

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
With $regs arg:

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
NULL

-- Iteration 3 --
Without $regs arg:

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
With $regs arg:

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
NULL

-- Iteration 4 --
Without $regs arg:

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
With $regs arg:

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
NULL

-- Iteration 5 --
Without $regs arg:

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
With $regs arg:

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
NULL

-- Iteration 6 --
Without $regs arg:

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
With $regs arg:

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
NULL

-- Iteration 7 --
Without $regs arg:

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
With $regs arg:

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
NULL

-- Iteration 8 --
Without $regs arg:

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
With $regs arg:

Warning: onig_ereg(): empty pattern in %s on line %d
bool(false)
NULL
