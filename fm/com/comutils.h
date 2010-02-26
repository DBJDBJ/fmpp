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
#pragma once

#ifndef MS_COMUTILS_H
#define MS_COMUTILS_H 1

namespace dbjsys {
	namespace fm {
//--------------------------------------------------------------------------------------------

            //--------------------------------------------------------------------
            const inline wchar_t * UNKNOWN_EXCEPTION_MSG() { return L"Unknown Exception" ; }
            
			//--------------------------------------------------------------------
            inline _bstr_t getErrMsg( const _com_error & e ) 
            {
			    _variant_t  verr_( e.Error() ) ;
                _bstr_t err_(verr_) ;
                // _bstr_t err_( variantL e.Error() )) ; 
                // does not compile? DBJ 19NOV2000

                _bstr_t desc( e.Description() ) ;
                _bstr_t emsg( e.ErrorMessage() ) ;

                // HRESULT
                _bstr_t retStr = L"HR : " ;
                retStr += err_ ;

                // desciption
                retStr += L", DESC : " ;
                if( desc.length() > 0 )
                    retStr += desc ;
                else
                    retStr += L"null" ;

                // error message
                retStr += L", ERRMSG : " ;
                if( emsg.length() > 0 )
                    retStr += emsg ;
                else
                    retStr += L"null" ; 

                return retStr ;
            }

			//--------------------------------------------------------------------
            // _com_error to any output stream conforming to standard library
            template< typename OSTR >
            inline 
            OSTR & operator << ( OSTR & ostr , const _com_error & err_ )
            {
                return ostr << getErrMsg(err_) ;
            }
			inline 
			std::wostream & operator << ( std::wostream & ostr , const _com_error & err_ )
            {
                return ostr << static_cast<const wchar_t *>(getErrMsg(err_)) ;
            }
			inline 
			std::ostream & operator << ( std::ostream & ostr , const _com_error & err_ )
            {
                return ostr << static_cast<const char *>(getErrMsg(err_)) ;
            }

            //------------------------------------------------------------------
            // This method depends on type REQUESTOR which is supposed to have 
            // nested type Err which in turn is supposed to have ctor like this
            // Err( const char *, const char *, const char *)
            // Dubious design in my mind!
            // DBJ 19NOV2000
            template < class OBJECT_PTR, class REQUESTOR >
                inline 
            OBJECT_PTR comCreateFromProgID( const _bstr_t & progID )
            {
                HRESULT     hRes        = S_OK ;
                OBJECT_PTR  objectPtr   = NULL ;
                try
                {     
                    // MH - Does not compile without this line
                    LPOLESTR szProgID = static_cast<LPOLESTR>(progID) ;
                    hRes = objectPtr.CreateInstance( szProgID ) ;
                    return objectPtr ;
                }
                catch ( const _com_error & e ) 
                {
                    throw REQUESTOR::Err( getErrMsg(e), __FILE__, __LINE__ ) ;
                }
            }


// 
            template < const CLSID * clsid, const IID * iid  > 
            class ErrorReporter : public ISupportErrorInfo
            {
                public:
                    
                    // This method need to compare riid with all the 
                    // interfaces the sub class is implementing
                    // N.B. This is a default implementation, if your
                    // object is implemenating more than one interface
                    // then overide this!!
                    // ISupportsErrorInfo
	// 
	                STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid)
                    {
						if ( ::InlineIsEqualGUID( *iid, riid) )
			                return S_OK;
                    
	                    return S_FALSE;
                    }

                //protected:

	            // also UNICODE ready
	// 
                    HRESULT reportErr( const _com_error & e )
                    {
                        try
                        {
                            return AtlReportError( *clsid, (OLECHAR*)getErrMsg(e) , 
                                *iid, e.Error() ) ;
                        }
                        catch( ... )
                        {
                            return E_UNEXPECTED ;
                        }
                    }

	// 
                HRESULT reportErr( 
						const wchar_t * msg = UNKNOWN_EXCEPTION_MSG() , 
						HRESULT hr = 0 )
                    {
                        try
                        {
	                        return AtlReportError( *clsid, msg, *iid, hr ) ;
                        }
                        catch( ... )
                        {
                            return E_UNEXPECTED ;
                        }
                    } 
	// 
				HRESULT reportErr( 
					const std::exception & e , 
						HRESULT hr = 0 )
                    {
                        try
                        {
							return AtlReportError( *clsid, e.what(), *iid, hr ) ;
                        }
                        catch( ... )
                        {
                            return E_UNEXPECTED ;
                        }
                    }
            };

//--------------------------------------------------------------------------------------------
	} // namespace fm 
} // namespace dbjsys 
//--------------------------------------------------------------------------------------------



// Macro to have a non-creatable class that has a category map
#define OBJECT_ENTRY_NON_CREATEABLE_EX(clsid, class) \
	{&clsid, class::UpdateRegistry, \
	NULL, NULL, NULL, 0, NULL, class::GetCategoryMap, class::ObjectMain },

#endif // MS_COMUTILS_H
