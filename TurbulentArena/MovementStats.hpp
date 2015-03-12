//MovementStats.hpp

#pragma once
#include <SFML\Graphics.hpp>

namespace bjoernligan
{
	struct MovementStats
	{
	private:
		sf::Vector2f m_MaxWalkVelocity;
		sf::Vector2f m_MaxRunVelocity;
		//float m_Acceleration;
		//float m_SlowDownRadius;

	public:
		void Initiate(const sf::Vector2f &p_MaxWalkVelocity, const sf::Vector2f &p_MaxRunVelocity);
	
		const sf::Vector2f& GetMaxWalkVelocity();
		const sf::Vector2f& GetMaxRunVelocity();
		//const float& GetAcceleration();
		//const float& GetSlowDownRadius();
	};
}