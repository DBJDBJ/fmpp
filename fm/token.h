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
//  $Author: DBJ  $
//  $Date: $
//  $Revision: $
//*****************************************************************************/
#ifndef _SSTOKEN_H_
#define _SSTOKEN_H_

//---------------------------------------------------------------------------------------
namespace dbjsys {
	namespace fm {
//
		using std::size_t ;
//---------------------------------------------------------------------------------------
//
template< typename STYPE, typename CHARTYPE >
class tokenizerT
{
private:
	//
	// 
	std::vector<size_t> VBEG;
	//
	// 
	std::vector<size_t> VEND;
	// internally we use std::wstring for implementation
    // UNICODE is supposed to be quicker under W2K and NT
	// 
    mutable std::wstring src_;
	// 
    mutable std::wstring tag_;
    // return not-a-position value for this string type
	// 
    size_t NPOS () const 
    {
        return std::wstring::npos ;
    }
    // find tag position in the source begining at given position
    // for this string type
	// 
    size_t FIND_TAG_POS( size_t starting_position ) const
    {
        return src_.find( tag_, starting_position ) ;
    }
    // return substring from the source
    // starting from VBEG[pos_] using
    // of length VEND[pos_] - VBEG[pos_]
	// 
    std::wstring SRC_SUBSTR ( size_t pos_ ) const
    {
		if( 0 == VEND[pos_] - VBEG[pos_] )
			return L"" ;

        return src_.substr( VBEG[pos_], VEND[pos_] - VBEG[pos_] );
    }
    // size() semantics for the STYPE used
	// 
    size_t TAG_SIZE() const
    {
        return tag_.size();
    }
	// 
    size_t SRC_SIZE() const
    {
        return src_.size();
    }
    // transform std::wstring to STYPE
	// 
    STYPE asSTYPE( const std::wstring & wstr ) const
    {
        return wstr.c_str() ;
    }
    //
	//IMPLEMENTATION
	//
	// 
	void parseLine()
	{
		size_t tagSize_ = TAG_SIZE();
		size_t tagStartPos_ = 0, tagEndPos_ = 0;

		VBEG.erase(VBEG.begin(), VBEG.end()) ;
		VEND.erase(VEND.begin(), VEND.end()) ;

		if ( tagSize_ < 1 )
		    return ;
        if ( SRC_SIZE() < 1 )
            return ;
 
		VBEG.push_back( tagStartPos_ );
		
		while( ( tagEndPos_ = FIND_TAG_POS( tagStartPos_ ) ) != NPOS() )
		{
			VEND.push_back( tagEndPos_ );
			tagStartPos_ = tagEndPos_ + tagSize_;
			VBEG.push_back( tagStartPos_ );
		}	
		VEND.push_back( SRC_SIZE() );
	}


public:

    typedef STYPE string_type ; // DBJ 19NOV2000
	typedef std::size_t size_t ;
    typedef Error<tokenizerT> Err ;

	// 
	tokenizerT( )
	{
		reset( STYPE(""), STYPE("") );
	}

	//
	// 
	tokenizerT( const STYPE & mSrc_, const STYPE & mTag_ )
	{
		reset( mSrc_, mTag_);
	}

	//
	// 
    tokenizerT( const STYPE & mSrc_, const CHARTYPE & mTag_ )
	{
		reset( mSrc_, STYPE(mTag_) );
	}

	//
	// 
	~tokenizerT()
	{   // 210598 JovanovD added
		VBEG.erase( VBEG.begin(), VBEG.end() ) ;
		VEND.erase( VEND.begin(), VEND.end() ) ;
	}

	//--------------------------------------------------
	// use this method when you want to reuse the only
	// instance of this class
	//
	// 
	void reset(const STYPE & mSrc_, const STYPE & mTag_ )
	{
		src_ = mSrc_ ;
		tag_ = mTag_ ;
		parseLine()  ;
	}

