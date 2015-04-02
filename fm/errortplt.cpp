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
//#if defined(_CPPRTTI)
//#include <typeinfo.h>
//#endif
/*
#include "fm.h"
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
_bstr_t  & DBJSYSError::makeErrMsg
( const _bstr_t & typeName, const _bstr_t & msg, const _bstr_t & file, const long line)
{
  	static const wchar_t prompt1[] = L"\nException!From : " ;
    static const wchar_t prompt2[] = L"\nWhat : " ;
    static const wchar_t prompt3[] = L"\nFile : " ;
	static const wchar_t prompt4[] = L"\nLine : " ;

	static _bstr_t NEM = L"Not enough memory! " + _bstr_t(__FILE__) + L":" + _bstr_t(_variant_t(__LINE__));
	static _bstr_t text;
    try 
    {
		text  = L"";
		text  = prompt1 + typeName;
		text += prompt2 + msg;
		text += prompt3 + file;
		text += prompt4 + line;
    } 
	catch (...)
    {
	    return NEM ;
    }
	return text;
}
//---------------------------------------------------------------------------------------
static _bstr_t & errstring_ ( DWORD lastErrorCode  ) ;
// make win32 error message
_bstr_t  & DBJSYSError::makeErrMsg( const _bstr_t & typeName, DWORD err_code , const _bstr_t & file, const long line ) 
{
	return DBJSYSError::makeErrMsg( 
								typeName,
								errstring_( err_code ),
								file,
								line ) ;
}
//--------------------------------------------------------------------------------
static _bstr_t & errstring_ ( DWORD lastErrorCode  )
{
	static _bstr_t result ;
	LPWSTR lpMsgBuf = NULL;

	if ( lastErrorCode != 0 )
    {
 
		_ASSERT(FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			lastErrorCode, // result of GetLastError()
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPWSTR)&lpMsgBuf, 0, NULL)
			);
		::LocalFree(lpMsgBuf);
        result = lpMsgBuf ;
	}
	else {
		result = L"Not an Win32 error";
	}
		return result ;
}

// ----------------------------------------------------------------------
}	//	namespace fm 
} //namespace dbjsys 
//---------------------------------------------------------------------------------------
*/
