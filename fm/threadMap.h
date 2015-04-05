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
    namespace fm {
//--------------------------------------------------------------------------------
// 
        struct FakeLock 
        {
	// 
            FakeLock( const ScopeLocker & ) {}
	// 
           ~FakeLock(  ) {}
        } ;
//--------------------------------------------------------------------------------
// Most of the time thread_map will be NOT used direclty but through
// tsignleton<> bellow. Therefoe tsingleton<>  will hide it and 
// resolve locking issues, so we shall use the FakeLock by default 
// If this class is used directly and real locking is required than
// provide dbjsys::fm::Lock as the second template argument
template < class T, class LOCK = FakeLock >
class thread_map
{
        public:
                typedef Error< thread_map<T> > Err ;
                typedef std::map< DWORD, T *, std::less<DWORD> > MAP ;
        private:
                MAP threadMap_ ;
                ScopeLocker cs_ ;

				const void is_thread_still_active( const DWORD & threadID_ ) const
				{
					DWORD thread_exit_code = NULL ;
					HANDLE thread_handle = 
						::OpenThread(
							  THREAD_QUERY_INFORMATION,  // access right
							  (BOOL)FALSE,    // handle inheritance option
							  (DWORD)threadID_        // thread identifier
							);
					
					_ASSERT( thread_handle ) ;

					BOOL retval = ::GetExitCodeThread( thread_handle, & thread_exit_code ) ;

					_ASSERT( retval ) ;

					// only in _DEBUG builds, but with a nice message box
					_ASSERT( STILL_ACTIVE == thread_exit_code ) ;
					// only in release builds
					dbjVERIFY(STILL_ACTIVE == thread_exit_code) ;
				}
        public:

			T * firstItem () const
			{
                LOCK lock( cs_ ) ;
                MAP::const_iterator iter_ = threadMap_.begin() ;
                if ( iter_ != threadMap_.end() )
				{
					is_thread_still_active( iter_->first ) ;
                    return iter_->second ;
				}
                return 0 ;   
			}

            T * getItem( ) const
            {
                LOCK lock( cs_ ) ;
                DWORD threadID_ = GetCurrentThreadId() ;
				
				is_thread_still_active( threadID_ ) ;

                MAP::const_iterator iter_ = threadMap_.find( threadID_ ) ;
                if ( iter_ != threadMap_.end() )
                    return iter_->second ;
                return 0 ;   
            }

            void addItem( T & item )
            {
                LOCK lock( cs_ ) ;
                DWORD threadID_ = GetCurrentThreadId() ;
                    MAP::const_iterator iter_ = threadMap_.find( threadID_ ) ;
                    if ( iter_ != threadMap_.end() )
                        return ; // already here for this thread
                // thread id is the key, address of the item is the map item
                threadMap_[ threadID_ ] = &item ;
            }
    //
    // this class is not a singleton
    // therefore for type T we can have more than one map per process
    //
    thread_map() {
            #if defined( _DEBUG )
            clearAll() ;
            #endif
    }

    ~thread_map()
     {
        try {
            clearAll() ;
        } catch ( ... ) {
            #if defined( _DEBUG )
            std::wclog << L"\nUnknown Exception in ~thread_map(), in file: " << __FILE__ << L", at line: " << __LINE__ ;
            #endif
        }
    }
private:
            void clearAll()
            {
                LOCK lock( cs_ ) ;
                // in this implementation we keep adresses of items in the map
                // we do not know how item is made , therefore we can not 
                // now is it safe to delete it explicitly, therefore we
                // do only this:
                threadMap_.erase( threadMap_.begin(), threadMap_.end() ) ;
            }

    thread_map ( const thread_map & ) ;
    thread_map & operator = ( const thread_map & ) ;

}; // thread_map

/**
 * On request
 * make hold and return single instance of type T 
 * for each thread present.
 * Be sure to provide real locking class dbjsys::fm::Lock when
 * locking is required, by default FakeLock is used!
 *
 * @stereotype MixInClass
 * @author DBJ
 * @version 1
 * @since 21112000
 */
// 
template < class T, class LOCK > 
class tBaseSingleton 
{
protected:
	typedef std::list<T>  itemlist_type ;
	// 
    static ScopeLocker & cs()
    {
        static ScopeLocker cs_ ;
        return cs_ ;
    }

    // all instances of this class share one and the same
    // thread map
    typedef dbjsys::fm::thread_map<T, LOCK> TMap ;
    //
	// 
    static TMap & theTMap () 
    {
        static TMap theTMap_ ; 
        return theTMap_ ;
    }
    // thread map contains adresses of items made and kept in here
	// 
    static itemlist_type & items ()
    {
        static itemlist_type * itemsPtr = 0 ;
        if( 0 == itemsPtr )
        {
            static itemlist_type items_ ;
            itemsPtr = &items_ ;
        }

        return *itemsPtr ;
    }
    // if we forget to clear them items will be cleared on process exit
public:
    // explicit clear 
	// 
    static const void clear ()
    {
       LOCK lock( cs() ) ;
            items().clear() ;
    }

	// 
    virtual ~tBaseSingleton ()
    {
       LOCK lock( cs() ) ;
            try {
				clear() ;
            }
        catch ( ... ) {
            #if defined( _DEBUG )
            std::wclog << L"\nUnknown Exception in ~tsingleton(): " << __FILE__ << "@" << __LINE__ ;
            #endif
        }
    }
};	// tBaseSingleton

template < class T, class LOCK = FakeLock > 
class tStatelessSingleton : public tBaseSingleton<T, LOCK>
{
public:
    //
    static T & instance()
    {
        LOCK lock( cs() ) ;

        T * item = theTMap().getItem() ;
        if ( item == 0 )
        {
            items().push_back( T() ) ;	// make a new instance and store it
            theTMap().addItem( items().back() ) ;
            item = theTMap().getItem() ;
        }
        return  * item ;
    }
} ;  // tStatelessSingleton

template < class T, class LOCK = FakeLock > 
class tStatefullSingleton : public tBaseSingleton<T, LOCK>
{
public:
    //
    static T & instance()
    {
        LOCK lock( cs() ) ;
		//
		// are there items created already ?

        T * item = theTMap().getItem() ;
        if ( item == 0 )
        {
		    T * first_item = theTMap().firstItem() ; // 0 if no items

		    // if this is not the first instance we copy the state from the 
		    // first instance
			if ( first_item != 0 ) {
				// store the clone of the first instance of T made
				items().push_back( dynamic_cast<T&>(* first_item->deep_clone()) ) ; 
			}
			else {
				// make the first item
				items().push_back( T() ) ; 
			}

            theTMap().addItem( items().back() ) ;

            item = theTMap().getItem() ;
        }

		return * item ;
    }
} ;  // tStatefullSingleton

//--------------------------------------------------------------------------------
#if defined ( testing_thread_map )
//--------------------------------------------------------------------------------
// Used for testing the tStatelessSingleton object.
//--------------------------------------------------------------------------------

// thread_map & tsingleton test
// 
class StateLessInnocent  
{
	// 
    const wchar_t id ;
	// 
    StateLessInnocent () : id(L'X') {}
public:
    typedef tStatelessSingleton< StateLessInnocent > Single ;
    // we need friendship if we want to hide
    // ctor in the user class
    friend Single ;
	// 
    StateLessInnocent & operator = ( const StateLessInnocent & ) { return *this ; }
} ; // StateLessInnocent

//
inline void test_stateless_thread_map() {
    StateLessInnocent & thisThreadInstance = StateLessInnocent::Single::instance() ;
}

//-----------------------------------------------------------------------
#include <process.h>    

//--------------------------------------------------------------------------------
// Used for testing the tStatefullSingleton object.
// USE CASE: 
//		1. Request an instance of StateFullInnocent from tStatefullSingleton and set the state.
//		2. Create a new thread		
//		2. Request an instance of StateFullInnocent from the new thread
//--------------------------------------------------------------------------------
// 
class StateFullInnocent : public StatefullObject
{
	// 
	wchar_t * data_ ;
public:
	typedef dbjsys::fm::Error<StateFullInnocent> Err ;
    typedef tStatefullSingleton< StateFullInnocent > Single ;
    // we need friendship if we want to hide
    // ctor in the user class
    friend Single ;

	// 
	void set_data( const wchar_t * const new_data) { 

		_ASSERT( new_data ) ;
		
		if ( data_ != 0 ) 
			free( data_ );

		data_ = _wcsdup( new_data ) ;

	}

	// 
	const wchar_t * const  get_data( ) const { 
		return data_ ; 
	}


	// 
	StateFullInnocent() : data_(0)
	{}
	//
	// 
	virtual SREF<StatefullObject> deep_clone ( ) const 
	{
		StateFullInnocent * result = new StateFullInnocent() ;
		result->set_data( this->get_data() ) ;
		return result ;
	}

	// 
    StateFullInnocent & operator = ( const StateFullInnocent & ) { return *this ; }
} ;  // StateFullInnocent

//--------------------------------------------------------------------------------
static long finished = 0 ;

inline void getAnotherInstnace( void * )
{
	StateFullInnocent & thisThreadInstance = StateFullInnocent::Single::instance() ;
	::Sleep(0) ;
	::InterlockedIncrement(&finished)  ;
}

inline void test_statefull_thread_map() 
{
    StateFullInnocent & thisThreadInstance = StateFullInnocent::Single::instance() ;
	thisThreadInstance.set_data( L"StateFullInnocent" ) ;
	_beginthread( getAnotherInstnace, 0, NULL ) ;
	while( ! finished )
	{
        /* Wait one second between loops. */
        ::Sleep( 1000L );
	}
}

//--------------------------------------------------------------------------------
#endif // testing_thread_map
//--------------------------------------------------------------------------------
} //    namespace fm 
} // namespace dbjsys 
//--------------------------------------------------------------------------------

