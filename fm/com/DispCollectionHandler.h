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

#pragma once

#include <dispex.h>

#if ! defined( IDispatchExPtr )
_COM_SMARTPTR_TYPEDEF(IDispatchEx , __uuidof(IDispatchEx));
#endif


namespace dbjsys {
	namespace fm {
//--------------------------------------------------------------------------------------------

/*
----------------------------------------------------------------------------
A collection provides a set of objects over which iteration can be performed. 
All collection objects must provide the following properties:

Property name   Return type     Description 
-------------------------------------------
Count           VT_I4           Returns the number of items in the collection; 
                                read only. Required. 
_NewEnum        VT_DISPATCH     A special property that returns an enumerator 
                                object that implements IEnumVARIANT. Required. 
----------------------------------------------------------------------------
*/
// 
class DispCollectionHandler
{
	// 
    IDispatchPtr dispPtr ;
public:
    //-----------------------------------------------------------------------
	// 
    DispCollectionHandler( const IDispatchPtr & handle_this) : dispPtr(handle_this) 
    {
    }
    //-----------------------------------------------------------------------
	// 
    IDispatchExPtr getDispatchEx()
    {
        return IDispatchExPtr(dispPtr);
    }

	// 
	IDispatchPtr getDispatch()
    {
        return IDispatchPtr(dispPtr);
    }

    //-----------------------------------------------------------------------
	// 
    void showMembers ( std::wostream & target )
    {
        IDispatchExPtr exPtr = getDispatchEx() ;

	// HRESULT GetMemberName( [in] DISPID id, [out] BSTR *pbstrName);
	// HRESULT GetNextDispID( [in] DWORD grfdex, [in] DISPID id,[out] DISPID *pid);

        DISPID dispid = (DISPID)DISPID_STARTENUM ; // -1 works but strangely?
        DISPID nextdispid = (DISPID)0 ;
        BSTR   name ;
        while ( ! FAILED( exPtr->GetNextDispID(fdexEnumAll /*fdexEnumDefault*/, dispid, &nextdispid )) )
        {
            if ( nextdispid < 0 ) break ;
            exPtr->GetMemberName( nextdispid, &name ) ;
            if ( name != NULL )
            target << (wchar_t *)_bstr_t(name) << std::endl ;
            dispid = nextdispid ;
        }
    }
    //-----------------------------------------------------------------------
	// 
	virtual ~DispCollectionHandler()
    {
        dispPtr = 0 ;
    }
    //-----------------------------------------------------------------------
	// 
    _variant_t getCount()
    {
       return this->getProperty(OLESTR("Count")) ;
    }
    //-----------------------------------------------------------------------
    // return IenumVARIANT from this collection
	// 
    IEnumVARIANTPtr getEnum()
    {
        VARIANT result ;
        _com_dispatch_propget( dispPtr, DISPID_NEWENUM, VT_VARIANT, &result ) ;
        IEnumVARIANTPtr enum_result = _variant_t(result, false) ;
        return enum_result ;
    }
    //-----------------------------------------------------------------------
	// 
    HRESULT OutputEnumeratedItems(std::wostream & target_stream, 
        const wchar_t * delimiter = L"\n" )
        {
	        // Get the VARIANT enumerator from this collection handler
	        IEnumVARIANTPtr spEnum = this->getEnum();
	        // nBatchSize is the number of items that we request in each call to 
            // IEnumVARIANT::Next. 
	        // The actual number of items returned may not equal nBatchSize.
	        const ULONG nBatchSize = 1;
	        // nReturned will store the number of items returned by a call 
            // to IEnumVARIANT::Next
	        ULONG nReturned = 0;
	        // arrVariant is the array used to hold the returned items
	        VARIANT     arrVariant[nBatchSize]  = {0};
	        HRESULT     hr                      = E_UNEXPECTED;
	        do
	        {
		        hr = spEnum->Next(nBatchSize, &arrVariant[0], &nReturned);
		        if (FAILED(hr)) return hr;
                // S_OK indicates nBatchSize was returned
		        if ( hr == S_OK )
		        {
			        _bstr_t bstrTemp = static_cast<_variant_t>(arrVariant[0]);
			        target_stream << static_cast<const wchar_t*>(bstrTemp) << delimiter ;
			        ::VariantClear(&arrVariant[0]);
		        }

	        } while (hr != S_FALSE); 
            // S_FALSE indicates end of collection

	        return S_OK ;
        }

