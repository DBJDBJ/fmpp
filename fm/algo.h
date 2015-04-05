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
//
// common reusable algorithms
//

#pragma once

#include <io.h>
#include <errno.h>
#include "fm.h"
#include "glob.h"
#include "timestamp.h"
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

actively remove the cosnequences vs using this macros

#define _CRT_SECURE_NO_WARNINGS 1
#define _CRT_NON_CONFORMING_SWPRINTFS 1
#define _SCL_SECURE_NO_WARNINGS 1

*/


//--------------------------------------------------------------------------------
namespace dbjsys {
    namespace fm {
        namespace algo {
//--------------------------------------------------------------------------------
// Here we define Error<> handler that must be called Err
// 
			class NameSpace_algo{} ;
			dbjMAKE_ERR(NameSpace_algo);
//--------------------------------------------------------------------------------

#if ! defined( npos )
static const size_t npos = (size_t)-1;
#endif

//--------------------------------------------------------------------------------
/*
One macro we do not want as a macro

#define issign(c) ((c) == '+' ? 1 : ( (c) == '-' ? 1 : 0) ) 

*/
__forceinline bool issign ( const wchar_t & c )
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
/*
TODO
this introduces dependancy on std
reimplement using _bstr_t only
*/
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

       //if( 0 == result )
       //  throw DBJSYSError(dbjsys::fm::doctor::errstring() ) ;
	  
	   dbjVERIFY(result);

