//-----------------------------------------------------------------------
//
// $Author: Dusan $
//
// $Revision: 8 $
//
//
#include "precomp.h"
//------------------------------------------------------------------------
using namespace dbjsys::fm ;
using namespace dbjsys::fm::bstrex ;
//------------------------------------------------------------------------
//strcmp
struct NocaseStrEqual : public std::equal_to<std::string>
{
bool operator()( const std::string& x, const std::string& y ) const 
{
    const char * lpString1 = x.c_str() ;
    const char * lpString2 = y.c_str() ;

   size_t result = CompareStringA(
              LOCALE_SYSTEM_DEFAULT,// locale identifier
              NORM_IGNORECASE,      // comparison-style options
              lpString1,            // first string
              (long)x.size(),             // size of first string
              lpString2,            // second string
              (long)y.size()              // size of second string
    );

   return result == CSTR_EQUAL ;
}
} ;
//----------------------------------------------------------------------
// strcmp but ignore case
inline bool operator || ( 
            const std::string & s1,  
            const std::string & s2
         )
{
    static NocaseStrEqual comparator ;
    return comparator(s1, s2 ) ;
}
//-----------------------------------------------------------------------
inline char compareAndUpperizeChars ( const char & c1 , const char & c2 )
{
    return  toupper( c1 > c2 ? c1 : c2 ) ;
}
//-----------------------------------------------------------------------
extern "C" void test1( void )
{
    std::string                     s("AaBbCcDdeEfF") ;
    std::string                     r( "Hello" ) ;
    std::string::iterator sbegin =  s.begin() ;
    std::string::iterator send =    s.end() ;
    std::string::iterator rbegin =  r.begin() ;
    NocaseStrEqual                  nse ;

    std::transform( sbegin, send, sbegin, toupper ) ;
    std::transform( sbegin, send, sbegin, sbegin, compareAndUpperizeChars ) ;

    prompt( L"After transformation: " ) << _bstr_t(s.c_str()) ;
    prompt( _bstr_t(s.c_str()) ) << L"||" << _bstr_t(r.c_str()) << L" returned: " 
        << ( s || r  ? L"equal" : L"not equal" ) ;
}
//-----------------------------------------------------------------------
extern"C" void test2()
{
    _bstr_t bst = "ANSI" ;

    dbjsys::fm::prompt( bst ) ;

    bst = L"UNICODE" ;

    dbjsys::fm::prompt( bst ) ;
}
//-----------------------------------------------------------------------
// Hash algo testing
// depends on Hash_algo_testing being defined!
extern"C"  void test3()
{
#if defined (Hash_algo_testing)
    dbjsys::fm::algo::hash_test() ;
#endif
}
//-----------------------------------------------------------------------
// resource string testing
extern"C"  void test4()
{
   for ( register j = 0 ; j < 13; j++ )
    dbjsys::fm::stringRtest( IDS_STRING1 ) ;
}
//-----------------------------------------------------------------------
extern"C"  void test5_timestamp()
{
	dbjsys::fm::Timestamp ts_ ;

	dbjsys::fm::prompt( ts_() ) ;
}
//-----------------------------------------------------------------------
// tokenizers testing
//requires #define tokenizers_testing
extern"C"  void test6()
{
#if defined (tokenizers_testing)
    dbjsys::fm::token_test() ;
#endif
}
//
extern"C"  void test7_testing_thread_map()
{
#if defined ( testing_thread_map )
    dbjsys::fm::test_statefull_thread_map() ;
    dbjsys::fm::test_stateless_thread_map() ;
#endif
}

//
extern"C"  void test8()
{
#ifdef dbjsys_fm_tlsot_testing
    dbjsys::fm::tls_test() ;
#endif
}

//-----------------------------------------------------------------------
// 
//-----------------------------------------------------------------------
static    dummy1 = DBJTESTREG( test1 ) ;
static    dummy2 = DBJTESTREG( test2 ) ;
static    dummy3 = DBJTESTREG( test3 ) ;
static    dummy4 = DBJTESTREG( test4 ) ;
static    dummy5 = DBJTESTREG( test5_timestamp ) ;
static    dummy6 = DBJTESTREG( test6 ) ;
static    dummy7 = DBJTESTREG( test7_testing_thread_map ) ;
static    dummy8 = DBJTESTREG( test8 ) ;
//-----------------------------------------------------------------------
// eofile
//-----------------------------------------------------------------------
