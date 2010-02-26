//*****************************************************************************/
//
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
//  $Revision: 19 $
//*****************************************************************************/
#pragma once

#include "../fm.h"
#include "../com/filestream.h"


#include "PtlException.H"

namespace {
    // storage file suffix
	const wchar_t * STOREFILESUFFIX = L".dbjstore" ;
}

namespace PersistentTemplateLibrary	{


// ========================================================
//                  PTLStore class
//  class to encapsulate container store disk file
// ========================================================
//
class PTLStore {
	//
    _bstr_t name;
	//
    bool isreading;
	//
    int objcount;
	//
	static const wchar_t * storeFileSuffix_ ;
public:
	//
    //std::wifstream ifile ; may be also used
	dbjsys::fm::SREF<dbjsys::fm::FileStream::Reader> ifile ;
	//
    //std::wofstream ofile; may be also used
	dbjsys::fm::SREF<dbjsys::fm::FileStream::Writer> ofile ;
	//
    PTLStore() : isreading(false), objcount(0)
	    { }
	//
    void open(const _bstr_t& nm);
	//
    void open(int ct);
	//
    void close();
	//
    int count() const
	    { return objcount; }
};
// ========================================================
// overloaded insertion and extraction 
// operator template functions
// to manage disk I/O of intrinsic types
// and non-polymorphic user-defined types
// that have no pointer or reference data members
// ========================================================
// These functions will allow your code to compile,
// you have to give the exact template function definitions 
// to the linker
template <class T>
PTLStore& operator<<(PTLStore& cs, const T& t) 
{
    cs.ofile->write(&t, sizeof t);
    return cs;
}

template <class T>
PTLStore& operator>>(PTLStore& cs, T& t) 
{
    cs.ifile->read(&t, sizeof t);
    return cs;
}

// ========================================================
// Specialzations of the above, for bstr_t and variant_t
// ========================================================

// ========================================================
// overloaded insertion and extraction operators
// to manage disk I/O of Standard VC++ BSTR objects
// ========================================================
PTLStore& operator<<(PTLStore& cs, const _bstr_t& str);
PTLStore& operator>>(PTLStore& cs, _bstr_t& str);

// ========================================================
// overloaded insertion and extraction operators
// to manage disk I/O of VARIANT type
// ========================================================
PTLStore& operator<<(PTLStore& cs, const _variant_t & );
PTLStore& operator>>(PTLStore& cs, _variant_t & );


} // namespace PersistentTemplateLibrary

