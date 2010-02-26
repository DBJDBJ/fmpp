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
//  $Author: Dusan $
//  $Date: $
//  $Revision: $
//*****************************************************************************/

#include "../fm.h"
#include "io.h"

//-------------------------------------------------------------------------
namespace dbjsys {
namespace fm {
//-------------------------------------------------------------------------
void mutex::reportOnMutexWaitError ( 
	const DWORD mutexWaitResult, 
	wchar_t * mutexName, 
	std::wostream & os , 
	int sayIfOk ) const
{
	switch ( mutexWaitResult )
	{
		// If the function fails, the return value is WAIT_FAILED. 
		// To get extended error information, call GetLastError. 
	case WAIT_FAILED :
		os << std::endl << mutexName << L" Wait failed ?" 
			<< std::endl << std::flush;
		break ;

	case WAIT_ABANDONED :
		// The specified object is a mutex object that was not released 
		// by the thread that owned the mutex object before 
		// the owning thread terminated. 
		// Ownership of the mutex object is granted to the calling thread, 
		// and the mutex is set to nonsignaled.
		os << std::endl << mutexName << L"Mutex abandoned on wait ?" 
			<< std::endl << std::flush;
		break ;

	case WAIT_OBJECT_0 :
		// The state of the specified object is signaled.
		if ( sayIfOk != 0 )
		{
			os << std::endl << mutexName << L"Wait returned ok"
				<< std::endl << std::flush ;
		}
	break ;
	case WAIT_TIMEOUT :
		// The time-out interval elapsed, and the object’s state is nonsignaled. 
		os << std::endl << mutexName << L"Mutex timout on wait ?"
			<< std::endl << std::flush ;
		break ;
	default :
		break ;
	} ; // 

}

//-------------------------------------------------------------------------
}
}
//-------------------------------------------------------------------------
