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
#if ! defined(_RCIPTR_H_)
#define _RCIPTR_H_ 1
#pragma once
#include "rcobject.h"
/******************************************************************************
*                  Template Class IREF (from pp. 209-210)                   *
*                                                                             *
* The code for IREF that appeared in the first three printings of More      *
* Effective C++ had bugs.  (For details, see the More Effective C++ errata,   *
* which is available at http://www.aw.com/cp/mec++.html or via anonymous FTP  *
* from ftp.aw.com in the directory cp/mec++.)  The bugs are fixed in the      *
* code below.  Each line of code that was added or modified (compared to what *
* appeared in the first three printings of the book) is flagged.  If you have *
* the fourth or later printings of More Effective C++, you can ignore these   *
* comments, as the corrected code already appears in your book.               *
******************************************************************************/
//---------------------------------------------------------------------------------------
namespace dbjsys {
	namespace fm {
//---------------------------------------------------------------------------------------

template<class T>
class IREF {
public:
  IREF(T* realPtr = 0);
  IREF(const IREF& rhs);
  ~IREF();
  IREF& operator=(const IREF& rhs);
  T* operator->();                           // added
  const T* operator->() const;               // modified
  T& operator*();                            // added
  const T& operator*() const;                // modified

   bool isNull () const { return counter->pointee == 0 ; }

            //	Required by STL (OSPACE bug)
      bool operator <(const IREF<T>& right) const;

            //	Required by STL (OSPACE bug)
      bool operator >(const IREF<T>& right) const;

            //	Required by STL (OSPACE bug)
      bool operator ==(const IREF<T>& right) const;
	  bool operator()(const IREF<T>& right) const;  // added

private:
  struct CountHolder: public rcobject {
    ~CountHolder() { delete pointee; }
    T *pointee;
  };
  
  CountHolder *counter;

  void init();
  void makeCopy();                           // added
};

template<class T>
inline
void IREF<T>::init()                       // modified (the test of count
{                                            // against 0 was unnecessary
                                             // and thus removed)
  if (counter->isShareable() == false) {
    T *oldValue = counter->pointee;          // added 
    counter = new CountHolder;
    counter->pointee = new T(*oldValue);     // modified
  }

  counter->addReference();
}

template<class T>
inline
IREF<T>::IREF(T* realPtr)
: counter(new CountHolder)
{ 
  counter->pointee = realPtr;
  init();
}

template<class T>
inline
IREF<T>::IREF(const IREF<T> & rhs)
: counter(rhs.counter)
{
  init();
}

template<class T>
inline
IREF<T>::~IREF()
{
  counter->removeReference();                // modified
}

template<class T>
inline
IREF<T>& IREF<T>::operator=(const IREF<T> & rhs)
{
  if (counter != rhs.counter) {                   
    counter->removeReference();              // modified
    counter = rhs.counter;
    init();
  }
  
  return *this;
}

template<class T>
inline
T* IREF<T>::operator->()                     // this whole function is added
{
  makeCopy();
  return counter->pointee;
}

template<class T>
inline
const T* IREF<T>::operator->() const         // modified
{
  return counter->pointee;
}

template<class T>
inline
T& IREF<T>::operator*()                      // this whole function is added
{
  makeCopy();
  return *(counter->pointee);
}

template<class T>
inline
const T& IREF<T>::operator*() const          // modified
{
  return *(counter->pointee);
}

template<class T>                              // This whole function is added.
inline
void IREF<T>::makeCopy()                     // Note that there is no need to
{                                              // test to see if counter->pointee
  if (counter->isShared()) {                   // is null, because this function
    T *oldValue = counter->pointee;            // is called only from 
    counter->removeReference();                // dereferencing operators
    counter = new CountHolder;
    counter->pointee = new T(*oldValue);
    counter->addReference();
  }
}

template <class T>
inline bool IREF<T>::operator <(const IREF<T>& right) const
{
	return (counter->pointee < right.counter->pointee);
}

template <class T>
inline bool IREF<T>::operator >(const IREF<T>& right) const
{
	return (counter->pointee > right.counter->pointee);
}

template <class T>
inline bool IREF<T>::operator ==(const IREF<T>& right) const
{
	return (counter->pointee == right.counter->pointee);
}

template <class T>
inline bool IREF<T>::operator()(const IREF<T>& right) const
{
	return (counter->pointee == right.counter->pointee);
}

//---------------------------------------------------------------------------------------
	} ; // fm
}; // dbjsys
//---------------------------------------------------------------------------------------

#endif
