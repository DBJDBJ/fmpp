//*****************************************************************************/
//
//                  
//
//                 Copyright (c)  2000-2006 by DBJSolutions LTD 
//                          All Rights Reserved
//
//        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF DBJSolutions LTD
//
//        The copyright notice above does not evidence any
//        actual or intended publication of such source code.
//
//  $Author: Dusan $
//  $Date: $
//  $Revision: $
//*****************************************************************************/

#if ! defined ( DBJ_HASH_ALGO_H_ )
#define DBJ_HASH_ALGO_H_ 1

#pragma once

//--------------------------------------------------------------------------------
//
// Hashing algorithms as function object[s]
// NOTE: See Stroustrup, C++, 3rd ed., 17.6.2.3 - Hashing, p.502
//
//--------------------------------------------------------------------------------
namespace dbjsys {
    namespace fm {
        namespace algo {
//--------------------------------------------------------------------------------
//
    template < typename T > struct Hash : std::unary_function<T, size_t>
    {
        // typedef Error<Hash> Err ;
        // generic case depends on the feature of language:
        // every pointer adress can be cast into char pointer
        size_t operator () ( const T & key ) const
        {
            size_t      res = 0 ;
            size_t      len = sizeof(T) ;
            const wchar_t * p  = reinterpret_cast<const wchar_t*>(&key) ;

            while ( len -- )
                res = ( res << 1 ) ^*p++ ; // use bytes of key's presentation

			return res ;
        }
    } ;

    // specialization for wchar_t *, needs no casting
    template <> struct Hash<wchar_t *> : std::unary_function< wchar_t *, size_t>
    {
        size_t operator () ( const wchar_t * key ) const
        {
                size_t   res = 0 ;
                while ( *key )
                    res = ( res << 1 ) ^*key++ ; // use int value of characters
                return res ;
        }
    } ;

    // specialization for _bstr_t, needs no casting
     template <> struct Hash<_bstr_t> : std::unary_function< _bstr_t , size_t>
    {
        size_t operator () ( const _bstr_t & key ) const
        {
            return Hash<wchar_t *>()((wchar_t *)key) ;
       }
    } ;

    // specialization for any kind of a basic_string<> needs no casting either
     template <> struct Hash<std::string> : std::unary_function< std::string , size_t>
    {
       size_t operator () ( const std::string & key ) const
        {
            size_t  res =   0;
            typedef std::string::const_iterator CI;
            CI  p   = key.begin() ;
            CI  end = key.end() ;

            while ( p != end ) res = (res<<1)^*p++ ; // use int value of characters
            return res ;
        }
    } ;
        //-----------------------------------------------------------------------
        #if defined (Hash_algo_testing)
		/*
        inline std::ostream operator << ( std::ostream & os , const _bstr_t & bs )
        {
            return os << (char *)bs ;
        }*/
			//
            template< typename T > struct show_hash
            {
                void operator () ( const T & name ) 
                {
					using namespace dbjsys::fm::bstrex ;

                    static const   dbjsys::fm::algo::Hash<T> hash ;
                    std::wcout << std::endl << L"hash(" 
						<< name << L") = " 
						<< (long)hash(name) ;
                }
            } ;
        inline void hash_test()
        {
            _bstr_t names [] = { L"Pera", L"Zika", L"Mika", L"Laza" } ;
            std::for_each( names, names + 4 , show_hash<_bstr_t>() ) ;

            std::string tricky_names [] = { "PeraPeri", "ZikaZiki", "MikaMiki", "LazaLazi" } ;
            std::for_each( tricky_names, tricky_names + 4 , show_hash<std::string>() ) ;

            wchar_t * very_tricky_names [] = { L"Deepak the Proof Rider", L"Mahmudul the Big Equalizer", L"Ryan is gone", L"DBJ the Curator" } ;
            std::for_each( very_tricky_names, very_tricky_names + 4 , show_hash<const wchar_t *>() ) ;

        }
        #endif // defined (Hash_algo_testing)
//--------------------------------------------------------------------------------
} ; // eof namespace algo
//--------------------------------------------------------------------------------
    } //    namespace fm 
} // namespace dbjsys 
//--------------------------------------------------------------------------------

#endif // ! defined ( DBJ_HASH_ALGO_H_ )
