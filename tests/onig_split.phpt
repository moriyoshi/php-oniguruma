--TEST--
onig_split()
--SKIPIF--
<?php
extension_loaded('onig') or die('skip onig not available');
function_exists('onig_split') or die("skip onig_split() is not available in this build");
?>
--INI--
onig.func_overload=0
--FILE--
<?php
	onig_regex_set_options( '' );
	onig_regex_encoding( 'EUC-JP' );

	function verify_split( $spliton, $str, $count = 0 )
	{
		$result1 = onig_split( $spliton, $str, $count );
		$result2 = split( $spliton, $str, $count );
		if ( $result1 == $result2 ) {
			print "ok\n";
		} else {
			print count($result1).'-'.count($result2)."\n";
		}	
	}

	var_dump( onig_split( b" ", b"a b c d e f g" )
	          == onig_split( b"[[:space:]]", b"a\nb\tc\nd e f g" ) );

	for ( $i = 0; $i < 5; ++$i ) {
		verify_split( b" ", b"a\tb\tc\td   e\tf g", $i );
	}

	for ( $i = 1; $i < 5; ++$i ) {
		verify_split( b"\xa1\xa1+", b"\xa1\xa1\xa1\xa2\xa2\xa1\xa1\xa1\xa1\xa1\xa1\xa2\xa2\xa1\xa1\xa1", $i );
	}
?>

--EXPECTF--
bool(true)

Deprecated: Function split() is deprecated in %s on line %d
ok

Deprecated: Function split() is deprecated in %s on line %d
ok

Deprecated: Function split() is deprecated in %s on line %d
ok

Deprecated: Function split() is deprecated in %s on line %d
ok

Deprecated: Function split() is deprecated in %s on line %d
ok

Deprecated: Function split() is deprecated in %s on line %d
ok

Deprecated: Function split() is deprecated in %s on line %d
2-2

Deprecated: Function split() is deprecated in %s on line %d
3-3

Deprecated: Function split() is deprecated in %s on line %d
4-4

