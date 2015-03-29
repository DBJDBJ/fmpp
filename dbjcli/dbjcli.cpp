// dbjcli.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// include the FM++
#include "../fm/cmdline.h"


int wmain(int argc, wchar_t ** argv)
{
	dbj::fm::CLI  cli;

	auto this_name = cli.module_name();

	long			qm		= cli.val<long>(L"-age");
	std::wstring	qm2		= cli.val(L"-name");
	bool			trained = cli.val<bool>(L"-trained");

	return 0;
}

