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

#include "../fm.h"

namespace dbjsys {
	namespace fm {

		static _bstr_t bstr_file = __FILE__;
		static wchar_t* EVENTLOG_REG_LOCATION = L"SYSTEM\\CurrentControlSet\\Services\\EventLog";
		static wchar_t* EVENTLOG_FILEPATH = L"%SystemRoot%\\system32\\config";

// interface to objectified WIN32 API for event logging -----------
template<class T> class IEvLog {

public:
	typedef T						Child;
	typedef IEvLog<T>				MyType;
	typedef Win32Error<MyType>		Err;
	typedef SREF<MyType>			Shared_Ref_Type;


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

	
	/*
	now we have what we need to implement the last part of the IEvLog<T>
	*/
	bool createEventLogSource(const SourceDetails & sourcedetails)
	{
		using namespace dbjsys::fm;
		using namespace dbjsys::fm::bstrex;


		//if( doesEventLogSourceExist( sourcedetails.eventLogName, sourcedetails.sourceName ) )
		//    return true ;


		RegKey hk;


		// Add your source name as a subkey under the EventLog 
		// key in the EventLog service portion of the registry. 

		_bstr_t fullName = EVENTLOG_REG_LOCATION;
		fullName += dbjsys::glob::backSlash();
		fullName += sourcedetails.eventLogName;
		fullName += dbjsys::glob::backSlash();
		fullName += sourcedetails.sourceName;


		if (RegCreateKeyW(
			HKEY_LOCAL_MACHINE,
			fullName,
			&hk))
		{
			// print out error msg to cerr
			prompt(bstr_file) << L" " << __LINE__ << " " <<
				doctor::errstring();
			return false;
		}

		const BYTE * data = 0;
		DWORD datalength = 0;

		// Add the Event ID message-file name to the subkey.  
		data = (const BYTE*)static_cast<const wchar_t*>(sourcedetails.eventMessageFile);
		datalength = sourcedetails.eventMessageFile.length() * sizeof(wchar_t);

		if (RegSetValueExW(hk,			// subkey handle         
			L"EventMessageFile",			// value name            
			0,							// must be zero          
			REG_EXPAND_SZ,				// value type            
			data,			// address of value data 
			datalength))						// length of value data  
		{
			prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring();
			return false;
		}

		if (RegSetValueExW(hk,			// subkey handle         
			L"CategoryMessageFile",			// value name            
			0,							// must be zero          
			REG_EXPAND_SZ,				// value type            
			data,			// address of value data 
			datalength))						// length of value data  
		{
			prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring();
			return false;
		}

		// Set the supported types flags.

		data = (const BYTE*)&sourcedetails.numTypesSupported;
		datalength = sizeof(sourcedetails.numTypesSupported);


		if (RegSetValueExW(hk,		// subkey handle            
			L"TypesSupported",		// value name                   
			0,						// must be zero                 
			REG_DWORD,				// value type           
			data,					// address of value data        
			datalength))			// length of value data      
		{
			prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring();
			return false;
		}

		data = (const BYTE*)&sourcedetails.numCategory;
		datalength = sizeof(sourcedetails.numCategory);

		if (RegSetValueExW(hk,  // subkey handle            
			L"CategoryCount",  // value name                   
			0,                 // must be zero                 
			REG_DWORD,         // value type           
			data,  // address of value data        
			datalength))    // length of value data      
		{
			prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring();
			return false;
		}

		// Append the sources key
		///////////////////////////// 
		/*
		short null = 0 ;
		data = (const BYTE*)&details.eventLogName ;
		datalength = details.eventLogName.length() ;

		if (RegSetValueExW(hk,		// subkey handle
		L"Sources",				// value name
		0,						// must be zero
		REG_MULTI_SZ,			// value type
		data,					// address of value data
		0))			// length of value data
		{
		prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring() ;
		return false;
		}
		*/ ///////////////////////////////// Append the sources key



		return true;
	}

	bool deleteEventLogSource(const _bstr_t name)
	{
		return true;
	}

	bool createEventLog(const Details & details)
	{
		using namespace dbjsys::fm;
		using namespace dbjsys::fm::bstrex;

		if (doesEventLogExist(details.eventLogName))
			return true; // need to tell user !!!

		RegKey hk;

		// Add your source name as a subkey under the EventLog 
		// key in the EventLog service portion of the registry. 

		_bstr_t fullName = EVENTLOG_REG_LOCATION;
		fullName += dbjsys::glob::backSlash();
		fullName += details.eventLogName;

		if (RegCreateKeyW(
			HKEY_LOCAL_MACHINE,
			fullName,
			&hk))
		{
			//TODO WTF?! 
			// print out error msg to cerr
			prompt(bstr_file) << L" " << __LINE__ << L" " <<
				doctor::errstring();
			return false;
		}

		const BYTE * data = 0;
		DWORD datalength = 0;

		// Set the filename. 
		fullName = EVENTLOG_FILEPATH;
		fullName += dbjsys::glob::backSlash();
		fullName += details.file;

		data = (const BYTE*)static_cast<const wchar_t*>(fullName);
		datalength = fullName.length() * sizeof(wchar_t);

		if (RegSetValueExW(hk,			// subkey handle         
			L"File",					// value name            
			0,							// must be zero          
			REG_EXPAND_SZ,				// value type            
			data,						// address of value data 
			datalength))				// length of value data  
		{
			//TODO WTF?! 
			prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring();
			return false;
		}

		// Set the PrimarModule to be Application. 
		fullName = EVENTLOG_FILEPATH;
		fullName += dbjsys::glob::backSlash();
		fullName += details.file;

		_bstr_t p_module(L"Application");
		data = (const BYTE*) static_cast<const wchar_t*>(p_module);
		datalength = p_module.length() * sizeof(wchar_t);

		if (RegSetValueExW(hk,			// subkey handle         
			L"PrimaryModule",					// value name            
			0,							// must be zero          
			REG_EXPAND_SZ,				// value type            
			data,						// address of value data 
			datalength))				// length of value data  
		{
			//TODO WTF?! 
			prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring();
			return false;
		}


		// Set the maxsize.
		data = (const BYTE*)&details.maxSize;
		datalength = sizeof(details.maxSize);


		if (RegSetValueExW(hk,		// subkey handle            
			L"MaxSize",		// value name                   
			0,						// must be zero                 
			REG_DWORD,				// value type           
			data,					// address of value data        
			datalength))			// length of value data      
		{
			prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring();
			return false;
		}

		// set the retention timeout
		data = (const BYTE*)&details.retention;
		datalength = sizeof(details.retention);

		if (RegSetValueExW(hk,		// subkey handle            
			L"Retention",			// value name                   
			0,						// must be zero                 
			REG_DWORD,				// value type           
			data,					// address of value data        
			datalength))			// length of value data      
		{
			//TODO WTF?! 
			prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring();
			return false;
		}

		// create the source key, it is a REG_MULTI_SZ!
		datalength = details.eventLogName.length() + 1;
		_bstr_t sourceStr = details.eventLogName;

		data = (const BYTE*)static_cast<const wchar_t *>(sourceStr);


		if (RegSetValueExW(hk,		// subkey handle            
			L"Sources",				// value name                   
			0,						// must be zero                 
			REG_MULTI_SZ,			// value type           
			data,				// address of value data        
			datalength * sizeof(wchar_t)))			// length of value data      
		{
			//TODO WTF?! 
			prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring();
			return false;
		}

		return true;
	}

	bool deleteEventLog(const _bstr_t & name)
	{
		return true;
	}

	bool doesEventLogExist(const _bstr_t & name)
	{
		_bstr_t fullName = EVENTLOG_REG_LOCATION;
		fullName += dbjsys::glob::backSlash();
		fullName += name;

		RegKey hk;
		DWORD m_lErr = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, fullName, 0, KEY_READ, &hk);

		return m_lErr == ERROR_SUCCESS;
	}

