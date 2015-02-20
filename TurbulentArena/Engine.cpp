//Engine.cpp

#include "stdafx.h"
#include "Engine.hpp"
#include "ServiceLocator.hpp"
#include "Map.hpp"
#include "Pathfinder.hpp"
#include "Visibility.hpp"

#include "Clan.hpp"
#include "Scout.hpp"

namespace bjoernligan
{
	namespace system
	{
		Engine::Engine()
			: m_map(nullptr),
			m_pathfinder(nullptr)
		{
			m_xDrawManager = nullptr;
			m_xSpriteManager = nullptr;
			m_xKeyboard = nullptr;
			m_xMouse = nullptr;
			m_xUtility = nullptr;
			m_xB2World = nullptr;
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
			m_xUtility = Utility::Create();

			ServiceLocator<DrawManager>::SetService(m_xDrawManager.get());
			ServiceLocator<SpriteManager>::SetService(m_xSpriteManager.get());
			ServiceLocator<input::Keyboard>::SetService(m_xKeyboard.get());
			ServiceLocator<input::Mouse>::SetService(m_xMouse.get());
			ServiceLocator<Utility>::SetService(m_xUtility.get());

			if (!m_xDrawManager->Initialize())
				return false;

			m_xB2World = new b2World(b2Vec2(0.0f, 0.0f));
			
			m_map = new Map("../data/map.txt");

			// PATHFINDER
			m_pathfinder = new Pathfinder(m_map->getSize());
			for (int x = 0; x < m_map->getWidth(); ++x)
			{
				for (int y = 0; y < m_map->getHeight(); ++y)
				{
					Map::Tile* tile = m_map->getTopmostTile(x, y);
					if (tile != nullptr)
					{
						m_pathfinder->getGrid().setWalkableAt(x, y, tile->isWalkable());
					}
				}
			}

			// VISIBILITY
			m_visibility = new Visibility();

			Map::Layer* layer = m_map->getLayer("objects");
			if (layer != nullptr)
			{
				float tileSize = static_cast<float>(m_map->getTileSize());
				for (int x = 0; x < m_map->getWidth(); ++x)
				{
					for (int y = 0; y < m_map->getHeight(); ++y)
					{
						Map::Tile* tile = layer->getTile(x, y);
						if (tile != nullptr)
						{
							sf::Vector2f tilePos = sf::Vector2f(tile->getPosition());
							m_visibility->addSegment(sf::Vector2f(tilePos.x * tileSize, tilePos.y * tileSize), sf::Vector2f((tilePos.x + 1.f) * tileSize, tilePos.y * tileSize));
							m_visibility->addSegment(sf::Vector2f((tilePos.x + 1.f) * tileSize, tilePos.y * tileSize), sf::Vector2f((tilePos.x + 1.f) * tileSize, (tilePos.y + 1.f) * tileSize));
							m_visibility->addSegment(sf::Vector2f((tilePos.x + 1.f) * tileSize, (tilePos.y + 1.f) * tileSize), sf::Vector2f(tilePos.x * tileSize, (tilePos.y + 1.f) * tileSize));
							m_visibility->addSegment(sf::Vector2f(tilePos.x * tileSize, (tilePos.y + 1.f) * tileSize), sf::Vector2f(tilePos.x * tileSize, tilePos.y * tileSize));
						}
					}
				}
			}
			
			// todo: move to an object manager
			// CLANS
			Clan* clan_mcHeist = new Clan(sf::Color::Blue);
			Clan* clan_mcPlank = new Clan(sf::Color::Red);

			{
				Class* scout = new Scout();
				clan_mcHeist->addMember(scout);
			}

			clan_mcHeist->addMember(new Scout);

			m_clans.push_back(clan_mcHeist);
			m_clans.push_back(clan_mcPlank);

			return m_bRunning = true;
		}

		void Engine::CleanUp()
		{
			delete m_xB2World;
			m_xB2World = nullptr;

			delete m_map;
			m_map = nullptr;

			delete m_pathfinder;
			m_pathfinder = nullptr;

			delete m_visibility;
			m_visibility = nullptr;
		}

		void Engine::RunLoop()
		{
			while (m_bRunning && m_xDrawManager->m_xWindow->isOpen())
			{
				PollEvents();

				if (m_xKeyboard->IsDown(sf::Keyboard::Escape))
					m_bRunning = false;

				//Updates
				m_visibility->update();

				//Draw
				m_xDrawManager->ClearScr();
				//insert stuff to draw
				m_xDrawManager->Draw(m_map);
				m_xDrawManager->Draw(m_visibility);
				m_xDrawManager->Display();
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
					m_xMouse->m_xPos = sf::Mouse::getPosition(*m_xDrawManager->m_xWindow);
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