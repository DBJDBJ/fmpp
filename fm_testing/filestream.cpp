//*****************************************************************************/
//
//                  MetaSocket (c) - The Missing Tier (tm)
//
//                 Copyright (c)  2000 by DBJSystems LTD 
//                          All Rights Reserved
//
//        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF DBJSystems LTD
//
//        The copyright notice above does not evidence any
//        actual or intended publication of such source code.
//
//  $Author: Muh $
//  $Date: 7/06/01 17:58 $
//  $Revision: 6 $
//*****************************************************************************/
#include "precomp.h"
#include "adotools.h"
#include <objbase.h>
#include <assert.h>
//---------------------------------------------------------------------------------
using namespace dbjsys::fm ;
//---------------------------------------------------------------------------------
//
struct com_start 
{
	com_start () { 
		assert(
		S_OK == CoInitializeEx( 
			0, ::COINIT_APARTMENTTHREADED 
			) 
		);
	}

	~com_start () { 
		CoUninitialize() ;
	}

	static const com_start & instance ()
	{
		static com_start instance_ ;
		return instance_ ;
	}
} ;
//---------------------------------------------------------------------------------
//
extern "C" void filestream_test()
{
	com_start::instance() ;

	_bstr_t filename = L"test.msock" ;
	_bstr_t filename2 = L"test2.msock" ;

	dbjsys::fm::FileStream::Writer  
			writer( filename ) ;
	dbjsys::fm::FileStream::Writer
			writer2( filename2 ) ;

	const wchar_t * sql_statement = L"SELECT * FROM orders WHERE customerid LIKE 'A%'" ;
	const wchar_t * mdb_filepath_ = L"x:/nwind.mdb" ;

	variant_t ado_rset = dbj::util::get_record_set(sql_statement, mdb_filepath_ ) ;

	// save the recordset to a file
	writer.write( ado_rset ) ;
	writer.close() ;

	ado_rset.Clear();

	// read back recordset from the file
	FileStream::Reader reader( filename ) ;
	ado_rset = reader.read() ;

	dbj::util::recordset_to_logfile( ado_rset, std::wclog ) ;

	// save recordset to another file
	writer2.write( ado_rset ) ;
}

