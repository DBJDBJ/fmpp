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
//  $Author: DBJ $
//  $Date: $
//  $Revision: $
//*****************************************************************************/
#pragma once
/* 
FM++ configuration

Some FM++ headers use guard statements to determine the system on which each function
 is supported. The following table describes these statements.

Guard Statement                 Meaning 
--------------------------------------------------------------------------------------
#if _WIN32_WINNT >= 0x0500      REQUIREMENT OK: Windows 2000 and later. 
#if _WIN32_IE >= 0x0500         REQUIREMENT OK: Internet Explorer 5.0 and later. 

*/

//////////////////////////////////////////////////////////////////////////////////
#if ! defined (_WIN32_WINNT)
// SDK not used so we define XP to be our default platform
#define _WIN32_WINNT 0x0501
#endif
//////////////////////////////////////////////////////////////////////////////////
#if ( _WIN32_WINNT < 0x0500 )
#pragma message ( "_WIN32_WINNT : " _WIN32_WINNT )
#error DBJ*FM++ requires Windows 2000 or better
#endif

#if ! defined ( _NATIVE_WCHAR_T_DEFINED )
#error For DBJ*FM++, wchar_t must be defined as a native type
#endif


#ifdef _UNICODE
	#ifndef UNICODE
		#define UNICODE
	#endif
// without this TCHAR may stay 'char' even if your build is unicode!
#else
	#error DBJ*FM++, must be built as UNICODE
#endif

#if ! defined( _CPPRTTI )
#error DBJ*FM++, must be built with RTTI switched on.
#endif
