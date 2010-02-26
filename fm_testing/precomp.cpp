
#include "precomp.h"
#pragma comment( lib, "kernel32.lib" )
//
//--------------------------------------------------------------------------------
namespace {
	// actually not required ...
	struct  _INITOR_ 
	{
		static UINT counter_ ;

	_INITOR_ () 
	{
		// use and increment
		if ( counter_ ++ == 0  )
		{
			// void * vp = ::malloc( sizeof( DBJtest ) ) ;
			// testing_ = new(vp) DBJtest() ;

#if _DEBUG
			const size_t nort = testing_.number_of_registered_tests() ;
#endif
		}
	}
	~_INITOR_ () 
	{
		// decrement and use
		if (  -- counter_ == 0 )
		{
		}
	}
	} ;

	static  _INITOR_	__dbjsys_test_initor__ ;
	UINT _INITOR_::counter_ = 0 ;
} // namespace
//--------------------------------------------------------------------------------
// 
dbj::test::implementation::sequence_type * dbj::test::implementation::sequence = 0 ;
// #pragma init_seg( "__DBJSYSTEMS_TESTING_INITIALIZATION__" )
#pragma warning( disable : 4073 ) 
#pragma init_seg( lib )
dbj::test::DBJtest testing_ ;

