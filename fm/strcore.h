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

/////////////////////////////////////////////////////////////////////////////
namespace dbjsys {
namespace fm {
    ///////////////////////////////////////////////////////////////////////////////
    #undef THIS_FILE
    #undef VERIFY
    #undef ASSERT
    //static TCHAR THIS_FILE[] = _T(__FILE__);
    //#define ASSERT(x) if( ! (x) ) \
    //throw _T("Runtime error: ") _T(#x) _T(", at: ") _T(__FILE__)
    //#define VERIFY(x) ASSERT(x) 
    // UNICODE-ANSI conversion helpers (these use the current system locale)
/*
    ///////////////////////////////////////////////////////////////////////////////
    inline size_t wcstombs(char* mbstr, const wchar_t* wcstr, size_t count)
    {
	    if (count == 0 && mbstr != NULL)
		    return 0;

	    size_t result = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1,
		    mbstr, count, NULL, NULL);
	    ASSERT(mbstr == NULL) ;
        ASSERT( result <= count );
	    if (result > 0)
		    mbstr[result-1] = 0;
	    return result;
    }

    ///////////////////////////////////////////////////////////////////////////////
    inline size_t mbstowcs(wchar_t* wcstr, const char* mbstr, size_t count)
    {
	    if (count == 0 && wcstr != NULL)
		    return 0;

	    size_t result = ::MultiByteToWideChar(CP_ACP, 0, mbstr, -1,
		    wcstr, count);
	    ASSERT(wcstr == NULL || result <= (int)count);
	    if (result > 0)
		    wcstr[result-1] = 0;
	    return result;
    }

    ///////////////////////////////////////////////////////////////////////////////
    inline LPWSTR  a2w(LPWSTR lpw, LPCSTR lpa, int nChars)
    {
	    if (lpa == NULL)
		    return NULL;
	    ASSERT(lpw != NULL);
	    // verify that no illegal character present
	    // since lpw was allocated based on the size of lpa
	    // don't worry about the number of chars
	    lpw[0] = '\0';
	    VERIFY(MultiByteToWideChar(CP_ACP, 0, lpa, -1, lpw, nChars));
	    return lpw;
    }

    ///////////////////////////////////////////////////////////////////////////////
    inline LPSTR  w2a(LPSTR lpa, LPCWSTR lpw, int nChars)
    {
	    if (lpw == NULL)
		    return NULL;
	    ASSERT(lpa != NULL);
	    // verify that no illegal character present
	    // since lpa was allocated based on the size of lpw
	    // don't worry about the number of chars
	    lpa[0] = '\0';
	    VERIFY(WideCharToMultiByte(CP_ACP, 0, lpw, -1, lpa, nChars, NULL, NULL));
	    return lpa;
    }
    //////////////////////////////////////////////////////////////
    // As VC6 STL does not give a fuck about UNICODE the following 
    // is required to transform from-to wide and multi-byte strings

    ////////////////////////////////////////////////////////////////
    // convert ansi char array to wide char array
    inline LPWSTR A2W( LPCSTR lpa) 
    {
        register _convert = 0 ;
        return ( lpa == NULL) ? NULL : 
        (
               _convert = (strlen(lpa)+1),
               a2w((LPWSTR)alloca(_convert*2), lpa, _convert)
        ) ;
    }
    ////////////////////////////////////////////////////////////////
    // convert wide char array to ansi char array
    inline LPCSTR W2A( LPCWSTR  lpw) 
    {
        register _convert = 0 ;
        return ( lpw == NULL) ? NULL : 
        (
               _convert = wcslen(lpw)+1 ,
               w2a((LPSTR)alloca(_convert*2), lpw, _convert)
        ) ;
    }
    ////////////////////////////////////////////////////////////////
    // convert std::string to std::wstring
    inline std::wstring s2w( const std::string & std_str )
    {
        return std::wstring( 
                A2W(std_str.c_str())
            ) ;
    }
    ////////////////////////////////////////////////////////////////
    // convert std::wstring to std::string
    inline std::string w2s( const std::wstring & w_str )
    {
        return std::string(
                W2A(w_str.c_str())
              ) ;
    }
    ////////////////////////////////////////////////////////////////
#ifdef UNICODE
    // convert tstring to std::string
    inline std::string t2s(const dbjsys::fm::tstring & tstr )
    {
        return std::string(W2A(tstr.c_str())) ;
    }
    // convert std::string to tstring
    inline dbjsys::fm::tstring  s2t( const std::string & std_str )
    {
        return tstring(A2W(std_str.c_str())) ;
    }
#else
    // convert tstring to std::string
    inline std::string t2s(const tstring & tstr )
    {
        return std::string(tstr.c_str()) ;
    }
    // convert std::string to tstring
    inline dbjsys::fm::tstring  s2t( const std::string & std_str )
    {
        return dbjsys::fm::tstring(std_str.c_str()) ;
    }
#endif
/////////////////////////////////////////////////////////////////////////////
inline void 
tstring_dump( 
     const dbjsys::fm::tstring & ts,
     std::basic_ostream<TCHAR, std::char_traits<TCHAR> > * osp = 0 
)
{
    if ( osp == 0 )
        ::OutputDebugString( ts.c_str() ) ;
    else
        (*osp) << ts.c_str ;
}
*/
/////////////////////////////////////////////////////////////////////////////
#undef VERIFY
#undef ASSERT
#undef THIS_FILE
///////////////////////////////////////////////////////////////////////////////
} // namespace fm 
} // namespace dbjsys 
///////////////////////////////////////////////////////////////////////////////
