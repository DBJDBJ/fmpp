//*****************************************************************************/
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
#include "fm.h"
//--------------------------------------------------------------------------------------
namespace dbjsys {
namespace fm {

	namespace { // hide the following typedef  do not make it "library wide"
#if defined( _UNICODE )
		typedef wchar_t char_type;
		typedef _bstr_t string_type;
#else
//		#warning DBJ*FM++ __FILE__ has to be compiled as UNICODE 
		typedef wchar_t char_type;
		typedef _bstr_t string_type;
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
namespace dbjsys { namespace fm  {
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
		class Error { public: const wchar_t * name() { return L"dbj::fm::CLI::Error"; } };
		class NotFound : public Error { 
			NotFound() {}
		public:
			std::wstring msg_;
			NotFound(const std::wstring & m_) : msg_(m_) {}
			const std::wstring what() { return std::wstring(name()) + L" not found " + msg_ ; } 
		};
		//
		CLI()
			: args_( __wargv, __wargv + __argc)
		{
			_ASSERTE( args_.size() );
		}

		/*
		main extractor : gets by tag and casts into the type desired
		*/
		template<typename T>
		T val (const char_type  &  cl_symbol)
		{
			try {
				Cli_type::iterator result_ = this->find(cl_symbol);

				if (result_ == args_.end())
					throw new CLI::NotFound(cl_symbol);

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

			if (result_ == args_.end())
				throw new CLI::NotFound(cl_symbol);

			return *result_;
		}

		// extract the module name inside which we are running
		const char_type & val (unsigned int idx_ ) const { return args_[idx_]; }

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
	//----------------------------------------------------------------------------------------------
	// use this as an universal cli method 
	/**/
	template<class T>
	inline const T clargument(const wchar_t  * const cl_tag, const T & def_val)
	{
		CLI cliarg_ ;
			try {
				return cliarg_.val<T>(cl_tag);
			}
			catch (CLI::NotFound &){
				return def_val;
		}
	}
	/**/
	// for string_type 
	inline const CLI::Value_type clargument(const wchar_t  * const cl_tag, const CLI::Value_type & def_val)
	{
		CLI cliarg_ ;
		try {
			return cliarg_.val(cl_tag);
		}
		catch (CLI::NotFound &){
			return def_val;
		}
	}
	// for long
	inline const long clargument(const wchar_t  * const cl_tag, const long & def_val)
	{
		CLI cliarg_;
		try {
			return cliarg_.val<long>(cl_tag);
		}
		catch (CLI::NotFound &){
			return def_val;
		}
	}
	/*
	// for bool
	inline const long clargument(const wchar_t  * const cl_tag, const bool & def_val)
	{
		CLI cliarg_;
		try {
			return cliarg_.val<bool>(cl_tag);
		}
		catch (CLI::NotFound &){
			return def_val;
		}
	}
	*/
	// just a "normal" indexed access to the CLI
	inline const CLI::Value_type clargument(unsigned int idx_ = 0)
	{
		CLI cliarg_;
		return cliarg_.val(idx_);
	}
	//--------------------------------------------------------------------------------------
} // namespace fm 
} // namespace dbj 
/*
 *--------------------------------------------------------------------------------------
Implementaion from cpp moved bellow
 */
namespace dbjsys {
	namespace fm {
		//--------------------------------------------------------------------------------------
		namespace {

#if defined( _UNICODE )
			typedef wchar_t char_type;
#else
#error DBJ*FM++ __FILE__ has to be compiled as UNICODE 
#endif

			// oo wrap up of the command line. ASCII version; uses __argv and __argc.
			// _UNICODE version uses __wargv and __argc.
			// NOTE:    this works regardless of the VS project type: 
			//          console, windows, even DLL app.
			//
			// Created  DBJ     03122000
			//
			// This is singleton as implementation
			// It is also thread safe
			// str::string or std:wstring are not used here
			// this depends on comutil.h
			//
			// 
			class CmdLineArguments {

				// 
				ScopeLocker critical_section__;

				//--------------------------------------------------------
				// return true it str begin's with prefix
				// 
				static const bool begins_with(const char_type * prefix, const char_type * str)
				{
					bool result_ = true;
					for (int j = 0; prefix[j] != 0; j++)
					{
						result_ = result_ && (prefix[j] == str[j]);
					}

					return result_;
				}
				//--------------------------------------------------------
				// return string part on the right of prefix given
				// we can assume that both arguments begin with the prefix string
				// and that prefix is shorter than str
				// 
				static const char_type * right_of(const char_type * prefix, const char_type * str)
				{
					int j = 0;
					while ((prefix[j]) && (prefix[j] == str[j])) j++;
					return str + j;
				}

				//--------------------------------------------------------
				// 
				char_type ** the_cli_arguments;
			public:
				// 
				int      number_of_arguments;
				//
				// UNICODE version only!
				CmdLineArguments()
					: the_cli_arguments(__wargv), number_of_arguments(__argc)
				{
					_ASSERTE(the_cli_arguments);
				}

				// 
				~CmdLineArguments()
				{
					the_cli_arguments = 0;
				}

