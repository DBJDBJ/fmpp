-----------------
   #FM++ 
   ##Quick Usage Overview
-----------------

This is exclusively Windows C++ API product, built with Microsoft Visual C++ compiler.

1.To use this lib add the path to this folder and include "fm.h"
2.There is no lib or dll. Everything is in headers.
3.Only UNICODE apps may be built with DBJ*FM++
4.TCHAR.H, is NOT included. FM++ is using _bstr_t and wchar_t everywhere.
5.Almost all non-trivial mechanisms have testing (fancy term "units"), at the bottom of their headers. The code is not part of the compilation unless #defines to compile them in,
are defined, in application using the FM++.

-------------------
Author &amp; Architect: DBJDBJ   
Licensing: MIT &amp; GPL
-------------------
####&copy; 2015 by [![dbj();](http://dbj.dbjdbj.org/wp-content/uploads/2014/06/dbj100x100.png)](http://www.dbj.org "dbj") 
