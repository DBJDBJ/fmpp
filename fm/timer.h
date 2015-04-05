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

#if ! defined ( _dbjsys_fm_timer_H_ )
#define _dbjsys_fm_timer_H_ 1
#pragma once
#include <time.h>
#include <sys/types.h> 
#include <sys/timeb.h>
#include <stdio.h>
//---------------------------------------------------------------------------------------
namespace dbjsys {
	namespace fm {
//---------------------------------------------------------------------------------------
//
//
// 
class timer
{
	// forbiden operations
	// 
	timer ( const timer & ) ;
	// 
	timer & operator = ( const timer & ) ;
public:
	// 
	timer( );
	// 
	timer( const _bstr_t & );
	// 
	~timer();

	// 
	const _bstr_t elapsed () const 
	{
		return const_cast<timer*>(this)->elapsed_() ;
	}
private:

	// 
	const _bstr_t & elapsed_ () ;
	// 
	const struct _timeb GetTimeElapsed();
	// 
	_bstr_t prompt_;
	// 
    struct _timeb timebuffer;
	
	// 
	friend void ss_timer_display ( const _bstr_t & ) ;
	// clients provide the implementation

	enum { timebuflen_ = 64 } ;
	// 
	_bstr_t timebuf_ ;
};

//-----------------------------------------------------------------------
//
//
inline timer::timer( ) 
: prompt_(" ")
{
	//for ( register int j = 0 ; j < timebuflen_ ; timebuf_[j++] = 0 )
	//{}
	_ASSERT(_ftime64_s( &timebuffer ));
};

//-----------------------------------------------------------------------
//
//
inline timer::timer( const _bstr_t & data_ ) 
: prompt_( data_ )
{
	//for ( register int j = 0 ; j < timebuflen_ ; timebuf_[j++] = 0 )
	//{}
	_ASSERT(_ftime64_s(&timebuffer));
};


//-----------------------------------------------------------------------
//
//
inline timer::~timer()
{
  try {
	_bstr_t timebuf( elapsed_() ) ;
	if ( wcsstr( static_cast<const wchar_t *>(prompt_), L"%s" ) )
	// user has given prompt with a '%s' to place the time inside it
	{
		wchar_t buff[ BUFSIZ * 4 ] = L""; 
		swprintf(buff, BUFSIZ * 4 , prompt_, timebuf);
			ss_timer_display( buff ) ;
	}
	else // just append the time to the prompt
	{
			prompt_ += timebuf ;
			ss_timer_display( prompt_ ) ;
	}
  }
  catch (...)
  {
#ifdef _DEBUG
      std::wcerr << std::endl 
          << L"Exception cought in timer::~timer() ?!" << std::flush ;
#endif
  }
};

//-----------------------------------------------------------------------
//
//
inline const _bstr_t & timer::elapsed_ ()  
{
	struct _timeb newtimebuffer = GetTimeElapsed();

	struct tm *newtime;

	_ASSERT( localtime_s(newtime, &newtimebuffer.time));

	_bstr_t buff(timebuflen_ * sizeof(wchar_t));

	swprintf( buff, buff.length(), L"%02d:%02d:%02d:%-3d",
		              newtime->tm_hour, 
					  newtime->tm_min, 
					  newtime->tm_sec, 
					  newtimebuffer.millitm 
		);

	timebuf_ = buff ;
	return timebuf_ ;
}
//-----------------------------------------------------------------------
//
//
inline const  _timeb timer::GetTimeElapsed()
{
   struct _timeb newtimebuffer;
	_ASSERT(_ftime64_s( &newtimebuffer ));

	if ( newtimebuffer.millitm >= timebuffer.millitm )
	{
		newtimebuffer.millitm -= timebuffer.millitm;
	}
	else
	{
		newtimebuffer.time -= 1;
		newtimebuffer.millitm += 1000 - timebuffer.millitm;
	}

	newtimebuffer.time -= timebuffer.time;

	return newtimebuffer;

};
//---------------------------------------------------------------------------------------
	} ; // fm
}; // dbjsys
//---------------------------------------------------------------------------------------
#endif // _dbjsys_fm_timer_H_ 1
