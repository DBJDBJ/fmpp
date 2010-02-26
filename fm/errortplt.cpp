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
#include "fm.h"
#if defined(_CPPRTTI)
#include <typeinfo.h>
#endif
// ----------------------------------------------------------------------
namespace dbjsys {
	namespace fm {
//-----------------------------------------------------------------------
//
//-----------------------------------------------------------------------
//
DBJSYSError::~DBJSYSError()
{
	if ( false == reported_ )
	{
		::Beep( 100, 300 ) ;
		prompt( errmsg_ ) ;
	}
}
//-----------------------------------------------------------------------
// 
_bstr_t  DBJSYSError::makeErrMsg
( const _bstr_t & typeName, const _bstr_t & msg, const _bstr_t & file, const long line)
{
  	static const wchar_t prompt1[] = L"\nFrom : " ;
    static const wchar_t prompt2[] = L"\nWhat : " ;
    static const wchar_t prompt3[] = L"\nFile : " ;
	static const wchar_t prompt4[] = L"\nLine : " ;
	static const size_t  promptlen_     = wcslen(prompt1) ;

    static _bstr_t NEM(L"Not enough memory "  ) ;

    _bstr_t retStr = NEM + _bstr_t(__FILE__) ;
    try 
    {
        // allocate a large enough buffer
        size_t buffsize = promptlen_*4 + typeName.length() + msg.length() + 
            file.length() ;
        //wchar_t * buff = new wchar_t[buffsize+1] ;
		wchar_t * buff = (wchar_t *)_alloca( (buffsize+1) * sizeof(wchar_t) ) ;
		if( 0 == buff ) return retStr ;
        wchar_t * start_of_buff = buff ;

        const wchar_t * start = prompt1 ;
        const wchar_t * end   = start+promptlen_;
        buff = std::copy( start, end, buff ) ;
        start = static_cast<const wchar_t *>(typeName) ;
        end   = start + typeName.length() ;
        buff = std::copy( start, end, buff ) ;
        _ASSERT( buff < start_of_buff+buffsize ) ;

        start = prompt2 ;
        end   = start+promptlen_;
        buff = std::copy( start, end, buff ) ;
        start = static_cast<const wchar_t *>(msg) ;
        end   = start + msg.length() ;
        buff = std::copy( start, end, buff ) ;
        _ASSERT( buff < start_of_buff+buffsize ) ;

        start = prompt3 ;
        end   = start+promptlen_;
        buff = std::copy( start, end, buff ) ;
        start = static_cast<const wchar_t *>(file) ;
        end   = start + file.length() ;
        buff = std::copy( start, end, buff ) ;
        _ASSERT( buff < start_of_buff+buffsize ) ;

        start = prompt4 ;
        end   = start+promptlen_;
        buff = std::copy( start, end, buff ) ;
        _ASSERT( buff == start_of_buff+buffsize ) ;
		*buff = 0 ; // terminator

		_bstr_t line_bstr = L"0" ;
		if( line ) 
			line_bstr = _bstr_t(_variant_t(line)) ;

        retStr = start_of_buff + line_bstr;
        //delete[] start_of_buff ; using _alloca

    }
    catch ( ... )
    {
	    retStr = NEM ;
    }
	
    return retStr ;
}
//---------------------------------------------------------------------------------------
static _bstr_t errstring_ ( DWORD lastErrorCode  ) ;
// make win32 error message
_bstr_t  DBJSYSError::makeErrMsg( const _bstr_t & typeName, DWORD err_code , const _bstr_t & file, const long line ) 
{
	return DBJSYSError::makeErrMsg( 
								typeName,
								errstring_( err_code ),
								file,
								line ) ;
}
//--------------------------------------------------------------------------------
static _bstr_t errstring_ ( DWORD lastErrorCode  )
{
	_bstr_t result( L"Not an Win32 error" ) ;

	if ( lastErrorCode != 0 )
    {
	    LPWSTR lpMsgBuf = NULL ;
 
	    if ( ! FormatMessage( 
		    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		    NULL,
		    lastErrorCode, // result of GetLastError()
		    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		    (LPWSTR) &lpMsgBuf,	0,	NULL )
		    )
        {
			_ASSERT(0) ; // no mercy >:)
        }	

        result = lpMsgBuf ;
	    ::LocalFree( lpMsgBuf );
	}

	return result ;
}

// ----------------------------------------------------------------------
}	//	namespace fm 
} //namespace dbjsys 
//---------------------------------------------------------------------------------------
