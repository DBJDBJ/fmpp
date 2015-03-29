//*****************************************************************************/
//
//                 Copyright (c)  2000 - 2010 by Dusan B. Jovanovic (dbj@dbj.org) 
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
//--------------------------------------------------------------------------------------
namespace dbjsys {
namespace fm {

	namespace { // hide the following typedef  do not make it "library wide"
#if defined( _UNICODE )
		typedef wchar_t char_type;
		typedef _bstr_t string_type;
#else
#error DBJ*FM++ __FILE__ has to be compiled as UNICODE 
#endif
	}

//--------------------------------------------------------------------------------------
//
// oo wrap up of the command line. ASCII version; uses __argv and __argc.
// _UNICODE version uses __wargv and __argc.
// NOTE:    this works regardless of the VS project type: 
//          console, windows, even DLL app.
//
// Created		DBJ     03122000
// Improved		DBJ     26122005
//
//--------------------------------------------------------------------------------------
/*
    the concept is to make a single command line argument encapsulation 
	whose value is also of an exact static type (not just a string)
	so that in one go user get's the vale from the cli that is also of a required type

	since CLI conceptualy proced only "words" and "numbers" there are two helper typedefs 

	dbjsys::fm::cli_argument_string;    // for words
	dbjsys::fm::cli_argument_long  ;    // for numbers

 Use as this:

	in this example we are expecting cli to have two tags with long values both
	with tags "-n" and "-s", so that cli is for example:

	whatever.exe -age 128 -s 2.0

    fun ( long, long ) ; // to be processed by this function internally

	// def.val must be given as a single ctor argument
	cli_argument_long     age( 12 ) ; 
    cli_argument_long     str_len( 256 ) ;        

	// now the usage is as simple as this
    fun ( age("-age"), str_len("-s") ) ; 

 Usage semantics variant 2: 

	      We do not give tag as a ctor argument so that we have greater flexibility
	      to re-use cl_argument<> instance for 2 or more tags but of the same type.
		  Example of this usage logic:

		  cli_argument_long  long_arg(13);

		  auto age = long_arg(L"-age") ;     // get the -age cli argument of type int
		  auto sln = long_arg(L"-str_len") ; // get the -str_len cli argument of the type int

		  The slight drawback is that 13 is the def.val. for both -age and -length

  Usage semantics variant 3:

         If this is all too confusing one can use a method provided to enforce yet another 
		 usage semantics

		 auto age = cli_argument<long>(L"-age",0);

NOTES: 

intantiation of cl_argument<> is not expensive because it shares a singleton
implementation in the back.

Said implementation is also thread resilient.

*/
template<typename T> class cl_argument
{
	// 
    T defval_ ; // default value
    T reqval_ ; // requested value
	// no default c-tor allowed
	cl_argument () {} 
    public:
		// type of the cli value for the cli tag given
		typedef T Type ;
		// type of the string abstraction used
		typedef string_type String_type;
	// copy constructor must receive a default value for the cli arguments
		explicit
		cl_argument( const T & defval ) : defval_(defval), reqval_(defval)
    {
    }

		const string_type operator [] (const unsigned int  & index) const;

	// return true if given symbol exists on the cmd line
    const bool exists ( const wchar_t  * const cl_symbol ) ;

    // resolve the actual value, and its type, by some cmd.line symbol 
    const T & operator () ( const wchar_t  * const cl_symbol ) ;

	// was the cmd line query made or/and was the last query OK ?
	const bool defval () { return defval_ == reqval_ ; } 
} ;
//----------------------------------------------------------------------------------------------
// common CLI arg types are just these two

typedef dbjsys::fm::cl_argument<string_type>   cli_argument_string;
typedef dbjsys::fm::cl_argument<long>          cli_argument_long  ;

//----------------------------------------------------------------------------------------------
// use this as an universal cli method that enforces slightly different semantics from above
/*
template<class T> 
inline const T cli_argument_val(const wchar_t  * const cl_tag, const T def_val )
{
	cl_argument<T> cliarg_(def_val);
	return cliarg_(cl_tag);
}
*/
// for string_type 
inline const string_type cli_argument_(const wchar_t  * const cl_tag, const string_type & def_val)
{
	cli_argument_string cliarg_(def_val);
	return cliarg_(cl_tag);
}
// for long
inline const long cli_argument_(const wchar_t  * const cl_tag, const long & def_val)
{
	cli_argument_long cliarg_(def_val);
	return cliarg_(cl_tag);
}
// just a "normal" indexed access to the CLI
inline const string_type cli_argument_(unsigned int idx_)
{
	cli_argument_string cliarg_(L"");
	return cliarg_[idx_];
}

//--------------------------------------------------------------------------------------
#if defined ( __test_CmdLineArguments__ )
// usage example
inline long saberi( long a, long b) 
{
    return a + b ;
}
inline const void test_CmdLineArguments()
{
    cl_argument<long>     no_of_elements( 512 ) ;
    cl_argument<long>     str_len( 256 ) ;
	// kukulele is made to prove that we do not need 
	// wchar_t forced instantiation since it is very nicely
	// substituted by _bstr_t instance of cl_argument<T>
	// 
	// so instead of this
	// cl_argument<wchar_t *> kukulele(L"uh") ;
	// declare this
	cl_argument<bstr_t> kukulele(L"uh") ;


	wchar_t * clarg = kukulele(L"-name") ;
    long r = saberi( no_of_elements(L"-n"), str_len(L"-s") ) ;

    // if cl was '-n 1024 -s 512' fun() above will receive 1024 and 512
    // if cl was '-n1024 -s512'   fun() above will receive 1024 and 512
    // if cl was '-n1024'         fun() above will receive 1024 and 256
    // if cl was ''               fun() above will receive  512 and 256
}
#endif
//--------------------------------------------------------------------------------------
} // namespace fm 
} // namespace dbj 
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
namespace dbj { namespace fm  {
//--------------------------------------------------------------------------------------
/*
std::vector based implementation

If you just want to process command line options yourself, the easiest way is to put:

vector<string> args(argv + 1, argv + argc);

at the top of your main(). This copies all command-line arguments into a vector of std::strings. Then you can use == to compare strings easily, 
instead of endless strcmp() calls. For example:

int main(int argc, char **argv) {
vector<string> args(argv + 1, argv + argc);
string infname, outfname;

// Loop over command-line args
// (Actually I usually use an ordinary integer loop variable and compare
// args[i] instead of *i -- don't tell anyone! ;)
for (vector<string>::iterator i = args.begin(); i != args.end(); ++i) {
if (*i == "-h" || *i == "--help") {
cout << "Syntax: foomatic -i <infile> -o <outfile>" << endl;
return 0;
} else if (*i == "-i") {
infname = *++i;
} else if (*i == "-o") {
outfname = *++i;
}
}
}
*/
	class CLI {
	public:
		typedef std::vector<std::wstring> Cli_type;
		typedef Cli_type::iterator Iterator_type;
		typedef Cli_type::iterator::value_type Value_type;
		typedef Value_type char_type;
		//
		CLI()
			: args_( __wargv, __wargv + __argc)
		{
			_ASSERTE( args_.size() );
		}

