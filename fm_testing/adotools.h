/*
*****************************************************************************

                 Copyright (c)  2000 - 2010 by Dusan B. Jovanovic (dbj@dbj.org) 
                          All Rights Reserved

        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Dusan B. Jovanovic (dbj@dbj.org)

        The copyright notice above does not evidence any
        actual or intended publication of such source code.

  $Author: $
  $Date: $
  $Revision: $
*****************************************************************************

	When doing transactions, the "Transaction DDL" is the key property  
	of the ADO  connection onobject. 

	value				Description 
	========================================================================
	0					Transactions are not supported. 
	------------------------------------------------------------------------
	1					Transactions can only contain DML statements. 
	------------------------------------------------------------------------
						DDL statements within a transaction cause an error. 
	2					Transactions can only contain DML statements. 
	------------------------------------------------------------------------
						DDL statements within a transaction cause the transaction to be committed. 
	4					Transactions can only contain DML statements. 
	------------------------------------------------------------------------
						DDL statements within a transaction are ignored. 
	8					Transactions can contain DDL and DML statements in any order. 
	------------------------------------------------------------------------
	16					Transactions can contain both DML and DDL statements that 
						modify tables or indexes, but modifying a table or index 
						within a transaction causes the table or index to be locked 
						until the transaction completes. This means you won't be able 
						to execute additional statements that affect the tables or 
						indexes once they are locked.  
	------------------------------------------------------------------------

	Note:   
	Not all providers support transactions. Verify that the provider-defined 
	property "Transaction DDL" appears in the Connection object's 
	Properties collection, indicating that the provider supports transactions. 
	If the provider does not support transactions, calling one of these 
	methods will return an error.

	DBJ 29.MAY.2001

*/
//---------------------------------------------------------------------------------
#import "C:\Program Files\Common Files\SYSTEM\ADO\msado15.dll" rename ( "EOF", "adoEOF" )
//

//-----------------------------------------------------------------------
namespace dbj {
namespace util {
//-----------------------------------------------------------------------
class ADOconnection // wrapper
{
	ADODB::_ConnectionPtr pConnection ;
	int transaction_support ;
public :
	typedef dbjsys::fm::Error<ADOconnection> Err ;

	// create and open the connection
	ADOconnection( const _bstr_t & strCnn) 
		: pConnection( NULL ) 
		, transaction_support(0)
	{
		//ITransactionContextExPtr ctx = dbjsys::fm::tx::new_context() ;
		//dbjsys::fm::tx::new_in_context( ctx, pConnection, __uuidof(ADODB::Connection) ) ;
		dbjCOMVERIFY( pConnection.CreateInstance(__uuidof(ADODB::Connection)) ) ;
    
		try	{
			pConnection->Open(strCnn,"","",ADODB::adConnectUnspecified) ;
		}
		catch( const _com_error & e )
		{
			std::wostringstream strm ;
			PrintProviderError( strm ) ;
			dbjTHROWERR( strm.str().c_str() ) ;
		}

		transaction_support = (long)get_property(L"Transaction DDL") ;
	
	}
	~ADOconnection() {
		pConnection->Close();
	}

	operator IDispatch* () {
		return  (IDispatch*)pConnection ;
	}

	operator ADODB::_ConnectionPtr & () {
		return pConnection ;
	}

	ADODB::_ConnectionPtr & operator * () {
		return pConnection ;
	}

	// get particular property, throw exception if not  found
	_variant_t get_property ( const wchar_t * prop_name )
	{
		ADODB::PropertiesPtr properties  = pConnection->GetProperties() ;
		ADODB::PropertyPtr   property    = properties->GetItem( prop_name ) ;

		return property->GetValue() ;
	}

	// transactional  methods can be called only if provider support transactions
	// for other limitations see the  table on the top of this file
	void BeginTrans() {
		if ( transaction_support ) pConnection->BeginTrans();
	}
	void CommitTrans() {
		if ( transaction_support ) pConnection->CommitTrans();
	}
	void RollbackTrans() {
		if ( transaction_support ) pConnection->RollbackTrans();
	}

	// print ADO error in full ,using ostream given
	void PrintProviderError( std::wostream & wos )
	{
		// Print Provider Errors from Connection object.
		// pErr is a record object in the Connection's Error collection.
		ADODB::ErrorPtr		pErr    = NULL;
		long				nCount  = 0;
		long				i       = 0;

		if( (pConnection->Errors->Count) > 0)
		{
			nCount = pConnection->Errors->Count;
			// Collection ranges from 0 to nCount -1.
			for(i = 0; i < nCount; i++)
			{
				pErr = pConnection->Errors->GetItem(i);

				wos << 	L"ADO Error, Number: [" 
					<< pErr->Number << L"]"
					<< L" Description [" 
					<< (LPWSTR) pErr->Description << L"]" ;
			}
		}
	}

} ;

/*
	example  of a valid call for the get_recordset() function
	const wchar_t * sql_statement = L"SELECT * FROM orders WHERE customerid LIKE 'A%'" ;
	const wchar_t * mdb_filepath_ = L"x:/nwind.mdb" );

	variant_t rs = get_record_set(sql_statement, mdb_filepath_ ) ;
*/

// obtain recordset depending on the sql  given. Look into the mdb file name given.
	inline _variant_t get_record_set(
			const wchar_t * sql_statement ,	
			const wchar_t * connection_string  
			)
	{
		_variant_t retval ; // VT_EMPTY
		_bstr_t conn_str = connection_string ;
		ADOconnection	the_connection( conn_str ) ;


		try	{

		ADODB::_RecordsetPtr   the_record_set   = NULL;

		dbjCOMVERIFY( the_record_set.CreateInstance( __uuidof( ADODB::Recordset) ) );

		the_record_set->CursorLocation = ADODB::adUseClient ;

		the_connection.BeginTrans() ;

		dbjCOMVERIFY( the_record_set->Open( 
								sql_statement, 
								_variant_t((IDispatch*)the_connection) , 
								ADODB::adOpenStatic,
								ADODB::adLockBatchOptimistic, 
								0L // options? what options?
							 ) ) ;

		the_connection.CommitTrans() ;
			
		// job done, disconnect from the active  connection
		the_record_set->PutRefActiveConnection( NULL ) ;
			
		retval = the_record_set.GetInterfacePtr() ;

		}
		catch( const _com_error & e )
		{
			the_connection.RollbackTrans() ;
			the_connection.PrintProviderError( std::wclog ) ;
			retval = _variant_t( e.Description() ) ; // cludge?
		}
				
		return retval ;
}

// dump the recordset given,  as string to the ostream given
inline void recordset_to_logfile( _variant_t  & rs_in_a_variant , std::wostream & wos )
{
	static _variant_t  null_var ;
	static _bstr_t     null_bst ;
	static long		   all_rows(-1) ;

	ADODB::_RecordsetPtr   the_record_set   = NULL;
	dbjCOMVERIFY( the_record_set.CreateInstance( __uuidof( ADODB::Recordset) ) );

	the_record_set = (IDispatch*)rs_in_a_variant ; // oops?!

	_variant_t varOutput = the_record_set->GetString(
		ADODB::adClipString, all_rows,  null_bst, null_bst, L"NULL"
		) ;

	wos << (LPCWSTR)(_bstr_t)varOutput  ;
   
}
//-----------------------------------------------------------------------
} // namespace util 
} // namespace dbj 
//-----------------------------------------------------------------------
