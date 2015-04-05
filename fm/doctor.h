//*****************************************************************************/
//
//                  
//
//                 Copyright (c)  1997 - 2015 by Dusan B. Jovanovic (dbj@dbj.org) 
//                          All Rights Reserved
//
//        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Dusan B. Jovanovic (dbj@dbj.org)
//
//        The copyright notice above does not evidence any
//        actual or intended publication of such source code.
//
//  $Author: DBJ  $
//  $Date: $
//  $Revision: $
//*****************************************************************************/
#if ! defined( DBJ_DOCTOR_H )
#define DBJ_DOCTOR_H
#pragma once

/*
Warning	413	warning C4996: '_ftime64': This function or variable may be unsafe. 
Consider using _ftime64_s instead. To disable deprecation, use 
_CRT_SECURE_NO_WARNINGS. See online help for details.	

Warning	412	warning C4996: 'swprintf': swprintf has been changed to conform with the ISO C standard, 
adding an extra character count parameter. To use traditional Microsoft swprintf, 
set _CRT_NON_CONFORMING_SWPRINTFS.	

Warning	2	warning C4996: 'std::copy': Function call with parameters that may be unsafe - 
this call relies on the caller to check that the passed values are correct. 
To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. 
See documentation on how to use Visual C++ 'Checked Iterators'

*/
#define _CRT_SECURE_NO_WARNINGS 1
#define _CRT_NON_CONFORMING_SWPRINTFS 1
#define _SCL_SECURE_NO_WARNINGS 1

/*
USAGE :
static void ssDoctorTest () 
{
	// should give OK message
	 doctor::ErrMessage( GetLastError() ) ; 
	// should give Error message
	 doctor::ErrMessage( 1 ) ; 
}
//---------------------------------------------------------------
int main ( int argc, char ** argv )
{
	SetLastError(0) ; // Kraig Brokschmidt says so ?

	doctor::setHmodule(doctor::thisModuleHandle()) ;
	
	ssDoctorTest () ;

	return 0;
}
*/
//--------------------------------------------------------------------------------
namespace dbjsys {
    namespace fm {
//--------------------------------------------------------------------------------
// 
struct doctor {


	// 
    static const wchar_t * NEMMSG() { return L"SYSTEM ERROR : NOT ENOUGH MEMORY" ; }
	// 
    static const wchar_t * PROMPT() { return L"Diagnostic Message" ; }
	// 
    static const wchar_t * TITLE() { return L"DBJ FM Message" ; }
	// 
	static const wchar_t *	_swprintf ( wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *, unsigned int );
	// 
	static const wchar_t *	title () ;
	// 
	static void		MBox (	const wchar_t * , 
							const wchar_t * = doctor::PROMPT(), int = 0 ) ;
	// 
	static void		ErrMessage ( DWORD lastErrorCode ) ;
	// 
	static _bstr_t errstring () ;

	// 
	static const wchar_t * FileName ( HINSTANCE hModule ) ;
	// 
	static HINSTANCE doctor::thisModuleHandle( ) ;

