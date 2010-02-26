// event_log.h: interface for the event_log class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

//////////////////////////////////////////////////////////////////////
namespace dbjsys {
	namespace fm {
//////////////////////////////////////////////////////////////////////
// 
		class event_log : public IEvLog
		{
		public:
	// 
			event_log();
	// 
			~event_log();
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

		};
//////////////////////////////////////////////////////////////////////
	} // fm
} // dbjsys
//////////////////////////////////////////////////////////////////////