	bool doesEventLogSourceExist(const _bstr_t logname, const _bstr_t & sourcename)
	{
		_bstr_t fullName = EVENTLOG_REG_LOCATION;
		fullName += dbjsys::glob::backSlash();
		fullName += logname;
		fullName += dbjsys::glob::backSlash();
		fullName += sourcename;

		RegKey hk;
		DWORD m_lErr = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, fullName, 0, KEY_READ, &hk);

		return m_lErr == ERROR_SUCCESS;
	}


	static Shared_Ref_Type make() {
		return new Child;
	}

//-------------------------------------------------------------
} ; // eof IEvLog
//-------------------------------------------------------------
// class that takes care of closing a registry key
// 
class RegKey
{
public:
	// 
	RegKey() : hk_(0) {}
	// 
	~RegKey()
	{
		if (hk_)
			::RegCloseKey(hk_);
		hk_ = 0;
	}
	// 
	HKEY * operator &()
	{
		return &hk_;
	}
	// 
	operator const HKEY & () const
	{
		return hk_;
	}
	// 
	operator HKEY & ()
	{
		return hk_;
	}
private:
	// no copy or assginment allowed
	// 
	RegKey(const RegKey &);
	// 
	RegKey & operator=(const RegKey &);


	// 
	HKEY hk_;
};

