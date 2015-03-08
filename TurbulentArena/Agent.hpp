//Agent.hpp

#pragma once
#include "AIObserver.hpp"
#include "Visibility.hpp"
#include "SteeringManager.hpp"
#include "Timer.hpp"
#include "MovementStats.hpp"

namespace bjoernligan
{
	class ClanMember;
	class Pathfinder;
	class Utility;

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
			void Act();

			virtual void OnNotify(/*add parameters*/);

			BehaviorTree* GetBehaviorTree();
			void setSenseRadius(float p_senseRadius);
			void setSenseVisibleArea(Visibility::Light * p_senseVisibleArea);

			//Steering stuff
			void InitializeSteering(b2Body* p_CurrentBody,MovementStats* p_MovementStats);
			/*void Wander();
			void Seek(const sf::Vector2f& p_TargetPos);
			void Flee(const sf::Vector2f& p_TargetPos);
			void Pursuit(b2Body* p_TargetBody);
			//void Evade(b2Body* p_TargetBody);
			void UpdateSteering();*/

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

			/*Pathfinder* m_Pathfinder;
			Pathfinder::Path m_CurrentPath;*/
			Utility* m_Utility;


			//tomas BT-variables (bad solution)
			sf::Vector2f m_xMoveTarget;
			ClanMember* m_xOwner;
		};
	}
}