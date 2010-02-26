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
//  $Author: DBJ  $
//  $Date: $
//  $Revision: $
//*****************************************************************************/

#if ! defined ( _PairParser_H_ )
#define _PairParser_H_ 1
#pragma once
//--------------------------------------------------------------------------------
namespace dbjsys {
    namespace fm {
//--------------------------------------------------------------------------------
//
// Makes two std::string from one containing delimiter specified

// 
class PairParser {
	// 
    _bstr_t delimSTR_;
public :

    typedef dbjsys::fm::Error<PairParser> Err ;

	// 
PairParser (_bstr_t DELIMCHR) : delimSTR_(DELIMCHR)
{
	dbjVERIFY ( delimSTR_.length() > 0 ) ;	
}

	// 
~PairParser ( ) 
{
}


	// 
void makeTwo ( const _bstr_t & inBSTR , _bstr_t & head_, _bstr_t & body_ ) const
{
    std::wstring in_( inBSTR ) ;
    unsigned int  pos = (int)in_.find(delimSTR_ );

	if( (pos <= 0) || (pos > in_.length() ) )
	{
		_bstr_t msg( L"makeTwo: Could not make two from ") ; msg +=  delimSTR_  + inBSTR;
		dbjTHROWERR( msg );
	}


	try
	{
		head_ = in_.substr( 0, pos ).c_str() ;

		pos = pos+1;

		if( pos > in_.size() )
		{
			_bstr_t msg( L"makeTwo: Could not make two from ") ; msg += delimSTR_; msg += in_.c_str();
			dbjTHROWERR( msg ) ;
		}

        body_ = in_.substr( pos, in_.size() ).c_str() ;

		if( head_.length() == 0 )
		{
			_bstr_t msg( L"makeTwo: Unable to get the head from ") ;	msg += in_.c_str();
			dbjTHROWERR( msg ) ;
		}

		if( body_.length() == 0 )
		{
			_bstr_t msg( L"makeTwo: Unable to get the body from ") ;	msg += in_.c_str();
			dbjTHROWERR( msg ) ;
		}

	}
	catch( std::exception & e )
	{
		_bstr_t msg( e.what() ) ;	
        msg += L" " ;
        msg += in_.c_str();

		dbjTHROWERR( msg ) ;
	}
}

	// 
_bstr_t makeOne( const _bstr_t & head_ ,const _bstr_t & body_ ) const 
{
	_bstr_t theOne( head_ + delimSTR_ ) ;
	return	theOne += body_ ;
}

	// 
static PairParser & commaParser()
{
    static PairParser commaParser_( L"," ) ;
    return commaParser_ ;
}

} ;

//--------------------------------------------------------------------------------
} //    namespace fm 
} // namespace dbjsys 
//--------------------------------------------------------------------------------

#endif // _PairParser_H_ 1