    //-----------------------------------------------------------------------
    // This method will put data into any type T container, that has method 
    // T.push_back( T.value_type v )
    //
    template< class T >
    HRESULT OutputEnumeratedItems(T & target_container )
        {
	        // Get the VARIANT enumerator from this collection handler
	        IEnumVARIANTPtr spEnum = this->getEnum();
	        // nBatchSize is the number of items that we request in each call to 
            // IEnumVARIANT::Next. 
	        // The actual number of items returned may not equal nBatchSize.
	        const ULONG nBatchSize = 1;
	        // nReturned will store the number of items returned by a call 
            // to IEnumVARIANT::Next
	        ULONG nReturned = 0;
	        // arrVariant is the array used to hold the returned items
	        VARIANT     arrVariant[nBatchSize]  = {0};
	        HRESULT     hr                      = E_UNEXPECTED;
	        do
	        {
		        hr = spEnum->Next(nBatchSize, &arrVariant[0], &nReturned);
		        if (FAILED(hr)) return hr;
                // S_OK indicates nBatchSize was returned
		        if ( hr == S_OK )
		        {
			        _variant_t temp = static_cast<_variant_t>(arrVariant[0]);
                    target_container.push_back( static_cast<T::value_type>(temp) ) ;
			        ::VariantClear(&arrVariant[0]);
		        }

	        } while (hr != S_FALSE); 
            // S_FALSE indicates end of collection

	        return S_OK ;
        }
    /*-----------------------------------------------------------------------
    Returns the indicated item in the collection. Required. 
    The Item method may take one or more arguments to indicate the element 
    within the collection to return. 
    This method is the default member (DISPID_VALUE) for the collection object.
    */
	// 
    _variant_t getItem ( ULONG index_ ) 
    {
        const _variant_t var_index_(static_cast<long>(index_)) ;
        VARIANT _result;
        _com_dispatch_method(
            dispPtr, 
            DISPID_VALUE, 
            DISPATCH_METHOD, 
            VT_VARIANT, 
            (void*)&_result, 
            L"\x000c", 
            &var_index_);
        return _variant_t(_result, false);
    }


            //-----------------------------------------------------------------------
	// 
            _variant_t getProperty( LPCOLESTR name, DISPID dwDispID = 0 ) 
            {
                // we have to worry about MT issues here ! (UNRESOLVED!)
                if ( dwDispID == 0 )
                _com_util::CheckError( getIDOfName_( dispPtr, name, &dwDispID) ) ;

                _variant_t var ;
                _com_util::CheckError( getPropertyNoArgs_( dispPtr, dwDispID, var ) ) ;
        
                return var ;
            }
protected:
            //-----------------------------------------------------------------------
	// 
            HRESULT getIDOfName_(IDispatchPtr & dispPtr, LPCOLESTR lpsz, DISPID* pdispid)
	        {
		        return dispPtr->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpsz, 1, 
                    LOCALE_SYSTEM_DEFAULT, pdispid);
	        }
            //-----------------------------------------------------------------------
	// 
            HRESULT getPropertyNoArgs_(IDispatchPtr & dispPtr, DISPID dwDispID, _variant_t & var)
	        {
		        DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
		        return dispPtr->Invoke(dwDispID, IID_NULL,
				        LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
				        &dispparamsNoArgs, &var, NULL, NULL);
	        }

//-----------------------------------------------------------------------
};
//-----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
	} // namespace fm 
} // namespace dbjsys 
//--------------------------------------------------------------------------------------------
