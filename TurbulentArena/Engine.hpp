//Engine.hpp

#pragma once
#include "DrawManager.hpp"
#include "SpriteManager.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "Utility.hpp"

namespace bjoernligan
{
	class Map;
	class Pathfinder;
	class Visibility;
	class ClanManager;
	class Physics;

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

			std::unique_ptr<Visibility> m_visibility;
			std::unique_ptr<Pathfinder> m_pathFinder;
			std::unique_ptr<Map> m_map;
			std::unique_ptr<ClanManager> m_clanManager;
			std::unique_ptr<Physics> m_physics;
		};
	}
}