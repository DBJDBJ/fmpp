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

//--------------------------------------------------------------------
#pragma once
#if _MSC_VER < 1200
#error This code must be compiled using Visual C++ version 6.0 or better.
#endif
//--------------------------------------------------------------------
namespace dbjsys {
    namespace fm {
//--------------------------------------------------------------------
		class loadablestring_string
		{
		public:
			dbjMAKE_ERR(loadablestring_string);

			typedef _bstr_t String_type;
			//
			// load from resource and return const reference to self
			// CAUTION: current content is lost after sucesfull load
			static _bstr_t load(unsigned int id_)
			{
				TCHAR arr[BUFSIZ];
				_bstr_t buf(arr);

				if (!
					::LoadString((HINSTANCE)NULL, (UINT)id_, (TCHAR*)buf, buf.length())
					)
				{
					Win32Error<loadablestring_string>::report_only(::GetLastError(), __FILE__, __LINE__);
					buf = "";
				}

				return buf;
			}

			loadablestring_string( unsigned int id_) {
				buffer_ = load(id_);
			}
			virtual ~loadablestring_string() {
				buffer_ = "";
			}
			//
			operator const String_type & () const { return this->buffer_; }
		private:
			/* forbidden operations */
			loadablestring_string() {}
			loadablestring_string(loadablestring_string  &) { }
			loadablestring_string  & operator = (loadablestring_string  &) { }
			//
			_bstr_t buffer_;
		};
/****************************************************************************************************************
deliberately left this in so that it can be compared to _bstr_t version
*/
#if (!1)
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
#endif // !1
//-----------------------------------------------------------------------
#define loadablestring_testing
#ifdef loadablestring_testing
//-----------------------------------------------------------------------
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
	loadablestring_string ls_(IDS);

	prompt(L"\nLoaded string  id: [") << IDS << L"]" <<  (bstr_t &)ls_ ;
	
    //
	_bstr_t ls2 = loadablestring_string::load(0); // runtime_error
    //
}
//--------------------------------------------------------------------
#endif // loadablestring_testing
//--------------------------------------------------------------------
    } //    namespace fm 
} // namespace dbj 
//--------------------------------------------------------------------

