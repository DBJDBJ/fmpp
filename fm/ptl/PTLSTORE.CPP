//*****************************************************************************/
//
//                 
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
//  $Revision: 11 $
//*****************************************************************************/
//
#include "../dbjio.h"
#include "ptl.h"
//*****************************************************************************/
namespace PersistentTemplateLibrary {


    // storage file suffix
	const wchar_t * PTLStore::storeFileSuffix_ = STOREFILESUFFIX ;

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
        name = nm + PTLStore::storeFileSuffix_ ;
        //ifile.open(name, std::ios::binary);
        //if (!ifile.fail())
        //    ifile.read(reinterpret_cast<wchar_t*>(&objcount), sizeof objcount);
	    //else
		    //throw PtlException("PTLStore::open() ifile is BAD");

	    ifile = new dbjsys::fm::FileStream::Reader( name, OPEN_ALWAYS ) ;
        if( ifile->size() )
            objcount = ifile->read().lVal ;
    }
    catch( const dbjsys::fm::FileStream::Err & e )
    {
        PtlException::PTL_HIT( dbjsys::fm::getErrMsg(e) ) ; 
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
	    ofile = new dbjsys::fm::FileStream::Writer( name, CREATE_ALWAYS ) ;

	    //if ( ! ofile.fail() )
        //ofile.write(reinterpret_cast<wchar_t*>(&objcount), sizeof objcount);
	    //else
	    //{
	    //	PtlException::hit(L"PTLStore::open() ofile is BAD", __FILE__, __LINE__);
	    //}

	    _variant_t var = long(objcount) ;
	    ofile->write( var ) ;
    }
    catch( const dbjsys::fm::FileStream::Err & e )
    {
        PtlException::PTL_HIT( dbjsys::fm::getErrMsg(e) ) ; 
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
    catch( const dbjsys::fm::FileStream::Err & e )
    {
        PtlException::PTL_HIT( dbjsys::fm::getErrMsg(e) ) ; 
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
	cs.ofile->write( str ) ;
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

	str = cs.ifile->read() ;
    return cs;
}

// ========================================================
// overloaded insertion and extraction operators
// to manage disk I/O of VARIANT type
// ========================================================

PTLStore& operator<<(PTLStore& cs, const _variant_t & vart )
{
    //_bstr_t varstr = (_bstr_t)vart ;
    //int len = varstr.length() ;
    
    //if (( len < 0 ) || ( len > LONG_MAX ))
	//	PtlException::hit(L"Illegal length on writing variant to PTL store", __FILE__, __LINE__);

    //cs.ofile.write( reinterpret_cast<wchar_t*>(&len), sizeof len);
    //cs.ofile.write( (wchar_t*)varstr , len);

	cs.ofile->write( vart ) ;

    return cs;
}

PTLStore& operator>>(PTLStore& cs, _variant_t & vart )
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
	
	vart = cs.ifile->read() ;

    return cs;
}





} // namespace PersistentTemplateLibrary
