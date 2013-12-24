Various SQLite3 Extensions
==========================

*Note:* These are not Unicode compatible. Support will be added later.


How to build
------------
Currently the makefiles are targeted to compile the extensions as win32 dll files in MinGW.

Simply clone the repo and run mingw_msys_make.bat. If you have installed MinGW to a different default directory you will need to edit the batch file.


Don't want to build them?
------------------------
I have included the precompiled win32 32bit binary DLL files in the repository. You can find a release of them on the releases page.


How to use the extensions
-------------------------
Open the sqlite3 shell application. Type in one or all of the following:
* .load <path to release>/sqlite3_base64_ext.dll
* .load <path to release>/sqlite3_hex_ext.dll
* .load <path to release>/sqlite3_pad_ext.dll


Extensions
----------

*	<b>sqlite3_base64_ext</b>
	Adds scalar functions:
	* MIXED base64_encode(value MIXED)
	* MIXED base64_decode(value MIXED)
    
	Examples:
		SELECT base64_encode("testing"); // Result: "dGVzdGluZw=="
		SELECT base64_decode("dGVzdGluZw=="); // Result: "testing"

*	<b>sqlite3_hex_ext</b>
	Adds scalar functions:
	* TEXT hex tohex(MIXED value, uppercase BOOLEAN DEFAULT FALSE)
	* INT64 value fromhex(MIXED hex)
    
	Examples:
		SELECT fromhex(tohex(1234)); // Result: 1234
		SELECT tohex(3735928559, 1); // Result: DEADBEEF

*	<b>sqlite3_pad_ext</b>
	Adds scalar functions:
	* TEXT lpad(str TEXT, INTEGER length, padstr TEXT)
	* TEXT rpad(str TEXT, INTEGER length, padstr TEXT)
    
	Examples:
		SELECT lpad("testing", 16, "12"); // Result: "121212121testing"
		SELECT rpad("testing", 16, "12"); // Result: "testing121212121"
