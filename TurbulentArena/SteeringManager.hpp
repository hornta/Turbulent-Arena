//SteeringManager.hpp

#pragma once
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>



namespace bjoernligan
{
	namespace ai
	{
		class SteeringManager
		{
		public:
			SteeringManager();
			~SteeringManager();

			void Wander();
			void Seek(sf::Vector2f p_Target, int p_SlowdownRadius /*slowingRadius : Number = 20*/);
			void Flee(sf::Vector2f p_Target);
			void FollowLeader();
			
		private:
			int SlowdownRadius;
			sf::Vector2f m_Steering;
			sf::Vector2f m_Velocity;
			sf::Vector2f m_DesiredVelocity;
			sf::Vector2f m_Position;
		};
	}
}