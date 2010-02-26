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
//  $Author: DBJ  $
//  $Date: $
//  $Revision: $
//*****************************************************************************/

#include "../fm.h"
#include "ievlog.h"


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
		if( hk_ )
			::RegCloseKey(hk_);
		hk_ = 0 ;
	}
	// 
	HKEY * operator &() 
	{
		return &hk_ ;
	}
	// 
	operator const HKEY & () const
	{
		return hk_ ;
	}
	// 
	operator HKEY & ()
	{
		return hk_ ;
	}
private:
	// no copy or assginment allowed
	// 
	RegKey( const RegKey & ) ;
	// 
	RegKey & operator=( const RegKey & ) ;


	// 
	HKEY hk_ ;  
} ;

static _bstr_t bstr_file( __FILE__ ) ;
static wchar_t* EVENTLOG_REG_LOCATION = L"SYSTEM\\CurrentControlSet\\Services\\EventLog" ;
static wchar_t* EVENTLOG_FILEPATH = L"%SystemRoot%\\system32\\config" ;

namespace dbjsys {
	namespace fm {

LoggingFile::Reader::Reader( const _bstr_t & source_name, const _bstr_t & server_name ) :
	source_name_(source_name), 
	server_name_(server_name), 
	evlog_handle_(0),
	evlog_( IEvLog::make() ) 
{
		ready_the_event_log() ;
}

LoggingFile::Reader::Reader( const LoggingFile::Reader & other ) :
	source_name_(other.source_name_), 
	server_name_(other.server_name_), 
	evlog_handle_(0),
	evlog_(other.evlog_)
{
		ready_the_event_log() ;
}

LoggingFile::Reader & LoggingFile::Reader::operator=( const LoggingFile::Reader & rhs )
{
	if( this != &rhs )
	{
		source_name_ = rhs.source_name_ ;
		server_name_ = rhs.server_name_ ;
		evlog_		 = rhs.evlog_ ;
		ready_the_event_log() ;
	}

	return *this ;
}

LoggingFile::Reader::~Reader()
{
	if( evlog_handle_ )
		::CloseEventLog( evlog_handle_ ) ;
	evlog_handle_ = 0 ; 
}

void LoggingFile::Reader::ready_the_event_log()
{
	if( evlog_handle_ )
		evlog_->closeEventLog( evlog_handle_ ) ;
	evlog_handle_ = 0 ;
	
	// these can be null
	LPCWSTR server = static_cast<const wchar_t*>(server_name_) ;
	LPCWSTR source = static_cast<const wchar_t*>(source_name_) ;

	evlog_->openEventLog( evlog_handle_, server, source ) ;
	_ASSERT( evlog_handle_ != NULL ) ;
}


LoggingFile::Writer::Writer( const _bstr_t & source_name, const _bstr_t & server_name ) :
	source_name_(source_name), 
	server_name_(server_name), 
	evlog_handle_(0),
	evlog_( IEvLog::make() ) 
{
	ready_the_event_log() ;
}

LoggingFile::Writer::Writer( const LoggingFile::Writer & other ) :
	source_name_(other.source_name_), 
	server_name_(other.server_name_), 
	evlog_handle_(0),
	evlog_(other.evlog_)
{
	ready_the_event_log() ;
}

LoggingFile::Writer & LoggingFile::Writer::operator=( const LoggingFile::Writer & rhs ) 
{
	if( this != &rhs )
	{
		source_name_ = rhs.source_name_ ;
		server_name_ = rhs.server_name_ ;
		evlog_       = rhs.evlog_ ;
		ready_the_event_log() ;
	}

	return *this ;
}

LoggingFile::Writer::~Writer()
{
	if( evlog_handle_ )
		evlog_->deregisterEventSource( evlog_handle_ ) ;
	evlog_handle_ = 0 ; 
}

void LoggingFile::Writer::report( const LoggingFile::LogEvent & event)
{
	const wchar_t * arrayOfStrings[2] ;
	arrayOfStrings[0] =	(static_cast<const wchar_t*>(event.message)) ;

	if ( ! arrayOfStrings[0]    ) 
		return ;	// nothing to log
	
	evlog_->reportEvent(
				 evlog_handle_ ,		// handle of event source
				 event.type, 				// event type
				 event.category,			// event category
				 event.eventID,				// event ID
				 NULL,						// current user's SID
				 1,							// num of strings
				 0,							// no bytes of raw data
				 arrayOfStrings,			// array of error strings
				 NULL
				 ) ;
}

void LoggingFile::Writer::ready_the_event_log( )
{
	if( evlog_handle_ )
		evlog_->deregisterEventSource( evlog_handle_ ) ;
	evlog_handle_ = 0 ;
	
	// these can be null
	LPCWSTR server = static_cast<const wchar_t*>(server_name_) ;
	LPCWSTR source = static_cast<const wchar_t*>(source_name_) ;

	evlog_->registerEventSource( evlog_handle_, server, source ) ;
	_ASSERT( evlog_handle_ != NULL ) ;
}

void LoggingFile::read( const LoggingFile::LogEvent & event ) 
{
	// NOT IMPLEMENTED YET
	return ;
}

void LoggingFile::report( const LoggingFile::LogEvent & event ) 
{
	try
	{
		log_writer_->report( event ) ;
	}
	catch( Writer::W32Err & e )
	{
		dbjTHROWERR( e.what() ) ;
	}
}


bool IEvLog::createEventLogSource( const IEvLog::SourceDetails & sourcedetails ) 
{
	using namespace dbjsys::fm ;
	using namespace dbjsys::fm::bstrex ;


    //if( doesEventLogSourceExist( sourcedetails.eventLogName, sourcedetails.sourceName ) )
    //    return true ;


	RegKey hk;  

 
	// Add your source name as a subkey under the EventLog 
	// key in the EventLog service portion of the registry. 

	_bstr_t fullName = EVENTLOG_REG_LOCATION ;
	fullName += dbjsys::glob::backSlash() ;
	fullName += sourcedetails.eventLogName ;
	fullName += dbjsys::glob::backSlash() ;
	fullName += sourcedetails.sourceName ;


    if (RegCreateKeyW(
		HKEY_LOCAL_MACHINE, 
		fullName, 
		&hk)) 
	{
		// print out error msg to cerr
        prompt(bstr_file) << L" " << __LINE__ << " " <<
			doctor::errstring() ;
		return false;
	}

	const BYTE * data = 0 ;
	DWORD datalength = 0 ;

	// Add the Event ID message-file name to the subkey.  
	data = (const BYTE*)static_cast<const wchar_t*>(sourcedetails.eventMessageFile) ;
	datalength = sourcedetails.eventMessageFile.length() * sizeof(wchar_t) ;

	if (RegSetValueExW(hk,			// subkey handle         
		L"EventMessageFile",			// value name            
		0,							// must be zero          
		REG_EXPAND_SZ,				// value type            
		data,			// address of value data 
		datalength) )						// length of value data  
	{
        prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring() ;
		return false;
	}

	if (RegSetValueExW(hk,			// subkey handle         
		L"CategoryMessageFile",			// value name            
		0,							// must be zero          
		REG_EXPAND_SZ,				// value type            
		data,			// address of value data 
		datalength) )						// length of value data  
	{
        prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring() ;
		return false;
	}

	// Set the supported types flags.

	data = (const BYTE*)&sourcedetails.numTypesSupported ;
	datalength = sizeof(sourcedetails.numTypesSupported) ;
 

	if (RegSetValueExW(hk,		// subkey handle            
		L"TypesSupported",		// value name                   
		0,						// must be zero                 
        REG_DWORD,				// value type           
        data,					// address of value data        
        datalength ))			// length of value data      
	{
        prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring() ;
		return false;
	}

	data = (const BYTE*)&sourcedetails.numCategory ;
	datalength = sizeof(sourcedetails.numCategory) ;

	if (RegSetValueExW(hk,  // subkey handle            
		L"CategoryCount",  // value name                   
		0,                 // must be zero                 
        REG_DWORD,         // value type           
        data,  // address of value data        
        datalength))    // length of value data      
	{
        prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring() ;
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
	

 
	return true ;
}
bool IEvLog::deleteEventLogSource( const _bstr_t name ) 
{
	return true ;
}

bool IEvLog::createEventLog( const IEvLog::Details & details ) 
{
	using namespace dbjsys::fm ;
	using namespace dbjsys::fm::bstrex ;

    if( doesEventLogExist( details.eventLogName ) )
        return true ; // need to tell user !!!

	RegKey hk;  
 
	// Add your source name as a subkey under the EventLog 
	// key in the EventLog service portion of the registry. 

	_bstr_t fullName = EVENTLOG_REG_LOCATION ;
	fullName += dbjsys::glob::backSlash() ;
	fullName += details.eventLogName ;

    if (RegCreateKeyW(
		HKEY_LOCAL_MACHINE, 
		fullName, 
		&hk)) 
	{
		// print out error msg to cerr
        prompt(bstr_file) << L" " << __LINE__ << L" " << 
			doctor::errstring() ;
		return false;
	}

	const BYTE * data = 0 ;
	DWORD datalength = 0 ;

	// Set the filename. 
	fullName = EVENTLOG_FILEPATH ;
	fullName += dbjsys::glob::backSlash() ;
	fullName += details.file ;

	data = (const BYTE*)static_cast<const wchar_t*>(fullName) ;
	datalength = fullName.length() * sizeof(wchar_t) ;

	if (RegSetValueExW(hk,			// subkey handle         
		L"File",					// value name            
		0,							// must be zero          
		REG_EXPAND_SZ,				// value type            
		data,						// address of value data 
		datalength) )				// length of value data  
	{
        prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring() ;
		return false;
	}

	// Set the PrimarModule to be Application. 
	fullName = EVENTLOG_FILEPATH ;
	fullName += dbjsys::glob::backSlash() ;
	fullName += details.file ;

	_bstr_t p_module(L"Application") ;
	data = (const BYTE*) static_cast<const wchar_t*>(p_module);
	datalength = p_module.length() * sizeof(wchar_t) ;

	if (RegSetValueExW(hk,			// subkey handle         
		L"PrimaryModule",					// value name            
		0,							// must be zero          
		REG_EXPAND_SZ,				// value type            
		data,						// address of value data 
		datalength) )				// length of value data  
	{
        prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring() ;
		return false;
	}


	// Set the maxsize.
	data = (const BYTE*)&details.maxSize ;
	datalength = sizeof(details.maxSize) ;
 

	if (RegSetValueExW(hk,		// subkey handle            
		L"MaxSize",		// value name                   
		0,						// must be zero                 
        REG_DWORD,				// value type           
        data,					// address of value data        
        datalength ))			// length of value data      
	{
        prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring() ;
		return false;
	}

	// set the retention timeout
	data = (const BYTE*)&details.retention ;
	datalength = sizeof(details.retention) ;

	if (RegSetValueExW(hk,		// subkey handle            
		L"Retention",			// value name                   
		0,						// must be zero                 
        REG_DWORD,				// value type           
        data,					// address of value data        
        datalength))			// length of value data      
	{
        prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring() ;
		return false;
	}

	// create the source key, it is a REG_MULTI_SZ!
	datalength = details.eventLogName.length() + 1 ;
	wchar_t * sourceStr = new wchar_t[datalength] ;
	sourceStr[datalength - 1] = 0 ;
	wcsncpy ( sourceStr, static_cast<const wchar_t *>(details.eventLogName), datalength - 1 ) ;
	data = (const BYTE*)sourceStr ;
	

	if (RegSetValueExW(hk,		// subkey handle            
		L"Sources",				// value name                   
		0,						// must be zero                 
        REG_MULTI_SZ,			// value type           
        data,				// address of value data        
        datalength * sizeof(wchar_t)))			// length of value data      
	{
        prompt(bstr_file) << L" " << __LINE__ << L" " << doctor::errstring() ;
		return false;
	}
 
	return true ;
}

bool IEvLog::deleteEventLog( const _bstr_t & name ) 
{
	return true ;
}

bool IEvLog::doesEventLogExist( const _bstr_t & name )
{
	_bstr_t fullName = EVENTLOG_REG_LOCATION ;
	fullName += dbjsys::glob::backSlash() ;
	fullName += name ;

	RegKey hk ;
	DWORD m_lErr = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, fullName, 0, KEY_READ, &hk);
        
	return m_lErr == ERROR_SUCCESS;
}

bool IEvLog::doesEventLogSourceExist( const _bstr_t logname, const _bstr_t & sourcename ) 
{
	_bstr_t fullName = EVENTLOG_REG_LOCATION ;
	fullName += dbjsys::glob::backSlash() ;
	fullName += logname ;
	fullName += dbjsys::glob::backSlash() ;
	fullName += sourcename ;

	RegKey hk ;
	DWORD m_lErr = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, fullName, 0, KEY_READ, &hk);
        
	return m_lErr == ERROR_SUCCESS;
}

// convert a vector of string to MULTI_SZ
void strings2MULTI_SZ( const std::vector<_bstr_t> & strings, char * buff, size_t & size )
{
    // work out the total space we need

    // allocate the buffer, upto client to free the memory (use delete[])

    // add strings one by one, separate using null character

    // terminate using two null characters

}


// convert a MULTI_SZ to a vector of strings
void MULTI_SZ2string(const char * buff, size_t size, std::vector<_bstr_t> & strings)
{
}



	} // namespace fm
} // namespace dbjsys
