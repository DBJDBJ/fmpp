==========================================================================================
   FM++ :: Quick Usage Overview
==========================================================================================

This is exclusively Windows product, built with Microsoft Visual C++ compiler.

1. 
to use this lib add the path to this folder and include "fm.h"
2.
lib names are used by fm.h to link-in proper lib, like this :

#if defined _DEBUG
#pragma comment( lib, "dbj_fmdbg.lib"  ) 
#else
#pragma comment( lib, "dbj_fm.lib"  ) 
#endif  // _DEBUG

If these are not found C++ precomp will stay quiet, so be sure you link them in.

3.
Only UNICODE apps may be built with DBJ*FM++
4.
TCHAR.H, is NOT included. FM++ is using _bstr_t and wchar_t everywhere.
5.
Almost all non-trivial mechanisms have testing code, at the bottom of their headers.
The code is not part of the compilation unless #defines to compile them in,
are defined, in application using the FM++.

==========================================================================================
