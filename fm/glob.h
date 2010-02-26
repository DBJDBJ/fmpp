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

#if ! defined ( DBJ_GLOBALS_H_ )
#define DBJ_GLOBALS_H_ 1

//--------------------------------------------------------------------------------
namespace dbjsys {
	// system wide globals
    namespace glob {
//--------------------------------------------------------------------------------
		// the rock bottom default log file name
		 const wchar_t * const DFLT_LOG_FILE() ;
		// the rock bottom default log folder name
		 const wchar_t * const LOG_DIR() ;
		// the end of string
		 const wchar_t EOS() ;
		 const wchar_t * const dot() ;
		 const wchar_t * const backSlash() ;
		 const wchar_t * const forwardSlash() ;
//--------------------------------------------------------------------------------
    } //    namespace fm 
} // namespace dbjsys 
//--------------------------------------------------------------------------------

#endif // ! defined ( DBJ_GLOBALS_H_ )
