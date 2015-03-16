#include "stdafx.h"
#include "ClanMember.hpp"
#include "Map.hpp"

namespace bjoernligan
{
	ClanMember::ClanMember(ai::Sense* sense, const sf::Color &p_xTeamColor, Clan* p_xClan)
		: m_xHealthBar(sf::Vector2f(-16, -32), 32, p_xTeamColor)
		, m_clan(p_xClan)
		, m_xAgent(nullptr)
		, m_eClass(EClass::EClassInvalid)
	{
		m_sprite = std::make_unique<sf::Sprite>();
		m_xHealthBar.SetCombatStats(&m_xCombatStats);
		m_xAgent = std::make_unique<ai::Agent>(this, sense);
		m_userData = std::make_unique<ClanMemberUD>(this);

		m_xMood = { { 0.5f, 0.5f, 0.5f } };

		m_xSelectionRect.setSize(sf::Vector2f(32, 32));
		m_xSelectionRect.setOrigin(m_xSelectionRect.getSize() / 2.0f);
		m_xSelectionRect.setFillColor(sf::Color(0, 0, 0, 0));
		m_xSelectionRect.setOutlineColor(sf::Color(0, 255, 0, 0));
		m_xSelectionRect.setOutlineThickness(3);
	}

	ClanMember::~ClanMember()
	{
	}

	void ClanMember::update(float deltatime)
	{
		Object::update(deltatime);
		if (m_sprite && m_xPhysicsBody->m_body)
		{
			m_sprite->setRotation(DEGREES(m_xPhysicsBody->m_body->GetAngle()));

			//sf::Vector2f x;

			//float fAngle = dot(sf::Vector2f(0, 1), vectorToPix(m_xPhysicsBody->m_body->GetLinearVelocity()));

			//float fTangent(m_xPhysicsBody->m_body->GetLinearVelocity().y);
			//float fHypo(m_xPhysicsBody->m_body->GetLinearVelocity().Length());
			//float fTangent(5);
			//float fHypo(10);

			//float fAngle = DEGREES(std::cos(fTangent / fHypo));
			//m_sprite->setRotation(fAngle);
		}

		m_xHealthBar.Update(m_xPos);

		if (m_sprite)
			m_sprite->setPosition(m_xPos);
		if (m_xAgent)
			m_xAgent->Update(deltatime);

		m_xSelectionRect.setPosition(m_xPos);
	}

	void ClanMember::setBody(Physics::Body* body)
	{
		Object::setBody(body);
		body->m_body->SetUserData(m_userData.get());
		InitSteeringStats();
	}
	void ClanMember::InitSteeringStats()
	{
		if (m_xAgent)
		{
			//Init Steering, with Body and Movementstats
			m_xAgent->InitializeSteering(m_xPhysicsBody->m_body, m_MovementStats);
		}
	}
	void ClanMember::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(*m_sprite.get(), states);

		if (m_drawPathfinder)
		{
			if (!m_xAgent->m_CurrentPath.isDone())
			{
				sf::Vector2f size = m_xAgent->m_map->getTileSize();
				sf::RectangleShape nodeShape;
				nodeShape.setSize(sf::Vector2f(size.x - 2, size.y - 2));
				nodeShape.setOutlineThickness(1);

				for (int i = 0; i < m_xAgent->m_CurrentPath.length; ++i)
				{
					if (i < m_xAgent->m_CurrentPath.currentNode)
					{
						nodeShape.setFillColor(sf::Color(0, 255, 0, 130));
						nodeShape.setOutlineColor(sf::Color(0, 255, 0, 200));
					}
					else
					{
						nodeShape.setFillColor(sf::Color(255, 0, 0, 130));
						nodeShape.setOutlineColor(sf::Color(255, 0, 0, 200));
					}
					nodeShape.setPosition(m_xAgent->m_CurrentPath.nodes[i].x * size.x, m_xAgent->m_CurrentPath.nodes[i].y * size.y);
					target.draw(nodeShape, states);
				}
			}
		}

		target.draw(m_xSelectionRect, states);
	}

	sf::Sprite* ClanMember::getSprite()
	{
		return m_sprite.get();
	}

	void ClanMember::drawHpBar(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_xHealthBar, states);
	}

	ai::Agent* ClanMember::getAgent() const
	{
		return m_xAgent.get();
	}

	void ClanMember::drawPathfinder(bool value)
	{
		m_drawPathfinder = value;
	}

	bool ClanMember::IsFriend(ClanMember* p_xMember)
	{
		return m_clan == p_xMember->GetClan();
	}

	Clan* ClanMember::GetClan()
	{
		return m_clan;
	}
	MovementStats& ClanMember::GetMovementStats()
	{
		return m_MovementStats;
	}

	CombatStats* ClanMember::GetCombat()
	{
		return &m_xCombatStats;
	}

	void ClanMember::SetMoodValue(const ai::Mood::EMoodType &p_eMoodType, const float &p_fValue)
	{
		m_xMood.SetMood(p_eMoodType, p_fValue);
	}

	ai::Mood* ClanMember::GetMood()
	{
		return &m_xMood;
	}

	void ClanMember::SetSelection(const bool &p_bValue)
	{
		if (p_bValue)
			m_xSelectionRect.setOutlineColor(sf::Color(
			m_xSelectionRect.getOutlineColor().r,
			m_xSelectionRect.getOutlineColor().g,
			m_xSelectionRect.getOutlineColor().b,
			175));
		else
			m_xSelectionRect.setOutlineColor(sf::Color(
			m_xSelectionRect.getOutlineColor().r,
			m_xSelectionRect.getOutlineColor().g,
			m_xSelectionRect.getOutlineColor().b,
			0));
	}

	const ClanMember::EClass &ClanMember::GetClass() const
	{
		return m_eClass;
	}
}