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
#include "../dbjio.h"
#include "ptl.h"
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
// see the specialization immediatelly bellow
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
// to manage disk I/O of BSTR objects
// ========================================================
PTLStore& operator<<(PTLStore& cs, const _bstr_t& str);
PTLStore& operator>>(PTLStore& cs, _bstr_t& str);

// ========================================================
// overloaded insertion and extraction operators
// to manage disk I/O of VARIANT type to/from PTL Store
// ========================================================
PTLStore& operator<<(PTLStore& cs, const _variant_t & );
PTLStore& operator>>(PTLStore& cs, _variant_t & );



/*****************************************************************************
Implementation moved from cpp here
*****************************************************************************/


	// storage file suffix
	const wchar_t * PTLStore::storeFileSuffix_ = STOREFILESUFFIX;

	// ========================================================
	//                  PTLStore class
	// class to encapsulate container store disk file
	// ========================================================
	void PTLStore::open(const _bstr_t& nm)
	{
		try
		{
			// --- opening for input
			isreading = true;
			name = nm + PTLStore::storeFileSuffix_;
			//ifile.open(name, std::ios::binary);
			//if (!ifile.fail())
			//    ifile.read(reinterpret_cast<wchar_t*>(&objcount), sizeof objcount);
			//else
			//throw PtlException("PTLStore::open() ifile is BAD");

			ifile = new dbjsys::fm::FileStream::Reader(name, OPEN_ALWAYS);
			if (ifile->size())
				objcount = ifile->read().lVal;
		}
		catch (const dbjsys::fm::FileStream::Err & e)
		{
			PtlException::PTL_HIT(dbjsys::fm::getErrMsg(e));
		}
	}
	void PTLStore::open(int ct)
	{
		try
		{
			// --- opening for output
			objcount = ct;
			isreading = false;
			//ofile.open(name, std::ios::binary);
			ofile = new dbjsys::fm::FileStream::Writer(name, CREATE_ALWAYS);

			//if ( ! ofile.fail() )
			//ofile.write(reinterpret_cast<wchar_t*>(&objcount), sizeof objcount);
			//else
			//{
			//	PtlException::hit(L"PTLStore::open() ofile is BAD", __FILE__, __LINE__);
			//}

			_variant_t var = long(objcount);
			ofile->write(var);
		}
		catch (const dbjsys::fm::FileStream::Err & e)
		{
			PtlException::PTL_HIT(dbjsys::fm::getErrMsg(e));
		}
	}

	void PTLStore::close()
	{
		try
		{
			if (isreading)
				ifile->close();
			else
				ofile->close();
		}
		catch (const dbjsys::fm::FileStream::Err & e)
		{
			PtlException::PTL_HIT(dbjsys::fm::getErrMsg(e));
		}
	}
	// ========================================================
	// overloaded insertion and extraction operators
	// to manage disk I/O of Standard C++ string objects
	// ========================================================
	PTLStore& operator<<(PTLStore& cs, const _bstr_t& str)
	{
		//int len = str.length();
		//cs.ofile.write(reinterpret_cast<wchar_t*>(&len), sizeof len);
		//cs.ofile.write(str, len);
		cs.ofile->write(str);
		return cs;
	}
	PTLStore& operator>>(PTLStore& cs, _bstr_t& str)
	{
		//int len;
		//cs.ifile.read(reinterpret_cast<wchar_t*>(&len), sizeof len);
		//wchar_t* cstr = new wchar_t[len+1];
		//cs.ifile.read(cstr, len);
		//cstr[len] = '\0';
		//str = cstr;

		str = cs.ifile->read();
		return cs;
	}

	// ========================================================
	// overloaded insertion and extraction operators
	// to manage disk I/O of VARIANT type
	// ========================================================

	PTLStore& operator<<(PTLStore& cs, const _variant_t & vart)
	{
		//_bstr_t varstr = (_bstr_t)vart ;
		//int len = varstr.length() ;

		//if (( len < 0 ) || ( len > LONG_MAX ))
		//	PtlException::hit(L"Illegal length on writing variant to PTL store", __FILE__, __LINE__);

		//cs.ofile.write( reinterpret_cast<wchar_t*>(&len), sizeof len);
		//cs.ofile.write( (wchar_t*)varstr , len);

		cs.ofile->write(vart);

		return cs;
	}

	PTLStore& operator>>(PTLStore& cs, _variant_t & vart)
	{
		/*
		int len = 0 ;
		cs.ifile.read(reinterpret_cast<wchar_t*>(&len), sizeof len);

		if (( len < 0 ) || ( len > LONG_MAX ))
		PtlException::hit(L"Illegal length on reading variant from PTL store", __FILE__, __LINE__);

		wchar_t* cstr = new wchar_t[len+1];

		if (cstr == 0)
		PtlException::hit(L"Not enough memory on reading variant from PTL store", __FILE__, __LINE__);

		cs.ifile.read(cstr, len);
		cstr[len] = '\0';
		_bstr_t bstr(cstr) ;
		delete cstr ;
		vart = bstr ;
		*/

		vart = cs.ifile->read();

		return cs;
	}





} // namespace PersistentTemplateLibrary

