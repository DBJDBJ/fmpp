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
//  $Author: Mh $
//  $Date: $
//  $Revision: $
//*****************************************************************************/
#pragma once
//*****************************************************************************/
namespace dbjsys {
	namespace fm {
//--------------------------------------------------------------------------		
#if ( _WIN32_WINNT >= 0x0400 )
inline BOOL is_Windows2000_SP1_or_later( void )
{
    OSVERSIONINFOEX  vi;
    DWORDLONG        dwlConditionMask = 0;
  
    ZeroMemory(&vi, sizeof(OSVERSIONINFOEX));
  
    // Must always initialize structure size
    vi.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );
    // Check for Windows 2000, SP1 or later
    vi.dwMajorVersion = 5;
    vi.dwMinorVersion = 0;
    vi.wServicePackMajor = 1;
  
    // Initialize the condition mask for each field
    // being used in the OSVERSIONINFOEX structure
    VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, 
                       VER_GREATER_EQUAL );
    VER_SET_CONDITION( dwlConditionMask, VER_MINORVERSION, 
                       VER_GREATER_EQUAL );
    VER_SET_CONDITION( dwlConditionMask, VER_SERVICEPACKMAJOR, 
                       VER_GREATER_EQUAL );
  
    // Perform the test and return the result
  
    return( VerifyVersionInfo( &vi, 
                               VER_MAJORVERSION | 
                               VER_MINORVERSION | 
                               VER_SERVICEPACKMAJOR,
                               dwlConditionMask ) );
}
#else // not W2K environment
inline BOOL is_Windows2000_SP1_or_later( void )
{
    return 1 == 0  ;
}
#endif // ( _WIN32_WINNT >= 0x0400 )
  
//--------------------------------------------------------------------------		
} //	namespace fm 
} // namespace dbj 
//*****************************************************************************/
//*****************************************************************************/
