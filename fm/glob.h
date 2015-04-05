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

#pragma once

//--------------------------------------------------------------------------------
namespace dbjsys {
	// system wide globals
	namespace glob {
		//--------------------------------------------------------------------------------
		// the rock bottom default log file name
		static const wchar_t * const DFLT_LOG_FILE() { return L"c:/DBJDBJ.log"; }
		// the rock bottom default log folder name
		static const wchar_t * const LOG_DIR() { return L"C:"; /* ::_wgetenv( L"WINDIR"); */ }
		// the end of string
		static const wchar_t EOS() { return (wchar_t)0; }
		static const wchar_t * const  dot() { return L"."; }
		static const wchar_t * const  backSlash() { return L"\\"; }
		static const wchar_t * const  forwardSlash() { return L"/"; }
		//--------------------------------------------------------------------------------
		// moved after 2015 MAR
		// If this envvar
		static const wchar_t ENVVAR_NAME_DBJLOGSIZE[] = L"DBJLOGSIZE";
		//is not currently set then this  value 
		static const volatile long DBJLOGSIZE_MAX = 64 * 1024;
		// will be used as maximum allowed size for the log file in the method bellow.
		static const wchar_t * const  FMPP_NAME = L"Fm++";
		static const wchar_t * const  FMPP_YEAR = L"2015";
		static const wchar_t * const  FMPP_MJRR = L"1";
		static const wchar_t * const  FMPP_MINR = L"0";

		static const _bstr_t  FMPP_TITLE = FMPP_NAME + _bstr_t(FMPP_YEAR) + dot() + FMPP_MJRR + dot() + FMPP_MINR;

		//--------------------------------------------------------------------------------
	} //    namespace glob
} // namespace dbjsys 
