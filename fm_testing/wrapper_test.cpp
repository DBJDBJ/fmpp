#include "precomp.h"

using namespace dbjsys::fm ;
using namespace dbjsys::fm::bstrex ;
//
static volatile const bstr_t BSTRFILE (__FILE__) ;
//--------------------------------------------------------------------------------
extern "C" void wrap_test()
{
	typedef WWrap<std::list<_bstr_t> > llist_type ;
	
	llist_type llist ;

	llist->push_back( L"0" ) ;
	llist->push_back( L"1" ) ;
	llist->push_back( L"2" ) ;
	llist->push_back( L"3" ) ;
	llist->push_back( L"4" ) ;
	llist->push_back( L"5" ) ;
	llist->push_back( L"6" ) ;
	llist->push_back( L"7" ) ;
	llist->push_back( L"8" ) ;
	llist->push_back( L"9" ) ;

	std::for_each( llist->begin(), llist->end(), val2prompt<bstr_t> ) ;
	llist.for_each( val2prompt<bstr_t> ) ;

	wrapper_test::robust_wrap_2() ;
}
//----------------------------------------------------------------------------
//
typedef std::map<int, bstr_t > IntStringMap ;
//
extern "C" void wrapwrap_test ( void) 
{
	WWrap<IntStringMap> ism_ ;

	ism_->insert( IntStringMap::value_type( 1, L"One" ) ) ;

	ism_->find(1) ;
}
//-----------------------------------------------------------------------
// Register the test[s]
//-----------------------------------------------------------------------
static    dummy1 = DBJTESTREG( wrap_test ) ;
static    dummy2 = DBJTESTREG( wrapwrap_test ) ;
//-----------------------------------------------------------------------
// eofile
//-----------------------------------------------------------------------
