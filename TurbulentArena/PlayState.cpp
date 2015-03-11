//PlayState.cpp

#include "stdafx.h"
#include "PlayState.hpp"
#include "ServiceLocator.hpp"
#include "DrawManager.hpp"
#include "SpriteManager.hpp"
#include "AudioManager.hpp"
#include "Map.hpp"
#include "Physics.hpp"
#include "Sense.hpp"
#include "Visibility.hpp"
#include "ClanManager.hpp"
#include "ClanMember.hpp"
#include "Clan.hpp"
#include "UIManager.hpp"
#include "DebugWindow.hpp"
#include "Pathfinder.hpp"
#include "Settings.hpp"
#include "Scout.hpp"
#include "Axeman.hpp"
#include "UIButton.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "GameStateManager.hpp"
#include "MainMenuState.hpp"

namespace bjoernligan
{
	PlayState::PlayState(const std::string &p_sName, const bool &p_bExclusive)
		: GameState(p_sName, p_bExclusive)
		, m_fScrollSpeed(5.0f)
	{

	}

	void PlayState::Enter()
	{
		std::cout << "Entered PlayState." << std::endl;

		m_xSpriteManager = ServiceLocator<system::SpriteManager>::GetService();
		m_xAudioManager = ServiceLocator<system::AudioManager>::GetService();
		m_xUIManager = ServiceLocator<UIManager>::GetService();
		m_xKeyboard = ServiceLocator<input::Keyboard>::GetService();
		m_xMouse = ServiceLocator<input::Mouse>::GetService();
		m_xUtility = ServiceLocator<Utility>::GetService();
		m_xDebugWindow = ServiceLocator<DebugWindow>::GetService();
		m_xMouse = ServiceLocator<input::Mouse>::GetService();
		m_xKeyboard = ServiceLocator<input::Keyboard>::GetService();

		m_xAudioManager->PlayMusic("Battle");

		m_map = std::make_unique<Map>("../data/");
		m_map->load("map.tmx");

		m_clanManager = std::make_unique<ClanManager>();

		m_physics = std::make_unique<Physics>(0.f, 0.f, m_xDrawManager->getWindow());
		m_physics->setDebug(false);

		m_sense = std::make_unique<ai::Sense>();

		m_pathFinder = std::make_unique<Pathfinder>(m_map->getSize());

		m_visibility = std::make_unique<Visibility>();

		ServiceLocator<Visibility>::SetService(m_visibility.get());
		ServiceLocator<Pathfinder>::SetService(m_pathFinder.get());
		ServiceLocator<Map>::SetService(m_map.get());
		ServiceLocator<ClanManager>::SetService(m_clanManager.get());
		ServiceLocator<Physics>::SetService(m_physics.get());

		m_view = m_xDrawManager->getWindow()->getView();

		// PATHFINDER
		for (int y = 0; y < m_map->getHeight(); ++y)
		{
			for (int x = 0; x < m_map->getWidth(); ++x)
			{
				Map::Tile* tile = m_map->getLayer("objects")->getTile(x, y);
				if (tile != nullptr)
				{
					m_pathFinder->getGrid().setWalkableAt(x, y, tile->getTileInfo()->m_properties.hasProperty("walkable"));
					if (!tile->getTileInfo()->m_properties.hasProperty("walkable"))
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
					m_visibility->addSegment(p0, p1);
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

		Physics::Params clanMemberBodyDef;
		clanMemberBodyDef.m_xBodyDef.linearDamping = 1.5f;
		clanMemberBodyDef.m_xBodyDef.angularDamping = 1.5f;
		clanMemberBodyDef.m_eShapeType = Physics::Circle;
		clanMemberBodyDef.m_xFixtureDef.friction = 0.5f;
		clanMemberBodyDef.m_xFixtureDef.density = 1.0f;
		clanMemberBodyDef.m_xFixtureDef.restitution = 0.2f;
		clanMemberBodyDef.m_xShapeSize.m_fCircleRadius = 16.f;
		clanMemberBodyDef.m_xBodyDef.type = b2_dynamicBody;

		{
			Clan* clan = m_clanManager->createClan("MacDonald", sf::Color(70, 70, 255));
			m_xGameOverChecker.AddClan(clan);

			for (int32_t i = 0; i < 8; ++i)
			{
				ClanMember* member = clan->createMember<Axeman>(m_sense.get());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/axeman.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
		}
		{
			Clan* clan = m_clanManager->createClan("MacMuffin", sf::Color(255, 70, 70));
			m_xGameOverChecker.AddClan(clan);

			for (int32_t i = 0; i < 8; ++i)
			{
				ClanMember* member = clan->createMember<Axeman>(m_sense.get());
				member->getSprite()->setTexture(*m_xSpriteManager->GetTexture("classes/axeman.png"));
				member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
				member->setBody(m_physics->createBody(clanMemberBodyDef));
				member->initiate();
			}
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
		xDef.m_xFunction = std::bind(&bjoernligan::PlayState::SetScrollSpeed, this, std::placeholders::_1);
		xDef.m_fWidth = 240.0f;
		xDef.m_fMin = 1.0f;
		xDef.m_fMax = 20.0f;
		xDef.m_fCurrent = 5.0f;
		xDef.m_bContinous = true;

		m_xUIManager->AddSlider("PlayState", xDef, sf::Vector2f((float)Settings::m_xWindowSize.x - 300.0f, (float)Settings::m_xWindowSize.y - 80.0f), 1.0f);

		UIButton* xButton = static_cast<UIButton*>(m_xUIManager->AddElement<UIButton>("PlayState", 1.0f));
		xButton->Initialize("Debug: World", sf::IntRect(Settings::m_xWindowSize.x - (128 + 32), 96, 140, 32),
			std::bind(&bjoernligan::PlayState::SetDebugMode, this, std::placeholders::_1));

		xButton = static_cast<UIButton*>(m_xUIManager->AddElement<UIButton>("PlayState", 1.0f));
		xButton->Initialize("Debug: Window", sf::IntRect(Settings::m_xWindowSize.x - (128 + 32), 133, 140, 32),
			std::bind(&bjoernligan::DebugWindow::SetActive, &*m_xDebugWindow, std::placeholders::_1));

		xButton = static_cast<UIButton*>(m_xUIManager->AddElement<UIButton>("PlayState", 1.0f));
		xButton->Initialize("Debug: Pathfinder", sf::IntRect(Settings::m_xWindowSize.x - (128 + 32), 170, 140, 32),
			std::bind(&bjoernligan::PlayState::ToggleDebugPathfinder, this, std::placeholders::_1));
	}

	void PlayState::Exit()
	{
		m_xUIManager->RemoveElementsByLabel("PlayState");
	}

	bool PlayState::Update(const float &p_fDeltaTime)
	{
		m_physics->update(p_fDeltaTime);
		m_sense->update(p_fDeltaTime);
		m_visibility->update();
		m_clanManager->Update(p_fDeltaTime);
		updateCamera(p_fDeltaTime);

		if (m_xKeyboard->IsDownOnce(sf::Keyboard::Escape))
		{
			m_xAudioManager->StopAllMusic();
			m_xAudioManager->PlayMusic("Menu");
			m_xStateMngr->CreateState<MainMenuState>("MainMenu");
			m_bDeleteMe = true;
		}

		return true;
	}

	void PlayState::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		m_xDrawManager->getWindow()->setView(m_view);

		m_map->draw(target, states);
		m_visibility->draw(target, states);
		m_clanManager->draw(target, states);
		m_physics->draw();
		m_xDebugWindow->draw(target, states);
	}

	void PlayState::SetDebugMode(const bool &p_bValue)
	{
		m_physics->setDebug(p_bValue);
	}

	void PlayState::ToggleDebugPathfinder(bool value)
	{
		m_debugPathfinder = value;

		for (auto& clan : m_clanManager->getClans())
		{
			for (auto& member : clan->getMembers())
			{
				member->drawPathfinder(m_debugPathfinder);
			}
		}
	}

	void PlayState::SetScrollSpeed(const float &p_fNewSpeed)
	{
		m_fScrollSpeed = p_fNewSpeed;
	}

	void PlayState::updateCamera(const float &p_fDeltaTime)
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
			m_view.move(direction * m_fScrollSpeed * p_fDeltaTime);
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
}