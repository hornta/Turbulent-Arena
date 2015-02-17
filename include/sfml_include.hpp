//sfml_include.hpp

#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>

#if defined(_DEBUG) || defined(DEBUG)
	#if defined(SFML_STATIC)
		#pragma comment(lib,"../external/SFML/lib/sfml-graphics-s-d.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-window-s-d.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-system-s-d.lib")
	#else
		#pragma comment(lib,"../external/SFML/lib/sfml-graphics-d.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-window-d.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-system-d.lib")
	#endif
	#pragma comment(lib,"../external/SFML/lib/sfml-main-d.lib")
#else
	#if defined(SFML_STATIC)
		#pragma comment(lib,"../external/SFML/lib/sfml-graphics-s.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-window-s.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-system-s.lib")
	#else
		#pragma comment(lib,"../external/SFML/lib/sfml-graphics.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-window.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-system.lib")
	#endif
	#pragma comment(lib,"../external/SFML/lib/sfml-main.lib")
#endif

