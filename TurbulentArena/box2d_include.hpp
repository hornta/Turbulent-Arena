//box2d_include.hpp

#pragma once

//#pragma error(disable:2038)

#include <Box2D\Box2D.h>

#if defined(_DEBUG) || defined(DEBUG)
	#pragma comment(lib, "../external/Box2D/lib/Box2D-d.lib")
#else
	#pragma comment(lib, "../external/Box2D/lib/Box2D.lib")
#endif