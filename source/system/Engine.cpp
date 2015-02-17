//Engine.cpp

#include "stdafx.h"
#include <system\Engine.hpp>
#include <ServiceLocator.hpp>
#include <Windows.h>

namespace bjoernligan
{
	namespace system
	{
		Engine::Engine()
		{

		}

		Engine::~Engine()
		{

		}

		bool Engine::Initialize()
		{
			m_xDrawManager = DrawManager::Create();
			m_xSpriteManager = SpriteManager::Create();
			m_xKeyboard = input::Keyboard::Create();
			m_xMouse = input::Mouse::Create();

			ServiceLocator<DrawManager>::SetService(m_xDrawManager.get());
			ServiceLocator<SpriteManager>::SetService(m_xSpriteManager.get());
			ServiceLocator<input::Keyboard>::SetService(m_xKeyboard.get());
			ServiceLocator<input::Mouse>::SetService(m_xMouse.get());

			if (!m_xDrawManager->Initialize())
				return false;

			return m_bRunning = true;
		}

		void Engine::CleanUp()
		{

		}

		void Engine::RunLoop()
		{
			while (m_bRunning && m_xDrawManager->m_xWindow->isOpen())
			{
				PollEvents();

				if (m_xKeyboard->IsDown(sf::Keyboard::Escape))
					m_bRunning = false;

				//Updates
				//insert stuff to update

				//Draw
				m_xDrawManager->ClearScr();
				//insert stuff to draw
				m_xDrawManager->Display();

				::Sleep(2);
			}
		}

		void Engine::UpdateDeltaTime()
		{
			m_fDeltaTime = m_xDeltaClock.getElapsedTime().asSeconds();
			if (m_fDeltaTime > 0.02f)
				m_fDeltaTime = 0.02f;
			m_xDeltaClock.restart();
		}

		void Engine::PollEvents()
		{
			sf::Event p_xEvent;
			while (m_xDrawManager->m_xWindow->pollEvent(p_xEvent))
			{
				if (p_xEvent.type == sf::Event::Closed)
					m_xDrawManager->m_xWindow->close();

				if (p_xEvent.type == sf::Event::MouseMoved)
				{
					m_xMouse->m_xPos.x = sf::Mouse::getPosition(*m_xDrawManager->m_xWindow).x;
					m_xMouse->m_xPos.y = sf::Mouse::getPosition(*m_xDrawManager->m_xWindow).y;
				}

				//keyboard keys pressed or released
				if (p_xEvent.type == sf::Event::KeyPressed)
					m_xKeyboard->SetCurrent(p_xEvent.key.code, true);
				if (p_xEvent.type == sf::Event::KeyReleased)
					m_xKeyboard->SetCurrent(p_xEvent.key.code, false);

				//mouse buttons pressed or released
				if (p_xEvent.type == sf::Event::MouseButtonPressed)
					m_xMouse->SetCurrent(p_xEvent.mouseButton.button, true);
				if (p_xEvent.type == sf::Event::MouseButtonReleased)
					m_xMouse->SetCurrent(p_xEvent.mouseButton.button, false);
			}
		}
	}
}