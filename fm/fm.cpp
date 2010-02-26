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
//  $Author: Dusan $
//  $Date: $
//  $Revision: $
//*****************************************************************************/

#include "fm.h"
#include "dbjio.h"
//--------------------------------------------------------------------------------
namespace dbjsys {
	// system wide globals
    namespace glob {
//--------------------------------------------------------------------------------
		// the rock bottom default log file name
		const wchar_t * const DFLT_LOG_FILE() { return L"c:/DBJSolutionserror.log"; }
		// the rock bottom default log folder name
		const wchar_t * const LOG_DIR() { return L"C:"; /* ::_wgetenv( L"WINDIR"); */ }
		// the end of string
		const wchar_t EOS() { return (wchar_t)0 ; }
		const wchar_t * const  dot() { return L"." ; }
		const wchar_t * const  backSlash() { return L"\\"; } 
		const wchar_t * const  forwardSlash() { return L"/"; } 
//--------------------------------------------------------------------------------
    } //    namespace fm 
} // namespace dbjsys 
//--------------------------------------------------------------------------------
namespace dbjsys {
namespace fm {
//--------------------------------------------------------------------------------
extern "C" void switchErrLog ( const wchar_t * name , const int appendLog ) ;
static const unsigned volatile int APPEND_TO_LOG = 1 ;

//--------------------------------------------------------------------------------
	__DBJSYS_FM_LIB_INITOR__::__DBJSYS_FM_LIB_INITOR__ () 
	{
		// use and increment
		if (  counter_ ++ == 0 )
		{
			switchErrLog( dbjsys::glob::DFLT_LOG_FILE(), APPEND_TO_LOG) ;
	
		}
	}
	__DBJSYS_FM_LIB_INITOR__::~__DBJSYS_FM_LIB_INITOR__ () 
	{
		// decrement annd use
		if ( -- counter_ == 0 )
		{
			using namespace std ;
			// IOSTREAM OBJECTS
			cout.flush() ;
			cerr.flush() ;
			clog.flush() ;
			// WIDE IOSTREAM OBJECTS
			wcout.flush() ;
			wcerr.flush() ;
			wclog.flush() ;
		}
	}

#pragma comment(exestr, "DBJ*FM++ (c) 2000-2006 by DBJSolutions Ltd.")
#pragma warning( disable : 4073 ) 
#pragma init_seg( lib )
long __DBJSYS_FM_LIB_INITOR__::counter_ = 0;
//--------------------------------------------------------------------------------
}
}
//--------------------------------------------------------------------------------
