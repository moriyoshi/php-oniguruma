--TEST--
onig_ereg_replace()
--SKIPIF--
<?php
extension_loaded('onig') or die('skip onig not available');
function_exists('onig_ereg_replace') or die("skip onig_ereg_replace() is not available in this build");
?>
--FILE--
<?php
	onig_regex_set_options( '' );
	print onig_ereg_replace( ' ', '-', 'a b c d e' )."\n";
	print onig_ereg_replace( '([a-z]+)','[\\1]', 'abc def ghi' );
?>

--EXPECT--
a-b-c-d-e
[abc] [def] [ghi]

