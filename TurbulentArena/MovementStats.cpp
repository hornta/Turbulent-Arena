//MovementStats.cpp

#include "stdafx.h"
#include "MovementStats.hpp"

namespace bjoernligan
{
	void MovementStats::Initiate(const sf::Vector2f p_MaxVelocity)
	{
		m_MaxVelocity = p_MaxVelocity;
		//m_Acceleration = p_Acceleration;
	}
	const sf::Vector2f& MovementStats::GetMaxVelocity()
	{
		return m_MaxVelocity;
	}

	void MovementStats::SetMaxVelocity(const sf::Vector2f p_NewMaxVelocity)
	{
		m_MaxVelocity = p_NewMaxVelocity;
	}
}