/*
	$Author: DBJ  $
	$Revision: $
	$Date: $
---------------------------------------------------------------------------------------------
Event Logging Functions
The following functions are used with event logging. 
---------------------------------------------------------------------------------------------
Function Description 
BackupEventLog				Saves the specified event log to a backup file. 
ClearEventLog				Clears the specified event log, and optionally saves the current 
							copy of the logfile to a backup file. 
CloseEventLog				Closes a read handle to the specified event log. 
DeregisterEventSource		Closes a write handle to the specified event log. 
GetEventLogInformation		Retrieves information about the specified event log. 
GetNumberOfEventLogRecords	Retrieves the number of records in the specified event log. 
GetOldestEventLogRecord		Retrieves the absolute record number of the oldest record in 
							the specified event log. 
NotifyChangeEventLog		Enables an application to receive notification when an event 
							is written to the specified event logfile. 
OpenBackupEventLog			Opens a handle to a backup event log. 
OpenEventLog				Opens a handle to an event log. 
ReadEventLog				Reads a whole number of entries from the specified event log. 
RegisterEventSource			Retrieves a registered handle to an event log. 
ReportEvent					Writes an entry at the end of the specified event log. 
---------------------------------------------------------------------------------------------
*/

#pragma once

namespace dbjsys {
	namespace fm {

// interface to objectified WIN32 API for event logging -----------
interface IEvLog {

	// dbjMAKE_ERR(IEvLog) ;
	// dbj_ERR(SomeSpecificLogicalError) ;

	typedef Win32Error<IEvLog> W32Err ;

	//-------------------------------------------------------------
	virtual void backupEventLog				
	// Saves the specified event log to a backup file. 
	(
		  HANDLE hEventLog,          // handle to event log
		  LPCWSTR lpBackupFileName   // name of backup file
	) const = 0 ;
	//-------------------------------------------------------------
	virtual  void
	clearEventLog
	// lears the specified event log, and optionally saves the current copy of the logfile to a backup file. 
	( 
		  HANDLE hEventLog,          // handle to event log
		  LPCWSTR lpBackupFileName  =0 // optional -- name of backup file
	) const = 0 ;
	//-------------------------------------------------------------
	virtual void
	closeEventLog				
	// Closes a read handle to the specified event log. 
	( 
			HANDLE hEventLog   // handle to event log
	) const = 0 ;
	//-------------------------------------------------------------
	virtual void
	deregisterEventSource		
	//Closes a write handle to the specified event log. 
	( 
		  HANDLE hEventLog   // handle to event log
	) const = 0 ;
	//-------------------------------------------------------------
	virtual void
	getEventLogInformation		
	//Retrieves information about the specified event log. 
	( 
		  HANDLE  hEventLog,      // handle to event log
		  DWORD   dwInfoLevel,    // information to retrieve
		  LPVOID  lpBuffer,       // buffer for read data
		  DWORD   cbBufSize,      // size of buffer in bytes
		  LPDWORD pcbBytesNeeded  // number of bytes needed
	) const = 0 ;
	//-------------------------------------------------------------
	virtual void
	getNumberOfEventLogRecords	
	//Retrieves the number of records in the specified event log. 
	( 
		  HANDLE hEventLog,        // handle to event log
		  PDWORD NumberOfRecords   // buffer for number of records
	) const = 0 ;
	//-------------------------------------------------------------
	virtual void
	getOldestEventLogRecord		
	//Retrieves the absolute record number of the oldest record in the specified event log. 
	( 
		HANDLE hEventLog,     // handle to event log
		PDWORD OldestRecord   // buffer for number of records
	) const = 0 ;
	//-------------------------------------------------------------
	virtual void
	notifyChangeEventLog		
	//Enables an application to receive notification when an event is written to the specified event logfile. 
	( 
		 HANDLE hEventLog,  // handle to event log
		 HANDLE hEvent      // handle to event object
	) const = 0 ;
	//-------------------------------------------------------------
	virtual void
	openBackupEventLog			
	//Opens a handle to a backup event log. 
	(
		  HANDLE & handle  ,        // result
		  LPCWSTR lpUNCServerName,  // backup file server name
		  LPCWSTR lpFileName        // backup file name
	) const = 0 ;
	//-------------------------------------------------------------
	virtual void
	openEventLog				
	//Opens a handle to an event log. 
	( 
		HANDLE & handle  ,        // result
		LPCWSTR lpUNCServerName,  // server name
		LPCWSTR lpSourceName      // file name
	) const = 0 ;
	//-------------------------------------------------------------
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
	) const = 0 ;
	//-------------------------------------------------------------
	virtual void
	registerEventSource			
	//Retrieves a registered handle to an event log. 
	( 
	    HANDLE & handle  ,        // result
		LPCWSTR lpUNCServerName,  // server name
		LPCWSTR lpSourceName      // source name
	) const = 0 ;
	//-------------------------------------------------------------
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
	) const = 0 ;
	//-------------------------------------------------------------
	// factory method for making instances of implementation
	// of this interface
	
