//MovingObject.cpp

#include "stdafx.h"
#include "MovingObject.hpp"
#include "Agent.hpp"

namespace bjoernligan
{
	MovingObject::MovingObject()
	{
		m_xAgent = nullptr;
	}

	MovingObject::~MovingObject()
	{
		if (m_xAgent)
		{
			delete m_xAgent;
			m_xAgent = nullptr;
		}
	}

	void MovingObject::Update(const float &p_fDeltaTime)
	{
		Object::Update(p_fDeltaTime);
		m_xPos += m_xVelocity * p_fDeltaTime;

		if (m_xAgent)
		{
			m_xAgent->Sense();
			m_xAgent->Decide();
		}
	}

	void MovingObject::SetVelocity(const sf::Vector2f &p_xVel)
	{
		m_xVelocity = p_xVel;
	}
}