//*****************************************************************************/
//
//                 Copyright (c)  2000 - 2010 by Dusan B. Jovanovic (dbj@dbj.org) 
//                          All Rights Reserved
//
//        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Dusan B. Jovanovic (dbj@dbj.org)
//
//        The copyright notice above does not evidence any
//        actual or intended publication of such source code.
//
//  $Author: DBJ  $
//  $Date: $
//  $Revision: $
//*****************************************************************************/
#include "fm.h"
#include <assert.h>
//--------------------------------------------------------------------------------------
namespace dbjsys {
namespace fm {
//--------------------------------------------------------------------------------------
namespace {

#if defined( _UNICODE )
typedef wchar_t char_type ;
#else
#error DBJ*FM++ __FILE__ has to be compiled as UNICODE 
#endif

// oo wrap up of the command line. ASCII version; uses __argv and __argc.
// _UNICODE version uses __wargv and __argc.
// NOTE:    this works regardless of the VS project type: 
//          console, windows, even DLL app.
//
// Created  DBJ     03122000
//
// This is singleton as implementation
// It is also thread safe
// str::string or std:wstring are not used here
// this depends on comutil.h
//
// 
class CmdLineArguments {

	// 
                ScopeLocker critical_section__ ;

    //--------------------------------------------------------
    // return true it str begin's with prefix
	// 
    static const bool begins_with( const char_type * prefix, const char_type * str ) 
    {
        bool result_ = true ;
        for ( int j = 0 ; prefix[j] != 0 ; j++ )
        {
            result_ = result_ && ( prefix[j] == str[j] ) ;
        }

        return result_ ;
    }
    //--------------------------------------------------------
    // return string part on the right of prefix given
    // we can assume that both arguments begin with the prefix string
    // and that prefix is shorter than str
	// 
    static const char_type * right_of( const char_type * prefix, const char_type * str ) 
    {
        int j = 0 ;
        while ((prefix[j]) && ( prefix[j] == str[j])) j++ ;
        return str + j ;
    }

    //--------------------------------------------------------
	// 
			char_type ** the_cli_arguments ;
public :
	// 
    int      number_of_arguments ;
    //
	// UNICODE version only!
    CmdLineArguments()
		: the_cli_arguments(__wargv) , number_of_arguments(__argc)
    {
        _ASSERTE( the_cli_arguments ) ;
    }

	// 
    ~CmdLineArguments () 
	{
		the_cli_arguments = 0 ;
    }

	// return true if symbol exist on the current command line
    const bool symbol_exists ( const char_type * prefix ) 
    {
        Lock auto_lock(critical_section__) ; // lock the whole instance

        const bool result_ = false ; // default

        if (( ! prefix ) && ( ! *prefix )) 
            return result_ ; // anti jokers measure

        register int j = 0 ;

		const char_type * candidate_ = NULL ;

        while ( NULL != ( candidate_ = operator [] (j++)))
        {
            if ( ! begins_with( prefix, candidate_ ) )
                continue ;
			return true ; // FOUND!
        }
        return result_ ;
    }
	// 
    const char_type * operator [] ( const int index ) const 
    {
        Lock auto_lock( critical_section__) ; // lock the whole instance
        if ( index < 0 ) return NULL ;
        if ( index > (number_of_arguments - 1) ) return NULL ;
        return the_cli_arguments[index] ;
    }
    //
    // return argument value of the argument that begins with a prefix
    // return NULL if prefix not found
    // argument prefix and value may have a space in between them, eg.:
    // "-f output.txt"
    // or not, eg.: "-foutput.txt"
    // In either case the same value shall be returned
    // In case of illegal cli rubish will be returned, eg.:
    // "-f -eoutput.err"  will return "-eoutput.err" if "-f" was the prefix given.
    // "-f <eol>"  will return "" if "-f" was the prefix given.
    // 
    //
	// 
    const char_type * operator [] ( const char_type * prefix ) const
    {
        Lock auto_lock(critical_section__) ; // lock the whole instance

        const char_type * result_ = (char_type*)0 ;

        if (( ! prefix ) && ( ! *prefix )) 
            return result_ ; // anti jokers measure

        register int j = 0 ;

        while ( NULL != ( result_ = operator [] (j++)))
        {
            if ( ! begins_with( prefix, result_ ) )
                continue ;
                    result_ = right_of( prefix, result_ ) ;
                        if (( result_ ) && ( *result_ ) ) 
                            break ;
                result_ = operator [] (j) ; // next argument
                    break ;
        }
        return result_ ;
    }
    //------------------------------------------------------------
	// return argument found by its name
	// ret type is variant
    _variant_t operator () ( const char_type * arg_name_ ) const
	{
		const char_type * cl_str = this->operator [] ( arg_name_ ) ;
        if ( (cl_str != 0) && (cl_str[0] != 0) )
			return _variant_t( cl_str ) ;
		else
			return _variant_t() ; // VT_EMPTY
	}
    //------------------------------------------------------------
    // the singleton method delivers one instance for all threads
    // I think we need no locking here ? DBJ 08042001
	// 
    static CmdLineArguments & instance()
    {
        static CmdLineArguments instance__ ;
        return instance__ ;
    }

} ;

