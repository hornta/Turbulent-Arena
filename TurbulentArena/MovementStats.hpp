//MovementStats.hpp

#pragma once
#include <SFML\Graphics.hpp>

namespace bjoernligan
{
	struct MovementStats
	{
	private:
		sf::Vector2f m_MaxVelocity;
		float m_Acceleration;
		float m_SlowDownRadius;

	public:
		void Initiate(const sf::Vector2f p_MaxVelocity, const float p_Acceleration, const float p_SlowDownRadius);
	
		const sf::Vector2f& GetMaxVelocity();
		const float& GetAcceleration();
		const float& GetSlowDownRadius();
	};
}