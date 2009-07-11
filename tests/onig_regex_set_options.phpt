--TEST--
onig_regex_set_options() 
--SKIPIF--
<?php
extension_loaded('onig') or die("skip onig not available\n");
function_exists('onig_regex_set_options') or die("skip\n");
?>
--FILE--
<?php
	onig_regex_set_options( 'x' );
	print onig_ereg_replace(' -', '+', '- - - - -' );

	onig_regex_set_options( '' );
	print onig_ereg_replace(' -', '+', '- - - - -' );
?>

--EXPECT--
+ + + + +-++++