		const char_type & module_name() { return args_[0] ;  }
		/*
		main extractor : gets by tag and casts into the type desired
		*/
		template<typename T>
		T val (const char_type  &  cl_symbol)
		{
			try {
				Cli_type::iterator result_ = this->find(cl_symbol);

				if (result_ == args_.end()) return T();

				_variant_t vart( (*result_).c_str()) ;

				if (VT_EMPTY != ((VARIANT)vart).vt)
				{
					return T(vart);
				}
			}
			catch (const ::_com_error & cerr_) {
				assert(cerr_.ErrorMessage()); /* conversion failed */
			}
					return T(); // empty value
		}

		char_type val(const char_type  &  cl_symbol) {

			Cli_type::iterator result_ = this->find(cl_symbol);

			return (result_ == args_.end())
				? char_type()
				: *result_;
		}

	private :
		Cli_type args_;
		// not allowed
		CLI(const CLI &) {}
		const CLI & operator = (const CLI &) {}

		Cli_type::iterator find(const char_type  & tag_) {
			// skip the first arg as that is a called module name
			// usually exe in which we are ... 
			Cli_type::iterator flag_ = std::find(++args_.begin(), args_.end(), tag_);
			/*
			flag is found now reach for the value
			*/
			return flag_ == args_.end() ? flag_ : ++flag_ ;
		}
	};
//--------------------------------------------------------------------------------------
} // namespace fm 
} // namespace dbj 
//--------------------------------------------------------------------------------------

