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
/*  
    Thread local storage (TLS) enables multiple threads of the same process 
    to use an index allocated by the TlsAlloc function to store and retrieve 
    a value that is local to the thread. When each thread starts, it can allocate
    a block of dynamic memory and store a pointer to this memory 
    by using the *same* TLS index, encapsulated in the TLSlot class bellow. 
    The TLS index may be used by the common function to access 
    the data local to the calling thread. 
*/
//*****************************************************************************/

#pragma once

namespace dbjsys {
    namespace fm {
        //
        // one TLS entry is handled by instances of this class
        // one entry is represented with one TLS index
        // In W2K one process can allocate 1088 TLS indexes 
        // In NT4 one process can allocate 64 TLS indexes
        //
// 
        class TLSlot {
	// 
            const DWORD tls_index_ ; 
            //instance of this class will be called from
            //multiple threads
	// 
            dbjsys::fm::ScopeLocker crit_sec_ ;

        public:
            typedef Error<TLSlot> Err ;
            // If not given by caller 
            // allocate an TLS index in ctor 
	// 
            explicit TLSlot( 
                    DWORD new_tls_index = TlsAlloc() 
                  )
                : tls_index_(new_tls_index)
            {
                dbjVERIFY ( tls_index_ > 0 ) ;
            }
            // Release a TLS index in dtor 
	// 
            ~TLSlot ()
            {
                try {
                    ::TlsFree( tls_index_ ) ;
                } catch ( ...) {
                }
            }

	// 
            void set ( LPVOID data_ ) const
            {
                dbjsys::fm::Lock  lock( crit_sec_ ) ;
                dbjVERIFY( this ) ;
                dbjVERIFY ( ::TlsSetValue(tls_index_, (LPVOID)data_)) ;
            }
            // NOTE: if get is called before set we will return NULL !
	// 
            LPVOID get () const
            {
                dbjsys::fm::Lock  lock( crit_sec_ ) ;
                dbjVERIFY( this ) ;
               LPVOID lpvData = ::TlsGetValue(tls_index_); 
                    dbjVERIFY (GetLastError() == 0) ;
               return lpvData ;
            }
        } ;
//---------------------------------------------------------------------------
// instances of this template are used to have type safe TLSlot
// 
        template< typename T >
            class TypedTLSlot 
        {
            //instance of this class will be called from
            //multiple threads
	// 
            dbjsys::fm::ScopeLocker crit_sec_ ;
            //
	// 
            const TLSlot & the_slot() const
            {   
                // shared slot instance
				static dbjsys::fm::SREF<TLSlot> the_slot_ ;
				if( the_slot_.isNull() )
					the_slot_ = new TLSlot() ;
                
                return *the_slot_ ;
            }
            // throws exception on bad_cast
	// 
            const void set ( T * t_ ) const
            {
                 the_slot().set( reinterpret_cast<LPVOID>(t_) ) ;
            }
            // throws exception on bad_cast
	// 
            T * get () const 
            {
                return reinterpret_cast<T*>(the_slot().get()) ;
            }
        public:
            typedef Error<TypedTLSlot> Err ;
            //
            typedef T val_type ;
			//
	// 
			T * initial_pointer ;

			// default ctor 
	// 
            TypedTLSlot ( ) 
			{
				initial_pointer = new T() ;
				set( initial_pointer ) ;
			}
            // 
			// through ctor we have to give the value with which to 
			// intialize the TLS!
	// 
            TypedTLSlot ( const T & initial_value ) 
				: initial_pointer(0)
			{
				initial_pointer = new T(initial_value) ;
				set( initial_pointer ) ;
			}
			//
	// 
			~TypedTLSlot ( ) 
			{
				if ( initial_pointer )
				{
					delete  initial_pointer ;
					initial_pointer = 0 ;
					set( initial_pointer ) ;
				}
			}

            // using this user can do:
            // TypedTLSlot<long> longs_tls ;
            // long * lp = longs_tls ;
            operator T * () const 
            { 
                    dbjsys::fm::Lock  lock( crit_sec_ ) ;
                    return get(); 
            }
            //
            // using this users can do:
            // longs_tls = lp ;
	// 
            TypedTLSlot &  operator = ( const T * other )
            {
                    dbjsys::fm::Lock  lock( crit_sec_ ) ;
                    set( const_cast<T*>(other) ) ;
                    return *this ;
            }
			//
			// friend TypedTLSlot &  operator = (TypedTLSlot & tls_, const T * other );
            // We deliberately do not want users to do:
            // longs_tls = 1L ;
            // because TLS stores pointers!
            // So we will strick to pointers to T 
            // so that users don't get carried away.
            //-------------------------------------------------------------
            // But nothing stops users to do:
            // long l = *longs_tls ; // what if NULL pointer ?
            // To throw exception if '*' is attempted on null pointer
            // we provide this operator with a trap inside
	// 
            T & operator * () const
            {
                dbjsys::fm::Lock  lock( crit_sec_ ) ;
                T * tp = get() ;

				_ASSERT( tp ) ;
                return *tp ;
            }
        } ;
//---------------------------------------------------------------------------
// Testing
#if defined ( dbjsys_fm_tlsot_testing )
//---------------------------------------------------------------------------
static const long THREADCOUNT = 4L ; 
static const long SET_GET_COUNT = 32L ; 
//---------------------------------------------------------------------------
// Make tls_ object. 
static dbjsys::fm::SREF<dbjsys::fm::TLSlot> tls_ ;
 
//---------------------------------------------------------------------------
inline VOID CommonFunc(VOID) 
{ 
   WCHAR * lpvData = NULL ; 
 
// Retrieve a data pointer for the current thread. 
 
   lpvData = (WCHAR*)tls_->get() ;
 
   std::wclog   << L"in CommonFunc(): thread " << GetCurrentThreadId() 
                << L", TLS data: " << lpvData ; 
} 
 
//---------------------------------------------------------------------------
inline DWORD WINAPI ThreadFunc(VOID) 
{ 
    long loop_counter = SET_GET_COUNT ;
    while( loop_counter -- )
    {
   WCHAR lpvData[BUFSIZ] = L"" ; 
       wsprintfW( lpvData, L"thread %d", GetCurrentThreadId() ); 
            tls_->set((LPVOID)lpvData) ; 
                CommonFunc(); 
        Sleep(0); 
    }
   return 0; 
} 
//---------------------------------------------------------------------------- 
inline const void tls_test(VOID) 
{ 
   DWORD IDThread; 
   HANDLE hThread[THREADCOUNT]; 
   int i; 
 
   tls_ = new dbjsys::fm::TLSlot() ;
// Create multiple threads. 
 
   for (i = 0; i < THREADCOUNT; i++) 
   { 
       hThread[i] = ::CreateThread(NULL, // no security attributes 
         0,                           // use default stack size 
         (LPTHREAD_START_ROUTINE) ThreadFunc, // thread function 
         NULL,                    // no thread function argument 
         0,                       // use default creation flags 
         &IDThread);              // returns thread identifier 
   } 
       ::WaitForMultipleObjects(THREADCOUNT, hThread, TRUE, INFINITE); 
} 
//---------------------------------------------------------------------------
#endif // dbjsys_fm_tlsot_testing 
//---------------------------------------------------------------------------
    } // fm
}  // dbjsys
//---------------------------------------------------------------------------
