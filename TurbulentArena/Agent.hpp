//Agent.hpp

#pragma once
#include "AIObserver.hpp"
#include "Visibility.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class SteeringManager;
		class BehaviorTree;
		class PhysicsBody;

		class Agent : public AIObserver
		{
		public:
			Agent();
			virtual ~Agent();

			void Sense();
			void Decide();
			//acting is performed by owner object

			virtual void OnNotify(/*add parameters*/);

			void SetBehaviorTree(BehaviorTree* p_xBT);
			void setSenseRadius(float p_senseRadius);
			void setSenseVisibleArea(Visibility::Light * p_senseVisibleArea);

			//Steering stuff
			void InitializeSteering(b2Body* p_CurrentBody, const b2Vec2& p_MaxVelocity, const float& p_SlowDownRadius);
			void Wander();
			void Seek(const b2Vec2& p_TargetPos);
			void Flee(const b2Vec2& p_TargetPos);
			void Pursuit(b2Body* p_TargetBody);
			void Evade(b2Body* p_TargetBody);
			void UpdateSteering();

			//tomas BT-methods (bad solution)
			bool EnemyClose();
			void ChooseEnemyTarget();
			void MoveToTargetPos();

		protected:
			BehaviorTree* m_xBT;
			float m_senseRadius;
			Visibility::Light* m_senseVisibleArea;

			SteeringManager* m_Steering;

			//tomas BT-variables (bad solution)

		};
	}
}