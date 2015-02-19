//sfml_include.hpp

#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Network.hpp>

#if defined(_DEBUG) || defined(DEBUG)
	#if defined(SFML_STATIC)
		#pragma comment(lib,"../external/SFML/lib/sfml-graphics-s-d.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-window-s-d.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-system-s-d.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-audio-s-d.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-network-s-d.lib")
	#else
		#pragma comment(lib,"../external/SFML/lib/sfml-graphics-d.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-window-d.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-system-d.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-audio-d.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-network-d.lib")
	#endif
	#pragma comment(lib,"../external/SFML/lib/sfml-main-d.lib")
#else
	#if defined(SFML_STATIC)
		#pragma comment(lib,"../external/SFML/lib/sfml-graphics-s.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-window-s.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-system-s.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-audio-s.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-network-s.lib")
	#else
		#pragma comment(lib,"../external/SFML/lib/sfml-graphics.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-window.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-system.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-audio.lib")
		#pragma comment(lib,"../external/SFML/lib/sfml-network.lib")
	#endif
	#pragma comment(lib,"../external/SFML/lib/sfml-main.lib")
#endif

#if defined(SFML_STATIC)
	#pragma comment(lib, "winmm.lib")
	#pragma comment(lib, "ws2_32.lib")
	#pragma comment(lib, "../external/SFML/lib/sndfile.lib")
	#pragma comment(lib, "../external/SFML/lib/openal32.lib")
	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "gdi32.lib")
	#pragma comment(lib, "../external/SFML/lib/glew.lib")
	#pragma comment(lib, "../external/SFML/lib/freetype.lib")
	#pragma comment(lib, "../external/SFML/lib/jpeg.lib")
#endif

