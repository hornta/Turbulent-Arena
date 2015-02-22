//SteeringManager.hpp

#pragma once
#include <SFML\Graphics.hpp>

namespace bjoernligan
{
	class Utility;
	namespace ai
	{
		class SteeringManager
		{
		public:
			SteeringManager();
			~SteeringManager();
			
			bool Initialize();

			void SetCurrentBody(b2Body* p_CurrentBody);

			void Wander();
			void Seek(sf::Vector2f p_TargetPos);
			void Flee(b2Body* p_CurrentBody);
			void Follow(b2Body* p_TargetBody);

			void Update();
			void Reset();
			
			void SetVelocity(sf::Vector2f p_Velocity);
			void SetMaxVelocity(sf::Vector2f p_MaxVelocity);
		private:
			void AddSteeringForce(sf::Vector2f p_TargetPos);
			void CalculateDesiredVelocity(sf::Vector2f p_TargetPos);
			sf::Vector2f Normalize(sf::Vector2f p_Vector);
			sf::Vector2f Truncate(sf::Vector2f p_TruncTarget, sf::Vector2f p_MaxValue);
			
		private:
			sf::Vector2f m_Steering;
			sf::Vector2f m_Velocity;
			sf::Vector2f m_MaxVelocity;
			sf::Vector2f m_MaxForce;

			sf::Vector2f m_DesiredVelocity;
			b2Body* m_CurrentBody;
			Utility* m_Utility;
		};
	}
}