--TEST--
Bug #43301 (onig_ereg*_replace() crashes when replacement string is invalid PHP expression and 'e' option is used)
--SKIPIF--
<?php extension_loaded('onig') or die('skip onig not available'); ?>
--FILE--
<?php

$ptr = 'hello';

$txt = <<<doc
hello, I have got a cr*sh on you
doc;

echo onig_ereg_replace($ptr,'$1',$txt,'e');

?>
--EXPECTF--
Parse error: syntax error, unexpected T_LNUMBER, expecting T_VARIABLE or '$' in %s/bug43301.php(%d) : mbregex replace on line 1

Fatal error: onig_ereg_replace(): Failed evaluating code: 
$1 in %s/bug43301.php on line %d
