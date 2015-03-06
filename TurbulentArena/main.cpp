//main.cpp

#include "stdafx.h"
#include <vld.h>
#include "Engine.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	argc, argv;

	bjoernligan::system::Engine xEngine;

	if (!xEngine.Initialize())
		return -1;

	xEngine.RunLoop();

	return 0;
}