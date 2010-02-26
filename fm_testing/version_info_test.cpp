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
//  $Author: Dusan $
//  $Date: 14/08/01 16:10 $
//  $Revision: 4 $
//*****************************************************************************/
#include "precomp.h"

using namespace dbjsys::fm ;
using namespace dbjsys::fm::bstrex ;
//
static volatile const bstr_t BSTRFILE (__FILE__) ;
//--------------------------------------------------------------------------------
extern "C" void version_info_test()
{
	static  const wchar_t mname[] = L"x:\\bin\\debug\\dbjcluster.dll" ;
	try 
	{
		file_version fv( mname ) ;

	prompt (L"Version information: ")
			<< L"\nFile Description: " <<  fv.GetFileDescription()  
			<< L"\nFile Version: " <<  fv.GetFileVersion()      
			<< L"\nFile Internal Name: " <<  fv.GetInternalName()     
			<< L"\nFile Company Name: " <<  fv.GetCompanyName()      
			<< L"\nFile Legal Copyright: " <<  fv.GetLegalCopyright()   
			<< L"\nFile Original Filename: " <<  fv.GetOriginalFilename() 
			<< L"\nFile Product Name: " <<  fv.GetProductName()      
			<< L"\nFile Product Version: " <<  fv.GetProductVersion()   ;

	}
	catch ( const DBJSYSError & x )
	{
		prompt( x.what() ) ;
	}
}
//-----------------------------------------------------------------------
// Register the test[s]
//-----------------------------------------------------------------------
static  const int    dummy1 = DBJTESTREG( version_info_test ) ;
//-----------------------------------------------------------------------
// eofile
//-----------------------------------------------------------------------
