//Engine.cpp

#include "stdafx.h"
#include "Engine.hpp"
#include "ServiceLocator.hpp"
#include "Map.hpp"
#include "Pathfinder.hpp"
#include "ContactListener.hpp"
#include "Box2DWorldDraw.h"
#include "Object.hpp"
#include "Visibility.hpp"
#include "ClanManager.hpp"
#include "Clan.hpp"
#include "Scout.hpp"
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
			m_xB2World = new b2World(b2Vec2(0.0f, 0.0f));
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
						if (!tile->isWalkable())
						{

						}
					}
				}
			}

			// VISIBILITY
			m_visibility = new Visibility();
			Map::Layer* layer = m_map->getLayer("objects");
			if (layer != nullptr)
			{
				float tileSize = static_cast<float>(m_map->getTileSize());

				// Looping all static objects in map
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
			
			// CLANS
			m_clanManager = new ClanManager();

			{
				Clan* clan = m_clanManager->createClan("MacDonald");
				clan;
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

			delete m_visibility;
			m_visibility = nullptr;
		}

		void Engine::RunLoop()
		{
			static Object* xObject = new Object;

			{
				PhysicsParams xParams;

				xParams.m_xBodyDef.type = b2_dynamicBody;
				xParams.m_xBodyDef.gravityScale = 1.0f;
				xParams.m_xBodyDef.fixedRotation = false;
				xParams.m_xBodyDef.bullet = false;

				xParams.m_xFixtureDef.friction = 1.0f;
				xParams.m_xFixtureDef.density = 1.0f;
				xParams.m_xFixtureDef.restitution = 1.0f;
				xParams.m_xFixtureDef.isSensor = false;

				xParams.m_eShapeType = EB2ShapeType::Box;
				xParams.m_xShapeSize.m_xBox.x = 64;
				xParams.m_xShapeSize.m_xBox.y = 32;

				PhysicsBody* xPhysBody = new PhysicsBody;
				xPhysBody->CreateBody(xParams);

				xObject->SetPhysicsBody(xPhysBody);
				xObject->SetPos(sf::Vector2f(50.0f, 50.0f), true);
			}

			static Object* xObject2 = new Object;
			{
				PhysicsParams xParams;

				xParams.m_xBodyDef.type = b2_staticBody;
				xParams.m_xBodyDef.gravityScale = 1.0f;
				xParams.m_xBodyDef.fixedRotation = false;
				xParams.m_xBodyDef.bullet = false;

				xParams.m_xFixtureDef.friction = 1.0f;
				xParams.m_xFixtureDef.density = 1.0f;
				xParams.m_xFixtureDef.restitution = 0.5f;
				xParams.m_xFixtureDef.isSensor = false;

				xParams.m_eShapeType = EB2ShapeType::Circle;
				xParams.m_xShapeSize.m_fCircleRadius = 12.0f;

				PhysicsBody* xPhysBody = new PhysicsBody;
				xPhysBody->CreateBody(xParams);

				xObject2->SetPhysicsBody(xPhysBody);
				xObject2->SetPos(sf::Vector2f(49.0f, 400.0f), true);
			}

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
				m_visibility->update();

				//Draw
				m_xDrawManager->ClearScr();
				//insert stuff to draw
				m_xDrawManager->Draw(m_map);
				m_xDrawManager->Draw(m_visibility);
				m_xB2World->DrawDebugData();
				m_xDrawManager->Display();

				::Sleep(2);
			}

			delete xObject;
			xObject = nullptr;
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