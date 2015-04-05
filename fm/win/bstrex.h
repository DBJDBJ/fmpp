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
//
// _bstr_t extensions
//

#pragma once

#if ! defined (__BSTREX_H_INCLUDED__)
#define __BSTREX_H_INCLUDED__

//*****************************************************************************/
namespace dbjsys {
	namespace fm {
		namespace bstrex {
//*****************************************************************************/
// 
		class bstr_conversion {
		public :
			typedef Error<bstr_conversion> Err ;
		} ;
		
		typedef bstr_conversion::Err Err ;
		//---------------------------------------------------------------------------------------
		// here we enforce our policies about bstr_t
// 
		class dbjBSTR 
		{
	// 
			_bstr_t the_bstr_ ;
		public :
	// 
			explicit dbjBSTR( const bstr_t & other_ ) 
				: the_bstr_( other_ ) // COPYING
			{
			}
			// POLICY: do not make bstr_t with 0 pointer inside
	// 
			explicit dbjBSTR( )
				: the_bstr_( L"") // avoid NULL bstr_t!
			{
			}
			// POLICY: do not make bstr_t with 0 pointer inside
	// 
			explicit dbjBSTR( const  wchar_t * other_ )
				: the_bstr_( other_ == 0 ? L"" : other_ ) // avoid NULL bstr_t!
			{
			}
			//
	// 
			operator bstr_t & () { return the_bstr_ ; }
	// 
			operator const bstr_t & () const { return the_bstr_; }
			// POLICY: do not let non const pointers out
	// 
			operator const wchar_t * () const 
			{ 
				return (const wchar_t *)the_bstr_; 
			}
			// POLICY: do not let char * out!
	// 
			operator const char * () const 
			{
				_ASSERT(0); return 0;
			}
		} ;
		//---------------------------------------------------------------------------------------
		// 
		// First  we define inline definitions to make compilations of the two templated 
		// operators for streaming to and from bstr_t.
		// These definitions are here because they  are for type which are used by FM
		// like std::wostream, std::wstring,etc.
		// The defintions for types which are specific will be defined in the modules
		// which are declaring those types. But in this same namespace.

		//---------------------------------------------------------------------------------------
		// assign std::wstring to _bstr_t
		inline 
			_bstr_t &
		assign_to_bstr_t_ ( _bstr_t & bstr, const std::wstring & ws_ ) 
		{
			return bstr = static_cast<const wchar_t *>(ws_.c_str()) ;
		}
		// assign _bstr_t to std::wstring
		inline
			std::wstring &
		assign_from_bstr_t_( std::wstring & ws_ , const _bstr_t & bstr_ )
		{
			return ws_ = static_cast<const wchar_t *>(bstr_) ;
		}

		//---------------------------------------------------------------------------------------
		// assign wchar_t * to _bstr_t
		inline 
			_bstr_t &
		assign_to_bstr_t_ ( _bstr_t & bstr, const wchar_t * ws_ ) 
		{
			dbjVERIFY( ws_ ) ;
			return bstr = static_cast<const wchar_t *>(ws_) ;
		}
		// assign _bstr_t to  wchar_t * 
		inline
			wchar_t * 
		assign_from_bstr_t_( wchar_t * ws_ , const _bstr_t & bstr_ )
		{
			dbjVERIFY( ws_ ) ;
			// as long as bstr_t exist ws_ will point to something valid
			// whatever ws_ was pointing to before is lost
			return ws_ = const_cast< wchar_t *>((const  wchar_t *)bstr_) ;
		}

		//---------------------------------------------------------------------------------------
		// 
		// These definitions are here because they  are for type which are used by FM
		// like std::wostream, std::wstring,etc.
		// The defintions for types which are specific should be defined in the modules
		// which are declaring those types. But in this same namespace.
		//---------------------------------------------------------------------------------------
		__forceinline
		std::wostream & assign_from_bstr_t_(std::wostream & t_, const bstr_t & bstr)
		{
			return t_ << ((const wchar_t*)bstr);
		}
		// wostream to bstr_t -- a tricky one !
		__forceinline
			bstr_t & assign_from_bstr_t_(bstr_t & bstr, const std::wostream & t_)
		{
			std::wostringstream wos;
			wos << t_.rdbuf();
			bstr += wos.str().c_str();
			return bstr;
		}

		//------------------------------------------------------------------------
		//
	    // By defining the two operatos bellow we provoke the compiler to 
		// use them and in turn to signal which versions of
		// assign_to_bstr_t_(), and assign_from_bstr_t_()
		// are yet to be defined. These are to be added to bstrex.cpp
		//
		//
		// streaming INTO the _bstr_t FROM the instance of type T
		template <typename T>
			__forceinline
		_bstr_t & operator << ( _bstr_t & bstr, const  T & t_ )
		{
			return dbjsys::fm::bstrex::assign_to_bstr_t_ ( bstr, t_ ) ;
		}

		// streaming FROM the _bstr_t INTO the instance of type T
		template <typename T>
			__forceinline
		T & operator << ( T & t_, const _bstr_t & bstr )
		{
			return dbjsys::fm::bstrex::assign_from_bstr_t_ ( t_, bstr ) ;
		}
		//--------------------------------------------------------------
		// here are the explicit  operators to help compiler to resolve
		// what it thinks is ambigious
			__forceinline
		bstr_t & operator << ( bstr_t & t_, bstr_t & bstr )
		{
			return t_ += bstr ;
		}

//*****************************************************************************/
		} // namespace bstrex 
	} // fm
}  // dbjsys
//*****************************************************************************/



#endif // __BSTREX_H_INCLUDED__