	// 
	static HINSTANCE moduleHandle ( ) ;
} ;


inline HMODULE doctor::moduleHandle ( ) 
{
    static HMODULE moduleHandle_ = NULL ;

	if ( moduleHandle_ == NULL ) // try making it
			moduleHandle_ = doctor::thisModuleHandle() ;

	return moduleHandle_ ;
}

//--------------------------------------------------------------------------------
//
//
inline HMODULE doctor::thisModuleHandle( ) 
{
	return 
	GetModuleHandleW( doctor::FileName( NULL ) ) ;
}
//--------------------------------------------------------------------------------
// Can't call this from DllEntryPoint, because dll is not loaded at that time yet!
//
inline const wchar_t * doctor::FileName ( HMODULE hModule )
{
	static wchar_t lpFilename[BUFSIZ] ;
	DWORD result = 
	GetModuleFileNameW(
		(HMODULE)hModule,		// handle to module to find filename for 
		lpFilename,				// pointer to buffer for module path 
		(DWORD) BUFSIZ 			// size of buffer, in characters 
	);

	if ( result == 0 )
	{
		// endless loop -> myErrMessage( result ) ;
		return 0 ;
	}

	return lpFilename ;
}

//--------------------------------------------------------------------------------
//
//
inline const wchar_t * doctor::title ()
{
    HMODULE mHandle_ = moduleHandle() ;

	if ( mHandle_ ) 
        return doctor::FileName( moduleHandle() ) ;

	return doctor::TITLE() ;

}
//--------------------------------------------------------------------------------
//
//
inline const wchar_t * doctor::_swprintf(wchar_t * res, const wchar_t * fmt,
					 const wchar_t * a1, const wchar_t * a2, unsigned int bufsize_ = BUFSIZ)
{
	if ( ! ::swprintf(res, bufsize_, fmt, a1 , a2  )) 
		(void)wcscpy_s(res, bufsize_, doctor::NEMMSG());
		return res ;
}
//--------------------------------------------------------------------------------
//
// TODO to be removed and merged with another version of the same function in algo.h!
//
inline void doctor::MBox ( const wchar_t * m , const wchar_t * prompt, int breakOption )
{
	wchar_t buf[BUFSIZ] = L"" ;

	// (void)memset(buf,0,BUFSIZ);

	if ( ! prompt ) 
			prompt = L"~" ;
	
	if ( m && ( m[0] != L'\x00' ) )
		(void)doctor::_swprintf(buf,L"\n%s\n%s\n", prompt , m ) ;
	else
		(void)doctor::_swprintf(buf,L"\n%s\n%s\n", prompt , L"" ) ;

	HWND	hWnd		= (HWND)NULL ;					// handle of owner window
	const wchar_t * lpText		= buf ;							// address of text  in message box
	const wchar_t * lpCaption	= 
		breakOption ? L"Press CANCEL for J.I.T. Debugging" : doctor::title() ;
	// address of title of message box 
	UINT	uType		= MB_OK | MB_ICONINFORMATION ;	// style of message box

	if ( breakOption == 0 )
		(void)MessageBoxW( hWnd, lpText, lpCaption, uType );	
	else
		if ( IDOK != MessageBoxW( hWnd, lpText, lpCaption, MB_OKCANCEL ) )
				DebugBreak() ;
}

//--------------------------------------------------------------------------------
// TODO also to be removed and to use errortplt.h
inline _bstr_t doctor::errstring ()
{
	DWORD lastErrorCode = ::GetLastError() ;
	_bstr_t result( L"No Windows error" ) ;

	if ( lastErrorCode != 0 )
    {
	    LPTSTR lpMsgBuf = NULL ;
 
	    if ( ! FormatMessage( 
		    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		    NULL,
		    lastErrorCode, // result of GetLastError()
		    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		    (LPTSTR) &lpMsgBuf,	0,	NULL )
		    )
        {
            _bstr_t xMessage( L"Error in " ) ;
            xMessage += __FILE__ ;
            xMessage += ", FormatMessage() failed" ;

            throw std::exception( xMessage ) ;
        }	

        result = lpMsgBuf ;
	    //_tcsnccpy(result, lpMsgBuf, BUFSIZ * 4) ;
	    ::LocalFree( lpMsgBuf );
	    ::SetLastError(0); // clear it up
	}

	return result ;
}
//--------------------------------------------------------------------------------
// Print the system message for the error code obtained by 
// calling GetLastError()
//
inline void doctor::ErrMessage ( DWORD lastErrorCode )
{
wchar_t * lpMsgBuf = NULL ;
 
FormatMessageW( 
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,
    lastErrorCode, // result of GetLastError()
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    lpMsgBuf,
    0,
    NULL 
);
	// Display the string.
	if ( 0 == lastErrorCode )
		doctor::MBox( lpMsgBuf, L"MetaSocket Windows Message" ) ;
	else
		doctor::MBox( lpMsgBuf, L"MetaSocket Windows Error!", TRUE ) ;
	// Free the buffer.
	::LocalFree( lpMsgBuf );

	::SetLastError(0); // clear it up
}




//--------------------------------------------------------------------------------
// I am ashamed of the macro being seen in my code 
// 030497 JovanovD
// #define THISEXENAME doctor::FileName( NULL )
// Now I am proudc because I removed it!
// 05082000 JovanovD
inline const wchar_t * THISEXENAME() { return doctor::FileName( NULL ); }
//--------------------------------------------------------------------------------
    } //    namespace fm 
} // namespace dbjsys 
//--------------------------------------------------------------------------------
//
#endif
