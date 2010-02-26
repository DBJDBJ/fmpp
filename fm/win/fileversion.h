//*****************************************************************************/
//
//                  
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
// This is based on the code found at http://www.codeguru.com
// FileVersion.h: interface for the file_version class. by Manuel Laflamme
//////////////////////////////////////////////////////////////////////

#ifndef __FILEVERSION_H_
#define __FILEVERSION_H_

#pragma once

//*****************************************************************************/
namespace dbjsys {
	namespace fm {
//*****************************************************************************/

// 
class file_version
{ 
public: 
	
	typedef Error<file_version> Err ;
	typedef Win32Error<file_version> WinErr ;

	// 
	file_version( const wchar_t * modulename = L"" ) 
		: m_lpVersionData( NULL ) 
		, m_dwLangCharset(0)
		, module_name_(modulename)
	{ 
	}

	// 
	~file_version() 
	{ 
		Close();
	} 

	// 
	void Close()
	{
		if ( NULL == m_lpVersionData ) delete[] m_lpVersionData; 
		m_lpVersionData = NULL;
		m_dwLangCharset = 0;
	}

// Operations	
	// 
    bool    Open(LPCWSTR lpszModuleName);

	// 
    bstr_t QueryValue(LPCWSTR lpszValueName, DWORD dwLangCharset = 0);
	// 
    bstr_t GetFileDescription()  {return QueryValue(L"FileDescription"); };
	// 
    bstr_t GetFileVersion()      {return QueryValue(L"FileVersion");     };
	// 
    bstr_t GetInternalName()     {return QueryValue(L"InternalName");    };
	// 
    bstr_t GetCompanyName()      {return QueryValue(L"CompanyName");     }; 
	// 
    bstr_t GetLegalCopyright()   {return QueryValue(L"LegalCopyright");  };
	// 
    bstr_t GetOriginalFilename() {return QueryValue(L"OriginalFilename");};
	// 
    bstr_t GetProductName()      {return QueryValue(L"ProductName");     };
	// 
    bstr_t GetProductVersion()   {return QueryValue(L"ProductVersion");  };

	// 
    bool    GetFixedInfo(VS_FIXEDFILEINFO& vsffi);
	// 
    bstr_t GetFixedFileVersion();
	// 
    bstr_t GetFixedProductVersion();

// Attributes
private :
	// 
    LPBYTE  m_lpVersionData; 
	// 
    DWORD   m_dwLangCharset; 
	// 
	bstr_t  module_name_   ;
}; 

//*****************************************************************************/
	} //	namespace fm 
} //namespace dbjsys 
//*****************************************************************************/

#endif  // __FILEVERSION_H_


