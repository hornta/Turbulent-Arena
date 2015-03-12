//MovementStats.cpp

#include "stdafx.h"
#include "MovementStats.hpp"

namespace bjoernligan
{
	void MovementStats::Initiate(const sf::Vector2f &p_MaxVelocity, const float &p_Acceleration)
	{
		m_MaxVelocity = p_MaxVelocity;
		m_Acceleration = p_Acceleration;
	}
	const sf::Vector2f& MovementStats::GetMaxVelocity()
	{
		return m_MaxVelocity;
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