       return result == CSTR_EQUAL ;
    }

} ;

//----------------------------------------------------------------------------
template< typename T >
__forceinline 
bool compareNoCase( const T & s1, const T & s2
                          /* , std::equal_to<std::string> & comparator =  NocaseStrEqual() */ 
                          ) 
{
    return NocaseStrEqual<T>()(s1,s2) ;
}
//----------------------------------------------------------------------------
//
//
__forceinline 
void stringtrim( _bstr_t & _bstr_arg )
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
/*
TODO 
this is clever but ot necessary
use _variant_t instead
*/
__forceinline
 int safeAtoi( const wchar_t * cstr_ , int & result_ )
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
// TODO This definitely has to be replaced with cmdline class argv[0]
//
__forceinline
 std::wstring this_module_full_path( HMODULE this_module_handle = NULL )
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
__forceinline
 _bstr_t basenameNoExtension( const wchar_t * fullPath_ )
{
	_bstr_t result = fullPath_ ; 
	result = L"" ;
	if ( fullPath_ == 0 ) return result ;

	// point the 'walker' to the end of the argument given
	wchar_t * walker = wcschr( (wchar_t *)result, dbjsys::glob::EOS()) ; 
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
// clever but should be replaced with  _bstr_t(_variant_t(l))
//
__forceinline
const _bstr_t ltocstr ( const long l )
{
	return _bstr_t(_variant_t(l));
#if CLEVER_BUT_USELESS
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
#endif
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
__forceinline const void errBox(const wchar_t * format_, ...)
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
		dbjVERIFY(vswprintf(buf_, MSG_BUF_SIZ, format_, argptr_) > 0);
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
// dependancy on std can be removed here
__forceinline void display_(const std::exception & e)
{
  	algo::errBox( L"%s", e.what() ) ;
}
//------------------------------------------------------------------------
// this ostream manipulator prints a current time stamp in a 'nice' format
/*
__forceinline std::ostream & current_time ( std::ostream & os )
{
struct tm	*newtime = 0 ;
time_t		 aclock;
   time( &aclock );                 // Get time in seconds 
   newtime = localtime( &aclock );  // Convert time to struct tm form 
   return os << asctime( newtime ) ;// Print local time as a string 
}
*/
// UNICODE version
__forceinline std::wostream & current_time(std::wostream & os)
{
struct tm	*newtime = 0 ;
time_t		 aclock;
wchar_t      buffer[BUFSIZ];
   time( &aclock );                 /* Get time in seconds */
   _ASSERTE( localtime_s( newtime, &aclock ));  /* Convert time to struct tm form */
   _ASSERTE(_wasctime_s(buffer,BUFSIZ, newtime));
   return os << buffer;/* Print local time as a w-string */
}

//----------------------------------------------------------------------------
// 010198	DBJ		Changed '.log' to be '.htm'
//
__forceinline const wchar_t * makeLogFileName(const wchar_t * fname)
{
	static const    wchar_t    LOGEXTENSION[]       = L".log" ;
	static          _bstr_t    result_              = L"" ; 
	static          _bstr_t    bname_               = L"" ; 

	result_ = L"";	bname_ = L"";

	if ((fname == 0) || (fname[0] == L'\x0'))
	{
		bname_ = glob::DFLT_LOG_FILE();
	}
	else {
		bname_ = algo::basenameNoExtension(fname);
	}

	result_ = glob::LOG_DIR(); result_ += L"\\" + bname_ + LOGEXTENSION;

	return result_;
}

//----------------------------------------------------------------------------
#if defined ( string2args2string )
//----------------------------------------------------------------------------
//
//
__forceinline void string2args( int & argc, wchar_t ** & argv, const std::wstring & str_ ) 
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
__forceinline void releaseargs( int argc, wchar_t ** & argv ) 
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
__forceinline static HRSRC getVersionInfoField ( LPCTSTR lpName, HMODULE hModule )
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
__forceinline void getVersionInfo ( const wchar_t * lpModuleName /* = _T("dbjfm.lib") */ )
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

/* of course ... */
__forceinline _bstr_t d2string_(const double & dn_)
{
	_variant_t var = dn_ ;
	return var ;
}

////////////////////////////////////////////////////////////////////////////////////////
} ; // eof namespace algo
////////////////////////////////////////////////////////////////////////////////////////


template< typename T >
__forceinline const bool EMPTY_CSTR(const T * const X) { return ((X == NULL) && (X[0] == 0)); }



//-----------------------------------------------------------------------------
__forceinline long wstr2long(wchar_t * wstr_) {
	long result = 0L;
	if (!wstr_) return result;
	try {
		result = (long)variant_t(wstr_);
	}
	catch (const _com_error & ) { result = 0L; /* conversion error */ }
	return result;
}
/*
TODO move this to WIN part 
*/
const wchar_t * envvar(const wchar_t * env_var_name_)
{
	static size_t bufsiz = BUFSIZ;
	static wchar_t * buffer[BUFSIZ];
	*buffer = L"";
	_ASSERT(_wdupenv_s(buffer, &bufsiz, env_var_name_));
	return *buffer;
}
//-----------------------------------------------------------------------------
// TODO constants and literals definitely to be moved to a separate single structure
// which exist and is called dbjsys::glob!

static bool cut_down_to_size(int logfile_handle_)
{
	bool was_downsized = false;
	
	const _bstr_t log_size_env_var = envvar(glob::ENVVAR_NAME_DBJLOGSIZE);

	long allowed_size_ = wstr2long(log_size_env_var);

	if (!allowed_size_) // envvar not  set
		allowed_size_ = glob::DBJLOGSIZE_MAX;

	if (_filelength(logfile_handle_) > allowed_size_)
	{
		if (-1 == _chsize(logfile_handle_, 0))
			switch (errno)
		{
			case EACCES:
				_ASSERT(!L"the specified file is locked against access");
			case EBADF:
				_ASSERT(!L"the specified file is read-only or the handle is invalid");
			case ENOSPC:
				_ASSERT(!L"no space is left on the device");
			default:
				_ASSERT(!L"unknown  error");
		}
		was_downsized = true;
	}
	/*
	_chsize returns the value 0 if the file size is successfully changed.
	A return value of -1 indicates an error: errno is set to EACCES if the specified
	file is locked against access, to EBADF if the specified file is read-only or
	the handle is invalid, or to ENOSPC if no space is left on the device.
	*/
	return was_downsized;
}
//-----------------------------------------------------------------------------
//
// Use this to switch 'cerr','stderr','clog' and 'wclog' output
// to file of your choice
// called with no arg's uses the default log file name
// Uses propmpt() to mark new log beginning
//
// 190997   JovanovD
// 070198   JovanovD	    added HTML header 
// 161100   DBJ             std::clog and std::wclog are streaming into stderr
//
// 040415   DBJ				Definite legacy function ... serious reqork needed
//
extern "C" void switchErrLog(const _bstr_t & name, const int appendLog)
{
	_bstr_t  fullpath = algo::makeLogFileName(name.length() > 1 ? name : glob::DFLT_LOG_FILE() );

	FILE * errLog = 0;
	errno_t error_num = 0;

	if (0 == appendLog)
	{
		error_num = _wfreopen_s(&errLog, fullpath, L"w+", stderr);
	}
	else
	{
		error_num = _wfreopen_s(&errLog, fullpath, L"a+", stderr);
	}

	if ( error_num != 0 || (FILE*)0 == errLog )
	{
		_wperror((const wchar_t *)_bstr_t(__FILE__ " : freopen() failed, at line: ") + __LINE__);
	}

	set_prompt_stream(std::wclog);

	if (
		(0 == appendLog) ||
		(cut_down_to_size(_fileno(stderr)))
		)
	{
		prompt(L"=", 79);
		prompt(glob::FMPP_TITLE);
		prompt(L"=", 79);
		prompt(L"LOG FILE ", L":", 0) << fullpath;
		prompt(L"=", 79);
		prompt(L"Timestamp: ") << static_cast<const wchar_t *>(_bstr_t(__TIMESTAMP__));
	}
	prompt(L"-", 79);

   Timestamp;

	prompt(L"LOG START", 0, 0) << L", Module : " << algo::this_module_full_path()
		<< L", " << algo::current_time << std::endl;

	reset_prompt_stream();

}

//--------------------------------------------------------------------------------
    } //    namespace fm 
} // namespace dbjsys 
//--------------------------------------------------------------------------------

