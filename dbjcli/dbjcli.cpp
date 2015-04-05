// dbjcli.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include  "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\crtdbg.h"
#include <wincon.h>


// include the FM++
#include "../fm/cmdline.h"
#include "../fm/errortplt.h"
#include "../fm/algo.h"

namespace dbj {
	namespace con {

		class DBJ_CON {
		public:
			typedef dbjsys::fm::Win32Error<DBJ_CON> Err;
		};

		typedef DBJ_CON::Err Err;

		inline void set_title(const _bstr_t & new_title_ = _bstr_t("UNKNOWN")) throw(Err)
		{
			HWND this_hwnd = GetConsoleWindow();

			dbjTHROWIF(NULL == this_hwnd, Err);

			dbjTHROWIF(
				! SetWindowText(this_hwnd, new_title_),Err
				);
		}

		enum STD_COLOR : unsigned short { BLACK = 0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14, WHITE };

		inline void set_color(STD_COLOR color)
		{
			HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hcon, color);
		}

		/*
		https://msdn.microsoft.com/en-us/library/windows/desktop/ms686028%28v=vs.85%29.aspx

		Value	Meaning
		CONSOLE_FULLSCREEN_MODE 		1		Text is displayed in full - screen mode.
		CONSOLE_WINDOWED_MODE   		2

		*/

		enum CONSOLE_MODE : DWORD { FULLSCREEN = 1, WINDOWED = 2 };

		inline void set_consoledisplaymode(
			CONSOLE_MODE  dwFlags,
			PCOORD lpNewScreenBufferDimensions = (PCOORD)NULL
			)
		{
			HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

			dbjTHROWIF(INVALID_HANDLE_VALUE == hConsoleOutput, Err);
			dbjTHROWIF(NULL                 == hConsoleOutput, Err);
			/*
			*/

			if (
				!SetConsoleDisplayMode(
				/*_In_       HANDLE */ hConsoleOutput,
				/*_In_       DWORD  */ dwFlags,
				/*_Out_opt_  PCOORD */ lpNewScreenBufferDimensions
				)
				){
				DBJ_CON::Err::report_only(::GetLastError(), __FILE__, __LINE__);
			}

		}

	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////

int wmain(int argc, wchar_t ** argv)
{
	using namespace dbj;
	using namespace dbjsys;

	try {
		con::set_consoledisplaymode(con::CONSOLE_MODE::WINDOWED);
		con::set_title("DBJ TESTING CONSOLE");

		con::set_color(con::STD_COLOR::C13);

		auto module_name = fm::clargument();

		// extractor of string command line argument
		// with default value to be used if no tag exist
		auto age = fm::clargument(L"-age", 0);

		auto name = fm::clargument(L"-name", L"Unknown");

		auto trained = fm::clargument(L"-trained", false);
	}
	catch (::dbj::con::Err & dbj_con_err_ ){

		dbj_con_err_.report(true);
	}
		catch (...){
        		fm::DBJSYSError::err_msg_box("Unhandled Exception");
	}

	return 0;
}

