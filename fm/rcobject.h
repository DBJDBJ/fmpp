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
#ifndef _BS_RCOBJECT_H
#define _BS_RCOBJECT_H 1
#pragma once
//---------------------------------------------------------------------------------------
namespace dbjsys {
	namespace fm {
//---------------------------------------------------------------------------------------

/******************************************************************************
*                       Class rcobject (from pp. 204-205)                     *
******************************************************************************/
// 
class rcobject {                       // base class for reference-
public:                                // counted objects
	// 
  void addReference();
	// 
  void removeReference();
	// 
  void markUnshareable();
	// 
  bool isShareable() const;
	// 
  bool isShared() const;

protected:
	// 
  rcobject();
	// 
  rcobject(const rcobject& rhs);
	// 
  rcobject& operator=(const rcobject& rhs);
	// 
  virtual ~rcobject();

private:
	// 
  int refCount;
	// 
  bool shareable;
};

inline
rcobject::rcobject()
: refCount(0), shareable(true) {}

inline
rcobject::rcobject(const rcobject&)
: refCount(0), shareable(true) {}

inline
rcobject& rcobject::operator=(const rcobject&)
{
  return *this;
}  

inline
rcobject::~rcobject() {}

inline
void rcobject::addReference() 
{
  ++refCount;
}

inline
void rcobject::removeReference()
{
  if (--refCount == 0) delete this;
}

inline
void rcobject::markUnshareable()
{
  shareable = false;
}

inline
bool rcobject::isShareable() const
{
  return shareable;
}

inline
bool rcobject::isShared() const
{
  return refCount > 1;
}  

//---------------------------------------------------------------------------------------
	} ; // fm
}; // dbjsys
//---------------------------------------------------------------------------------------

#endif // RCOBJECT_H