				// return true if symbol exist on the current command line
				const bool symbol_exists(const char_type * prefix)
				{
					Lock auto_lock(critical_section__); // lock the whole instance

					const bool result_ = false; // default

					if ((!prefix) && (!*prefix))
						return result_; // anti jokers measure

					register int j = 0;

					const char_type * candidate_ = NULL;

					while (NULL != (candidate_ = operator [] (j++)))
					{
						if (!begins_with(prefix, candidate_))
							continue;
						return true; // FOUND!
					}
					return result_;
				}
				// 
				const char_type * operator [] (const int index) const
				{
					Lock auto_lock(critical_section__); // lock the whole instance
					if (index < 0) return NULL;
					if (index >(number_of_arguments - 1)) return NULL;
					return the_cli_arguments[index];
				}
				//
				// return argument value of the argument that begins with a prefix
				// return NULL if prefix not found
				// argument prefix and value may have a space in between them, eg.:
				// "-f output.txt"
				// or not, eg.: "-foutput.txt"
				// In either case the same value shall be returned
				// In case of illegal cli rubish will be returned, eg.:
				// "-f -eoutput.err"  will return "-eoutput.err" if "-f" was the prefix given.
				// "-f <eol>"  will return "" if "-f" was the prefix given.
				// 
				//
				// 
				const char_type * operator [] (const char_type * prefix) const
				{
					Lock auto_lock(critical_section__); // lock the whole instance

					const char_type * result_ = (char_type*)0;

					if ((!prefix) && (!*prefix))
						return result_; // anti jokers measure

					register int j = 0;

					while (NULL != (result_ = operator [] (j++)))
					{
						if (!begins_with(prefix, result_))
							continue;
						result_ = right_of(prefix, result_);
						if ((result_) && (*result_))
							break;
						result_ = operator [] (j); // next argument
						break;
					}
					return result_;
				}
				//------------------------------------------------------------
				// return argument found by its name
				// ret type is variant
				_variant_t operator () (const char_type * arg_name_) const
				{
					const char_type * cl_str = this->operator [] (arg_name_);
					if ((cl_str != 0) && (cl_str[0] != 0))
						return _variant_t(cl_str);
					else
						return _variant_t(); // VT_EMPTY
				}
				//------------------------------------------------------------
				// the singleton method delivers one instance for all threads
				// I think we need no locking here ? DBJ 08042001
				// 
				static CmdLineArguments & instance()
				{
					static CmdLineArguments instance__;
					return instance__;
				}

			};

			CmdLineArguments & cline_ = CmdLineArguments::instance();
		}

		//--------------------------------------------------------------------------------------

		template<typename T>
		const string_type cl_argument<T>::operator [] (const unsigned int & index) const
		{
			return string_type(cline_[index]);
		}


		//--------------------------------------------------------------------------------------
		//
		//    cl_argument encapsulates single comand line argument. 
		//--------------------------------------------------------------------------------------
		template<typename T>
		const bool cl_argument<T>::exists(const char_type  * const cl_symbol)
		{
			return cline_.symbol_exists(cl_symbol);
		}
		//--------------------------------------------------------------------------------------
		//
		// resolve the actual value and its type by c.l. symbol used 
		//
		//--------------------------------------------------------------------------------------
		template<typename T>
		const T & cl_argument<T>::operator () (const char_type  * const cl_symbol)
		{
			try {
				_variant_t vart = cline_(cl_symbol);

				if (VT_EMPTY != ((VARIANT)vart).vt)
				{
					this->reqval_ = (T)vart;
				}
				else {
					this->reqval_ = this->defval_;
				}
			}
			catch (const ::_com_error & cerr_) {
				assert(cerr_.ErrorMessage()); /* conversion failed */
			}
			return this->reqval_;
		}
		//--------------------------------------------------------------------------------------
		//
		// in header we have this two typedefs
		//
		//	typedef dbjsys::fm::cl_argument<_bstr_t>   cli_argument_string ;
		//	typedef dbjsys::fm::cl_argument<long>      cli_argument_long;
		//
		// explicit instantiations
		//
		// Whenever code that uses FM and some cl_argument<T> does not link we have
		// to add cl_argument<> specialization for T here. This is not so bad because
		// number of different types T is small in the context of command line arguments. 
		// NOTE: since implementation of cl_argument uses variant_t, it depends on its
		// ability to do the conversion. Therefore you can not specialize here what 
		// variant_t can not convert and deliver as a result of one of its extractors.
		// DBJ 08042001
		DBJFMTXP template DBJFMAPI class cl_argument<long>;
		DBJFMTXP template DBJFMAPI class cl_argument<bstr_t>;
		// 
		// CAUTION:
		// Do NOT do the following :
		// DBJFMTXP template DBJFMAPI class cl_argument<wchar_t *> ;
		// If you do you will require a specialization for the wchar_t, because
		// cl_argument<wchar_t>::operator () ( .... )
		// can not be implemented as it is above. But all of this is NOT necessary
		// since cl_argument<bstr_t>, covers wchar_t uses perfectly well ...
		//--------------------------------------------------------------------------------------
	} // namespace fm 
} // namespace dbj 
