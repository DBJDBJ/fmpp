#include "precomp.h"

/*
------------------------------------------------------------------------------------
*/
inline
bstr_t & operator + ( bstr_t & b1, const bstr_t & b2 )
{
		b1.operator + ( b2 ) ;
		return b1 ;
}
/*
------------------------------------------------------------------------------------
*/
extern "C" void test10_bstr_extensions()
{
	using namespace dbjsys::fm::bstrex ;

	bstr_t bstr ;
	const wchar_t * const wsP = L" + WIDE STRING POINTER" ;
	wchar_t ws [] = L" + WIDE STRING" ;

	
	bstr << wsP ;
	bstr << ws  ;

	bstr << std::wstring(L" + STD::WSTRING") ;

	// 
	bstr << bstr ;
}

//-----------------------------------------------------------------

static void fun ( long a, long b) 
{
	long c = a == b ;
}

extern "C" void test_CmdLineArguments ()
{
#if defined(__test_CmdLineArguments__)
	
	using namespace dbjsys::fm ;
    
    cl_argument<long>     cla1( 512 ) ;
    cl_argument<long>     cla2( 256 ) ;

    fun ( cla1(L"-n"), cla2(L"-s") ) ;

	// in case command line was  
    // '-n 1024 -s 512' fun() above will receive 1024 and 512
    // '-n1024 -s512'   fun() above will receive 1024 and 512
    // '-n1024'         fun() above will receive 1024 and 256
    // empty            fun() above will receive  512 and 256

#endif
}

//-----------------------------------------------------------------------
// Register the test[s]
//-----------------------------------------------------------------------
static const int dummy1 = DBJTESTREG( test_CmdLineArguments ) ;
//-----------------------------------------------------------------------
// eofile
//-----------------------------------------------------------------------

