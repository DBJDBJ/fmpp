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
/*
	A simple mutex wrap-up. Uses windows.h types and structures in 
	its interface definition.
*/
//--------------------------------------------------------------------------------
namespace dbjsys {
	namespace fm {
		//--------------------------------------------------------------------------------
		// 
		class mutex {

			// 
			LPSECURITY_ATTRIBUTES	mutexAttributes_;	// pointer to security attributes 
			// 
			BOOL					initialOwner_;	// flag for initial ownership 
			// 
			_bstr_t	                name_;	// pointer to mutex-object name 

			// 
			HANDLE					mutex_;

			// 
			HANDLE create_mutex_()
			{
				LPCTSTR the_name_ = name_;

				if (the_name_)
					mutex_ = CreateMutex(mutexAttributes_, initialOwner_, the_name_);
				else
					mutex_ = NULL;

				return mutex_;
			}

			// 
			void release_mutex_()
			{
				if (NULL != mutex_)
				{
					::ReleaseMutex(mutex_); // as soon as possible
					::CloseHandle(mutex_);
					mutex_ = NULL;
				}

			}


		public:

			// TODO this should ne Win32Error<> me thinks ?
			typedef Win32Error<mutex> Err;
			// Constructor creates mutex. Create different ctors for different ways 
			// of calling CreateMutex()
			// 
			mutex(const wchar_t * n_)
				: mutexAttributes_(NULL), initialOwner_(FALSE), name_(n_), mutex_(NULL)
			{
				HANDLE rtn = create_mutex_();
				_ASSERTE(NULL != rtn);
			}

			// Dtor calls ReleaseMutex()
			// this is very important in presence of exceptions
			// 
			~mutex()
			{
				try {
					release_mutex_();
				}
				catch (...)	{
					_ASSERTE(!"WARNING: exception caught in ~mutex () ?!");
				}
			}

			// with following methods we use the mutex
			// Add more methods for more functionality.
			// 
			void infiniteWait() const
			{
				_ASSERTE(NULL != mutex_);
				int waitResult = ::WaitForSingleObject(mutex_, INFINITE);

				// we choose to ignore mutexWaitResult, we will only report 
				// on eventual error
				if (WAIT_OBJECT_0 != waitResult)
					reportOnMutexWaitError(waitResult, name_);
			}

			// 
			DWORD exactWait(DWORD milliSeconds_) const
			{
				_ASSERTE(NULL != mutex_);
				int waitResult = ::WaitForSingleObject(mutex_, milliSeconds_);
				// we will report on eventual error
				if (WAIT_OBJECT_0 != waitResult)
					reportOnMutexWaitError(waitResult, name_);

				return waitResult;
			}

		private:
			//
			// use this inline to report 
			//
			/*
			TODO
			this of course is bollocks ...err log should be used
			*/
			// 
			void reportOnMutexWaitError(
				const DWORD mutexWaitResult,
				wchar_t * mutexName,
				std::wostream & os = std::wclog,
				int sayIfOk = 0) const
			{
				switch (mutexWaitResult)
				{
					// If the function fails, the return value is WAIT_FAILED. 
					// To get extended error information, call GetLastError. 
				case WAIT_FAILED:
					os << std::endl << mutexName << L" Wait failed ?"
						<< std::endl << std::flush;
					break;

				case WAIT_ABANDONED:
					// The specified object is a mutex object that was not released 
					// by the thread that owned the mutex object before 
					// the owning thread terminated. 
					// Ownership of the mutex object is granted to the calling thread, 
					// and the mutex is set to nonsignaled.
					os << std::endl << mutexName << L"Mutex abandoned on wait ?"
						<< std::endl << std::flush;
					break;

				case WAIT_OBJECT_0:
					// The state of the specified object is signaled.
					if (sayIfOk != 0)
					{
						os << std::endl << mutexName << L"Wait returned ok"
							<< std::endl << std::flush;
					}
					break;
				case WAIT_TIMEOUT:
					// The time-out interval elapsed, and the object’s state is nonsignaled. 
					os << std::endl << mutexName << L"Mutex timeout on wait ?"
						<< std::endl << std::flush;
					break;
				default:
					break;
				}; // 

			}

			//-------------------------------------------------------
			// Process which calls this function first will grab the
			// static instanc eof the mutex inside it.
			// Every other process calling it with the same mutex name
			// will not be able to grab it and function will return 1
			// as a signal that mutex is already taken.
			//
			/*
			inline bool isInstanceRunning( const wchar_t * instanceTag )
			{
			static const int ONE_SECOND = 1000;
			// may throw exception, unlocks on dtor call
			static mutex	monimutex( instanceTag ) ;
			DWORD		    mutexWaitResult = monimutex.exactWait(ONE_SECOND) ;
			if( WAIT_TIMEOUT == mutexWaitResult )
			return 1;
			_ASSERTE( WAIT_OBJECT_0 == mutexWaitResult );
			return 0;
			}
			*/
		};
		//--------------------------------------------------------------------------------
	} // fm
} // dbjsys