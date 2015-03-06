//Engine.hpp

#pragma once
#include "DrawManager.hpp"
#include "SpriteManager.hpp"
#include "AudioManager.hpp"
#include "UIManager.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "Utility.hpp"
#include "DebugWindow.hpp"

namespace bjoernligan
{
	class Map;
	class ClanManager;
	class Physics;

	namespace ai
	{
		class Sense;
	}

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
			AudioManager::Ptr m_xAudioManager;
			DrawManager::Ptr m_xDrawManager;
			UIManager::Ptr m_xUIManager;
			input::Keyboard::Ptr m_xKeyboard;
			input::Mouse::Ptr m_xMouse;
			Utility::Ptr m_xUtility;
			DebugWindow::Ptr m_xDebugWindow;

			sf::View m_view;
			sf::Vector2f m_lastRightClick;
			sf::Clock m_xDeltaClock;
			float m_fDeltaTime, m_fDeltaTimeRaw;

			std::unique_ptr<ai::Sense> m_sense;
			std::unique_ptr<Map> m_map;
			std::unique_ptr<ClanManager> m_clanManager;
			std::unique_ptr<Physics> m_physics;
		};
	}
}