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
	class StatefullObject {
	public :
		//
		typedef Error<StatefullObject> Err ;
		//
	// 
		StatefullObject() 
		{
		}
	// 
		virtual ~StatefullObject () 
		{
		}
		// 
	// 
		StatefullObject( const StatefullObject & ) 
		{
			// dbjTHROWERR(L"StatefullObject derivations MUST implement a copy constructor. Also it has to implement a 'deep copy'");
		}
		// the mandatory method for every statefull object
		virtual SREF<StatefullObject> deep_clone ( ) const = 0 
		{
			dbjTHROWERR(L"StatefullObject derivations MUST implement a  deep_clone() in a 'deep copy' fashion.");
		}
	} ;
//--------------------------------------------------------------------------------
} // fm
} // dbj
//--------------------------------------------------------------------------------
