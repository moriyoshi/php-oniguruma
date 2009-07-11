--TEST--
onig_ereg()
--SKIPIF--
<?php
extension_loaded('onig') or die('skip onig not available');
function_exists('onig_ereg') or die("skip onig_ereg() is not available in this build");
?>
--INI--
output_handler=
--FILE--
<?php
	onig_regex_set_options( '' );

	$encs = array( 'EUC-JP', 'Shift_JIS', 'SJIS', 'UTF-8' );

	function test_ereg( $test_enc, $pat, $str, $in_enc = 'EUC-JP' ) {
		onig_regex_encoding( $test_enc );
		$pat = iconv( $in_enc, $test_enc, $pat  );
		$str = iconv( $in_enc, $test_enc, $str );
		printf( "(%d)%s\n", onig_ereg( $pat, $str, $reg ), ( is_array( $reg )? bin2hex(iconv( $test_enc, $in_enc, implode( b' ', $reg ) )) : '' ) );
	} 
	function do_tests( $enc ) {
		test_ereg( $enc, b'abc ([a-z]+) ([a-z]+) ([a-z]+)$', b"abc def ghi jkl" );
		$pat = b'([£á-£ú]+) ([ ¤¢-¤«]+)([¤«-¤Ê]+) ([¤ï-¤ó]+)$'; 
		test_ereg( $enc, $pat, b'£á£â£ã ¤¢¤ª¤¤ ¤«¤³¤Ê ¤ï¤ñ¤ó' );
		test_ereg( $enc, $pat, b'£í£ú£ø£æ£ð ¤¦¤ª¤« ¤­¤« ¤ò¤ð' );
	}

	foreach( $encs as $enc ) {
		do_tests( $enc );
	}
?>

--EXPECT--
(15)6162632064656620676869206a6b6c2064656620676869206a6b6c
(27)a3e1a3e2a3e320a4a2a4aaa4a420a4aba4b3a4ca20a4efa4f1a4f320a3e1a3e2a3e320a4a2a4aaa4a420a4ab20a4b3a4ca20a4efa4f1a4f3
(27)a3eda3faa3f8a3e6a3f020a4a6a4aaa4ab20a4ada4ab20a4f2a4f020a3eda3faa3f8a3e6a3f020a4a6a4aaa4ab2020a4ada4ab20a4f2a4f0
(15)6162632064656620676869206a6b6c2064656620676869206a6b6c
(27)a3e1a3e2a3e320a4a2a4aaa4a420a4aba4b3a4ca20a4efa4f1a4f320a3e1a3e2a3e320a4a2a4aaa4a420a4ab20a4b3a4ca20a4efa4f1a4f3
(27)a3eda3faa3f8a3e6a3f020a4a6a4aaa4ab20a4ada4ab20a4f2a4f020a3eda3faa3f8a3e6a3f020a4a6a4aaa4ab2020a4ada4ab20a4f2a4f0
(15)6162632064656620676869206a6b6c2064656620676869206a6b6c
(27)a3e1a3e2a3e320a4a2a4aaa4a420a4aba4b3a4ca20a4efa4f1a4f320a3e1a3e2a3e320a4a2a4aaa4a420a4ab20a4b3a4ca20a4efa4f1a4f3
(27)a3eda3faa3f8a3e6a3f020a4a6a4aaa4ab20a4ada4ab20a4f2a4f020a3eda3faa3f8a3e6a3f020a4a6a4aaa4ab2020a4ada4ab20a4f2a4f0
(15)6162632064656620676869206a6b6c2064656620676869206a6b6c
(39)a3e1a3e2a3e320a4a2a4aaa4a420a4aba4b3a4ca20a4efa4f1a4f320a3e1a3e2a3e320a4a2a4aaa4a420a4ab20a4b3a4ca20a4efa4f1a4f3
(39)a3eda3faa3f8a3e6a3f020a4a6a4aaa4ab20a4ada4ab20a4f2a4f020a3eda3faa3f8a3e6a3f020a4a6a4aaa4ab2020a4ada4ab20a4f2a4f0
