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

#pragma comment(lib, "version")

#ifdef _DEBUG
#undef THIS_FILE
const static bstr_t THIS_FILE(__FILE__);
#endif
//------------------------------------------------------------------
namespace dbjsys {
	namespace fm {
		//------------------------------------------------------------------
		// TODO: to be used also in doctor.h
		inline bstr_t & format(bstr_t & target, wchar_t * pszFormat, ...)
		{
			va_list arglist;
			va_start(arglist, pszFormat);
			wchar_t szBuff[1024];
			int rtn = wvsprintfW(szBuff, pszFormat, arglist);
			_ASSERTE(rtn < sizeof(szBuff) / sizeof(szBuff[0]));
			// OutputDebugStringW(szBuff);
			target = szBuff;
			va_end(arglist);
			return target;
		}

		//------------------------------------------------------------------
		// use this to have meaningfull error message if module from which
		// version info is required can not be located
		// 
		template< typename E >
		struct check_module_loadability
		{
			// 
			HMODULE mhandle;
			// 
			check_module_loadability(LPCWSTR file_name)
				: mhandle(0)
			{
				dbjTHROWIF(0 == (mhandle = LoadLibrary(file_name)), E);
			}
			// 
			~check_module_loadability()
			{
				dbjTHROWIF(0 == FreeLibrary(mhandle), E);
			}
		};

		//------------------------------------------------------------------
		bool file_version::Open(LPCWSTR lpszModuleName)
		{
			Close();

			_ASSERT(lpszModuleName != 0);
			_ASSERT(m_lpVersionData == NULL);

			// Get the version information size for allocate the buffer
			DWORD dwHandle = NULL;
			DWORD dwDataSize = NULL;

			check_module_loadability<WinErr> cml(lpszModuleName);

			if (0 == (dwDataSize = ::GetFileVersionInfoSize((LPWSTR)lpszModuleName, &dwHandle))
				)
				dbjTHROWERR(L"no versioning resource found in the executable module");

			// Allocate buffer and retrieve version information
			m_lpVersionData = new BYTE[dwDataSize];
			if (!::GetFileVersionInfo((LPWSTR)lpszModuleName, dwHandle, dwDataSize,
				(void**)m_lpVersionData))
			{
				Close();
				throw  WinErr(__FILE__, __LINE__);
			}

			// Retrieve the first language and character-set identifier
			UINT nQuerySize;
			DWORD* pTransTable;
			if (!::VerQueryValue(m_lpVersionData, L"\\VarFileInfo\\Translation",
				(void **)&pTransTable, &nQuerySize))
			{
				Close();
				throw  WinErr(__FILE__, __LINE__);
			}

			// Swap the words to have lang-charset in the correct format
			m_dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));

			return TRUE;
		}
		//------------------------------------------------------------------
		bstr_t file_version::QueryValue(LPCWSTR lpszValueName,
			DWORD dwLangCharset /* = 0*/)
		{
			// Must call Open() first
			if (m_lpVersionData == NULL)
			{
				if (module_name_.length() > 1)
					this->Open(module_name_);
				else
					dbjTHROWERR(L"resource has to be opened, but module name was not given");
			}

			if (m_lpVersionData == NULL)
				return L"";

			// If no lang-charset specified use default
			if (dwLangCharset == 0)
				dwLangCharset = m_dwLangCharset;

			// Query version information value
			UINT nQuerySize = 0;
			LPVOID lpData = 0;
			bstr_t strValue, strBlockName;
			format(strBlockName, L"\\StringFileInfo\\%08lx\\%s", dwLangCharset, lpszValueName);

			dbjTHROWIF(
				0 == ::VerQueryValue(
				(void **)m_lpVersionData,
				(wchar_t *)strBlockName, &lpData, &nQuerySize)
				, WinErr);

			strValue = (LPCWSTR)lpData;

			return strValue;
		}
		//------------------------------------------------------------------
		bool file_version::GetFixedInfo(VS_FIXEDFILEINFO& vsffi)
		{
			// Must call Open() first
			_ASSERT(m_lpVersionData != NULL);
			if (m_lpVersionData == NULL)
				return FALSE;

			UINT nQuerySize;
			VS_FIXEDFILEINFO* pVsffi;
			if (::VerQueryValue((void **)m_lpVersionData, L"\\",
				(void**)&pVsffi, &nQuerySize))
			{
				vsffi = *pVsffi;
				return TRUE;
			}

			return FALSE;
		}
		//------------------------------------------------------------------
		bstr_t file_version::GetFixedFileVersion()
		{
			bstr_t strVersion;
			VS_FIXEDFILEINFO vsffi;

			if (GetFixedInfo(vsffi))
			{
				format(strVersion, L"%u,%u,%u,%u", HIWORD(vsffi.dwFileVersionMS),
					LOWORD(vsffi.dwFileVersionMS),
					HIWORD(vsffi.dwFileVersionLS),
					LOWORD(vsffi.dwFileVersionLS));
			}
			return strVersion;
		}
		//------------------------------------------------------------------
		bstr_t file_version::GetFixedProductVersion()
		{
			bstr_t strVersion;
			VS_FIXEDFILEINFO vsffi;

			if (GetFixedInfo(vsffi))
			{
				format(strVersion, L"%u,%u,%u,%u", HIWORD(vsffi.dwProductVersionMS),
					LOWORD(vsffi.dwProductVersionMS),
					HIWORD(vsffi.dwProductVersionLS),
					LOWORD(vsffi.dwProductVersionLS));
			}
			return strVersion;
		}
		//------------------------------------------------------------------
	} //	namespace fm 
} // namespace dbjsys 
//------------------------------------------------------------------

#endif  // __FILEVERSION_H_


