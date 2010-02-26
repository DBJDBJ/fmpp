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
//	
//---------------------------------------------------------------------------------------
//	Library of Sub-System Mechanisms
#ifndef errortplt_h
#define errortplt_h 1
#pragma once
#if ! defined ( type_info )
#include <typeinfo.h> 
#endif
// ----------------------------------------------------------------------
namespace dbjsys {
	namespace fm {
//	---------------------------------------------------------
//	We want more info when we catch the exception
//	We want to know at least the class of the object
//	WHO has thrown the exception, besides the actual
//	message saying WHAT has happened. And because we
//	are pro-active we will want to know from WHERE this
//	exception is thrown.
//	This is the concept of what we want, and this is
//	the class built around that concept.
//
//	---------------------------------------------------------
//
//	The usage example :
//
//	class A {
//	public :
//		typedef Error<A> Err ; // this is all we have to do
//
//		A () {
//			// throw Err("A::A() failed") ; // re-active usage
//			throw Err("A::A() failed", __FILE__, __LINE__) ; //
//	pro-active usage
//		}
//	} ;
//
//	int main ( int argc , TCHAR ** argv )
//	{
//		try {
//			A aha ;
//		}
//		catch ( A::Err & x ) // when A is visible, we can use
//	it
//	    {	                 // to access it's interface
//			x.display() ;
//		}
//		catch ( exception & x ) // or like this when A is not
//	visible
//		{                       // but still with extended info
//			cerr << endl << x.what() << endl ;
//		}
//			return 0 ;
//	}
//
//	//
//	// The above test will print out the following :
//	//
//	// Exception from    : class A
//	// In File , On Line :
//	C:\architect\cpp\errtemplate\errtemplate.h , 81
//	// Exception message : A::A() failed
//	//
//	---------------------------------------------------------
//
//
template< typename T > 
inline
_bstr_t dbjtypename( T * t_null /* = (T*)0 */ ) 
{
	#if defined( _CPPRTTI )
		return _bstr_t(typeid(T).name()) ;
	#else
		return _bstr_t(L"UNKNOWN -- non RTTI build") ;
	#endif
}
// give the typename as _bstr_t
#define DBJTYPEID(T) dbjtypename<T>((T*)0)
//---------------------------------------------------------------------------------------
//
// interface to each and every DBJSolutions generated class
// 
		class DBJSYSError
		{
	// 
			const _bstr_t errmsg_ ;
	// 
			bool reported_ ;
			// no default ctor
	// 
			DBJSYSError() ;

		public:
	// 
			DBJSYSError( const _bstr_t & themsg, bool DO_NOT_BEEP_ON_EXIT = false )
				: errmsg_(themsg), reported_(DO_NOT_BEEP_ON_EXIT)
			{
			}		
	// 
			virtual ~DBJSYSError() ;
			//
	// 
			const _bstr_t & what () const 
			{ 
				const_cast<DBJSYSError*>(this)->reported_ = true ; 
				return errmsg_ ; 
			}
			// make 'standard' error message
	// 
			static _bstr_t  makeErrMsg( const _bstr_t & typeName, const _bstr_t & msg, const _bstr_t & file, const long line ) ;
			// make win32 error message
	// 
			static _bstr_t  makeErrMsg( const _bstr_t & typeName, DWORD err_code , const _bstr_t & file, const long line ) ;
		} ;
//-------------------------------------------------------------------------------------
// concrete class to be used as abase of all DBJSolutions exceptions
// that can be made
// 
template <class T> class Error : public DBJSYSError  
{
  
	typedef  DBJSYSError	parent_type ;
  public:
        //	The class that has thrown this exception.
    typedef T thrower_type;

	// 
      Error (const DWORD & win32error_code, bool DO_NOT_BEEP_ON_EXIT = false ) throw()
		  : parent_type(
            parent_type::makeErrMsg( DBJTYPEID(thrower_type), win32error_code,L"~",0),
			DO_NOT_BEEP_ON_EXIT )
      {
      }
  	//
	// 
      Error (const _bstr_t & msg, bool DO_NOT_BEEP_ON_EXIT = false ) throw()
		  : parent_type(
            parent_type::makeErrMsg( DBJTYPEID(thrower_type), msg,L"~",0),
			DO_NOT_BEEP_ON_EXIT )
      {
      }

	//
	// 
      Error (const  _bstr_t & msg, const  _bstr_t & file, const int line, bool DO_NOT_BEEP_ON_EXIT = false ) throw()
		  : parent_type( 
		    parent_type::makeErrMsg( DBJTYPEID(thrower_type) ,msg, file, line),
			DO_NOT_BEEP_ON_EXIT )
      {
      }
};
//-------------------------------------------------------------------------------------
// WIn32 error message + who did it,but not where
// 
template<class T> class Win32Error : public Error<T>
{
	typedef Error<T> Parent ;
	public :
	// 
		Win32Error () 
			: Parent( ::GetLastError() ) 
		{
			::SetLastError(0) ;
		}
} ;

//	----------------------------------------------
#undef DBJTYPEID
//	----------------------------------------------
// class _com_error ;
template < class T > inline
void __dbj_throw__ ( const _com_error & e, const _bstr_t & file, const int line , T * )
{
        throw T( dbjsys::fm::getErrMsg( e ),file,line) ;
}
//	----------------------------------------------
template < class T > inline
void __dbj_throw__ ( const _bstr_t &  msg, const _bstr_t &  file, const int line, T * )
{
        throw T(msg,file,line) ;
}
//	----------------------------------------------
template < class T > inline
void __dbj_throw__ ( const _bstr_t & msg, const char * file, const int line, T * )
{
        throw T(msg, _bstr_t(file) ,line) ;
}
//	----------------------------------------------
/* 
This in effect transforms std::exception to DBJSYSError
*/
template < class T > inline
void __dbj_throw__ ( const std::exception & msg, const char * file, const int line, T * )
{
    throw T(msg.what(), _bstr_t(file), line) ;
}
/* */
//	----------------------------------------------
	} ; // fm
}; // dbjsys
//	----------------------------------------------
//
#define dbjTHROWERR(m) dbjsys::fm::__dbj_throw__(m,__FILE__,__LINE__, (Err*)0)
//
// this assertion always works , e.g. in a release mode
// string argument is always taken as UNICODE (prefixed with  'L')
#define dbjVERIFY(x) if (!(x)) dbjTHROWERR( L#x )
//
#define dbjCOMVERIFY( x ) _com_util::CheckError( x )
//
#define dbjMAKE_ERR(x) typedef dbjsys::fm::Error<x> Err
// 
#define dbj_ERR(x) struct dbjNOVTABLE x : public Err { x() : Err(L#x, true ) {} } ;
//
#define dbjTHROWIF(x,y) if ( x ) throw y() 

//	----------------------------------------------
#endif
