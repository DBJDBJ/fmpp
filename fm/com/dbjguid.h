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
//
// 

#pragma once


//------------------------------------------------------------------------------
namespace dbjsys {
    namespace fm {
//------------------------------------------------------------------------------
		// Faster (but makes code fatter) inline version...use sparingly
		__forceinline
		bool isEqualGUID( const GUID & rguid1, const GUID & rguid2)
		{
		   return (
			  ((unsigned long *) &rguid1)[0] == ((unsigned long *) &rguid2)[0] &&
			  ((unsigned long *) &rguid1)[1] == ((unsigned long *) &rguid2)[1] &&
			  ((unsigned long *) &rguid1)[2] == ((unsigned long *) &rguid2)[2] &&
			  ((unsigned long *) &rguid1)[3] == ((unsigned long *) &rguid2)[3]);
		}

// 
		struct EqualGUID : public std::binary_function<GUID, GUID, bool> 
		{
	// 
			bool operator()(const GUID & x, const GUID & y) const
			{
				return isEqualGUID(x,y) ;
			}
		} ;

// 
		struct LessGUID : public std::less<GUID>
		{
	// 
			bool operator()(const GUID & rguid1, const GUID & rguid2) const
			{
		   return (
			  ((unsigned long *) &rguid1)[0] < ((unsigned long *) &rguid2)[0] &&
			  ((unsigned long *) &rguid1)[1] < ((unsigned long *) &rguid2)[1] &&
			  ((unsigned long *) &rguid1)[2] < ((unsigned long *) &rguid2)[2] &&
			  ((unsigned long *) &rguid1)[3] < ((unsigned long *) &rguid2)[3]);
			}
		} ;
//------------------------------------------------------------------------------
} //    namespace fm 
} // namespace dbjsys 
//------------------------------------------------------------------------------
