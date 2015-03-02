#include "stdafx.h"
#include "ClanMember.hpp"
#include "Agent.hpp"

namespace bjoernligan
{
	ClanMember::ClanMember()
	{
		m_sprite = std::make_unique<sf::Sprite>();
		m_xAgent = new ai::Agent(this);
	}

	ClanMember::~ClanMember()
	{
		if (m_xAgent)
		{
			delete m_xAgent;
			m_xAgent = nullptr;
		}
	}

	void ClanMember::update(float deltatime)
	{
		Object::update(deltatime);

		if (m_sprite)
			m_sprite->setPosition(m_xPos);
		if (m_xAgent)
			m_xAgent->Update(deltatime);
	}
	
	void ClanMember::setBody(Physics::Body* body)
	{
		Object::setBody(body);
		InitSteeringStats();
		
	}
	void ClanMember::InitSteeringStats()
	{
		if (m_xAgent)
		{
			//Init Steering, with Body and Movementstats
			m_xAgent->InitializeSteering(m_xPhysicsBody->m_body, &m_MovementStats);
		}
	}
	void ClanMember::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(*m_sprite.get(), states);
	}

	sf::Sprite* ClanMember::getSprite()
	{
		return m_sprite.get();
	}
}