	static dbjsys::fm::SREF<IEvLog> make () ;

	struct Details
	{
		_bstr_t eventLogName ;
		_bstr_t file ;
		DWORD	maxSize ;
		//_bstr_t primaryModule ;
		DWORD	retention ;
		//_bstr_t sources ;
	} ;

	struct SourceDetails
	{
		_bstr_t eventLogName ;
		_bstr_t sourceName ;
		DWORD	numCategory ;
		_bstr_t categoryMessageFile ;
		DWORD	numTypesSupported ;
		_bstr_t	eventMessageFile ;
	} ;

	
	static bool createEventLogSource( const IEvLog::SourceDetails & sourcedetails ) ;
	static bool deleteEventLogSource( const _bstr_t name ) ;
	static bool createEventLog( const IEvLog::Details & details ) ;
	static bool deleteEventLog( const _bstr_t & name ) ;
	static bool doesEventLogExist( const _bstr_t&  name ) ;
	static bool doesEventLogSourceExist( const _bstr_t logname, const _bstr_t & sourcename ) ;
	//-------------------------------------------------------------
} ; // eof IEvLog
	//-------------------------------------------------------------


	// encapsulation of logging file logic
// 
	class LoggingFile
	{
	public:

		struct LogEvent
		{			
			WORD	type ;			// event type
			WORD	category ;		// event category
			DWORD	eventID ;		// event identifier
			_bstr_t message ;		// array of strings to merge
		} ;

		class Writer
		{
		public:
			typedef IEvLog::W32Err W32Err ;

			Writer( const _bstr_t & source_name, const _bstr_t & server_name ) ;
			Writer( const Writer & other ) ;
			Writer & operator=( const Writer & rhs ) ;
			~Writer() ;

			void report( const LogEvent & event ) ;

		private:
			HANDLE evlog_handle_ ;
			_bstr_t server_name_ ;
			_bstr_t source_name_ ;

			dbjsys::fm::SREF<IEvLog> evlog_ ;

			void ready_the_event_log() ;
		} ;


		class Reader
		{
		public:
			typedef IEvLog::W32Err W32Err ;

			Reader( const _bstr_t & source_name, const _bstr_t & server_name ) ;
			Reader( const Reader & other ) ;
			Reader & operator=( const Reader & rhs ) ;
			~Reader() ;

			void read( const LogEvent & event ) ;

		private:
			HANDLE evlog_handle_ ;
			_bstr_t server_name_ ;
			_bstr_t source_name_ ;

			dbjsys::fm::SREF<IEvLog> evlog_ ;

			void ready_the_event_log() ;
		} ;


	public:
	// 
			dbjMAKE_ERR(LoggingFile) ;
			// dbj_ERR(SomeSpecificLogicalError) ;
	private:
		// default constructor has no sense
	// 
		LoggingFile () {}
		//
	
	protected: // implementation ----------------------------------
		// can be made only by offsprings
	// 
		LoggingFile ( const _bstr_t &  source_name, const _bstr_t &  server_name ) :
			log_reader_( new Reader(source_name, server_name) ),
			log_writer_( new Writer(source_name, server_name) ) 
		{
		}

	// 
		SREF<Reader> log_reader_ ;
	// 
		SREF<Writer> log_writer_ ;

	public : // interface -----------------------------------------

	// 
		bool valid() const 
		{ 
			return (this) && (!log_reader_.isNull()) && (!log_writer_.isNull()) ;
		}

	// 
		virtual ~LoggingFile()
		{
			log_reader_ = 0 ;
			log_writer_ = 0 ;
		}

	// 
		void read( const LogEvent & event ) ;
	// 
		void report( const LogEvent & event ) ;


	//-------------------------------------------------------------
	}; // eof LoggingFile
	//-------------------------------------------------------------

	} // fm
} // dbjsys
