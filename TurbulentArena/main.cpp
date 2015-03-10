//main.cpp

#include "stdafx.h"
INITIALIZE_EASYLOGGINGPP
#include "Engine.hpp"

#if defined(_DEBUG)
#include <vld.h>
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	argc, argv;

	bjoernligan::system::Engine xEngine;

	if (!xEngine.Initialize())
		return -1;

	xEngine.RunLoop();

	return 0;
}