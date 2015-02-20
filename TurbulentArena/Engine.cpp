//Engine.cpp

#include "stdafx.h"
#include "Engine.hpp"
#include "ServiceLocator.hpp"
#include "Map.hpp"
#include "Pathfinder.hpp"
#include "ContactListener.hpp"
#include "Box2DWorldDraw.h"
#include "Object.hpp"
#include <iomanip>
#include <Windows.h>

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
			m_xContactListener = nullptr;
			mB2DebugDraw = nullptr;
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
			m_xB2World = new b2World(b2Vec2(0.0f, 5.0f));
			m_map = new Map("../data/map.txt");

			ServiceLocator<DrawManager>::SetService(m_xDrawManager.get());
			ServiceLocator<SpriteManager>::SetService(m_xSpriteManager.get());
			ServiceLocator<input::Keyboard>::SetService(m_xKeyboard.get());
			ServiceLocator<input::Mouse>::SetService(m_xMouse.get());
			ServiceLocator<Utility>::SetService(m_xUtility.get());
			ServiceLocator<b2World>::SetService(m_xB2World);
			ServiceLocator<Map>::SetService(m_map);

			if (!m_xDrawManager->Initialize())
				return false;
			
			m_xContactListener = new ContactListener;
			m_xB2World->SetContactListener(m_xContactListener);

			mB2DebugDraw = new Box2DWorldDraw(m_xDrawManager->m_xWindow);
			mB2DebugDraw->SetFlags(b2Draw::e_jointBit | b2Draw::e_shapeBit);
			m_xB2World->SetDebugDraw(mB2DebugDraw);

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

			delete m_xContactListener;
			m_xContactListener = nullptr;

			delete mB2DebugDraw;
			mB2DebugDraw = nullptr;
		}

		void Engine::RunLoop()
		{
			static Object* xObject = new Object;
			xObject->CreateBody(b2BodyType::b2_dynamicBody);
			xObject->CreateB2Shape(sf::Vector2i(32,32));
			xObject->SetPos(sf::Vector2f(50.0f, 50.0f), true);

			static Object* xObject2 = new Object;
			xObject2->CreateBody(b2BodyType::b2_staticBody);
			xObject2->CreateB2Shape(sf::Vector2i(32, 32));
			xObject2->SetPos(sf::Vector2f(50.0f, 400.0f), true);

			while (m_bRunning && m_xDrawManager->m_xWindow->isOpen())
			{
				PollEvents();
				UpdateDeltaTime();

				if (m_xKeyboard->IsDown(sf::Keyboard::Escape))
					m_bRunning = false;

				//Updates
				m_xB2World->Step(m_fDeltaTime, 10, 10);
				xObject->Update(m_fDeltaTime);
				xObject2->Update(m_fDeltaTime);
				std::cout << std::fixed << std::setprecision(5) << m_fDeltaTime << std::endl;

				//Draw
				m_xDrawManager->ClearScr();
				//insert stuff to draw
				m_xDrawManager->Draw(m_map);
				m_xB2World->DrawDebugData();
				m_xDrawManager->Display();

				::Sleep(2);
			}

			delete xObject;
			xObject = nullptr;
			delete xObject2;
			xObject2 = nullptr;
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