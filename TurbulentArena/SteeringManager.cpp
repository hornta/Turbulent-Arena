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
			m_MaxVelocity = sf::Vector2f(0, 0);
			m_MaxForce = sf::Vector2f(0, 0);
			m_SlowDownRadius = 0.0f;
			m_HasSlowDown = false;
			m_CurrentBody = nullptr;
			m_Utility = nullptr;
		}
		SteeringManager::~SteeringManager()
		{

		}
		bool SteeringManager::Initialize()
		{
			m_Utility = ServiceLocator<Utility>::GetService();
			return true;
		}
		/*Add more parameters to this function*/
		void SteeringManager::SetCurrentBody(b2Body* p_CurrentBody, const sf::Vector2f& p_MaxVelocity, const float& p_SlowDownRadius)
		{
			Reset();
			m_CurrentBody = p_CurrentBody;
			//MaxForce and max velocity is now the same thing. this might change.
			m_MaxForce = p_MaxVelocity;
			m_MaxVelocity = p_MaxVelocity;
			m_SlowDownRadius = m_Utility->ConvertFloat_B2toSF(p_SlowDownRadius);
			if (m_SlowDownRadius > 0.0f)
			{
				m_HasSlowDown = true;
			}
		}
		void SteeringManager::Wander()
		{

		}
		void SteeringManager::Seek(const sf::Vector2f& p_TargetPos)
		{
			if (!m_CurrentBody)
				return;

			sf::Vector2f Velocity = m_Utility->ConvertVector_B2toSF(m_CurrentBody->GetLinearVelocity());
			sf::Vector2f Position = m_Utility->ConvertVector_B2toSF(m_CurrentBody->GetPosition());
			m_Steering = GetDesiredVelocity(Position, p_TargetPos, m_HasSlowDown) - Velocity;
		}
		void SteeringManager::Flee(const sf::Vector2f& p_TargetPos)
		{
			if (!m_CurrentBody)
				return;

			sf::Vector2f Velocity = m_Utility->ConvertVector_B2toSF(m_CurrentBody->GetLinearVelocity());
			sf::Vector2f Position = m_Utility->ConvertVector_B2toSF(m_CurrentBody->GetPosition());
			//send in positions in switched order compared to Seek() to get the negative.(the opposite side)
			m_Steering = GetDesiredVelocity(p_TargetPos, Position, false) - Velocity;
		}
		void SteeringManager::Pursuit(b2Body* p_TargetBody)
		{
			Seek(GetPredictedPosition(p_TargetBody));
		}
		void SteeringManager::Evade(b2Body* p_TargetBody)
		{
			Flee(GetPredictedPosition(p_TargetBody));
		}
		void SteeringManager::Update()
		{
			if ((abs(m_Steering.x) < 0.01f && abs(m_Steering.y) < 0.01f)
				|| !m_CurrentBody)
				return;

			//OBS all in here needs testing

			m_Steering = Truncate(m_Steering, m_MaxForce);
			//(steering.scaleBy(1 / host.getMass());)
			//Add Body mass to calculations
			/*float BodyMass = m_Utility->ConvertFloat_B2toSF(m_CurrentBody->GetMass());
			m_Steering = m_Steering / BodyMass;*/

			//try this
			m_CurrentBody->ApplyForce(m_Utility->ConvertVector_SFtoB2(m_Steering), m_CurrentBody->GetWorldCenter(), true);
			//or this?
			/*m_Velocity = Truncate(m_Velocity + m_Steering, m_MaxVelocity);*/
			//m_CurrentBody->SetLinearVelocity(m_Utility->ConvertVector_SFtoB2(m_Steering * 5000.0f));

			// (position.incrementBy(velocity);) eller gör box2d detta?
			/*p_CurrentPos = p_CurrentPos + m_Velocity;*/
		}
		void SteeringManager::Reset()
		{
			m_Steering = sf::Vector2f(0, 0);
			m_MaxVelocity = sf::Vector2f(0, 0);
			m_MaxForce = sf::Vector2f(0, 0);
			m_SlowDownRadius = 0.0f;
			m_HasSlowDown = false;
			m_CurrentBody = nullptr;
		}
		sf::Vector2f SteeringManager::GetDesiredVelocity(sf::Vector2f p_CurrentPos, sf::Vector2f p_TargetPos, bool p_HasSlowDown)
		{
			sf::Vector2f DesiredVelocity;
			//if has slow down radius
			if (p_HasSlowDown)
			{
				float Distance = GetEuclideanDistance(p_CurrentPos, p_TargetPos);

				if (Distance < m_SlowDownRadius)
				{
					// Inside the slowing area
					DesiredVelocity = Normalize(p_TargetPos - p_CurrentPos);
					DesiredVelocity.x *= m_MaxVelocity.x * (Distance / m_SlowDownRadius);
					DesiredVelocity.y *= m_MaxVelocity.y * (Distance / m_SlowDownRadius);
				}
				else
				{
					// Outside the slowing area.
					DesiredVelocity = Normalize(p_TargetPos - p_CurrentPos);
					DesiredVelocity.x *= m_MaxVelocity.x;
					DesiredVelocity.y *= m_MaxVelocity.y;
				}
			}
			//Used without slow down Radius
			else
			{
				DesiredVelocity = Normalize(p_TargetPos - p_CurrentPos);
				DesiredVelocity.x *= m_MaxVelocity.x;
				DesiredVelocity.y *= m_MaxVelocity.y;
			}
			return DesiredVelocity;
		}
		sf::Vector2f SteeringManager::GetPredictedPosition(b2Body* p_TargetBody)
		{
			sf::Vector2f TargetVelocity = m_Utility->ConvertVector_B2toSF(p_TargetBody->GetLinearVelocity());
			sf::Vector2f TargetPosition = m_Utility->ConvertVector_B2toSF(p_TargetBody->GetPosition());
			sf::Vector2f CurrentBodyPos = m_Utility->ConvertVector_B2toSF(m_CurrentBody->GetPosition());

			//distance / speed, (this is basicly a prediction number on where target will be in T number of iterations) 
			int PredictionCycle = static_cast<int>(round
				(GetEuclideanDistance(CurrentBodyPos, TargetPosition) / GetEuclideanDistance(CurrentBodyPos, m_MaxVelocity)));
			
			sf::Vector2f PredictedPos;
			PredictedPos.x = TargetPosition.x + TargetVelocity.x * PredictionCycle;
			PredictedPos.y = TargetPosition.y + TargetVelocity.y * PredictionCycle;
			return PredictedPos;
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
		float SteeringManager::GetEuclideanDistance(sf::Vector2f p_PositionA, sf::Vector2f p_PositionB)
		{
			//Distance by euclidean formula
			float Distance = pow((p_PositionA.x - p_PositionB.x), 2) + pow((p_PositionA.y - p_PositionB.y), 2);
			return sqrtf(Distance);
		}
	}
}