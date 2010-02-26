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

/******************************************************************************
 Category:  
 Infrastructure

 Title:
 Factory

 Description: 
 Factory template for type extendable factory.
 What this means ? This means , you make a base class, and you 
 make a factory for it. You then forget about it. You don't
 have to add new types as you go along, and derive classes from the base. 
 Everything is still in one factory :) Just register and factorize
 ( sort of a 'just wash and go' approach )
 
 Synopsis: 
 typedef dbjsys::fm::factory<Vehicle> factoryOfVehicles ;

 NOTE : 
 above spells : make a factory for any class which is 
 derived from Vehicle class.
 ******************************************************************************
*/
#pragma once
//------------------------------------------------------------
// This code must be compiled with RTTI switched on !
//------------------------------------------------------------
#if defined( _CPPRTTI )
// The /GR option causes the compiler to add code to check object types at run time. 
// When this option is specified, the compiler defines the _CPPRTTI preprocessor macro. 
//------------------------------------------------------------
#if ! defined ( type_info )
#include <typeinfo.h> 
#endif
//------------------------------------------------------------
#include "factorybaseimplementation.h"
//------------------------------------------------------------
namespace dbjsys {
    namespace fm {

//------------------------------------------------------------

template<class X>
inline const std::string type_to_name( const X & dummy ) {  return typeid(X).name() ; }

// TCHAR version
//#define type_to_nameT( T ) A2T(typeid(T).name())

template< class F, class T > class factory_unit ;

//------------------------------------------------------------
// 
template<typename T >
class factory {
public :
    //
    typedef dbjsys::fm::Error< factory > Err;
    //
	class ProductionMethod {
        	std::string modelName_ ; 
		public :
            //
            virtual const std::string & model_name() const { return modelName_ ; }
			// default c-tor uses factory type_to_uname()
            ProductionMethod ( const ProductionMethod & other ) 
                : modelName_(other.modelName_ ) 
            {
   				factory<T>::register_production_method( *this ) ;
            }
            // by using this c-tor we can bind this production method
            // to arbitrary name
            ProductionMethod ( const std::string & name_)
                :modelName_( name_ )
			{ 
				factory<T>::register_production_method( *this ) ;
			}
            // hidden stuff
            ProductionMethod & operator = (const ProductionMethod & other ) {
                if ( this != &other ) {
                    this->modelName_ = other.modelName_ ;
                }
                return *this ;
            }
			//
			virtual ~ProductionMethod ( ) { }
			// 
			// factory<T>::BaseType can not be used here
			// because template instance is still not fully defined
			// at this point, hence 'T' usage
			virtual T  * operator () () const = 0 ;
		} ;

		typedef T BaseType ;
		typedef factoryBaseImplementation< ProductionMethod, BaseType > Implementation ;

protected :
	// 
			Implementation implementation_ ;
public :
		
		typedef T ProductType ;

		friend class ProductionMethod ;
		
		typedef const ProductionMethod PMethod ;

	
protected:	// factory is a singleton, ditto :
	// 
    factory ()  { }
public :

	// 
		Implementation & imp() const 
		{ 
			return (Implementation &)implementation_ ; 
		}
	// destructor is public
	// 
        ~factory () { }
	// single instance provider
	// 
   		static factory<T> & instance ()
        {
	        static factory<T> singleton ;
	        return singleton ;
        }

    //
	// clean and simple, public interface ...
	//
    // find by the typeinfo of the type requested
    template< typename X > 
    X * operator () ( X * dummy ) const
    {
        return static_cast<X*>(this->get( type_to_name(dummy) )) ;
    }
	// find by key
	// 
	BaseType * get( const std::string & what ) const 
    {
	    dbjVERIFY( this ) ;
	    return implementation_.get(what) ;
    }

	// make product by index
	// 
	BaseType * get( const int index ) const
   	{
	    dbjVERIFY( this ) ;
	    return implementation_.get(index) ;
    }

	// find key by index
	// 
	const std::string & get_key ( const int index ) const
    {
        dbjVERIFY( this ) ;
        return implementation_.getKey(index) ;
    }

	// 
    const _bstr_t & to_string ( ) const
    {
            static _bstr_t keys("") ; // keep it here
            keys = "Factory:\n" ;
            keys += dbjsys::fm::type_to_name( *this ).c_str() ;
            keys += "\nKeys:\n" ;

        register j = 0 ;
            try {
                while(true)
                    keys += ("[" + get_key(j++) + "]\n").c_str() ;
            } catch ( ... ) {
            }
        return keys ;
    }

