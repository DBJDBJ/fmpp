#include "fm.h"
#include <io.h>
#include <errno.h>
using namespace dbjsys::fm ;
//-----------------------------------------------------------------------------
__forceinline long wstr2long ( wchar_t * wstr_ ) {
	long result = 0L ;
		if ( ! wstr_ ) return result ;
	try {	result = (long)variant_t( wstr_ ) ;
	} catch ( const _com_error & x ) { 	result = 0L ; /* conversion error */ }
		return result ;
}
//-----------------------------------------------------------------------------
// If this envvar
static const wchar_t ENVVAR_NAME_DBJLOGSIZE [] =  L"DBJLOGSIZE" ;
//is not currently set then this  value 
static const volatile long DBJLOGSIZE_MAX =  64 * 1024 ;
// will be used as maximum allowed size for the log file in the method bellow.
static bool cut_down_to_size ( int logfile_handle_ )
{
	bool was_downsized = false ;
	long allowed_size_ = wstr2long( _wgetenv(ENVVAR_NAME_DBJLOGSIZE) ) ;

	if ( ! allowed_size_ ) // envvar not  set
		allowed_size_ = DBJLOGSIZE_MAX ;

   if ( _filelength(logfile_handle_) > allowed_size_ ) 
   {
      if ( -1 == _chsize( logfile_handle_, 0 ) )
		  switch ( errno )
		  {
		  case EACCES :
			  _ASSERT(! L"the specified file is locked against access") ;
		  case EBADF:
			  _ASSERT(! L"the specified file is read-only or the handle is invalid") ;
		  case ENOSPC:
			  _ASSERT(! L"no space is left on the device") ;
		  default :
			  _ASSERT(! L"unknown  error") ;
		  }
	  was_downsized = true  ;
   }
/*
_chsize returns the value 0 if the file size is successfully changed. 
A return value of -1 indicates an error: errno is set to EACCES if the specified 
file is locked against access, to EBADF if the specified file is read-only or 
the handle is invalid, or to ENOSPC if no space is left on the device.
*/
    return was_downsized ;
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
extern "C" void switchErrLog ( const wchar_t * name , const int appendLog )
{
	if ( ( name == 0 ) || ( name[0] == L'\x0' ) )
		name = dbjsys::glob::DFLT_LOG_FILE() ;

	wchar_t fullpath[BUFSIZ] = L"" ; 

	if ( 0 == wcsncpy( fullpath, _bstr_t(algo::makeLogFileName(name)), BUFSIZ ) )
	// this MUST not fail, but ...
	wcsncpy( fullpath, _bstr_t(algo::makeLogFileName(dbjsys::glob::DFLT_LOG_FILE())), BUFSIZ ) ;

    FILE * errLog = 0 ;

    if( 0 == appendLog )
    {
	    errLog = _wfreopen( fullpath, L"w+", stderr ) ;
    }
    else
    {
	    errLog = _wfreopen( fullpath, L"a+", stderr ) ;
    }

	if ( (FILE*)0 == errLog )
    {
		_wperror( L"Warning from MetaSocket MECHANISMS CORE : freopen() failed :") ;
	}

	set_prompt_stream( std::wclog ) ;
	
    if(
		( 0 == appendLog ) || 
		(cut_down_to_size( _fileno( stderr ) ) )
	)
	{
		prompt(L"=", 79 );
		prompt(L"MetaSocket VERSION:") << DBJSYS_FM_VER_MAJOR() << L"." << DBJSYS_FM_VER_MINOR() ;
		prompt(L"=", 79 );
		prompt(L"LOG FILE ",L":",0 ) << fullpath ;
	    prompt(L"=", 79 );
	    prompt(L"Foundation Mechanisms BUILD: ") << static_cast<const wchar_t *>(_bstr_t(__TIMESTAMP__)) ;
	}
	    prompt(L"-", 79 );

		Timestamp  ;

	prompt(	L"LOG START",0,0) << L", Module : " << algo::this_module_full_path() 
        <<	L", " << algo::current_time  << std::endl ;

	reset_prompt_stream () ;

}
