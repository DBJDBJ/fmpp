//*****************************************************************************/
//
//                  
//
//                 Copyright (c)  2000 - 2010 by Dusan B. Jovanovic (dbj@dbj.org) 
//                          All Rights Reserved
//
//        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Dusan B. Jovanovic (dbj@dbj.org)
//
//        The copyright notice above does not evidence any
//        actual or intended publication of such source code.
//
//  $Author: Dusan $
//  $Date: $
//  $Revision: $
//*****************************************************************************/
//
// 

#pragma once


//------------------------------------------------------------------------------
namespace dbjsys {
    namespace fm {
/*******************************************************************************
WORK IN PROGRESS BEGIN
//------------------------------------------------------------------------------
//
        interface ClusterItem ;
interface dbjNOVTABLE ControlledInstance {
        static ClusterItem * make () { return 0; }
        static void destroy  ( ClusterItem * ) {  }
} ;
//------------------------------------------------------------------------------
template< class T>
class HeapCitizen : public ControlledInstance
{
    public :
        typedef HeapCitizen<T>  maker_type ;

        static ClusterItem * make () { 
            return ::new (malloc(sizeof(T))) T(); 
        }
        static void destroy  ( ClusterItem * tp ) { 
            if ( tp != 0 ) free(tp); tp = 0 ; 
        }
    protected:
    // here we can hide new() and delete() operators!
    // in case T inherits from this template 
        void * operator new ( size_t ) ;
        void * operator new [] ( size_t ) ;
} ;
WORK IN PROGRESS BEGIN
*******************************************************************************/
//------------------------------------------------------------------------------
template< class T> interface Identifiable {
    static GUID & get_guid () { 
		static GUID this_guid_ ;
		static HRESULT hr_ = ::CoCreateGuid( & this_guid_ )  ;
		_ASSERT( hr_ == S_OK ) ;
		return this_guid_ ; 
	}
} ;

//------------------------------------------------------------------------------
    } // namespace dbjsys 
} // namespace fm 
//------------------------------------------------------------------------------
