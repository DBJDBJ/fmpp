// stringvaluepair.h: interface for the key_value_map class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#if !defined(AFX_STRINGVALUEPAIR_H__842B53A4_C683_4E55_A100_108CF3935CF4__INCLUDED_)
#define AFX_STRINGVALUEPAIR_H__842B53A4_C683_4E55_A100_108CF3935CF4__INCLUDED_

//--------------------------------------------------------------------------------
namespace dbjsys {
    namespace fm {
//--------------------------------------------------------------------------------
// holds map of key-value pairs
// made from string in the format : "<key><delim><value><delim><key><delim><value>"
// only one delimiter is used and number of entries must be even, example :
// "a|b|c|d" , pairs are a and b, c and d .
//
// MUH  created
// DBJ  09102000    changed name from 'key_value_pair'
//                  second argument for find changed to pointer from reference
//
// 
class key_value_map  
{
public:
    // delimiter char may be different, this is default one
	// 
    static const wchar_t default_delimiter () { return L'|' ; }
    //
    typedef std::map< _bstr_t, _bstr_t> container_type ;
    typedef container_type::value_type value_type ; // what we store in the container
    typedef container_type::key_type key_type ; // std::string
    typedef container_type::referent_type referent_type ; // std::string
    typedef dbjsys::fm::Error<key_value_map> Err ;
    
	// 
    _bstr_t to_string( ) const ;
	// 
    void from_string( const _bstr_t & str ) ;

	// 
    bool find( const key_type & key, referent_type ** val ) const ;
	// 
    void insert( const key_type& key, const referent_type & val ) ;

	// 
    key_value_map( const wchar_t delim = key_value_map::default_delimiter()) 
        : delim_char_(delim)
    {
    }

	// 
    ~key_value_map()
    {
    }

private:
	// 
    container_type container_ ;
	// 
    const wchar_t delim_char_ ;

};

//--------------------------------------------------------------------------------
inline _bstr_t key_value_map::to_string( ) const 
{
	_bstr_t retVal ;
	wchar_t buff[2]  ;
	buff[0] = delim_char_ ;
	buff[1] = L'\0' ;
	_bstr_t delim_str(buff) ;
    try
    {
        container_type::const_iterator it = container_.begin() ;
    
        if( it == container_.end() )
            return _bstr_t(L"");

        retVal  = (*it).first ;
        retVal += delim_str ;
        retVal += (*it).second ;

        ++it ;
    
        while( it != container_.end() )
        {
            retVal += delim_str ;
            retVal += (*it).first ;
            retVal += delim_str ;
            retVal += (*it).second ;

            ++it ;
        }
    }
    catch( const key_value_map::Err & )
    {
        throw ;
    }
    catch( const std::exception & e )
    {
        dbjTHROWERR( e ) ;
    }

    return retVal ;
}

//--------------------------------------------------------------------------------
inline void key_value_map::from_string( const  _bstr_t  & str ) 
{
    try
    {
        container_.erase( container_.begin(), container_.end() ) ;

        if( str.length() < 1 ) 
            return ;

        btokenizer tokens( str, delim_char_ ) ;
        for( btokenizer::size_t i = 0 ; i < tokens.size() ; i = i+2 )
        {
            key_type key  = tokens[i] ;
            algo::stringtrim( key ) ;

            referent_type val  =  tokens[i+1]  ;
            algo::stringtrim( val ) ;

            dbjVERIFY( key.length() > 0 );
            dbjVERIFY( val.length() > 0 );

            container_[key] = val ;
        }
    }
    catch( const key_value_map::Err & )
    {
        throw ;
    }
    catch( const std::exception & e )
    {
        dbjTHROWERR( e ) ;
    }
}

//--------------------------------------------------------------------------------
// return the pointer to the value found in the map
// as long as map is ok , the pointer will be pointing to something valid
inline bool key_value_map::find( const key_type& key, referent_type ** val ) const
{
    dbjVERIFY( key.length() > 0 );
    try
    {
        container_type::const_iterator it = container_.find( key ) ;
        if( it != container_.end() )
        {
            *val = const_cast<referent_type *>(& ((*it).second) );
            return true ;
        }
    }
    catch( const key_value_map::Err & )
    {
        throw ;
    }
    catch( const std::exception & e )
    {
        dbjTHROWERR( e ) ;
    }
     
    return false ;
}

//--------------------------------------------------------------------------------
inline void key_value_map::insert( const key_type& key, const referent_type & val ) 
{
    dbjVERIFY( key.length() > 0 );
    dbjVERIFY( val.length() > 0 );
    try
    {
        container_type::value_type val_( key, val ) ;
        std::pair<container_type::iterator, bool> p = container_.insert( val_ ) ;
        if( p.second == false ) 
            dbjTHROWERR(L"Value already exists") ;
    }
    catch( const key_value_map::Err &  )
    {
        throw ;
    }
    catch( std::exception & e )
    {
        dbjTHROWERR( e ) ;
    }
}

//--------------------------------------------------------------------------------
} //    namespace fm 
} // namespace dbjsys 
//--------------------------------------------------------------------------------

#endif // !defined(AFX_STRINGVALUEPAIR_H__842B53A4_C683_4E55_A100_108CF3935CF4__INCLUDED_)
