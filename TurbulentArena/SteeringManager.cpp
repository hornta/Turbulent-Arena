//SteeringManager.cpp
#include "stdafx.h"
#include "SteeringManager.hpp"
#include "ServiceLocator.hpp"
#include "Utility.hpp"
#include <Box2D\Box2D.h>
#include <cmath>

namespace bjoernligan
{
	namespace ai
	{

		SteeringManager::SteeringManager()
		{
			m_Steering = sf::Vector2f(0, 0);
			m_Velocity = sf::Vector2f(0, 0);
			m_MaxVelocity = sf::Vector2f(0, 0);
			m_MaxForce = sf::Vector2f(0, 0);
			m_DesiredVelocity = sf::Vector2f(0, 0);
			m_CurrentBody = nullptr;
			m_Utility = nullptr;
		}
		SteeringManager::~SteeringManager()
		{

		}
		bool SteeringManager::Initialize()
		{
			m_Utility = ServiceLocator<Utility>::GetService();
			m_MaxForce = sf::Vector2f(2, 2);
			m_MaxVelocity = sf::Vector2f(5, 5);
			return true;
		}
		void SteeringManager::SetCurrentBody(b2Body* p_CurrentBody)
		{
			m_CurrentBody = p_CurrentBody;
		}
		void SteeringManager::Wander()
		{

		}
		void SteeringManager::Seek(sf::Vector2f p_TargetPos)
		{
			AddSteeringForce(p_TargetPos);
		}
		/*void SteeringManager::Flee(b2Body* p_PhysicsBody)
		{
		}
		void SteeringManager::Follow(b2Body* p_TargetBody)
		{

		}*/
		void SteeringManager::Update()
		{

		}
		void SteeringManager::Reset()
		{

		}
		void SteeringManager::AddSteeringForce(sf::Vector2f p_TargetPos)
		{
			m_Velocity = m_Utility->ConvertVector_B2toSF(m_CurrentBody->GetLinearVelocity());

			CalculateDesiredVelocity(p_TargetPos);
			m_Steering = m_DesiredVelocity - m_Velocity;


			m_Steering = Truncate(m_Steering, m_MaxForce);
			float mass = m_Utility->ConvertFloat_B2toSF(m_CurrentBody->GetMass());
			m_Steering = m_Steering / mass;


			m_Velocity = Truncate(m_Velocity + m_Steering, m_MaxVelocity);
			m_CurrentBody->SetLinearVelocity(m_Utility->ConvertVector_SFtoB2(m_Velocity));

			/*p_CurrentPos = p_CurrentPos + m_Velocity;*/
		}
		void SteeringManager::CalculateDesiredVelocity(sf::Vector2f p_TargetPos)
		{
			sf::Vector2f Position = m_Utility->ConvertVector_B2toSF(m_CurrentBody->GetPosition());
			m_DesiredVelocity = Normalize(p_TargetPos - Position);
			m_DesiredVelocity.x *= m_MaxVelocity.x;
			m_DesiredVelocity.y *= m_MaxVelocity.y;
		}
		sf::Vector2f SteeringManager::Normalize(sf::Vector2f p_Vector)
		{
			float Length = sqrtf(p_Vector.x * p_Vector.x + p_Vector.y * p_Vector.y);
			if (Length > 0.0f) {
				p_Vector.x /= Length;
				p_Vector.y /= Length;
			}
			return p_Vector;
		}
		sf::Vector2f SteeringManager::Truncate(sf::Vector2f p_TruncTarget, sf::Vector2f p_MaxValue)
		{
			if (p_TruncTarget.x > p_MaxValue.x)
			{
				p_TruncTarget.x = p_MaxValue.x;
			}
			if (p_TruncTarget.y > p_MaxValue.y)
			{
				p_TruncTarget.y = p_MaxValue.y;
			}
			return p_TruncTarget;
		}
	}
}