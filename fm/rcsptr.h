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
//---------------------------------------------------------------------------------------
namespace dbjsys {
	namespace fm {
//---------------------------------------------------------------------------------------

// Reference Counting SHARED Reference.
// No copying is done, so you can have a Ref to a virtual base class
// 
template<class T> 
class SREF
{
public:
	SREF( T * body ) throw() ;
	SREF( ) throw()  ;
	SREF( const SREF<T> & other ) throw()  ;
	SREF<T> & operator=( const SREF<T> & other ) ; //throw() ;
	
	// 
    ~SREF()
    { 
        release(); 
    }
	// 
	T * operator->() const throw() 
    { 
		_ASSERT( pointee_ ) ;
        return pointee_; 
    }
	// 
	const T & operator *() const throw() 
    { 
		_ASSERT( pointee_ ) ;
        return *pointee_; 
    }
	// 
	T & operator *() throw() 
    { 
		_ASSERT( pointee_ ) ;
        return *pointee_; 
    }

	// To easy to invoke this without realizing
    //operator T*(void) throw() 
    //{ 
    //    return pointee_; 
    //}
    // This is more explicit
	// 
    T * ptr() throw()
    {
        return pointee_ ;
    }

	/*
	cause problem in STL container
	T * operator &()
	{
		return pointee_ ;
	}*/
	
	// 
	bool isNull() const throw()
    { 
        return pointee_ == 0; 
    }

    T * detach() throw() ;
    bool operator<( const SREF<T> & rhs ) const throw() ;
    bool operator>( const SREF<T> & rhs ) const throw() ;
	// 
	bool operator==( const SREF<T> & rhs ) const throw() ;

private:
	void release() throw() ;
	void addref() throw() ;
	// 
	int * count_;
	// 
	T * pointee_;
};

template<class T>
inline SREF<T>::SREF( T * pointee ) throw()
    : count_( new int(1) ), pointee_( pointee )
{
}

template<class T>
inline SREF<T>::SREF() throw()
    : count_(0), pointee_(0)
{
}

template<class T>
inline SREF<T>::SREF( const SREF<T> & other ) throw()
    : count_( other.count_ ), pointee_( other.pointee_ )
{
	addref();
}

template<class T>
inline SREF<T> & SREF<T>::operator=( const SREF<T> & other ) // throw()
{
    if( this != &other )
	{  
		release() ; // may throw exception
		pointee_ = other.pointee_;
		count_ = other.count_;
		addref() ;
	}
	return *this ;
}

template<class T>
inline void SREF<T>::release() // throw()
{
    if( ! count_ )
        return ;    // ref. counting null !

    if( *count_ > 0 )
        --(*count_) ;   // dec. count

	if( *count_ == 0 )
	{
        if( pointee_ )
        {
            delete pointee_ ;   // destructor may throw exception
            pointee_ = 0;
        }

		delete count_;
		count_ = 0;
	}
}

template<class T>
inline void SREF<T>::addref() throw()
{
    if( ! count_ )
        return ;    // ref. counting null !

	if( *count_ > 0 )
		++(*count_) ;
}

template<class T>
inline T * SREF<T>::detach() throw()
{
	if( ! count_ )
        return 0 ;

    if( *count_ > 0 ) 
        *count_ = -1 ;  // pointee_ will not be deleted!

	return pointee_;
}

template<class T>
inline bool SREF<T>::operator<( const SREF<T> & rhs ) const throw()
{
	return pointee_ < rhs.pointee_;
}

template<class T>
inline bool SREF<T>::operator>( const SREF<T> & rhs ) const throw()
{
	return pointee_ > rhs.pointee_;
}

template<class T>
inline bool SREF<T>::operator==( const SREF<T> & rhs ) const throw()
{
	return rhs.pointee_ == pointee_;
}

//---------------------------------------------------------------------------------------
	} ; // fm
}; // dbjsys
//---------------------------------------------------------------------------------------