	CmdLineArguments & cline_ = CmdLineArguments::instance() ;
}

//--------------------------------------------------------------------------------------

template<typename T>
const string_type cl_argument<T>::operator [] (const unsigned int & index) const
{
	return string_type(cline_[index]) ;
}


//--------------------------------------------------------------------------------------
//
//    cl_argument encapsulates single comand line argument. 
//--------------------------------------------------------------------------------------
    template<typename T>
    const bool cl_argument<T>::exists ( const char_type  * const cl_symbol ) 
    {
		return cline_.symbol_exists(cl_symbol) ;
	}
//--------------------------------------------------------------------------------------
//
// resolve the actual value and its type by c.l. symbol used 
//
//--------------------------------------------------------------------------------------
    template<typename T>
    const T & cl_argument<T>::operator () ( const char_type  * const cl_symbol ) 
    {
       try {
				_variant_t vart = cline_(cl_symbol) ;

				if ( VT_EMPTY != ((VARIANT)vart).vt ) 
				{
					this->reqval_ = (T)vart ;
				} else {
					this->reqval_ = this->defval_ ;
				}
		} catch ( const ::_com_error & cerr_ ) { 
				assert( cerr_.ErrorMessage() ) ; /* conversion failed */ 
		}
					return this->reqval_ ;
    }
//--------------------------------------------------------------------------------------
//
// in header we have this two typedefs
//
//	typedef dbjsys::fm::cl_argument<_bstr_t>   cli_argument_string ;
//	typedef dbjsys::fm::cl_argument<long>      cli_argument_long;
//
// explicit instantiations
//
// Whenever code that uses FM and some cl_argument<T> does not link we have
// to add cl_argument<> specialization for T here. This is not so bad because
// number of different types T is small in the context of command line arguments. 
// NOTE: since implementation of cl_argument uses variant_t, it depends on its
// ability to do the conversion. Therefore you can not specialize here what 
// variant_t can not convert and deliver as a result of one of its extractors.
// DBJ 08042001
DBJFMTXP template DBJFMAPI class cl_argument<long> ;
DBJFMTXP template DBJFMAPI class cl_argument<bstr_t> ;
// 
// CAUTION:
// Do NOT do the following :
// DBJFMTXP template DBJFMAPI class cl_argument<wchar_t *> ;
// If you do you will require a specialization for the wchar_t, because
// cl_argument<wchar_t>::operator () ( .... )
// can not be implemented as it is above. But all of this is NOT necessary
// since cl_argument<bstr_t>, covers wchar_t uses perfectly well ...
//--------------------------------------------------------------------------------------
} // namespace fm 
} // namespace dbj 
//--------------------------------------------------------------------------------------
