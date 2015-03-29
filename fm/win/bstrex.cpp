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
//  $Author: Dusan $
//  $Date: $
//  $Revision: $
//*****************************************************************************/
//
// dbjBSTR extensions
//

#include "../fm.h"
#include "../dbjio.h"

//*****************************************************************************/
namespace dbjsys {
	namespace fm {
		namespace bstrex {
//*****************************************************************************/
		//---------------------------------------------------------------------------------------
		// 
		// These definitions are here because they  are for type which are used by FM
		// like std::wostream, std::wstring,etc.
		// The defintions for types which are specific should be defined in the modules
		// which are declaring those types. But in this same namespace.
		//---------------------------------------------------------------------------------------
		std::wostream & assign_from_bstr_t_ ( std::wostream & t_, const bstr_t & bstr ) 
		{
			return t_ << ((const wchar_t*)bstr) ;
		}
		// wostream to bstr_t -- a tricky one !
		bstr_t & assign_from_bstr_t_ ( bstr_t & bstr, const std::wostream & t_) 
		{
			std::wostringstream wos ;
			wos << t_.rdbuf() ;
			bstr += wos.str().c_str() ;
			return bstr ;
		}
//*****************************************************************************/
		} // namespace bstrex 
	} // fm
}  // dbjsys
//*****************************************************************************/

