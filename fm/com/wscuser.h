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

#if !defined(AFX_WSCUSER_H__9868993C_50AC_4332_B163_ADADB27BC1D4__INCLUDED_)
#define AFX_WSCUSER_H__9868993C_50AC_4332_B163_ADADB27BC1D4__INCLUDED_
// #include <comdef.h>
#pragma once
//
namespace dbjsys {
	namespace fm {
//--------------------------------------------------------------------------------------------

// 
        class WscUser 
        {
        public:
	// 
	        WscUser( const _bstr_t & progID )
                : progID_(progID) 
            {
            }

	// 
	        ~WscUser()
            {
            }
            
	// 
            _variant_t getProperty( const _bstr_t & name )
            {    
                PropertyMap::iterator it = propertyMap_.find( name ) ;
                if( it != propertyMap_.end() )
                {
                    return (*it).second ;
                }
                else
                {
                    _variant_t var = getProperty_( name ) ;
                    propertyMap_[ name ] = var ;
                    return var ;
                }
            }

        private:

	// 
            _variant_t getProperty_( const _bstr_t & name ) 
            {
                //HRESULT hr ;
                ///hr = ::CoInitialize(0) ;
                //_com_util::CheckError( hr ) ;
    
                // created everytime so we don't have to worry about MT issues
                // potentially slow, we may want to implement a cache
                IDispatchPtr dispPtr ;
                dbjCOMVERIFY( dispPtr.CreateInstance( static_cast<LPOLESTR>(progID_)) ) ;
                                
                DISPID dwDispID;
		        dbjCOMVERIFY( getIDOfName( dispPtr, static_cast<LPCOLESTR>(name), &dwDispID) ) ;

                _variant_t var ;
                dbjCOMVERIFY( getProperty( dispPtr, dwDispID, var ) ) ;
        
                dispPtr = 0 ;
                return var ;
            }

	// 
            HRESULT getIDOfName(IDispatchPtr & dispPtr, LPCOLESTR lpsz, DISPID* pdispid)
	        {
		        return dispPtr->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpsz, 1, 
                    LOCALE_USER_DEFAULT, pdispid);
	        }

	// 
            HRESULT getProperty(IDispatchPtr & dispPtr, DISPID dwDispID, _variant_t & var)
	        {
		        DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
		        return dispPtr->Invoke(dwDispID, IID_NULL,
				        LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
				        &dispparamsNoArgs, &var, NULL, NULL);
	        }

	// 
            _bstr_t progID_ ;

            // a cache of all the properties we have read
            // for speed and safety (i.e. when program is terminating we cannot
            // call CoInitialize() ;
            typedef std::map< _bstr_t, _variant_t > PropertyMap ;
	// 
            PropertyMap propertyMap_ ;
		} ;
//--------------------------------------------------------------------------------------------
	} // namespace fm 
} // namespace dbjsys 
//--------------------------------------------------------------------------------------------


#endif // !defined(AFX_WSCUSER_H__9868993C_50AC_4332_B163_ADADB27BC1D4__INCLUDED_)
