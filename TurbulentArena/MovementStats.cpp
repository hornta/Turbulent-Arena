//MovementStats.cpp

#include "stdafx.h"
#include "MovementStats.hpp"

namespace bjoernligan
{
	void MovementStats::Initiate(const sf::Vector2f &p_MaxWalkVelocity, const sf::Vector2f &p_MaxRunVelocity)
	{
		m_MaxWalkVelocity = p_MaxWalkVelocity;
		m_MaxRunVelocity = p_MaxRunVelocity;
		//m_Acceleration = p_Acceleration;
	}
	const sf::Vector2f& MovementStats::GetMaxWalkVelocity()
	{
		return m_MaxWalkVelocity;
	}
	const sf::Vector2f& MovementStats::GetMaxRunVelocity()
	{
		return m_MaxRunVelocity;
	}
	/*const float& MovementStats::GetAcceleration()
	{
		return m_Acceleration;
	}
	const float& MovementStats::GetSlowDownRadius()
	{
		return m_SlowDownRadius;
	}*/
}