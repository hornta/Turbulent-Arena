//Agent.hpp

#pragma once
#include "AIObserver.hpp"
#include "Visibility.hpp"
#include "SteeringManager.hpp"
#include "Timer.hpp"

namespace bjoernligan
{
	class ClanMember;

	namespace ai
	{
		class SteeringManager;
		class BehaviorTree;
		class PhysicsBody;

		class Agent : public AIObserver
		{
		public:
			Agent(ClanMember* p_xOwner);
			virtual ~Agent();

			void Update(const float &p_fDeltaTime);
			void Sense();
			void Decide();
			//acting is performed by owner object

			virtual void OnNotify(/*add parameters*/);

			BehaviorTree* GetBehaviorTree();
			void setSenseRadius(float p_senseRadius);
			void setSenseVisibleArea(Visibility::Light * p_senseVisibleArea);

			//Steering stuff
			void InitializeSteering(b2Body* p_CurrentBody, const sf::Vector2f& p_MaxVelocity, const float& p_SlowDownRadius);
			void Wander();
			void Seek(const sf::Vector2f& p_TargetPos);
			void Flee(const sf::Vector2f& p_TargetPos);
			void Pursuit(b2Body* p_TargetBody);
			void Evade(b2Body* p_TargetBody);
			void UpdateSteering();

			//tomas BT-methods (bad solution)
			int32_t SensedEnemyCount();
			void ChooseWanderPos();
			void MoveToTargetPos();
			bool AtMoveTarget();

		protected:
			std::unique_ptr<BehaviorTree> m_xBT;
			float m_senseRadius;
			Visibility::Light* m_senseVisibleArea;
			bjoernligan::Timer m_xSenseTimer, m_xDecideTimer;

			SteeringManager* m_Steering;

			//tomas BT-variables (bad solution)
			sf::Vector2f m_xMoveTarget;
			sf::Vector2f m_MaxVelocity;
			ClanMember* m_xOwner;
		};
	}
}