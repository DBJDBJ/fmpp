// dbjcli.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// include the FM++
#include "../fm/cmdline.h"


int wmain(int argc, wchar_t ** argv)
{
	using namespace dbjsys;

	auto module_name = dbjsys::fm::clargument();

	// extractor of string command line argument
	// with default value to be used if no tag exist
	auto age = fm::clargument(L"-age", 0);

	auto name = fm::clargument(L"-name", L"Unknown");

	auto trained = fm::clargument(L"-trained", false);

	return 0;
}

