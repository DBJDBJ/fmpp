//*****************************************************************************/
//
//                 Copyright (c)  2000-2006 by DBJSolutions LTD 
//                          All Rights Reserved
//
//        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF DBJSolutions LTD
//
//        The copyright notice above does not evidence any
//        actual or intended publication of such source code.
//
//  $Author: DBJ  $
//  $Date: $
//  $Revision: $
//*****************************************************************************/
#pragma once
//--------------------------------------------------------------------------------------
namespace dbjsys {
namespace fm {
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// oo wrap up of the command line. ASCII version; uses __argv and __argc.
// _UNICODE version uses __wargv and __argc.
// NOTE:    this works regardless of the VS project type: 
//          console, windows, even DLL app.
//
// Created		DBJ     03122000
// Improved		DBJ     26122005
//
//--------------------------------------------------------------------------------------
/*
    cl_argument encapsulates single comand line argument. Use as this:

    fun ( long, long ) ;

    cl_argument<long>     no_of_elements( 512 ) ;
    cl_argument<long>     str_len( 256 ) ;

    fun ( no_of_elements("-n"), str_len("-s") ) ;

    // if cl was '-n 1024 -s 512' fun() above will receive 1024 and 512
    // if cl was '-n1024 -s512'   fun() above will receive 1024 and 512
    // if cl was '-n1024'         fun() above will receive 1024 and 256
    // if cl was ''               fun() above will receive  512 and 256

*/
// 
template<typename T> class cl_argument
{
	// 
    T defval_ ; // default value
    T reqval_ ; // requested value
	// no default c-tor allowed
	cl_argument () {} 
    public:
	// copy constructor
		explicit
		cl_argument( const T & defval ) : defval_(defval), reqval_(defval)
    {
    }

	// return true if given symbol exists on the cmd line
    const bool exists ( const wchar_t  * const cl_symbol ) ;

    // resolve the actual value, and its type, by some cmd.line symbol 
    const T & operator () ( const wchar_t  * const cl_symbol ) ;

	// was the cmd line query made or/and was the last query OK ?
	const bool defval () { return defval_ == reqval_ ; } 
} ;
//--------------------------------------------------------------------------------------
#if defined ( __test_CmdLineArguments__ )
// usage example
inline long saberi( long a, long b) 
{
    return a + b ;
}
inline const void test_CmdLineArguments()
{
    cl_argument<long>     no_of_elements( 512 ) ;
    cl_argument<long>     str_len( 256 ) ;
	// kukulele is made to prove that we do not need 
	// wchar_t forced instantiation since it is very nicely
	// substituted by _bstr_t instance of cl_argument<T>
	// 
	// so instead of this
	// cl_argument<wchar_t *> kukulele(L"uh") ;
	// declare this
	cl_argument<bstr_t> kukulele(L"uh") ;


	wchar_t * clarg = kukulele(L"-name") ;
    long r = saberi( no_of_elements(L"-n"), str_len(L"-s") ) ;

    // if cl was '-n 1024 -s 512' fun() above will receive 1024 and 512
    // if cl was '-n1024 -s512'   fun() above will receive 1024 and 512
    // if cl was '-n1024'         fun() above will receive 1024 and 256
    // if cl was ''               fun() above will receive  512 and 256
}
#endif
//--------------------------------------------------------------------------------------
} // namespace fm 
} // namespace dbj 
//--------------------------------------------------------------------------------------
