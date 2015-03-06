//Engine.cpp

#include "stdafx.h"
#include "Engine.hpp"
#include "ServiceLocator.hpp"
#include "Map.hpp"
#include "Object.hpp"
#include "Physics.hpp"
#include "Settings.hpp"
#include "Sense.hpp"
#include "Pathfinder.hpp"
#include "Visibility.hpp"

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
			: m_physics(nullptr)
			, m_fScrollSpeed(5.0f)
		{
			m_xDrawManager = nullptr;
			m_xSpriteManager = nullptr;
			m_xAudioManager = nullptr;
			m_xUIManager = nullptr;
			m_xKeyboard = nullptr;
			m_xMouse = nullptr;
			m_xUtility = nullptr;
		}

		Engine::~Engine()
		{
		}

		bool Engine::Initialize()
		{
			m_xDrawManager = DrawManager::Create();
			m_xSpriteManager = SpriteManager::Create();
			m_xAudioManager = AudioManager::Create();
			m_xKeyboard = input::Keyboard::Create();
			m_xMouse = input::Mouse::Create();
			m_xUtility = Utility::Create();
			m_xUIManager = UIManager::Create();

			ServiceLocator<DrawManager>::SetService(m_xDrawManager.get());
			ServiceLocator<SpriteManager>::SetService(m_xSpriteManager.get());
			ServiceLocator<UIManager>::SetService(m_xUIManager.get());
			ServiceLocator<input::Keyboard>::SetService(m_xKeyboard.get());
			ServiceLocator<input::Mouse>::SetService(m_xMouse.get());
			ServiceLocator<Utility>::SetService(m_xUtility.get());
			ServiceLocator<Physics>::SetService(m_physics.get());
			ServiceLocator<Map>::SetService(m_map.get());
			ServiceLocator<AudioManager>::SetService(m_xAudioManager.get());

			m_xAudioManager->CreateSoundBuffer("Explode", "explode_0.wav");
			m_xAudioManager->CreateMusic("Battle", "dragons_lair.ogg");

			m_xAudioManager->PlayMusic("Battle");

			if (!m_xDrawManager->Initialize())
				return false;

			m_xDebugWindow = DebugWindow::Create(false);
			m_xDebugWindow->SetPos(16.0f, 16.0f);

			m_xDrawManager->getWindow()->create(sf::VideoMode(Settings::m_xWindowSize.x, Settings::m_xWindowSize.y), "Turbulent Arena"/*, sf::Style::None*/);
			m_view = m_xDrawManager->getWindow()->getView();
			m_xUIManager->setView(m_view);

			UIButton* xButton = static_cast<UIButton*>(m_xUIManager->AddElement<UIButton>(1.0f));
			xButton->Initialize("Debug: World", sf::IntRect(Settings::m_xWindowSize.x - (128 + 32), 96, 140, 32), std::bind(&bjoernligan::system::Engine::SetDebugMode, this, std::placeholders::_1));

			xButton = static_cast<UIButton*>(m_xUIManager->AddElement<UIButton>(1.0f));
			xButton->Initialize("Debug: Window", sf::IntRect(Settings::m_xWindowSize.x - (128 + 32), 133, 140, 32), std::bind(&bjoernligan::DebugWindow::SetActive, &*m_xDebugWindow.get(), std::placeholders::_1));

			m_xSpriteManager->setTexturePath("../data/sprites/");

			m_clanManager = std::make_unique<ClanManager>();
			m_map = std::make_unique<Map>("../data/");
			m_map->load("map.tmx");
			m_physics = std::make_unique<Physics>(0.f, 0.f, m_xDrawManager->getWindow());
			m_physics->setDebug(false);

			m_sense = std::make_unique<ai::Sense>(m_map->getSize());

			// PATHFINDER
			Pathfinder* pathfinder = m_sense->getPathfinder();
			for (int x = 0; x < m_map->getWidth(); ++x)
			{
				for (int y = 0; y < m_map->getHeight(); ++y)
				{
					Map::Tile* tile = m_map->getLayer("objects")->getTile(x, y);
					if (tile != nullptr)
					{
						pathfinder->getGrid().setWalkableAt(x, y, tile->hasProperty("walkable"));
						if (!tile->hasProperty("walkable"))
						{
							sf::Vector2f tileSize = m_map->getTileSize();
							Physics::Params xParams;
							xParams.m_xFixtureDef.friction = 0.2f;
							xParams.m_xFixtureDef.density = 1.0f;
							xParams.m_xFixtureDef.restitution = 0.5f;

							xParams.m_eShapeType = Physics::Box;
							xParams.m_xShapeSize.m_xBox.x = tileSize.x;
							xParams.m_xShapeSize.m_xBox.y = tileSize.y;

							Physics::Body* body = m_physics->createBody(xParams);
							body->setPosition(x * tileSize.x + tileSize.x * 0.5f, y * tileSize.y + tileSize.y * 0.5f);
						}
					}
				}
			}

			// VISIBILITY
			Visibility* visibility = m_sense->getVisibility();
			std::vector<Map::Object*> objects = m_map->getObjectGroup("light_segments")->getObjects();
			for (std::size_t i = 0; i < objects.size(); ++i)
			{
				std::vector<sf::Vector2f> points = objects[i]->m_points;
				for (std::size_t k = 0; k < points.size(); ++k)
				{
					if (k != points.size() - 1)
					{
						sf::Vector2f p0(points[k]);
						sf::Vector2f p1(points[k + 1]);
						visibility->addSegment(p0, p1);
					}
				}
			}
			//m_visibility->create(sf::Vector2f(100, 100), sf::Color::Red);
			
			// CLANS
			std::vector<Map::Object*> spawns = m_map->getObjectGroup("spawns")->getObjects();
			std::map<int, std::vector<sf::Vector2f>> team_spawns;
			for (std::size_t i = 0; i < spawns.size(); ++i)
			{
				Map::Object* object = spawns[i];
				if (object->hasProperty("team"))
				{
					team_spawns[std::stoi(object->getProperty("team"))].push_back(object->m_position);
				}
			}

			// Randomize spawn vector
			auto teamSpawnsIt = team_spawns.begin();
			while (teamSpawnsIt != team_spawns.end())
			{
				std::shuffle(teamSpawnsIt->second.begin(), teamSpawnsIt->second.end(), random::getEngine());
				++teamSpawnsIt;
			}

			////Creation of sliders:
			//float fSliderSpacing = 80.0f;

			//UISlider* xSlider = m_xUIManager->AddSlider("Social", 1.0f, sf::Vector2f((float)Settings::m_xWindowSize.x - 300.0f, (float)Settings::m_xWindowSize.y - fSliderSpacing*3.0f), 240.0f, 0.0f, 100.0f);
			//SliderBridge* xBridge = xSlider->GetBridge();
			//clan->AddSliderBridge(xBridge);

			//xSlider = m_xUIManager->AddSlider("Brave", 1.0f, sf::Vector2f((float)Settings::m_xWindowSize.x - 300.0f, (float)Settings::m_xWindowSize.y - fSliderSpacing*2.0f), 240.0f, 0.0f, 100.0f);
			//xBridge = xSlider->GetBridge();
			//clan->AddSliderBridge(xBridge);

			//xSlider = m_xUIManager->AddSlider("Agression", 1.0f, sf::Vector2f((float)Settings::m_xWindowSize.x - 300.0f, (float)Settings::m_xWindowSize.y - fSliderSpacing*1.0f), 240.0f, 0.0f, 100.0f);
			//xBridge = xSlider->GetBridge();
			//clan->AddSliderBridge(xBridge);

			Physics::Params clanMemberBodyDef;
			clanMemberBodyDef.m_eShapeType = Physics::Circle;
			clanMemberBodyDef.m_xFixtureDef.friction = 0.5f;
			clanMemberBodyDef.m_xFixtureDef.density = 1.0f;
			clanMemberBodyDef.m_xFixtureDef.restitution = 0.2f;
			clanMemberBodyDef.m_xShapeSize.m_fCircleRadius = 16.f;
			clanMemberBodyDef.m_xBodyDef.type = b2_dynamicBody;
			
			Clan* clan = m_clanManager->createClan("MacDonald");

			{
				ClanMember* member = clan->createMember<Scout>(m_sense.get());
				m_sense->addAgent(member->getAgent());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/axeman.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
			{
				ClanMember* member = clan->createMember<Scout>(m_sense.get());
				m_sense->addAgent(member->getAgent());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/scout.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
			{
				ClanMember* member = clan->createMember<Scout>(m_sense.get());
				m_sense->addAgent(member->getAgent());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/axeman.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
			{
				ClanMember* member = clan->createMember<Scout>(m_sense.get());
				m_sense->addAgent(member->getAgent());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/scout.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
			{
				ClanMember* member = clan->createMember<Scout>(m_sense.get());
				m_sense->addAgent(member->getAgent());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/axeman.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
			{
				ClanMember* member = clan->createMember<Scout>(m_sense.get());
				m_sense->addAgent(member->getAgent());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/scout.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
			{
				ClanMember* member = clan->createMember<Scout>(m_sense.get());
				m_sense->addAgent(member->getAgent());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/axeman.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
			{
				ClanMember* member = clan->createMember<Scout>(m_sense.get());
				m_sense->addAgent(member->getAgent());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/scout.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
			{
				ClanMember* member = clan->createMember<Scout>(m_sense.get());
				m_sense->addAgent(member->getAgent());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/axeman.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
			{
				ClanMember* member = clan->createMember<Scout>(m_sense.get());
				m_sense->addAgent(member->getAgent());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/scout.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
			{
				ClanMember* member = clan->createMember<Scout>(m_sense.get());
				m_sense->addAgent(member->getAgent());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/axeman.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
			{
				ClanMember* member = clan->createMember<Scout>(m_sense.get());
				m_sense->addAgent(member->getAgent());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/scout.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
			{
				ClanMember* member = clan->createMember<Scout>(m_sense.get());
				m_sense->addAgent(member->getAgent());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/axeman.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
			{
				ClanMember* member = clan->createMember<Scout>(m_sense.get());
				m_sense->addAgent(member->getAgent());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/scout.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
			{
				ClanMember* member = clan->createMember<Scout>(m_sense.get());
				m_sense->addAgent(member->getAgent());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/axeman.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
			{
				ClanMember* member = clan->createMember<Scout>(m_sense.get());
				m_sense->addAgent(member->getAgent());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/scout.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}

			std::vector<Clan*> clans = m_clanManager->getClans();
			std::vector<ClanMember*> members;
			for (std::size_t i = 0; i < clans.size(); ++i)
			{
				members = clans[i]->getMembers();
				for (std::size_t k = 0; k < members.size(); ++k)
				{
					members[k]->getBody()->setPosition(team_spawns[i][k]);
				}
			}

			UISlider::SliderDef xDef;
			xDef.m_sLabel = "Scrollspeed";
			xDef.m_xFunction = std::bind(&bjoernligan::system::Engine::SetScrollSpeed, this, std::placeholders::_1);
			xDef.m_fWidth = 240.0f;
			xDef.m_fMin = 1.0f;
			xDef.m_fMax = 20.0f;
			xDef.m_fCurrent = 5.0f;
			xDef.m_bContinous = true;

			m_xUIManager->AddSlider(xDef, sf::Vector2f((float)Settings::m_xWindowSize.x - 300.0f, (float)Settings::m_xWindowSize.y - 80.0f), 1.0f);

			return m_bRunning = true;
		}

		void Engine::CleanUp()
		{
		}

		void Engine::RunLoop()
		{
			while (m_bRunning && m_xDrawManager->getWindow()->isOpen())
			{
				PollEvents();
				UpdateDeltaTime();

				if (m_xKeyboard->IsDown(sf::Keyboard::Escape))
					m_bRunning = false;

				//Updates
				m_physics->update(m_fDeltaTime);
				m_sense->update(m_fDeltaTime);
				m_xUIManager->Update(m_fDeltaTime);
				m_clanManager->Update(m_fDeltaTime);
				m_xDebugWindow->Update(m_fDeltaTimeRaw);

				updateCamera();
				// Keep mouse inside window

				//Draw
				m_xDrawManager->ClearScr();
				m_xDrawManager->getWindow()->setView(m_view);
				m_xDrawManager->Draw(m_map.get());
				m_xDrawManager->Draw(m_clanManager.get());
				m_physics->draw();
				m_xDrawManager->getWindow()->setView(m_xUIManager->getView());
				m_xDrawManager->Draw(m_xUIManager.get());
				m_xDrawManager->Draw(m_xDebugWindow.get());
				m_xDrawManager->Display();

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
				{
					m_xMouse->m_xPos = sf::Mouse::getPosition(*m_xDrawManager->getWindow());
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

		void Engine::updateCamera()
		{
			if (m_xMouse->IsDown(sf::Mouse::Right))
			{
				if (m_xMouse->IsDownOnce(sf::Mouse::Right))
				{
					m_lastRightClick = sf::Vector2f(sf::Mouse::getPosition(*m_xDrawManager->getWindow()));
				}

				sf::Vector2f currentPos = sf::Vector2f(sf::Mouse::getPosition(*m_xDrawManager->getWindow()));

				sf::Vector2f direction = currentPos - m_lastRightClick;
				Vector2f vec(direction);
				vec.limit(300.f);
				direction.x = vec.x;
				direction.y = vec.y;
				m_view.move(direction * m_fScrollSpeed * m_fDeltaTime);
			}

			sf::Vector2f pos = m_view.getCenter();
			sf::Vector2f size = m_view.getSize();
			sf::Vector2f halfSize = size * 0.5f;
			sf::Vector2f newPos = pos;
			if ((pos.x - halfSize.x) < 0.f)
			{
				newPos.x = 0.f + halfSize.x;
			}
			else if (pos.x + halfSize.x > m_map->getWidth() * m_map->getTileSize().x)
			{
				newPos.x = m_map->getWidth() * m_map->getTileSize().x - halfSize.x;
			}

			if (pos.y - halfSize.y < 0.f)
			{
				newPos.y = 0.f + halfSize.y;
			}
			else if (pos.y + halfSize.y > m_map->getHeight() * m_map->getTileSize().y)
			{
				newPos.y = m_map->getHeight() * m_map->getTileSize().y - halfSize.y;
			}

			m_view.setCenter(newPos);
		}
		
		void Engine::SetDebugMode(const bool &p_bValue)
		{
			m_physics->setDebug(p_bValue);
			//m_xDebugWindow->SetActive(p_bValue);
		}

		void Engine::SetScrollSpeed(const float &p_fNewSpeed)
		{
			m_fScrollSpeed = p_fNewSpeed;
			//std::cout << std::fixed << std::setprecision(2) << "new value: " << p_fNewSpeed << std::endl;
		}
	}
}