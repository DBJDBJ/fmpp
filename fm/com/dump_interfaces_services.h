//*****************************************************************************/
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
namespace dbjsys {
	namespace fm {
//--------------------------------------------------------------------------------------------

//
// Intialy from :
// Trace registered interfaces and services available on the object
// by Andrew Nosenko <andien@geocities.com>,
// http://www.geocities.com/~andien/atlaux.htm
// Version: 1.01
//------------------------------------------------------------------
#if ! defined ( _ASSERTE )
#include <crtdbg.h>
#endif
//------------------------------------------------------------------
inline void _dbgout(wchar_t * pszFormat, ...) {
  va_list arglist;
  va_start(arglist, pszFormat);
  wchar_t szBuff[1024];
  int rtn = wvsprintfW(szBuff, pszFormat, arglist) ; 
  _ASSERTE( rtn < sizeof(szBuff)/sizeof(szBuff[0]) );
  OutputDebugStringW(szBuff);
  va_end(arglist);
}

// cannot use _t2ole inside the loop...
inline OLECHAR* _t2ole(OLECHAR* dst, const wchar_t* src) {
  OLECHAR* d = dst;
  while ( (*d++ = *src++) );
  return dst;
}

// dump all registered and supported interfaces

inline const void DumpIfaces(IUnknown* unk) 
{
  _dbgout(L"-- Interfaces:\n");
  if ( !unk ) return;

  HKEY hk;
  wchar_t szIID[80];
  wchar_t szValue[256];
  if ( RegOpenKeyW(HKEY_CLASSES_ROOT, L"Interface", &hk) == ERROR_SUCCESS )
  {
    for ( DWORD dwIndex = 0;
          RegEnumKeyW(hk, dwIndex, szIID, sizeof(szIID)) == ERROR_SUCCESS;
          ++dwIndex )
    {
      LONG cb = sizeof(szValue);
      if ( RegQueryValueW(hk, szIID, szValue, &cb) == ERROR_SUCCESS )
      {
        IID iid;
        OLECHAR szBuf[80];
        ::CLSIDFromString(_t2ole(szBuf, szIID), &iid);
        IUnknown* pfoo = NULL;
        if ( SUCCEEDED(unk->QueryInterface( iid, (void**)&pfoo)) )
        {
          pfoo->Release();
          _dbgout(L"%s, %s\n", szIID, szValue);
        }
      }
    }
    RegCloseKey(hk);
  }
  _dbgout(L"\n");
}

// dump all registered and supported services

EXTERN_C const GUID SID_SContainerDispatch;
EXTERN_C const GUID SID_STopLevelBrowser;

inline const void DumpServices(IUnknown* unk) {
  _dbgout(L"-- Services:\n");
  if ( !unk ) return;
  IServiceProvider* sp = NULL;
  unk->QueryInterface(IID_IServiceProvider, (void**)&sp);
  if ( !sp ) return;

  struct SIDINFO {
    const wchar_t * pszName;
    const IID* piid;
  };
  #define SID_INFO(sid) { L#sid, &sid }

  static SIDINFO sids[] = {
    SID_INFO(SID_SContainerDispatch),
    SID_INFO(SID_STopLevelBrowser),
  };

  for ( int i = 0; i < sizeof(sids)/sizeof(sids[0]); i++ ) {
    IUnknown* pfoo = NULL;
    if ( SUCCEEDED(sp->QueryService(*sids[i].piid, IID_IUnknown, (void**)&pfoo )) )
    {
      pfoo->Release();
      LPOLESTR pszIID;
      StringFromCLSID(*sids[i].piid, &pszIID);
      _dbgout(L"%ls, %s\n", pszIID, sids[i].pszName);
      CoTaskMemFree(pszIID);
    }
  }

  HKEY hk;
  wchar_t szIID[80];
  wchar_t szValue[256];
  if ( RegOpenKeyW(HKEY_CLASSES_ROOT, L"Interface", &hk) == ERROR_SUCCESS )
  {
    for ( DWORD dwIndex = 0;
          RegEnumKeyW(hk, dwIndex, szIID, sizeof(szIID)) == ERROR_SUCCESS;
          ++dwIndex )
    {
      LONG cb = sizeof(szValue);
      if ( RegQueryValueW(hk, szIID, szValue, &cb) == ERROR_SUCCESS )
      {
        IID iid;
        OLECHAR szBuf[80];
        ::CLSIDFromString(_t2ole(szBuf, szIID), &iid);
        IUnknown* pfoo = NULL;
        if ( SUCCEEDED(sp->QueryService(iid, iid, (void**)&pfoo )) )
        {
          pfoo->Release();
          _dbgout(L"%s, %s\n", szIID, szValue);
        }
      }
    }
    RegCloseKey(hk);
  }

  sp->Release();
  _dbgout(L"\n");
} ;
//------------------------------------------------------------------
} // namespace fm
} //      namespace dbjsys
//------------------------------------------------------------------
