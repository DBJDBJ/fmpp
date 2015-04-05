//*****************************************************************************/
//
//                  
//
//                 Copyright (c)  1997 - 2015 by Dusan B. Jovanovic (dbj@dbj.org) 
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
// This is string resource encapsulation.
// 14102000 DBJ Created
//////////////////////////////////////////////////////////////////////

#if _MSC_VER < 1200
#error This code must be compiled using Visual C++ version 6.0 or better.
#endif
//--------------------------------------------------------------------
#pragma once
//--------------------------------------------------------------------
namespace dbjsys {
    namespace fm {
//--------------------------------------------------------------------
template< class STRING >
class loadablestring_base : public STRING
{
protected:
    typedef STRING Parent ;
    //
    // my little auto buffer
    template< const int SIZE = BUFSIZ > struct buffer {
        const int size ;
        typename STRING::traits_type::char_type * lp ;
        buffer () : size(SIZE), lp( new STRING::traits_type::char_type[SIZE] ) {} 
        ~buffer () { delete [] lp ; }
    } ;
    //
    // load from resource and return const reference to self
    // CAUTION: current content is lost after sucesfull load
    virtual const loadablestring_base & load ( unsigned int id_ ) = 0 ;

public:
        loadablestring_base() {}
        virtual ~loadablestring_base() {}
        //
        // casting operator, *not* in the original basic_string<> !
        operator const typename STRING::traits_type::char_type * () const { return this->c_str(); }

};
//--------------------------------------------------------------------
// default std::string version
class stringr 
: public loadablestring_base<std::string >
{
public :
   stringr( unsigned int id_) { load( id_ ) ; }
   //
   const loadablestring_base<std::string > & load ( unsigned int id_ ) 
    {
        buffer<BUFSIZ> buffer ;

        if ( 0 ==
            ::LoadStringA((HINSTANCE)NULL, (UINT)id_, buffer.lp , buffer.size ) 
            )
            throw std::runtime_error( "stringr::load(), failed " ) ;
        Parent::operator = ( buffer.lp ) ;

        return *this ;
    }
} ;
//--------------------------------------------------------------------
// std::wstring version
class wstringr 
: public loadablestring_base<std::wstring > 
{
public :
        wstringr( unsigned int id_) { load( id_ ) ; }
   //
    const loadablestring_base<std::wstring > & load ( unsigned int id_ ) 
    {
        buffer<BUFSIZ> buffer ;

        if ( 0 ==
            ::LoadStringW((HINSTANCE)NULL, (UINT)id_, buffer.lp , buffer.size ) 
            )
            throw std::runtime_error( "wstringr::load(), failed" ) ;

        Parent::operator = ( buffer.lp ) ;

        return *this ;
    }
} ;
//-----------------------------------------------------------------------
#ifdef loadablestring_testing
//-----------------------------------------------------------------------
inline void showresult ( const std::wstring & ws )
{
   prompt(L"\nUNICODE string: [") << ws << L"]" ;
}
inline void showresult ( const std::string & ss )
{
   using namespace dbjsys::fm::bstrex;
   prompt( L"\nASCII string: [") << bstr_t(ss.c_str()) << "]" ;
}
//-----------------------------------------------------------------------
// call with string Id from resource
inline void stringRtest ( unsigned int IDS )
{
	//------------------------------------
    stringr asciiR(IDS) ;
    std::string & ascii = asciiR ;
    showresult( ascii  ) ;
    // asciiR = "From string to stringr does not work" ;
    // string resource semantics are read-only
    showresult( asciiR ) ;
    //
    wstringr unicodeR(IDS) ;
    std::wstring & unicode = unicodeR ;
    showresult( unicode  ) ;
    // (std::wstring)unicodeR = L"From wstring to wstringr does not work"  ;
    // string resource semantics are read-only
    showresult( unicodeR ) ;
    //
    stringr(0) ; // throws runtime_error
    //
}
//--------------------------------------------------------------------
#endif // loadablestring_testing
//--------------------------------------------------------------------
    } //    namespace fm 
} // namespace dbj 
//--------------------------------------------------------------------

