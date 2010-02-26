#pragma once
//-----------------------------------------------------------------------
// Small and primitive testing framework
// Last $Author: Dusan $
// $Revision: 18 $
//
#define TESTAPI extern "C"

// this is inside DBJ*FM++ (dbjfm.lib)
extern "C" void switchErrLog ( const wchar_t * name , const int appendLog ) ;
//
namespace dbj {
//-----------------------------------------------------------------------
namespace test {
	namespace implementation {
//-----------------------------------------------------------------------
typedef void (* TEST_FP) ( void ) ;

typedef struct std::pair<TEST_FP,bstr_t> PAIR ;

typedef std::list< PAIR > sequence_type ;

extern sequence_type * sequence ;
//---------------------------------------------------------------------------
inline void caller( PAIR & fp_name_pair_ )
{
	using namespace dbjsys::fm::bstrex;

    static const dbjsys::fm::wstringr underscore(IDS_UNDERSCORE) ;
    static const dbjsys::fm::wstringr space(IDS_SPACE) ;
    static const dbjsys::fm::wstringr prompt(IDS_PROMPT) ;
    static const dbjsys::fm::wstringr begin_msg(IDS_TEST_BEGIN) ;
    static const dbjsys::fm::wstringr end_msg(IDS_TEST_END) ;
    static const dbjsys::fm::wstringr err_msg(IDS_ERR_MSG_1) ;

	TEST_FP & test_fun = fp_name_pair_.first ;
	bstr_t & test_name = fp_name_pair_.second ;

    dbjsys::fm::prompt( underscore , 60 );
    dbjsys::fm::prompt( begin_msg, prompt, 0 ) << test_name ;
    dbjsys::fm::prompt( space, 0,0 ) ;
        try {
            test_fun() ;
        }
		catch ( _com_error & cer )
		{
			dbjsys::fm::prompt( dbjsys::fm::getErrMsg( cer ) ) ;
		}
        catch (  ... ) {
            dbjsys::fm::prompt( err_msg, 0, 0 ) ;
        }
    dbjsys::fm::prompt( space, 0, 0 ) ;
    dbjsys::fm::prompt( end_msg, prompt, 0 ) << test_name ;
}

//---------------------------------------------------------------------------
struct show_test_name  {

	static int counter_() { static int cnt_ = 0 ; return cnt_ ++ ; }

	std::wostream & os_ ;
	//
	show_test_name(std::wostream & os ) : os_(os) {
	}
	void operator () ( PAIR & the_pair ) const	
	{	
		using namespace dbjsys::fm::bstrex ;
		os_ << L"\t" << counter_() << L" :\t\t" << the_pair.second << std::endl ;
	}
} ;

//----------------------------------------------------------------------
static  const _bstr_t null_test_id(L"~") ;
//---------------------------------------------------------------------------
struct comparator  {

	const _bstr_t & look_for_ ;

	comparator( const _bstr_t & look_for ) : look_for_(look_for) {}

	int operator () ( PAIR & the_pair )
	{
		return dbjsys::fm::algo::compareNoCase( look_for_ , the_pair.second ) ;
	}
} ;
//----------------------------------------------------------------------
//
inline const void run( std::wostream & os , const _bstr_t & test_id = null_test_id )
{
    // redirect to some other output
    dbjsys::fm::set_prompt_stream( os ) ;
	//
	//  if no test axed for do them all
	if ( test_id == null_test_id )
	{
		std::for_each( sequence->begin(), sequence->end(), caller ) ;
	}
	else {
		// can  we find the test axed for ?
		sequence_type::iterator & location = std::find_if( 
			sequence->begin(), 
			sequence->end(), 
			comparator(test_id ) 
		) ;

		if ( location == sequence->end() )
			throw L"Test not  found!" ;

		caller( *location ) ;
	}

    dbjsys::fm::reset_prompt_stream( ) ;
}

//-----------------------------------------------------------------------
	} //	namespace implementation 
//-----------------------------------------------------------------------
struct DBJtest  
{
	DBJtest( int dummy = 0 )
	{
		implementation::sequence = new implementation::sequence_type()  ;
	}
	~DBJtest  ()
	{
		delete implementation::sequence ;
	}
	const int reg ( implementation::TEST_FP test_function_pointer, const _bstr_t & test_name )
	{
		_ASSERT( implementation::sequence ) ;

		implementation::PAIR pair_ = std::make_pair( test_function_pointer, test_name ) ;

		implementation::sequence->push_back( pair_ ) ;

		return ( 1 == 1 ) ;
	}

	//------------------------------------------------------
	// This  run() will runn all the tests as registered
	const void run ( std::wostream & os = std::wclog )
	{
		_ASSERT( implementation::sequence ) ;

		implementation::run( os ) ;
	}

	//------------------------------------------------------
	// This  run() will run only the test requested
	const void run ( const  _bstr_t & testname, std::wostream & os = std::wclog )
	{
		_ASSERT( implementation::sequence ) ;

		implementation::run( os, testname ) ;
	}

	//------------------------------------------------------
	const ::size_t number_of_registered_tests () const
	{
		_ASSERT( implementation::sequence ) ;

		return implementation::sequence->size() ;
	}

	//------------------------------------------------------
	const void show_registered_tests (std::wostream & os = std::wclog) const
	{
		_ASSERT( implementation::sequence ) ;

		std::for_each( implementation::sequence->begin(), 
			implementation::sequence->end(), 
			implementation::show_test_name(os) ) ;
	}

} ;
//------------------------------------------------------------------
// printf-like output to the result logfile
inline void dbgout(wchar_t * pszFormat, ...) 
{
  static const unsigned long BUFLEN =  1024 * 64 ; // plenty...
  static wchar_t szBuff[BUFLEN] = L"" ;

  memset( szBuff, 0, BUFLEN * sizeof( wchar_t ) ) ;

  va_list arglist;
  va_start(arglist, pszFormat);
 
  int rtn = wvsprintfW(szBuff, pszFormat, arglist) ; 
  _ASSERTE( rtn < sizeof(szBuff)/sizeof(szBuff[0]) );

  dbjsys::fm::prompt( szBuff ) ;
  
  va_end(arglist);
}

inline void dbgout(wchar_t * pszFormat, _variant_t & variant_arg ) 
{
	_bstr_t bstr((_bstr_t)variant_arg ) ;
	wchar_t * wstr = (wchar_t *)bstr ;
	_ASSERTE( wstr) ;
	dbgout( pszFormat, wstr ) ;
}
//-----------------------------------------------------------------------
} // test 
} // dbj
//-----------------------------------------------------------------------


extern dbj::test::DBJtest testing_ ;

#define DBJTESTREG(x) testing_.reg( x, bstr_t( #x )) ;

//-----------------------------------------------------------------------
//EOF