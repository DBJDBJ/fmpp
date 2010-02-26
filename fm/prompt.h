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
// This file contains simple inlines to aid simple ad-hoc testing app's
//190997 JovanovD
//
// Complete change, if interested in previous version look into the VSS DB
//14NOV20000 DBJ
#pragma once

#if ! defined( _TEST_UTILITIES_H_ )
#define _TEST_UTILITIES_H_ 1
//---------------------------------------------------------------------------------------
namespace dbjsys {
	namespace fm {
//---------------------------------------------------------------------------------------
//
	//
    std::wostream & prompt_stream () ;
    // switch to arbitrary std::wostream
    void set_prompt_stream ( std::wostream & new_stream );
    // switch back to default
    void reset_prompt_stream () ;
    // switch back to default
    unsigned int prompth_length ( unsigned int change_to = 0 ) ;


//-----------------------------------------------------------------------------
//
std::wostream & prompt ( const wchar_t *  p , /* prompt message */
				  const wchar_t * promptChar = 0 , /* use it if not 0 */
				  const wchar_t * fillChar = 0 /* use it if not 0 */
				  ) ;
/*
inline std::wostream & prompt ( const std::wstring &  p , // prompt message 
				  const wchar_t * promptChar = 0 , // use it if not 0 
				  const wchar_t * fillChar = 0 // use it if not 0 
				  )
{
	return prompt( p.c_str(), promptChar, fillChar ) ;
}
*/
//---------------------------------------------------------------------------------------
// repeat given char , N times into the given ostream
std::wostream & prompt ( const wchar_t * c_ , 
				  const unsigned int N_ 
				  ) ;
//--------------------------------------------------------------------------------
// this template and instantiations bellow can be used as functors
// when outputing values to prompt()
template< class T> 
inline 
void val2prompt( const T & arg )  
{
	prompt(arg) ;
}

inline void val2prompt ( const wchar_t* arg  ) 
{
	prompt( _bstr_t(arg)) ;
}
// instantiations (in prompt.cpp) are usable straight-away, eg:
// std::foreach( a,b, val2prompt<bstr_t> ) ;
//---------------------------------------------------------------------------------------
	} ; // fm
}; // dbjsys
//-----------------------------------------------------------------------------
#endif // _TEST_UTILITIES_H_ 1
//-----------------------------------------------------------------------------
