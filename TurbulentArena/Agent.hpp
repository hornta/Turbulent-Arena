//Agent.hpp

#pragma once
#include "AIObserver.hpp"
#include "Visibility.hpp"
#include "SteeringManager.hpp"
#include "Timer.hpp"
#include "MovementStats.hpp"
#include "Pathfinder.hpp"

namespace bjoernligan
{
	class ClanMember;
	class Utility;

	namespace ai
	{
		class SteeringManager;
		class BehaviorTree;
		class PhysicsBody;
		class SenseData;
		class Sense;

		class Agent : public AIObserver
		{
		public:
			Agent(ClanMember* p_xOwner, Sense* sense);
			virtual ~Agent();

			void Update(const float &p_fDeltaTime);
			void Sense();
			void Decide();
			void Act();
			SenseData* getSense() const;
			ClanMember* getOwner() const;

			virtual void OnNotify(/*add parameters*/);

			BehaviorTree* getBehaviorTree();
			void setSenseRadius(float p_senseRadius);
			void setSenseVisibleArea(Visibility::Light * p_senseVisibleArea);

			void InitializeSteering(b2Body* p_CurrentBody, MovementStats* p_MovementStats);

			//tomas BT-methods (bad solution)
			int32_t SensedEnemyCount();
			void ChooseWanderPos();
			void MoveToTargetPos();
			bool AtMoveTarget();
			bool canFindTarget();
			bool getPathToVisibleTarget(Agent* agent);
			bool getPathToRandomVisibleTarget();

		protected:
			std::unique_ptr<BehaviorTree> m_xBT;
			std::unique_ptr<SteeringManager> m_Steering;
			std::unique_ptr<SenseData> m_senseData;
			bjoernligan::Timer m_xSenseTimer, m_xDecideTimer;

			Pathfinder::Path m_CurrentPath;
			sf::Vector2i m_xCurrentMapPos;
			Utility* m_Utility;

			//tomas BT-variables (bad solution)
			sf::Vector2f m_xMoveTarget;
			ClanMember* m_xOwner;
		};
	}
}