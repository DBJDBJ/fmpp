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
#if ! defined( _RCPTR_H_)
#define _RCPTR_H_ 1
#pragma once
//---------------------------------------------------------------------------------------
namespace dbjsys {
	namespace fm {
//---------------------------------------------------------------------------------------
/******************************************************************************
*                 Template Class REF (from pp. 203, 206)                    *
******************************************************************************/
// 
template<class T>                      // template class for smart
class REF {                          // pointers-to-T objects; T
public:                                // must support the rcobject interface
  REF(T* realPtr = 0);
  REF(const REF& rhs);
  ~REF();
  REF& operator=(const REF& rhs);                     
  T* operator->() const;
  T& operator*() const;

	// 
  bool operator ! () const { return pointee == 0 ; }
  
	// 
  bool isNull () const { return pointee == 0 ; }

            //	Required by STL (OSPACE bug)
      bool operator <(const REF<T>& right) const;

            //	Required by STL (OSPACE bug)
      bool operator >(const REF<T>& right) const;

            //	Required by STL (OSPACE bug)
      bool operator ==(const REF<T>& right) const;

private:
	// 
  T *pointee;
  void init();
};

template<class T>
inline
void REF<T>::init()
{
  if( pointee ) {
  
  	if (pointee->isShareable() == false) {
    	pointee = new T(*pointee);
  	}
  
  	pointee->addReference();
  }

}

template<class T>
inline
REF<T>::REF(T* realPtr)
: pointee(realPtr)
{
  init();
}

template<class T>
inline
REF<T>::REF(const REF<T>& rhs)
: pointee(rhs.pointee)
{ 
  init();
}

template<class T>
inline
REF<T>::~REF()
{
  if (pointee) pointee->removeReference();
}

template<class T>
inline
REF<T>& REF<T>::operator=(const REF<T> & rhs)
{
  if (pointee != rhs.pointee) {                   
    if (pointee) pointee->removeReference();                

    pointee = rhs.pointee;
    init(); 
  }
  
  return *this;
}

template<class T>
inline
T* REF<T>::operator->() const 
{
  return pointee;
}

template<class T>
inline
T& REF<T>::operator*() const
{
  return *pointee;
}

template <class T>
inline bool REF<T>::operator <(const REF<T>& right) const
{
	return (pointee < right.pointee);
}

template <class T>
inline bool REF<T>::operator >(const REF<T>& right) const
{
	return (pointee > right.pointee);
}

template <class T>
inline bool REF<T>::operator ==(const REF<T>& right) const
{
	return (pointee == right.pointee);
}
//---------------------------------------------------------------------------------------
	} ; // fm
}; // dbjsys
//---------------------------------------------------------------------------------------


#endif