	// 
	STYPE getWord( size_t pos_ ) const
	{
		dbjVERIFY(!( pos_ > VBEG.size() || 0 > pos_));

		return asSTYPE( SRC_SUBSTR( pos_ ) ) ;
	}

	// added by JovanovD 21.05.98
	//
	STYPE operator [] ( size_t pos_ ) const
	{
		return getWord(pos_) ;
	}

	//
	size_t size() const throw()
	{
		return VBEG.size();
	}

	// 
     const tokenizerT & operator=(const tokenizerT &right)
     {
	    VBEG = right.VBEG;
	    VEND = right.VEND;

	    src_ = right.src_;
	    tag_ = right.tag_;

        return *this ;
     }

	// 
   	tokenizerT( const tokenizerT & right )
	{   // 210598 JovanovD added
	    VBEG = right.VBEG;
	    VEND = right.VEND;

	    src_ = right.src_;
	    tag_ = right.tag_;
	}

}; // tokenizerT

//--------------------------------------------------------------
// specializations necessary for tokenizerT<_bstr_t,wchar_t> to work
inline
tokenizerT<_bstr_t, wchar_t>::tokenizerT( const _bstr_t & mSrc_, const wchar_t & mTag_ )
{
	// make mTag_ into a character array (i.e. string)
	wchar_t buff[2] ;
	buff[0] = mTag_ ;
	buff[1] = L'\0' ; // NULL terminate the string
	reset( mSrc_, _bstr_t( buff ) );
}

//--------------------------------------------------------------
// specializations necessary for tokenizerT<std::string,char> to work
    // transform std::wstring to std::string
    inline
        std::string tokenizerT< std::string, char >::asSTYPE( const std::wstring & wstr ) const
    {
        return static_cast<const char *>(_bstr_t(wstr.c_str()) );
    }
    inline 
	void tokenizerT< std::string, char >::reset(
    const std::string & mSrc_, const std::string & mTag_ )
	{
		src_ = (wchar_t*)_bstr_t( mSrc_.c_str()) ;
		tag_ = (wchar_t*)_bstr_t( mTag_.c_str()) ; 
		parseLine()  ;
	}
//--------------------------------------------------------------
// most common used tokenizing types
typedef tokenizerT< _bstr_t, wchar_t >                btokenizer ;
typedef tokenizerT< std::string, char >               tokenizer ;
typedef tokenizerT< std::wstring, wchar_t >           wtokenizer ;
//typedef tokenizerT< std::basic_string<TCHAR>, TCHAR > ttokenizer ;
//--------------------------------------------------------------
/* TESTING */
#if defined ( tokenizers_testing )
//
inline
std::wostream & operator << ( std::wostream & wos, const std::string & ss )
{
    return wos << (wchar_t*)_bstr_t( ss.c_str() ) ;
}
//
inline void token_test()
{
	btokenizer btoken( L"01234;5678;9", L";" );
    prompt(L"tokenizerT<_bstr_t>") ;
    prompt(L"First word = ") << (wchar_t*)btoken[0] ;
    prompt(L"Second word = ") << (wchar_t*)btoken.getWord(1) ;
    prompt(L"Third word = ") << (wchar_t*)btoken.getWord(2) ;

    tokenizer token( "01234;5678;9", ";" );
    prompt(L"tokenizerT<std::string>") ;
    prompt(L"First word = ") << token[0] ;
    prompt(L"Second word = ") << token.getWord(1) ;
    prompt(L"Third word = ") << token.getWord(2) ;


    wtokenizer wtoken( L"01234;5678;9", L";" );
    prompt(L"tokenizerT<std::wstring>") ;
    prompt(L"First word = ") << wtoken[0] ;
    prompt(L"Second word = ") << wtoken.getWord(1).c_str() ;
    prompt(L"Third word = ") << wtoken.getWord(2).c_str() ;

    return;
}
#endif
//---------------------------------------------------------------------------------------
	} ; // fm
}; // dbjsys
//---------------------------------------------------------------------------------------
#endif //_SSTOKEN_H_
