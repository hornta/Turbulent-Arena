//Engine.cpp

#include "stdafx.h"
#include "Engine.hpp"
#include "ServiceLocator.hpp"
#include "Map.hpp"
#include "Pathfinder.hpp"

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
			m_pathfinder = new Pathfinder(m_map->getSize());
			
			// Set weight and walkable flag for pathfinder
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

			// Example of finding a path with astar
			for (int i = 0; i < 20; ++i)
			{
				Pathfinder::Path path;
				m_pathfinder->setStart(random(0, m_map->getWidth() - 1), random(0, m_map->getHeight() - 1));
				m_pathfinder->setGoal(random(0, m_map->getWidth() - 1), random(0, m_map->getHeight() - 1));
				if (m_pathfinder->findPath(path) == PathfinderInfo::PATHRESULT_SUCCEEDED)
				{
					std::cout << "A*: Path found with length: " << path.length << " and it took " << path.time.asSeconds() << " seconds!" << std::endl;
				}
			}

			// JPS
			for (int i = 0; i < 20; ++i)
			{
				Pathfinder::Path path;
				Pathfinder::Options options;
				options.algorithm = PathfinderInfo::ALGORITHM_JPS;
				options.diagonal = PathfinderInfo::DIAGONAL_NO_OBSTACLES;

				m_pathfinder->setStart(random(0, m_map->getWidth() - 1), random(0, m_map->getHeight() - 1));
				m_pathfinder->setGoal(random(0, m_map->getWidth() - 1), random(0, m_map->getHeight() - 1));
				if (m_pathfinder->findPath(path, options) == PathfinderInfo::PATHRESULT_SUCCEEDED)
				{
					std::cout << "JPS: Path found with length: " << path.length << " and it took " << path.time.asSeconds() << " seconds!" << std::endl;
				}
			}

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
				m_xDrawManager->Draw(m_map);
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