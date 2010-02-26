//*****************************************************************************/
//
//                  
//
//                 Copyright (c)  2000 - 2010 by Dusan B. Jovanovic (dbj@dbj.org) 
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
//
// common reusable algorithms
//

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


//--------------------------------------------------------------------------------
namespace dbjsys {
    namespace fm {
        namespace algo {
//--------------------------------------------------------------------------------

// 
			class NameSpace_algo{} ;
				typedef Error<NameSpace_algo> Err ;
//--------------------------------------------------------------------------------

#if ! defined( npos )
static const size_t npos = -1;
#endif

//--------------------------------------------------------------------------------
/*
#define issign(c) ((c) == '+' ? 1 : ( (c) == '-' ? 1 : 0) ) 
*/
inline bool issign ( wchar_t c )
{
	if ( c == L'+' ) 
		return true ;
	else if ( c == L'-' )
		return true ;
	else
		return false ;
}
//--------------------------------------------------------------------------------
// case insensitive string compare
//
// this would work only for ANSI code
//
template< typename T = std::string >
class NocaseStrEqual : public std::equal_to<T> {
public:
	//strcmp
    bool operator()(const std::string & x, const std::string & y ) const
    {
        LPCTSTR lpString1 = x.c_str() ;
        LPCTSTR lpString2 = y.c_str() ;

       int result = CompareString(
                  LOCALE_SYSTEM_DEFAULT,// locale identifier
                  NORM_IGNORECASE,      // comparison-style options
                  lpString1,            // first string
                  x.size(),             // size of first string
                  lpString2,            // second string
                  y.size()              // size of second string
        );

       return result == CSTR_EQUAL ;
    }
    bool operator()(const T & x, const T & y ) const
    {
        const wchar_t * lpString1 = static_cast<const wchar_t *>(x);
        const wchar_t * lpString2 = static_cast<const wchar_t *>(y) ;

       size_t result = CompareStringW(
                  LOCALE_SYSTEM_DEFAULT,// locale identifier
                  NORM_IGNORECASE,      // comparison-style options
                  lpString1,            // first string
                  (long)wcslen(lpString1),             // size of first string
                  lpString2,            // second string
                  (long)wcslen(lpString2)              // size of second string
        );

       if( 0 == result )
           throw DBJSYSError(dbjsys::fm::doctor::errstring() ) ;
       return result == CSTR_EQUAL ;
    }

} ;

//----------------------------------------------------------------------------
template< typename T >
inline bool compareNoCase( const T & s1, const T & s2
                          /* , std::equal_to<std::string> & comparator =  NocaseStrEqual() */ 
                          ) 
{
    return NocaseStrEqual<T>()(s1,s2) ;
}
//----------------------------------------------------------------------------
//
//
inline void stringtrim( _bstr_t & _bstr_arg )
{
	static const wchar_t SPACE_CHAR = L' ' ;

	if( _bstr_arg.length() < 1 ) return ;

	std::wstring s_(_bstr_arg) ;

	std::wstring	result ;
	size_t		nonSpacePos = s_.find_first_not_of( SPACE_CHAR,0 ) ;

	if ( nonSpacePos != npos ) // found leading spaces
	{
		result = s_.substr( nonSpacePos ) ;
		s_ = result ;  // change the string sent
	}

	nonSpacePos = s_.find_last_not_of( SPACE_CHAR ) ;

	if ( nonSpacePos != npos ) // found trailing spaces
	{
		result = s_.substr( 0, nonSpacePos+1 ) ;
		s_ = result ;  // change the string sent
	}
	//
	_bstr_arg = s_.c_str() ;
}
//--------------------------------------------------------------
inline int safeAtoi( const wchar_t * cstr_ , int & result_ )
{
	static const int BAD = 0 , GOOD = ! BAD ;

	result_ = 0 ;

	if ((! cstr_) && (cstr_[0] == L'\x00')) return BAD ;

	wchar_t * intBegin_ = const_cast<wchar_t *>(&(cstr_[0])) ;

	while (! iswdigit(intBegin_[0]) ) // find the first digit
	{
		intBegin_++ ;
		if (intBegin_[0] == L'\x00') return BAD ;
	}
   
	const int negative = (intBegin_-1)[0] == L'-' ;
	const int zero     = (intBegin_  )[0] == L'0' ; 

	if ( negative )
			result_ = _wtoi(intBegin_-1) ;
	else
		    result_ = _wtoi(intBegin_) ;

	if ((! zero) && (! result_))
			return BAD ;
	
	return GOOD ;
}
//----------------------------------------------------------------------------
//
//
//

inline std::wstring this_module_full_path( HMODULE this_module_handle = NULL )
{
	wchar_t dllPath_[_MAX_PATH] = L"";
	// this must match the name of the DLL!!!
	wchar_t baseName[128] = L"" ;	
	HMODULE hModule = NULL ;

	if ( NULL == this_module_handle )
	{
		// first get the base name of this module
		DWORD ret1 = GetModuleFileNameW(
			NULL,			// handle to module
			baseName,			// file name of module
			128			// size of buffer
		);

		_ASSERT( ret1 ) ;
		// now get the module handle 
		hModule = GetModuleHandleW(
			baseName   // module name
		);
	}
	else {
		hModule = this_module_handle ;
	}

	_ASSERT( hModule ) ;
	DWORD ret2 = GetModuleFileNameW(
		hModule,			// handle to module
		dllPath_,			// file name of module
		_MAX_PATH			// size of buffer
	);

	_ASSERT( ret2 ) ;
#ifdef _DEBUG
    std::wstring dll_ = dllPath_;
	return dll_ ;
#else
	return dllPath_ ;
#endif
}
//----------------------------------------------------------------------------
inline _bstr_t basenameNoExtension( const wchar_t * fullPath_ )
{
	wchar_t result[_MAX_FNAME] ; // put enough space in
	result[0] = L'\x00' ;
	if ( fullPath_ == 0 ) return result ;
	if ( fullPath_[0] == result[0] ) return result ;

	// second copy the argument to the 'result'
    const wchar_t * rtn = wcsncpy( result, fullPath_, _MAX_FNAME ) ;
	_ASSERT( rtn ) ;
	// point the 'walker' to the end of the argument given
	wchar_t * walker = wcschr( result, dbjsys::glob::EOS()) ; 
	// do until found first slash or backslash going from right
	// or not reached the left end
	do
	{
		// while walking to left change dot to EOS() 
		if ( walker[0] == * dbjsys::glob::dot() )
			 walker[0] = dbjsys::glob::EOS() ;
		else
		if ( walker[0] == * dbjsys::glob::backSlash() ) 
				break ;
		else
		if ( walker[0] == * dbjsys::glob::forwardSlash() )
				break ;
			
		--walker ;
	}
	while ( walker > static_cast<wchar_t*>(result) ) ;

	// walker points to result
	// so this is not necessary -- wcsncpy( result, ++walker, _MAX_FNAME ) ;
	walker ++ ;
	return walker ;

}
//----------------------------------------------------------------------------
//
//
inline const wchar_t * ltocstr ( const long l )
{
	static wchar_t digitstring[] = 
	{ L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9' } ;

	static wchar_t buf[34] = L"" ;
	
	if (( l > -10 ) && ( l < 10 ) )
	{
	static wchar_t quickResult[3] = L"" ;
		if ( l < 0 )
		{
			quickResult[0] = L'-' ;
			quickResult[1] = digitstring[abs(l)] ;	
		}
		else
		{
			quickResult[0] = digitstring[abs(l)] ;	
		}
	    return quickResult ;
	}

	memset( buf,0, (34 * sizeof(wchar_t) ) ) ;
	_ltow( l , buf , 10 ) ;
	return buf ;
}
//----------------------------------------------------------------------------
//+
//-----------------------------------------------------------------------------
//  FUNCTION    :       errBox()
//  DESCRIPTION :       Allows errors to be output
//  ARGUMENTS   :       Variable
//  RETURNS     :       void
//  NOTE        :       This function depends on WIN32 API
//-----------------------------------------------------------------------------
//-
//FNC//
inline const void errBox ( const wchar_t * format_, ... )
{
	static const unsigned MSG_BUF_SIZ = BUFSIZ * 4 ;

#if defined( _DEBUG )
    static const wchar_t title[] = L"DBJ Error Message, press Cancel to debug" ;
    UINT mbox_style = MB_OKCANCEL | MB_ICONERROR | MB_SYSTEMMODAL ;
#else
    static const wchar_t title[] = L"DBJ Error Message" ;
    UINT mbox_style = MB_OK | MB_ICONERROR | MB_SYSTEMMODAL ;
#endif
    HWND        winHand = NULL ;
    wchar_t       buf_[MSG_BUF_SIZ] = L"" ; // reserve ...

    //use arguments to construct error string
	if ( format_ != 0 )
    {
        va_list argptr_ ;
        va_start( argptr_, format_ ) ;
        dbjVERIFY ( vswprintf ( buf_, format_, argptr_ ) > 0 ) ;
        va_end( argptr_ ) ;
    }
        	buf_[wcslen(buf_)] = L'\x00' ;
    try {
    // may be inside WIN32 but we can try neverthelesss
    // to the log file, because someone could redirect stderr
    // to FILE * using reopen()
    if ( stderr ) fwprintf( stderr, L"%s", buf_ ) ; 
    } catch ( ... ) { 
    }

	if ( winHand == NULL )
        winHand = ::GetDesktopWindow() ; 	
    //---------------------------------------------
    ::Sleep(0) ; // always helps
    //
    if (
        IDCANCEL ==	::MessageBoxW( winHand, 
			    buf_, title , mbox_style
			    )
     )
    {
#if defined( _DEBUG )
	    DebugBreak() ; // break here and fire up the debugger
#endif
    }
}// end of errBox
//------------------------------------------------------------------------
inline void display_ ( const std::exception & e )
{
  	algo::errBox( L"%s", e.what() ) ;
}
//------------------------------------------------------------------------
// this ostream manipulator prints a current time stamp in a 'nice' format
/*
inline std::ostream & current_time ( std::ostream & os )
{
struct tm	*newtime = 0 ;
time_t		 aclock;
   time( &aclock );                 // Get time in seconds 
   newtime = localtime( &aclock );  // Convert time to struct tm form 
   return os << asctime( newtime ) ;// Print local time as a string 
}
*/
// UNICODE version
inline std::wostream & current_time ( std::wostream & os )
{
struct tm	*newtime = 0 ;
time_t		 aclock;
   time( &aclock );                 /* Get time in seconds */
   newtime = localtime( &aclock );  /* Convert time to struct tm form */
   return os << _wasctime( newtime ) ;/* Print local time as a w-string */
}

//----------------------------------------------------------------------------
// 010198	DBJ		Changed '.log' to be '.htm'
//
inline const wchar_t * makeLogFileName ( const wchar_t * fname ) 
{
	static const    wchar_t    LOGEXTENSION[]       = L".log" ;
	static          wchar_t    result_[_MAX_PATH]   = L"" ; 
	static          wchar_t    bname_ [_MAX_PATH]   = L"" ; 

	if ( ( fname == 0 ) || ( fname[0] == L'\x0' ) )
	{

        const wchar_t * rtn = wcsncpy( bname_, dbjsys::glob::DFLT_LOG_FILE(), _MAX_PATH ) ;
        _ASSERT( rtn ) ;
	}
	else {

		const wchar_t * rtn = wcsncpy( bname_, algo::basenameNoExtension(fname), _MAX_PATH );
        _ASSERT( rtn ) ;
	}

	if ( 0 == 
		swprintf( result_ , L"%s%s%s%s", dbjsys::glob::LOG_DIR(),
			L"\\", bname_, LOGEXTENSION )
		)
	{
		// thus MUST not happen, but ...
        const wchar_t * rtn = wcsncpy( result_, dbjsys::glob::DFLT_LOG_FILE(), _MAX_PATH ) ;
		_ASSERT( rtn ) ;
	}

	return result_ ;
}

//----------------------------------------------------------------------------
#if defined ( string2args2string )
//----------------------------------------------------------------------------
//
//
inline void string2args( int & argc, wchar_t ** & argv, const std::wstring & str_ ) 
{
wchar_t *	strVal	= _wcsdup(str_.c_str()) ;
wchar_t	seps[]  = { L' ', L'\x00' } ;
wchar_t *	token	= 0 ;

std::vector<std::wstring> sstore_ ;
   /* Establish std::string and get the first token: */
   token = wcstok( strVal , seps );
   argc  = 0 ;
   while( token != NULL )
   {
      /* While there are tokens in "std::string" */
      sstore_.push_back(std::wstring(token)) ;
      /* Get next token: */
      token = wcstok( NULL, seps );
   }

   free( strVal ) ; // free it immediately

   argc = sstore_.size() ;

   argv = (wchar_t **)malloc(argc * sizeof(wchar_t *));

   for ( register int j = 0 ; j < argc ; j++ )
   {
		*(argv+j) = _wcsdup( sstore_[j].c_str() ) ;
   }

}

//----------------------------------------------------------------------------
//
//
inline void releaseargs( int argc, wchar_t ** & argv ) 
{
try {
   for ( register int j = 0 ; j < argc ; j++ )
   {
		free (argv[j]) ;
		argv[j] = NULL ;
   }

   free(argv); argv= NULL ;
}
catch (...)
{
	// ignore, probably free() failed ...
}
}
//----------------------------------------------------------------------------
#endif // string2args2string 
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
#ifdef DBJ_VERSIONNING
// get the version info of one specific field from the resource
//-----------------------------------------------------------------------------
//
inline static HRSRC getVersionInfoField ( LPCTSTR lpName, HMODULE hModule )
{
	LPCTSTR lpType		=	_T("RT_VERSION") ;
	WORD	wLanguage	=	MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL) ;

HRSRC result = FindResourceEx(
								 hModule,	// resource-module handle 
								 lpType,	// pointer to resource type 
								 lpName,	// pointer to resource name  
								 wLanguage 	// resource language 
							  );
if ( ! result )
		doctor::ErrMessage( ::GetLastError() ) ; 

return result ;

}

//-----------------------------------------------------------------------------
// Get
inline void getVersionInfo ( const wchar_t * lpModuleName /* = _T("dbjfm.lib") */ )
{
	HGLOBAL resH		=	NULL ;
	HMODULE hModule		=	NULL ;
	HRSRC hResInfo		=	NULL ;
	DWORD resSize		=   NULL ;

	// address of module name to return handle for  
#ifdef _DEBUG
		hModule = GetModuleHandle( lpModuleName ) ;
#else
		hModule = GetModuleHandle( lpModuleName ) ;
#endif
	if ( hModule == NULL )
		 hModule = doctor::moduleHandle( ); 	


    const _bstr_t lptstrFilename	= (doctor::FileName( doctor::moduleHandle() ) );
    LPDWORD lpdwHandle 		=	NULL ;// pointer to variable to receive zero
    DWORD	dwLen			=	0 ;
    LPVOID	lpData 			=	NULL ;

if ( 0 == ( dwLen = GetFileVersionInfoSize(
    lptstrFilename,	// pointer to filename string
    lpdwHandle 	) // pointer to variable to receive zero
   ) )
   doctor::ErrMessage( ::GetLastError() ) ; 

if ( FALSE == GetFileVersionInfo(
    lptstrFilename,	// pointer to filename string
    *lpdwHandle,	// ignored 
    dwLen+1,	// size of buffer
    lpData 	// pointer to buffer to receive file-version info.
   ))
   doctor::ErrMessage( ::GetLastError() ) ; 

UINT resultSize = BUFSIZ ;
wchar_t buffer_[BUFSIZ] = L"" ;
void * result = buffer_ ; 

if ( 0 == VerQueryValue(
    (const LPVOID) lpData,	// address of buffer for version resource
    TEXT("\\StringFileInfo\\040904E4\\FileDescription"),	// address of value to retrieve
    &result,	// address of buffer for version pointer
    &resultSize 	// address of version-value length buffer
   ) )
   doctor::ErrMessage( ::GetLastError() ) ; 
else
algo::errBox( "FileDescription = %s", result ) ;

/*
VerQueryValue(pBlock,  
              TEXT("\\StringFileInfo\\040904E4\\FileDescription"), 
              &lpBuffer, 
              &dwBytes); 
 */

}

#endif // DBJ_VERSIONNING


inline _bstr_t d2string_ ( const double dn_ )
{
	_variant_t var = dn_ ;
	return var ;
}

////////////////////////////////////////////////////////////////////////////////////////
} ; // eof namespace algo
////////////////////////////////////////////////////////////////////////////////////////

template< typename T >
inline const bool EMPTY_CSTR( const T * const X ) { return ((X==NULL) && (X[0] == 0)) ; }

//--------------------------------------------------------------------------------
    } //    namespace fm 
} // namespace dbjsys 
//--------------------------------------------------------------------------------

