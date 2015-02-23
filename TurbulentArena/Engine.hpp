//Engine.hpp

#pragma once
#include "DrawManager.hpp"
#include "SpriteManager.hpp"
#include "UIManager.hpp"
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
			void updateCamera();
			void SetDebugMode(const bool &p_bValue);
			void SetScrollSpeed(const float &p_fNewSpeed);

			bool m_bRunning;
			float m_fScrollSpeed;
			SpriteManager::Ptr m_xSpriteManager;
			DrawManager::Ptr m_xDrawManager;
			UIManager::Ptr m_xUIManager;
			input::Keyboard::Ptr m_xKeyboard;
			input::Mouse::Ptr m_xMouse;
			Utility::Ptr m_xUtility;

			sf::View m_view;
			sf::Vector2f m_lastRightClick;
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