//-------------------------------------------------------------
// encapsulation of logging file logic
// 
template<class T> class LoggingFile
	{
	public:
		typedef LoggingFile<T>		MyType;
		typedef Win32Error<MyType>  Err;
		typedef T IEVLOG;

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
			typedef Win32Error<Writer> Err;

            Writer(const _bstr_t & source_name, const _bstr_t & server_name) :
				source_name_(source_name),
				server_name_(server_name),
				evlog_handle_(0),
				evlog_(IEVLOG::make())
			{
				ready_the_event_log();
			}

			Writer(const Writer & other) :
				source_name_(other.source_name_),
				server_name_(other.server_name_),
				evlog_handle_(0),
				evlog_(other.evlog_)
			{
				ready_the_event_log();
			}

			Writer & operator=(const Writer & rhs)
			{
				if (this != &rhs)
				{
					source_name_ = rhs.source_name_;
					server_name_ = rhs.server_name_;
					evlog_ = rhs.evlog_;
					ready_the_event_log();
				}

				return *this;
			}

            ~Writer()
			{
				if (evlog_handle_)
					evlog_->deregisterEventSource(evlog_handle_);
				evlog_handle_ = 0;
			}

			void report(const LogEvent & event)
			{
				const wchar_t * arrayOfStrings[2];
				arrayOfStrings[0] = (static_cast<const wchar_t*>(event.message));

				if (!arrayOfStrings[0])
					return;	// nothing to log

				evlog_->reportEvent(
					evlog_handle_,		// handle of event source
					event.type, 				// event type
					event.category,			// event category
					event.eventID,				// event ID
					NULL,						// current user's SID
					1,							// num of strings
					0,							// no bytes of raw data
					arrayOfStrings,			// array of error strings
					NULL
					);
			}


		private:
			HANDLE evlog_handle_ ;
			_bstr_t server_name_ ;
			_bstr_t source_name_ ;

			SREF<IEVLOG> evlog_;

			void ready_the_event_log()
			{
				if (evlog_handle_)
					evlog_->deregisterEventSource(evlog_handle_);
				evlog_handle_ = 0;

				// these can be null
				LPCWSTR server = static_cast<const wchar_t*>(server_name_);
				LPCWSTR source = static_cast<const wchar_t*>(source_name_);

				evlog_->registerEventSource(evlog_handle_, server, source);
				_ASSERT(evlog_handle_ != NULL);
			}
		} ;


		class Reader
		{
		public:
			typedef Win32Error<Reader> Err;

			Reader(const _bstr_t & source_name, const _bstr_t & server_name) :
				source_name_(source_name),
				server_name_(server_name),
				evlog_handle_(0),
				evlog_(IEVLOG::make())
			{
				ready_the_event_log();
			}

			Reader(const Reader & other) :
				source_name_(other.source_name_),
				server_name_(other.server_name_),
				evlog_handle_(0),
				evlog_(other.evlog_)
			{
				ready_the_event_log();
			}

			Reader & operator=(const Reader & rhs)
			{
				if (this != &rhs)
				{
					source_name_ = rhs.source_name_;
					server_name_ = rhs.server_name_;
					evlog_ = rhs.evlog_;
					ready_the_event_log();
				}

				return *this;
			}

			~Reader()
			{
				if (evlog_handle_)
					::CloseEventLog(evlog_handle_);
				evlog_handle_ = 0;
			}

		private:
			HANDLE evlog_handle_ ;
			_bstr_t server_name_ ;
			_bstr_t source_name_ ;

			SREF<IEVLOG> evlog_;

			void ready_the_event_log()
			{
				if (evlog_handle_)
					evlog_->closeEventLog(evlog_handle_);
				evlog_handle_ = 0;

				// these can be null
				LPCWSTR server = static_cast<const wchar_t*>(server_name_);
				LPCWSTR source = static_cast<const wchar_t*>(source_name_);

				evlog_->openEventLog(evlog_handle_, server, source);
				_ASSERT(evlog_handle_ != NULL);
			}
		} ;


	private:
		// forbidden behaviour
		LoggingFile () {}
		LoggingFile(const LoggingFile & ) {}
		LoggingFile & operator = (const LoggingFile &) {}

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
		void read(const LogEvent & event)
		{
			// NOT IMPLEMENTED YET
			return;
		}

		void report(const LogEvent & event)
		{
			try
			{
				log_writer_->report(event);
			}
			catch (Writer::Err & e)
			{
				throw e.what();
			}
		}


	//-------------------------------------------------------------
	}; // eof LoggingFile
//-------------------------------------------------------------
	/*
	// convert a vector of string to MULTI_SZ
	void strings2MULTI_SZ(const std::vector<_bstr_t> & strings, char * buff, size_t & size)
	{
		// work out the total space we need

		// allocate the buffer, upto client to free the memory (use delete[])

		// add strings one by one, separate using null character

		// terminate using two null characters

		dbjTHROWERR(L"strings2MULTI_SZ() Not implemented yet");
	}
	// convert a MULTI_SZ to a vector of strings
	void MULTI_SZ2string(const char * buff, size_t size, std::vector<_bstr_t> & strings)
	{
		dbjTHROWERR(L"MULTI_SZ2string() Not implemented yet");
	}
	*/

	} // namespace fm
} // namespace dbjsys
