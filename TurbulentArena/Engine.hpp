//Engine.hpp

#pragma once
#include "DrawManager.hpp"
#include "SpriteManager.hpp"
#include "GameStateManager.hpp"
#include "AudioManager.hpp"
#include "UIManager.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "Utility.hpp"
#include "DebugWindow.hpp"

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
			void RunLoop();

		private:
			void UpdateDeltaTime();
			void PollEvents();

			SpriteManager::Ptr m_xSpriteManager;
			GameStateManager::Ptr m_xStateManager;
			AudioManager::Ptr m_xAudioManager;
			DrawManager::Ptr m_xDrawManager;
			UIManager::Ptr m_xUIManager;
			input::Keyboard::Ptr m_xKeyboard;
			input::Mouse::Ptr m_xMouse;
			Utility::Ptr m_xUtility;
			DebugWindow::Ptr m_xDebugWindow;

			sf::Vector2f m_lastRightClick;
			sf::Clock m_xDeltaClock;
			float m_fDeltaTime, m_fDeltaTimeRaw;
		};
	}
}