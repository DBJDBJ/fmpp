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
/***********************************************************************************************
Bellow is the implementation moved from cpp
************************************************************************************************/
namespace dbjsys {
	namespace fm{

		namespace  prompt_imp
		{
			typedef dbjsys::fm::SREF<CriticalSection> CRIT_REF;

			__forceinline CRIT_REF critical_section()
			{
				static CRIT_REF cs_;
				if (cs_.isNull())
					cs_ = new NOTCriticalSection();
				return cs_;
			}

			__forceinline  std::wostream & the_stream()
			{
				static std::wostream & the_stream_ = std::wcout;
				return the_stream_;
			}

		}
		//----------------------------------------------------------------------------------------------
		//                static std::wostream & the_stream_ = std::wcout ;
		//--------------------------------------------------------------------------------------------
		std::wostream & dbjsys::fm::prompt_stream()
		{
			AutoLocker auto_lock(*prompt_imp::critical_section());
			// default is 'wcout'
			return prompt_imp::the_stream();
		}
		//----------------------------------------------------------------------------------------------
		// switch to arbitrary std::wostream. But how?
		void dbjsys::fm::set_prompt_stream(std::wostream & new_stream)
		{
			AutoLocker auto_lock(*prompt_imp::critical_section());
			std::wostream & wos = prompt_imp::the_stream();
#if PIGS_ARE_FLYING
			wos = new_stream;
#endif
		}
		//----------------------------------------------------------------------------------------------
		void dbjsys::fm::reset_prompt_stream()
		{
			AutoLocker auto_lock(*prompt_imp::critical_section());
#if PIGS_ARE_FLYING
			prompt_imp::the_stream() = std::wcout;
#endif
		}
		// switch back to default
		//----------------------------------------------------------------------------------------------
		unsigned int dbjsys::fm::prompth_length(unsigned int change_to)
		{
			AutoLocker auto_lock(*prompt_imp::critical_section());
			static unsigned int current_length = 0; // default

			if (0 < change_to)
				current_length = change_to;
			return current_length;
		}

		//----------------------------------------------------------------------------------------------
		std::wostream & dbjsys::fm::prompt(const wchar_t *  p, /* prompt message */
			const wchar_t * promptChar, /* use it if not 0 */
			const wchar_t * fillChar  /* use it if not 0 */
			)
		{
			AutoLocker auto_lock(*prompt_imp::critical_section());
			const int PROMPT_LENGTH = prompth_length();
			prompt_imp::the_stream()
				<< std::endl
				<< std::setw(PROMPT_LENGTH)
				<< std::setfill(0 != fillChar ? fillChar[0] : L' ')
				<< p
				<< std::setw(0);

			if (0 != promptChar) prompt_imp::the_stream() << promptChar;

			prompt_imp::the_stream().flush();
			return prompt_imp::the_stream();
		}

		//---------------------------------------------------------------------------------------------------------
		std::wostream & dbjsys::fm::prompt(const wchar_t * c_, const unsigned int N_)
		{
			AutoLocker auto_lock(*prompt_imp::critical_section());
#ifdef _DEBUG
			if (c_ == 0) return prompt_imp::the_stream();
			if (N_ < 2) return prompt_imp::the_stream(); // for jokers using this
			if (!iswprint(c_[0])) return prompt_imp::the_stream(); // more anti-joker measures
#endif
			return   prompt_imp::the_stream()
				<< std::endl
				<< std::setw(N_ - 1)
				<< std::setfill(c_[0])
				<< c_ /* provoke output */
				<< std::setw(0)
				<< std::setfill(L' ');
		}

		//--------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------
		// this template and instantiations bellow can be used as functors
		// when outputing values to prompt
		//
		template<> DBJFMAPI void val2prompt<std::wstring>(const std::wstring & arg)
		{
			prompt(arg.c_str());
		}
		//
		// instantiations bellow are usable straight-away, eg:
		// std::foreach( a,b, val2prompt<bstr_t> ) ;
		// all of this makes code smaler and faster in presence
		// of template instantiations
#pragma warning ( disable : 4231 )
		//
		DBJFMTXP template DBJFMAPI void val2prompt<bstr_t>(const bstr_t & arg);
		//
		// DBJFMTXP template DBJFMAPI void val2prompt<wchar_t*>( const wchar_t* & arg  ) ;
		//
		// DBJFMTXP template DBJFMAPI void val2prompt<std::wstring>( const std::wstring & arg  ) ;
		// VC9 throws Error C3416, above
		//
		//--------------------------------------------------------------------------------
	}
}
//--------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#endif // _TEST_UTILITIES_H_ 1
//-----------------------------------------------------------------------------
