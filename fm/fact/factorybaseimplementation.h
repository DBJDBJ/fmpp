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

#pragma once

//------------------------------------------------------------
// This code must be compiled with RTTI switched on !
//------------------------------------------------------------
#if defined( _CPPRTTI )
// The /GR option causes the compiler to add code to check object types at run time. 
// When this option is specified, the compiler defines the _CPPRTTI preprocessor macro. 
//------------------------------------------------------------

#if ! defined( UINT_MAX )
#include <climits>
#endif

//------------------------------------------------------------
namespace dbjsys {
    namespace fm {
//------------------------------------------------------------
//
// Simple factory and its two implementations 
// form a 'Strategy' pattern
// 21.05.97 dbj
//
// MAHMUDUL of course we don't need MAX if we use vector
// 
template < class PMETHOD, class BASETYPE >
class factoryBaseImplementation
{
public :
	//--------------------------------------------
		struct Product {

			std::string name ;
			PMETHOD & productionMethod ;

			Product( const std::string & n , const PMETHOD & pM )
			: name( n ) , 
              productionMethod( const_cast<PMETHOD & >(pM) )
			{
			}
            Product ( const Product & other )
			: name( other.name ) , 
              productionMethod( const_cast<PMETHOD & >(other.productionMethod) )
            {
            }
            Product & operator = ( const Product & other ) {
                if ( this != &other ) {
                    name = other.name ;
                    productionMethod = other.productionMethod ;
                }
                return *this ;
            }
		} ;
	//--------------------------------------------
    typedef Error< factoryBaseImplementation > Err;
    typedef std::vector< Product > ProductPtrVec;
	//--------------------------------------------
	// 
	factoryBaseImplementation (){};
	//--------------------------------------------
	// 
    ~factoryBaseImplementation () { this->products.clear() ; }
        /*=
        -------------------------------------------------------------------------
         Registration implementation
         Here we make and store new product. Each product is made 
         with two key attributes: 'name' & 'production method'
        =*/
	// 
        void reg ( const std::string & name , const PMETHOD & productionMethod ) 
	     {
	            dbjVERIFY( this ) ;
		        dbjVERIFY( products.size() >= 0 ) ;
                dbjVERIFY( products.size() <= UINT_MAX ) ;
                    // first check if this name is already registered
                    ProductPtrVec::const_iterator iter;
                    for( iter = products.begin(); iter != products.end(); iter++ )
                    {
                        if( (*iter).name == name )
                            dbjTHROWERR( "Model name already found. Re-registering is not allowed." );
                    }
                // Product* p = new Product( name, productionMethod );
	            products.push_back( Product( name.c_str(), productionMethod ) );	
	    }
    //
	// clean and simple, public interface ...
	//
        /*=
        -------------------------------------------------------------------------
         Return a new product found by its name by which it was registered.
         Use production method , operator ()().
        =*/
	// 
        BASETYPE * get ( const std::string & what ) const
        {
           BASETYPE * result = 0 ;
   
           ProductPtrVec::const_iterator iter;
           for( iter = products.begin(); iter != products.end(); iter++ )
           {
                if( what == (*iter).name )
                {
                    result = (*iter).productionMethod(); // calling operator ()
                    break;
                }
           }
        #ifdef _DEBUG
			        if ( result ) ::OutputDebugStringA(typeid(result).name()) ;
        #endif
          return result ;
        }

    /*=
    -------------------------------------------------------------------------
     make by index number
    =*/
	// 
    BASETYPE * get( const int index ) const
    {
        // Validate the index
		if ( index < 0 ) 
			throw std::out_of_range(__FILE__ " -- bad index") ;
		if ( index > ( products.size() - 1 ) ) 
			throw std::out_of_range(__FILE__ " -- bad index") ;

        BASETYPE * result = 0 ;
        try
        {
            const Product & p  = products[index];
            result      = p.productionMethod() ; // calling operator ();
        }
        catch( const std::exception & evnt )
        {
            dbjTHROWERR( evnt.what() ) ;
        }
        return result ;
    }
    /*=
    -------------------------------------------------------------------------
     find key by index
    =*/
	// 
    const std::string & getKey( const int index ) const  
    {
        // Validate the index
		if ( index < 0 ) 
			throw std::out_of_range(__FILE__ " -- bad index") ;
		if ( index > ( products.size() - 1 ) ) 
			throw std::out_of_range(__FILE__ " -- bad index") ;

		const Product & p  = products[index];
        return p.name;
    }

private:
	// 
    ProductPtrVec products;
} ;
//-------------------------------------------------------------------------
    } //    namespace fm 
} // namespace dbj 
//-------------------------------------------------------------------------
//------------------------------------------------------------
// This code must be compiled with RTTI switched on !
//------------------------------------------------------------
#endif // defined( _CPPRTTI )
// The /GR option causes the compiler to add code to check object types at run time. 
// When this option is specified, the compiler defines the _CPPRTTI preprocessor macro. 
//------------------------------------------------------------
