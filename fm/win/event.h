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


namespace dbjsys {
	namespace fm {
		namespace win {
//---------------------------------------------------------------------------------------

// 
class Event  
{
public:
	typedef  Error<Event> Err ;

    enum CREATE_FLAG
    {
        EITHER ,    
        EXISTING,
        NEW,
    } ;

	Event(	bool initialState = false ,					// initial state
			bool manualReset = false ,					// reset type
			const _bstr_t & name = L"" ,  				// object name
            CREATE_FLAG flag = EITHER
			
		) : handle_(0), 
			initState_(initialState),
			name_(name),
			manReset_(manualReset)
	{
		init(flag) ;
	} 

    Event( const Event & e ) :
        handle_(0), initState_(e.initState_), manReset_(e.manReset_), name_(e.name_) 
    {
        handle_ = duplicate_handle( e.handle_ ) ;
    }

    Event & operator = ( const Event & e ) 
    {
        if( this != &e )
        {
            handle_    = duplicate_handle( e.handle_ ) ;
            initState_ = e.initState_ ;
            manReset_  = e.manReset_  ;
            name_      = e.name_ ;
        }

        return *this ;
    }

	// 
	operator HANDLE() const
	{
		return handle_ ;
	}

	// set the event
	// 
	void set()
	{
		if( 0 == ::SetEvent( handle_ ) )
			dbjTHROWERR(dbjsys::fm::doctor::errstring()) ;
	}

	// reset the events
	// 
	void reset()
	{
		if( 0 == ::ResetEvent( handle_ ) )
			dbjTHROWERR(dbjsys::fm::doctor::errstring()) ;
	}

	// pulse the events
	// 
	void pulse()
	{
		if( 0 == ::PulseEvent( handle_ ) )
			dbjTHROWERR(dbjsys::fm::doctor::errstring()) ;
	}

	// 
	~Event()
	{
		if( handle_ )
			::CloseHandle( handle_ ) ;

		handle_ = 0 ;
	}

private:
	// 
	HANDLE handle_ ;
	// 
	bool initState_ ;
	// 
	_bstr_t name_ ;
	// 
	bool manReset_ ;

	// 
	void init( CREATE_FLAG flag ) 
	{
		if( name_.length() < 1 )
			handle_ = ::CreateEvent( NULL, manReset_, initState_, NULL ) ;
		else
			handle_ = ::CreateEvent( NULL, manReset_, initState_, name_ ) ;


        DWORD err = ::GetLastError() ;

        if( ERROR_ALREADY_EXISTS == err )
        {
            if( NEW == flag )
            {
                ::CloseHandle( handle_ ) ;
                dbjTHROWERR(L"Event with this name already exists") ;
            }
        }
        else
        {
            if( EXISTING == flag )
            {
                ::CloseHandle( handle_ ) ;
                dbjTHROWERR(L"No Event with this name") ;
            }
        }

		if( 0 == handle_ )
			dbjTHROWERR(dbjsys::fm::doctor::errstring()) ;
	}

    static HANDLE duplicate_handle( HANDLE handle )
    {
        HANDLE ret_handle ;
        BOOL fSuccess = DuplicateHandle(GetCurrentProcess(), handle,
            GetCurrentProcess(), &ret_handle , 0,  FALSE, DUPLICATE_SAME_ACCESS);
        if( !fSuccess )
            dbjTHROWERR(dbjsys::fm::doctor::errstring()) ;
        return ret_handle ;
    }

	
} ;

} ;	// namespace win
} ; // namespace fm
} ; // namespace dbjsys

