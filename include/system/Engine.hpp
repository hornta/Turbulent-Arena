//Engine.hpp

#pragma once
#include <system/DrawManager.hpp>
#include <system/SpriteManager.hpp>
#include <input/Mouse.hpp>
#include <input/Keyboard.hpp>

namespace bjoernligan
{
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

			sf::Clock m_xDeltaClock;
			float m_fDeltaTime;
		};
	}
}