        //------------------------------------------------------------
        // using this function we 'marry' factory with 
        // new factory_unit, which is returned as a result
        // so that callers can use it latter to make instances
        // of type T
        template< class NewType >
        inline factory_unit< factory<BaseType>, NewType> & add_unit( NewType * t_ )
        {
            // first of all there is no point to have *t_
            _ASSERTE( 0 == t_ ) ;
            // to understand what is happening here look into the 
            // factory_unit constructor
            return factory_unit< factory<BaseType>, NewType >::instance() ;
        }

protected :

        //--------------------------------------------------------------
        // One must call this first in order to be able to use the 
        // factory second
	// 
        static void register_production_method
        (
	        const /* factory<T>:: */ PMethod	& productionMethod ,
	        const std::string & name = ""
        )
        {
	        factory<T>::Implementation & i = factory<T>::instance().imp();
            if ( name.size() < 1 ) 
	            i.reg( productionMethod.model_name(), productionMethod ) ;
            else
	            i.reg( name, productionMethod ) ;
        } 

} ; // eof class factory //////////////////////////////////////////////
//
// New Factory unit is added for each new type makeable by the existing
// factory.
//
#pragma warning( disable : 4101 )

// 
template< class F, class T >
class factory_unit {
public :
		//
		// How do we add new offsprings of factory::BaseType to 
		// the existing factory ?
		// Also how are they made, when factory needs to make them ?
		// Answer is bellow ...
		// Pointer to instance of 'Producer' is kept inside the
		// factory, associated with a name
		// 
		class Producer : public F::ProductionMethod
		{
		public :
                ~Producer() { }
				Producer ()
				: F::ProductionMethod( type_to_name( static_cast<T*>(0) ) )
				{
				}
				// this operator must be defined
				virtual typename F::BaseType * operator () () const 
				{
					return new T ;
                    // if F::BaseType is not parent or superclass 
                    // of T compilation will fail here >:)
				}
		} ;
	//----------------------------------------------------------
    // return single instance
	// 
	static factory_unit & instance ()
	{
		static factory_unit singleton ;
		return singleton ;
	}
	//
	//return the actual product pointer
    //
    operator T * () const
    {
		try {
		return dynamic_cast<T*>(
                factory_.get( worker_.model_name() ) 
            ) ;
		}
		catch ( std::exception & e ) { throw ; }
    }

private:
	// worker will be making the model required 
	// 
	Producer worker_ ; 
	// 
    F        factory_;
    //
	// 
    factory_unit () 
        : worker_() , 
        factory_(F::instance())
	{
	}
} ;

//------------------------------------------------------------
// using this function we 'marry' factory with 
// new factory_unit, which is returned as a result
// so that callers can use it latter to make instances
// of type T
template< class F, class T >
inline factory_unit<F,T> & add_unit_to_factory( const F & factory_,  T * t_ )
{
    // first of all there is no point to have *t_
    _ASSERTE( 0 == t_ ) ;
    // to understand what is happening here look into the 
    // factory_unit constructor
    return factory_unit<F,T>::instance() ;
}

//////////////////////////////////////////////////////////////////
#if defined( TESTING_DBJSYS_FM_FACTORY )
//////////////////////////////////////////////////////////////////
//Instance of this class locks entry to the method. Unlocking 
//is done automagicaly on destruction.
// 
class MethodLocker 
{
public:
	// 
    MethodLocker() {
    }

	// 
    virtual ~MethodLocker() {
    }
};
//////////////////////////////////////////////////////////////////
// 
struct FakeMethodLocker : public MethodLocker
{
};
//////////////////////////////////////////////////////////////////
// 
class RealMethodLocker : 
public MethodLocker, 
protected dbjsys::fm::GlobalLocker
{
public:
    //
	// 
    RealMethodLocker() {
    }
	// 
    ~RealMethodLocker() {
    }
};

//////////////////////////////////////////////////////////////////
// declaring the factory creates an empty factory
// without factory_units
typedef dbjsys::fm::factory<MethodLocker> MLFactory ;

// 
struct init_method_locking_factory 
{
	// 
    const int counter () {
        static int counter_ = 0 ;
        return counter_ += 1 ;
    }
	// 
    init_method_locking_factory()
    {
        if ( counter() > 1 ) return ;

        MLFactory   & mlf   = MLFactory::instance() ;

        mlf.add_unit((RealMethodLocker*)0 ) ;
        mlf.add_unit((FakeMethodLocker*)0 ) ;
        
        // this will not compile
        // mlf.add_unit((AnyFreesStandingType*)0 ) ;
    }
} ;

static init_method_locking_factory yes_init_now ;
//////////////////////////////////////////////////////////////////
#endif // TESTING_DBJSYS_FM_FACTORY 
//////////////////////////////////////////////////////////////////

//------------------------------------------------------------
} //    namespace fm 
} // namespace dbj 
//------------------------------------------------------------

//------------------------------------------------------------
#endif // defined( _CPPRTTI )
//------------------------------------------------------------

/*EOF FACTORY_DBJ_H*/

