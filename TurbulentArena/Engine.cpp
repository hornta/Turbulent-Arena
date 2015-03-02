//Engine.cpp

#include "stdafx.h"
#include "Engine.hpp"
#include "ServiceLocator.hpp"
#include "Map.hpp"
#include "Pathfinder.hpp"
#include "Object.hpp"
#include "Visibility.hpp"
#include "Physics.hpp"
#include "Settings.hpp"
#include "PlayState.hpp"

#include "ClanManager.hpp"
#include "Clan.hpp"
#include "Scout.hpp"
#include "Axeman.hpp"

#include "Physics.hpp"
#include "Settings.hpp"
#include "UISlider.hpp"
#include "UIButton.hpp"
#include <Windows.h>

#include <iomanip>

namespace bjoernligan
{
	namespace system
	{
		Engine::Engine()
			: m_xStateManager(nullptr)
			, m_xDrawManager(nullptr)
			, m_xSpriteManager(nullptr)
			, m_xAudioManager(nullptr)
			, m_xUIManager(nullptr)
			, m_xKeyboard(nullptr)
			, m_xMouse(nullptr)
			, m_xUtility(nullptr)
			, m_xDebugWindow(nullptr)
		{

		}

		Engine::~Engine()
		{

		}

		bool Engine::Initialize()
		{
			m_xStateManager = GameStateManager::Create();
			m_xDrawManager = DrawManager::Create();
			m_xSpriteManager = SpriteManager::Create();
			m_xAudioManager = AudioManager::Create();
			m_xKeyboard = input::Keyboard::Create();
			m_xMouse = input::Mouse::Create();
			m_xUtility = Utility::Create();
			m_xUIManager = UIManager::Create();
			m_xDebugWindow = DebugWindow::Create(false);

			ServiceLocator<GameStateManager>::SetService(m_xStateManager.get());
			ServiceLocator<DrawManager>::SetService(m_xDrawManager.get());
			ServiceLocator<SpriteManager>::SetService(m_xSpriteManager.get());
			ServiceLocator<UIManager>::SetService(m_xUIManager.get());
			ServiceLocator<input::Keyboard>::SetService(m_xKeyboard.get());
			ServiceLocator<input::Mouse>::SetService(m_xMouse.get());
			ServiceLocator<Utility>::SetService(m_xUtility.get());
			ServiceLocator<AudioManager>::SetService(m_xAudioManager.get());
			ServiceLocator<DebugWindow>::SetService(m_xDebugWindow.get());

			m_xSpriteManager->setTexturePath("../data/sprites/");

			m_xAudioManager->CreateSoundBuffer("Explode", "explode_0.wav");
			m_xAudioManager->CreateMusic("Battle", "dragons_lair.ogg");
			m_xAudioManager->PlayMusic("Battle");

			if (!m_xDrawManager->Initialize())
				return false;
			m_xDrawManager->getWindow()->create(sf::VideoMode(Settings::m_xWindowSize.x, Settings::m_xWindowSize.y), "Turbulent Arena"/*, sf::Style::None*/);

			if (!m_xDebugWindow->Initialize())
				return false;
			m_xDebugWindow->SetPos(16.0f, 16.0f);

			if (!m_xStateManager->Initialize())
				return false;
			m_xStateManager->CreateState<PlayState>("PlayState");

			return true;
		}

		void Engine::RunLoop()
		{
			while (m_xDrawManager->getWindow()->isOpen())
			{
				//Poll
				PollEvents();

				//Updates
				UpdateDeltaTime();
				m_xStateManager->UpdateStates(m_fDeltaTime);

				//Draw
				m_xDrawManager->ClearScr();
				m_xStateManager->draw(*m_xDrawManager->getWindow(), sf::RenderStates::Default);
				m_xDrawManager->getWindow()->setView(m_xUIManager->getView());
				m_xDrawManager->Draw(m_xUIManager.get());
				m_xDrawManager->Display();

				//Post-updates
				m_xMouse->PostUpdate();
				m_xKeyboard->PostUpdate();
			}
		}

		void Engine::UpdateDeltaTime()
		{
			m_fDeltaTimeRaw = m_fDeltaTime = m_xDeltaClock.getElapsedTime().asSeconds();
			if (m_fDeltaTime > 0.02f)
				m_fDeltaTime = 0.02f;
			m_xDeltaClock.restart();
		}

		void Engine::PollEvents()
		{
			sf::Event p_xEvent;
			while (m_xDrawManager->getWindow()->pollEvent(p_xEvent))
			{
				if (p_xEvent.type == sf::Event::Closed)
					m_xDrawManager->getWindow()->close();

				if (p_xEvent.type == sf::Event::MouseMoved)
					m_xMouse->m_xPos = sf::Mouse::getPosition(*m_xDrawManager->getWindow());

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