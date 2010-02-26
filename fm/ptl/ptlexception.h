//*****************************************************************************/
//                 Copyright (c)  2000 - 2010 by DBJ Ltd. 
//                          All Rights Reserved
//
//        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Dusan B. Jovanovic (dbj@dbj.org)
//
//        The copyright notice above does not evidence any
//        actual or intended publication of such source code.
//
//  $Author: DBJ $
//  $Date: $
//  $Revision: 13 $
//*****************************************************************************/

#pragma once

// make bstr_t as a macro
#if ! defined ( BSTRIT )
#define BSTRIT(x) bstr_t(#x)
#endif

namespace PersistentTemplateLibrary	{
//
struct PtlException
{
	typedef dbjsys::fm::Error<PtlException> Err ;

	static void hit( const wchar_t * msg, const bstr_t & filename, int line )
	{
		throw PtlException::Err(msg, filename, line) ;
	}
} ;

// may use this to call hit from above
#if ! defined ( PTL_HIT )
#define PTL_HIT( x ) hit( x, BSTRIT( __FILE__ ) , __LINE__ )
#endif

} // eof namespace PersistentTemplateLibrary



