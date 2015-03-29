#include "stdafx.h"
#include "ClanManager.hpp"
#include "Keyboard.hpp"
#include "Clan.hpp"
#include "ClanMember.hpp"
#include "Axeman.hpp"
#include "Scout.hpp"
#include "Sense.hpp"
#include "SpriteManager.hpp"

#include "Physics.hpp"

namespace bjoernligan
{
	ClanManager::ClanManager()
		: m_bDrawHealthBars(true)
	{
	}

	ClanManager::~ClanManager()
	{
	}

	Clan* ClanManager::createClan(const std::string& name, const sf::Color &p_xTeamColor, const uint32_t &p_iAxemanCount, const uint32_t &p_iScoutCount)
	{
		m_clans.emplace_back(std::make_unique<Clan>(name, p_xTeamColor));
		Clan* xClan = m_clans.back().get();

		Physics::Params clanMemberBodyDef;
		clanMemberBodyDef.m_xBodyDef.linearDamping = 3.0f;
		clanMemberBodyDef.m_xBodyDef.angularDamping = 2.5f;
		clanMemberBodyDef.m_eShapeType = Physics::Circle;
		clanMemberBodyDef.m_xFixtureDef.friction = 0.5f;
		clanMemberBodyDef.m_xFixtureDef.density = 0.15f;
		clanMemberBodyDef.m_xFixtureDef.restitution = 0.2f;
		clanMemberBodyDef.m_xShapeSize.m_fCircleRadius = 15.f;
		clanMemberBodyDef.m_xBodyDef.type = b2_dynamicBody;

		ai::Sense* xSense = ServiceLocator<ai::Sense>::GetService();
		system::SpriteManager* xSpriteManager = ServiceLocator<system::SpriteManager>::GetService();

		for (uint32_t i = 0; i < p_iAxemanCount; ++i)
		{
			ClanMember* member = xClan->createMember<Axeman>(xSense);
			member->getSprite()->setTexture(*xSpriteManager->GetTexture("classes/axeman.png"));
			member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
			member->setBody(ServiceLocator<Physics>::GetService()->createBody(clanMemberBodyDef));
			member->initiate();
		}

		clanMemberBodyDef.m_xFixtureDef.friction = 1.f;

		for (uint32_t i = 0; i < p_iScoutCount; ++i)
		{
			ClanMember* member = xClan->createMember<Scout>(xSense);
			member->getSprite()->setTexture(*xSpriteManager->GetTexture("classes/scout.png"));
			member->getSprite()->setOrigin(member->getSprite()->getGlobalBounds().width * 0.5f, member->getSprite()->getGlobalBounds().height * 0.5f);
			member->setBody(ServiceLocator<Physics>::GetService()->createBody(clanMemberBodyDef));
			member->initiate();
		}

		return xClan;
	}
	
	std::size_t ClanManager::getAmount() const
	{
		return m_clans.size();
	}

	void ClanManager::Update(const float &p_fDeltaTime)
	{
		for (std::size_t i = 0; i < m_clans.size(); ++i)
		{
			m_clans[i]->Update(p_fDeltaTime);
		}
	}

	void ClanManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (std::size_t i = 0; i < m_clans.size(); ++i)
		{
			target.draw(*m_clans[i].get(), states);
		}
		if (m_bDrawHealthBars || ServiceLocator<input::Keyboard>::GetService()->IsDown(sf::Keyboard::LAlt) || ServiceLocator<input::Keyboard>::GetService()->IsDown(sf::Keyboard::LAlt))
		{
			for (std::size_t i = 0; i < m_clans.size(); ++i)
			{
				m_clans[i]->drawHpBars(target, states);
			}
		}
	}

	std::vector<Clan*> ClanManager::getClans() const
	{
		std::vector<Clan*> clans;
		for (std::size_t i = 0; i < m_clans.size(); ++i)
		{
			clans.push_back(m_clans[i].get());
		}
		return clans;
	}

	void ClanManager::SetDrawHealthBars(const bool &p_bValue)
	{
		m_bDrawHealthBars = p_bValue;
	}
}