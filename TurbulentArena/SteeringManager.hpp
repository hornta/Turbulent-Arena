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

			void SetCurrentBody(b2Body* p_CurrentBody, const sf::Vector2f& p_MaxWalkVelocity, const sf::Vector2f& p_MaxRunVelocity);
			void Wander();
			void Seek(const sf::Vector2f& p_TargetPos, bool p_Run);
			void Flee(const sf::Vector2f& p_TargetPos, bool p_Run);
			void Pursuit(b2Body* p_TargetBody, bool p_Run);
			void Evade(b2Body* p_TargetBody, bool p_Run);
			void Arrival(const sf::Vector2f& p_TargetPos, bool p_Run, const float& p_SlowDownRadius);

			void Update();

		private:
			void Reset();
			sf::Vector2f GetDesiredVelocity(sf::Vector2f p_CurrentPos, sf::Vector2f p_TargetPos, bool p_Run, bool p_HasSlowDown);
			sf::Vector2f GetPredictedPosition(b2Body* p_TargetBody);
			sf::Vector2f Normalize(sf::Vector2f p_Vector);
			sf::Vector2f Truncate(sf::Vector2f p_TruncTarget, sf::Vector2f p_MaxValue);
			float GetEuclideanDistance(sf::Vector2f p_PositionA, sf::Vector2f p_PositionB);
			
		private:
			sf::Vector2f m_Steering;
			sf::Vector2f m_MaxForce;
			sf::Vector2f m_MaxWalkVelocity;
			sf::Vector2f m_MaxRunVelocity;
			
			float m_Distance;
			float m_SlowDownRadius;
			b2Body* m_CurrentBody;
			Utility* m_Utility;
		};
	}
}