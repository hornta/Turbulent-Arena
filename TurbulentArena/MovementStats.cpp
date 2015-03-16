//MovementStats.cpp

#include "stdafx.h"
#include "MovementStats.hpp"

namespace bjoernligan
{
	void MovementStats::Initiate(const float &p_MaxVelocity)
	{
		m_MaxVelocity = sf::Vector2f(p_MaxVelocity, p_MaxVelocity);
		//m_Acceleration = p_Acceleration;
	}
	const sf::Vector2f& MovementStats::GetMaxVelocity()
	{
		return m_MaxVelocity;
	}

	void MovementStats::SetMaxVelocity(const float &p_NewMaxVelocity)
	{
		m_MaxVelocity = sf::Vector2f(p_NewMaxVelocity, p_NewMaxVelocity);
	}
}