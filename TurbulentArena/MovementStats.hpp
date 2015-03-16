//MovementStats.hpp

#pragma once
#include <SFML\Graphics.hpp>

namespace bjoernligan
{
	struct MovementStats
	{
	private:
		sf::Vector2f m_MaxVelocity;
		//float m_Acceleration;

	public:
		void Initiate(const float &p_MaxVelocity);
	
		const sf::Vector2f& GetMaxVelocity();
		void SetMaxVelocity(const float &p_NewMaxVelocity);
	};
}