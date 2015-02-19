//Engine.hpp

#pragma once
#include "DrawManager.hpp"
#include "SpriteManager.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "Utility.hpp"
#include <Box2D\box2d.h>

namespace bjoernligan
{
	class Map;
	class Pathfinder;
	namespace system
	{
		class Engine
		{
		public:
			Engine();
			~Engine();

			bool Initialize();
			void CleanUp();
			void RunLoop();

		private:
			void UpdateDeltaTime();
			void PollEvents();

			bool m_bRunning;
			SpriteManager::Ptr m_xSpriteManager;
			DrawManager::Ptr m_xDrawManager;
			input::Keyboard::Ptr m_xKeyboard;
			input::Mouse::Ptr m_xMouse;
			Utility::Ptr m_xUtility;

			sf::Clock m_xDeltaClock;
			float m_fDeltaTime;

			Map* m_map;
			Pathfinder* m_pathfinder;

			//Box2D
			b2World* m_xB2World;			
		};
	}
}