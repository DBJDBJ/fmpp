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

// This code adapted from an article by Michael Lindig on www.codeguru.com

// Usage :

/*
Change: 

template <class T>
class CProxy_IMyEvents : 
public IConnectionPointImpl<T, 
                            &DIID__ISchedulerEvents, 
                            CComDynamicUnkArray>
To 


#include "CProxyEvent.h"

template <class T>
class CProxy_IMyEvents : 
public IConnectionPointImpl<T, 
                            &DIID__ISchedulerEvents, 
                            CComDynamicUnkArray_GIT>
*/

/*
and replace :
Dispatch* pDispatch = reinterpret_cast< IDispatch* >(sp.p) 
with:
CComQIPtr< IDispatch, &IID_IDispatch > pDispatch( sp.p ) 

and 
pDispatch != NULL 
to 
pDispatch.p != NULL
*/

#pragma once

#ifndef __CEVENT_PROXY__
#define __CEVENT_PROXY__

namespace dbjsys {
	namespace fm {
//--------------------------------------------------------------------------------------------


// 
class CComDynamicUnkArray_GIT : public CComDynamicUnkArray
{
    private:
	// 
        IGlobalInterfaceTable*  GIT;

    public:

	// 
        CComDynamicUnkArray_GIT() : CComDynamicUnkArray()
        { 
            GIT = NULL;

            CoCreateInstance( CLSID_StdGlobalInterfaceTable, 
                              NULL, 
                              CLSCTX_INPROC_SERVER, 
                              __uuidof(IGlobalInterfaceTable), 
                              reinterpret_cast< void** >(&GIT) );
        }

	// 
	    ~CComDynamicUnkArray_GIT()
	    {
            clear();
            if( GIT != NULL )
            {
                GIT->Release();
            }
	    }
	// 
	    DWORD Add(IUnknown* pUnk);
	// 
	    BOOL Remove(DWORD dwCookie);

	// 
        CComPtr<IUnknown> GetAt(int nIndex)
	    {
            DWORD dwCookie = (DWORD)CComDynamicUnkArray::GetAt( nIndex );
            if( dwCookie == 0 )
			    return NULL;

            if( CookieMap.find( dwCookie ) == CookieMap.end() )
            {
    		    return (IUnknown*)dwCookie;
            }
            if( GIT != NULL )
            {
                CComPtr<IUnknown>   ppv;

                HRESULT hr = GIT->GetInterfaceFromGlobal(
                  CookieMap[dwCookie],              //Cookie identifying the desired global 
                                                    //interface and its object
                  __uuidof(IUnknown),               //IID of the registered global interface
                  reinterpret_cast< void** >(&ppv)  //Indirect pointer to the desired interface
                );
                if( hr == S_OK )
                {
                    return ppv;
                }
            }
		    return (IUnknown*)dwCookie;
	    }

	// 
	    void clear()
	    {
            CComDynamicUnkArray::clear();

            if( GIT != NULL )
            {
				std::map< DWORD, DWORD >::iterator iter;
                for( iter = CookieMap.begin(); iter != CookieMap.end(); ++iter )
                {
                    GIT->RevokeInterfaceFromGlobal(
                      iter->second              //Cookie that was returned from 
                                                //RegisterInterfaceInGlobal
                    );
                }
            }
            CookieMap.clear();
	    }
    protected:
	// 
		std::map<DWORD, DWORD> CookieMap;
};

inline DWORD CComDynamicUnkArray_GIT::Add(IUnknown* pUnk)
{
    DWORD Result = CComDynamicUnkArray::Add( pUnk );

    HRESULT hr;
    DWORD   pdwCookie = 0;
    if( GIT != NULL )
    {
        hr = GIT->RegisterInterfaceInGlobal(
          pUnk,                 //Pointer to interface of type riid of object 
                                //containing global interface
          __uuidof(IUnknown),   //IID of the interface to be registered
          &pdwCookie            //Supplies a pointer to the cookie that provides 
                                //a caller in another apartment access to the 
                                //interface pointer
        );
    }
    if( hr == S_OK )
    {
        CookieMap[Result] = pdwCookie;
    }

	return Result;
}

inline BOOL CComDynamicUnkArray_GIT::Remove(DWORD dwCookie)
{
    BOOL Result = CComDynamicUnkArray::Remove( dwCookie );

    if( GIT != NULL )
    {
        if( CookieMap.find( dwCookie ) != CookieMap.end() )
        {
            GIT->RevokeInterfaceFromGlobal(
              CookieMap[dwCookie]   //Cookie that was returned from 
                                    //RegisterInterfaceInGlobal
            );
            CookieMap.erase(dwCookie);
        }
    }
    return Result;
}

//--------------------------------------------------------------------------------------------
	} // namespace fm 
} // namespace dbjsys 
//--------------------------------------------------------------------------------------------


#endif
