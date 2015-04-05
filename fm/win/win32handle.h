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

#pragma once

#include "../fm.h"
/*

TODO			Can we do invalid handle here?

*/
namespace dbjsys {
	namespace fm {

// 
	class Win32Handle
	{
	public:
		typedef Win32Error<Win32Handle> W32Err ;
		//
	// 
		explicit Win32Handle( const HANDLE & handle  ) : handle_(handle) {
			_ASSERT( handle_ ) ;
		}

        Win32Handle( const Win32Handle & other )
            : handle_(0) {
            handle_ = duplicate_handle( other.handle_ ) ;
        }

        Win32Handle & operator = ( const Win32Handle & rhs ) {
            if( this != &rhs )
				handle_ = duplicate_handle(rhs.handle_);

            return *this ;
        }
		//
	// 
		~Win32Handle() {
			if( handle_ ) {
				dbjTHROWIF( 0 == ::CloseHandle(handle_), W32Err ) ;
			}
			handle_ = NULL  ;
		}
		//
	// 
		operator HANDLE & () {	_ASSERT( handle_ ) ;
			return handle_ ;
		}
		//
	// 
		operator const HANDLE & () const {	_ASSERT( handle_ ) ;
			return handle_ ;
		}
		private:
	// 
			HANDLE handle_ ;

        static HANDLE duplicate_handle( HANDLE handle ) {
            HANDLE ret_handle ;
            BOOL fSuccess = DuplicateHandle(GetCurrentProcess(), handle,
                GetCurrentProcess(), &ret_handle , 0,  FALSE, DUPLICATE_SAME_ACCESS);
            if( !fSuccess )
                dbjTHROWERR(dbjsys::fm::doctor::errstring()) ;
            return ret_handle ;
        }
	} ;

	} // namespace fm
} // namespace dbjsys
