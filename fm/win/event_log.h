// event_log.h: interface for the event_log class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "../fm.h"
//////////////////////////////////////////////////////////////////////
// Required for event log WIN32 API 
//////////////////////////////////////////////////////////////////////
#pragma comment( lib, "advapi32.lib" ) 

//////////////////////////////////////////////////////////////////////
namespace dbjsys {
	namespace fm {
//////////////////////////////////////////////////////////////////////
// 
		class event_log : public IEvLog<event_log>
		{
		public:

			typedef  IEvLog<event_log>		Parent;

			//////////////////////////////////////////////////////////////////////
			event_log::event_log()
			{

			}

			event_log::~event_log()
			{

			}

	//-------------------------------------------------------------
	// 
	virtual void backupEventLog				
	// Saves the specified event log to a backup file. 
	(
		  HANDLE hEventLog,          // handle to event log
		  LPCWSTR lpBackupFileName   // name of backup file
	) const  ;
	//-------------------------------------------------------------
	// 
	virtual  void
	clearEventLog
	// lears the specified event log, and optionally saves the current copy of the logfile to a backup file. 
	( 
		  HANDLE hEventLog,          // handle to event log
		  LPCWSTR lpBackupFileName  =0 // optional -- name of backup file
	) const  ;
	//-------------------------------------------------------------
	// 
	virtual void
	closeEventLog				
	// Closes a read handle to the specified event log. 
	( 
			HANDLE hEventLog   // handle to event log
	) const  ;
	//-------------------------------------------------------------
	// 
	virtual void
	deregisterEventSource		
	//Closes a write handle to the specified event log. 
	( 
		  HANDLE hEventLog   // handle to event log
	) const  ;
	//-------------------------------------------------------------
	// 
	virtual void
	getEventLogInformation		
	//Retrieves information about the specified event log. 
	( 
		  HANDLE  hEventLog,      // handle to event log
		  DWORD   dwInfoLevel,    // information to retrieve
		  LPVOID  lpBuffer,       // buffer for read data
		  DWORD   cbBufSize,      // size of buffer in bytes
		  LPDWORD pcbBytesNeeded  // number of bytes needed
	) const  ;
	//-------------------------------------------------------------
	// 
	virtual void
	getNumberOfEventLogRecords	
	//Retrieves the number of records in the specified event log. 
	( 
		  HANDLE hEventLog,        // handle to event log
		  PDWORD NumberOfRecords   // buffer for number of records
	) const  ;
	//-------------------------------------------------------------
	// 
	virtual void
	getOldestEventLogRecord		
	//Retrieves the absolute record number of the oldest record in the specified event log. 
	( 
		HANDLE hEventLog,     // handle to event log
		PDWORD OldestRecord   // buffer for number of records
	) const  ;
	//-------------------------------------------------------------
	// 
	virtual void
	notifyChangeEventLog		
	//Enables an application to receive notification when an event is written to the specified event logfile. 
	( 
		 HANDLE hEventLog,  // handle to event log
		 HANDLE hEvent      // handle to event object
	) const  ;
	//-------------------------------------------------------------
	// 
	virtual void
	openBackupEventLog			
	//Opens a handle to a backup event log. 
	(
		  HANDLE & handle  ,        // result
		  LPCWSTR lpUNCServerName,  // backup file server name
		  LPCWSTR lpFileName        // backup file name
	) const  ;
	//-------------------------------------------------------------
	// 
	virtual void
	openEventLog				
	//Opens a handle to an event log. 
	( 
        HANDLE & handle  ,        // result
		LPCWSTR lpUNCServerName,  // server name
		LPCWSTR lpSourceName      // file name
	) const  ;
	//-------------------------------------------------------------
	// 
	virtual void
	readEventLog				
	//Reads a whole number of entries from the specified event log. 
	( 
		  HANDLE hEventLog,                // handle to event log
		  DWORD dwReadFlags,               // how to read log
		  DWORD dwRecordOffset,            // offset of first record
		  LPVOID lpBuffer,                 // buffer for read data
		  DWORD nNumberOfBytesToRead,      // bytes to read
		  DWORD *pnBytesRead,              // number of bytes read
		  DWORD *pnMinNumberOfBytesNeeded  // bytes required
	) const  ;
	//-------------------------------------------------------------
	// 
	virtual void
	registerEventSource			
	//Retrieves a registered handle to an event log. 
	( 
	    HANDLE & handle  ,        // result
		LPCWSTR lpUNCServerName,  // server name
		LPCWSTR lpSourceName      // source name
	) const  ;
	//-------------------------------------------------------------
	// 
	virtual void
	reportEvent					
	//Writes an entry at the end of the specified event log. 
	( 
		  HANDLE hEventLog,    // handle to event log
		  WORD wType,          // event type
		  WORD wCategory,      // event category
		  DWORD dwEventID,     // event identifier
		  PSID lpUserSid,      // user security identifier
		  WORD wNumStrings,    // number of strings to merge
		  DWORD dwDataSize,    // size of binary data
		  LPCWSTR *lpStrings,  // array of strings to merge
		  LPVOID lpRawData     // binary data buffer
	) const  ;

		}; // event_log


//////////////////////////////////////////////////////////////////////
///                                                               ////
//////////////////////////////////////////////////////////////////////
		//-------------------------------------------------------------
		// Saves the specified event log to a backup file. 
		void event_log::backupEventLog
			(
			HANDLE hEventLog,          // handle to event log
			LPCWSTR lpBackupFileName   // name of backup file
			) const  {

			dbjTHROWIF(0 == BackupEventLog(hEventLog, lpBackupFileName), W32Err);
		}
		//-------------------------------------------------------------
		// clears the specified event log, and optionally saves the current copy of the logfile to a backup file. 
		void event_log::clearEventLog
			(
			HANDLE hEventLog,          // handle to event log
			LPCWSTR lpBackupFileName   // optional -- name of backup file
			) const  {
			dbjTHROWIF(0 == ClearEventLog(hEventLog, lpBackupFileName), W32Err);
		}
		//-------------------------------------------------------------
		// Closes a read handle to the specified event log. 
		void event_log::closeEventLog
			(
			HANDLE hEventLog   // handle to event log
			) const  {
			dbjTHROWIF(0 == CloseEventLog(hEventLog), W32Err);
		}
		//-------------------------------------------------------------
		//Closes a write handle to the specified event log. 
		void event_log::deregisterEventSource
			(
			HANDLE hEventLog   // handle to event log
			) const  {
			dbjTHROWIF(0 == DeregisterEventSource(hEventLog), W32Err);
		}
		//-------------------------------------------------------------
		//Retrieves information about the specified event log. 
		void event_log::getEventLogInformation
			(
			HANDLE  hEventLog,      // handle to event log
			DWORD   dwInfoLevel,    // information to retrieve
			LPVOID  lpBuffer,       // buffer for read data
			DWORD   cbBufSize,      // size of buffer in bytes
			LPDWORD pcbBytesNeeded  // number of bytes needed
			) const  {
			dbjTHROWIF(0 ==
				::GetEventLogInformation(
				hEventLog, dwInfoLevel, lpBuffer, cbBufSize, pcbBytesNeeded
				), W32Err);
		}
		//-------------------------------------------------------------
		void event_log ::
			getNumberOfEventLogRecords
			//Retrieves the number of records in the specified event log. 
			(
			HANDLE hEventLog,        // handle to event log
			PDWORD NumberOfRecords   // buffer for number of records
			) const  {
			dbjTHROWIF(0 ==
				GetNumberOfEventLogRecords(hEventLog, NumberOfRecords)
				, W32Err);
		}
		//-------------------------------------------------------------
		//Retrieves the absolute record number of the oldest record in the specified event log. 
		void event_log::getOldestEventLogRecord
			(
			HANDLE hEventLog,     // handle to event log
			PDWORD OldestRecord   // buffer for number of records
			) const  {
			dbjTHROWIF(0 ==
				GetOldestEventLogRecord(hEventLog, OldestRecord)
				, W32Err);
		}
		//-------------------------------------------------------------
		//Enables an application to receive notification when an event is written to the specified event logfile. 
		void event_log::notifyChangeEventLog
			(
			HANDLE hEventLog,  // handle to event log
			HANDLE hEvent      // handle to event object
			) const  {
			dbjTHROWIF(0 ==
				NotifyChangeEventLog(hEventLog, hEvent)
				, W32Err);
		}
		//-------------------------------------------------------------
		//Opens a handle to a backup event log. 
		void event_log::openBackupEventLog
			(
			HANDLE & handle,        // result
			LPCWSTR lpUNCServerName,  // backup file server name
			LPCWSTR lpFileName        // backup file name
			) const  {
			handle = OpenBackupEventLog(lpUNCServerName, lpFileName);
			dbjTHROWIF(NULL == handle, W32Err);
		}
		//-------------------------------------------------------------
		//Opens a handle to an event log. 
		void event_log::openEventLog
			(
			HANDLE & handle,        // result
			LPCWSTR lpUNCServerName,  // server name
			LPCWSTR lpSourceName      // file name
			) const  {
			handle = OpenEventLog(lpUNCServerName, lpSourceName);
			dbjTHROWIF(NULL == handle, W32Err);
		}
		//-------------------------------------------------------------
		//Reads a whole number of entries from the specified event log. 
		void event_log::readEventLog
			(
			HANDLE hEventLog,                // handle to event log
			DWORD dwReadFlags,               // how to read log
			DWORD dwRecordOffset,            // offset of first record
			LPVOID lpBuffer,                 // buffer for read data
			DWORD nNumberOfBytesToRead,      // bytes to read
			DWORD *pnBytesRead,              // number of bytes read
			DWORD *pnMinNumberOfBytesNeeded  // bytes required
			) const  {
			dbjTHROWIF(0 ==
				ReadEventLog(hEventLog, dwReadFlags, dwRecordOffset, lpBuffer,
				nNumberOfBytesToRead, pnBytesRead, pnMinNumberOfBytesNeeded)
				, W32Err);
		}
		//-------------------------------------------------------------
		//Retrieves a registered handle to an event log. 
		void event_log::registerEventSource
			(
			HANDLE & handle,        // result
			LPCWSTR lpUNCServerName,  // server name
			LPCWSTR lpSourceName      // source name
			) const  {
			handle = RegisterEventSource(lpUNCServerName, lpSourceName);
			dbjTHROWIF(0 == handle, W32Err);
		}
		//-------------------------------------------------------------
		//Writes an entry at the end of the specified event log. 
		void event_log::reportEvent
			(
			HANDLE hEventLog,    // handle to event log
			WORD wType,          // event type
			WORD wCategory,      // event category
			DWORD dwEventID,     // event identifier
			PSID lpUserSid,      // user security identifier
			WORD wNumStrings,    // number of strings to merge
			DWORD dwDataSize,    // size of binary data
			LPCWSTR *lpStrings,  // array of strings to merge
			LPVOID lpRawData     // binary data buffer
			) const  {
			dbjTHROWIF(0 ==
				ReportEvent(hEventLog, wType, wCategory, dwEventID,
				lpUserSid, wNumStrings, dwDataSize, lpStrings, lpRawData)
				, W32Err);
		}

		//////////////////////////////////////////////////////////////////////
	} //	namespace fm 
} // namespace dbjsys 
//////////////////////////////////////////////////////////////////////