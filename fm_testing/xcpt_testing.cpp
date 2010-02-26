
#include "precomp.h"
using namespace std; 
using namespace dbjsys::fm ;
//
#define BSTRFILE bstr_t(__FILE__)
//
interface BaseException {} ;
interface DerivedException : public BaseException {} ;
//
class The_Class ; 
//
typedef dbjsys::fm::Error<The_Class> Err ;
//
class The_Class
{
public:

	void throwErr()
	{
		//throw DerivedException();
		throw Err( L"Error from The_Class", BSTRFILE , __LINE__ ) ;
	}
} ;


//-----------------------------------------------------------------------
extern "C" void exception_testing(  void )
{
	prompt ( L"BEGIN exception_testing()") ;
	try
	{
		The_Class tc;
		tc.throwErr() ;
	}
	catch( const dbjsys::fm::DBJSYSError & e )
	{
		prompt (e.what()) ;
	}
	/*
	catch( const Err & e )
	{
		prompt ( e.what()) ;
	}
	*/
	catch ( const BaseException & bx )
	{
		prompt( L"Base Exception" ) ;
	}
	catch( ... )
	{
		prompt( L"Unknown Exception" ) ;
	}

	prompt ( L"END exception_testing()") ;

}

//-----------------------------------------------------------------------
extern "C" void test_exception_rethrowing()
{
    try {
        try {
            try {
                try {
		            The_Class tc;
		            tc.throwErr() ;        
                }
                catch( const dbjsys::fm::DBJSYSError e ) { throw ; }
            }
            catch( const dbjsys::fm::DBJSYSError e ) { throw ; }
        }
        catch( const dbjsys::fm::DBJSYSError e ) { throw ; }
    }
    catch( const dbjsys::fm::DBJSYSError e ) { /*throw ;*/ }
}

//-----------------------------------------------------------------------
// Register the test[s]
//-----------------------------------------------------------------------
static    dummy1 = DBJTESTREG( test_exception_rethrowing ) ;
static    dummy2 = DBJTESTREG( exception_testing ) ;
//-----------------------------------------------------------------------
// eofile
//-----------------------------------------------------------------------

