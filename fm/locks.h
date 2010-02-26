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
//  $Author: DBJ  $
//  $Date: $
//  $Revision: $
//*****************************************************************************/

#pragma once

// ----------------------------------------------------------------------
namespace dbjsys {
	namespace fm {
// ----------------------------------------------------------------------
		class AutoLocker ;

// objectified CRITIAL_SECTION interface
// initialize on instantiation, delete on destruction
// used to implement different inter-process locking mechanisms
interface CriticalSection
{
		public:
			typedef Error<CriticalSection> Err ;
		private :
	friend class AutoLocker ;

	// CRITICAL_SECTION instance can not be moved or copied!
	CriticalSection( const CriticalSection & ) 
	{
		dbjTHROWERR(L"This should not be called?") ;
	}

	CriticalSection & operator = ( const CriticalSection & ) ;
	// On top of that we will ban making it on the heap!
	// void * operator new ( size_t ) ;
#if ! defined ( _MSC_VER )
	void operator delete ( void * ) ;
#endif
	// including making arrays of this on the heap
	void * operator new []  ( size_t ) ;
	void operator delete [] ( void * , size_t ) ;
protected :
	//-------------------------------------------------
	virtual void take_ () const = 0 ;
	//-------------------------------------------------
	virtual void release_ () const = 0 ;
public :
	//
		CriticalSection () { 
		}
	//
	   virtual ~CriticalSection () {
	   }
} ;

// 
class Win32CriticalSection : public CriticalSection
{
	friend class AutoLocker ;
protected :
	//
	// 
	CRITICAL_SECTION	criticalSectionObject_ ;

	//-------------------------------------------------
	//
	// 
	CRITICAL_SECTION * noconstCSpointer_ () const
	{
		return const_cast<CRITICAL_SECTION *>(&criticalSectionObject_) ;
	}
	//-------------------------------------------------
	// 
	virtual void take_ () const
	{
		::EnterCriticalSection( noconstCSpointer_() );
	}
	//-------------------------------------------------
	// 
	virtual void release_ () const
	{
		::LeaveCriticalSection( noconstCSpointer_() );
	}
public :
	//
	// 
		Win32CriticalSection () 
		{ 
			::InitializeCriticalSection( & criticalSectionObject_ ) ;
		}
	//
	// 
	   ~Win32CriticalSection () 
	   {
		   if ( 0 !=  & criticalSectionObject_ )
		   ::DeleteCriticalSection( & criticalSectionObject_ ) ;
	   }
} ;

// Can you guess what is this for ?
// 
class NOTCriticalSection : public CriticalSection
{
	friend class AutoLocker ;
protected :
	//-------------------------------------------------
	// 
	virtual void take_ () const	{
	}
	// 
	virtual void release_ () const{
	}
public :
	// 
		NOTCriticalSection () { 
		}
	// 
	   ~NOTCriticalSection () {
	   }
} ;

// ----------------------------------------------------------------------
// void f1 ()
// {
//      AutoLocker auto_lock ;
// }
// 
class AutoLocker 
{
	// 
	CriticalSection & crit_sect_ ;
	// 
	AutoLocker ( ) ;
	// 
	AutoLocker ( const AutoLocker & ) ;
	// 
	AutoLocker & operator = ( const AutoLocker & ) ;
public :
	// 
	AutoLocker ( CriticalSection & crit_sect ) 
	: crit_sect_(crit_sect) 	
	{  
		crit_sect_.take_() ;	
	}
	// 
   ~AutoLocker () {
	   crit_sect_.release_() ;
   }
} ;
// ----------------------------------------------------------------------
//
// class ScopeLocker ;
// This class provides a shared locking to all
// of it's clients, in the scope of it's instance.
// Now what that means ? Read on ...
// When used consistently in all the methods of
// client class it will provide instance-wide locking
// Example :
/*

 class X {
		ScopeLocker instance_ ;
	public :
	   X () { }
	  ~X () { }

	void m1 () { Lock l(instance_); }
	void m2 () { Lock l(instance_); }
 } ;
			
*/
// Every of the 'Lock()' calls above lock's the whole instance of X
// that is none of the methods may be entered until the one working
// finishes. 
// This means also that if m1() above calls m2() , or opposite, the
// DEADLOCK will happen. So use this locking method CAREFULY !
// If the class interface is well defined, then just by locking public
// 'interface' methods one should not have problems !
//
class ScopeLocker ;
class Lock ;

// 
class ScopeLocker : protected Win32CriticalSection
{
	friend class Lock ;

public :
	// 
		ScopeLocker () 
		{ 
		}
	// 
	   ~ScopeLocker () 
	   {
	   }
} ;

//------------------------------------------------------------
// the instances of this class activates the instance locker
// 
// 
class Lock {
	// 
	const ScopeLocker & locker_ ;
public :
	// 
	  explicit Lock (const ScopeLocker & lock) 
		   : locker_( lock ) 
	  { 
		   locker_.take_(); 
		   // will not return from here if any previous 
		   // is made and not destructed yet
	  }
	// 
	  ~Lock ( ) 
	  { 
		  if ( & locker_ != 0 )
				locker_.release_(); 
		  // will allow next instance to immediatelly 
		  // return from it's constructor
	  }
} ;

//------------------------------------------------------------
	} ; // namespace fm 
} ; // namespace dbjsys 
//------------------------------------------------------------


