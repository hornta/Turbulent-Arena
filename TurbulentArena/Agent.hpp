//Agent.hpp

#pragma once
#include "AIObserver.hpp"
#include "SteeringManager.hpp"
#include "Timer.hpp"
#include "MovementStats.hpp"
#include "Pathfinder.hpp"
#include "BehaviorTree.hpp"
#include "Sense.hpp"

namespace bjoernligan
{
	class ClanMember;
	class Utility;
	class Map;

	namespace ai
	{
		class SteeringManager;
		class PhysicsBody;
		class Sense;

		class Agent : public AIObserver
		{
			friend ClanMember;
		public:
			Agent(ClanMember* p_xOwner, Sense* sense);

			void Update(const float &p_fDeltaTime);
			void Sense();
			void Decide();
			void Act();
			SteeringManager* GetSteering() const;
			Pathfinder::Path * GetPath();
			SenseData* getSense() const;
			ClanMember* getOwner() const;

			virtual void OnNotify(/*add parameters*/);

			BehaviorTree* getBehaviorTree();
			void setSenseRadius(float p_senseRadius);

			void InitializeSteering(b2Body* p_CurrentBody,MovementStats &p_MovementStats);

			//tomas BT-methods (bad solution)
			int32_t SensedEnemyCount();
			void ChooseWanderPos();
			bool AtMoveTarget();
			bool canFindTarget();
			bool getPathToVisibleTarget(SenseAgentData* senseAgentData);
			bool getPathToRandomVisibleTarget();

			void MoveToTargetPos();

			bool IsEnemyWithinAttackRange();
			bool CanAttack() const;

		protected:
			std::unique_ptr<BehaviorTree> m_xBT;
			std::unique_ptr<SteeringManager> m_Steering;
			std::unique_ptr<SenseData> m_senseData;
			sf::Clock m_senseDecideTimer;


			Pathfinder::Path m_CurrentPath;
			sf::Vector2i m_xCurrentMapPos;
			Map* m_map;

			ClanMember* m_xOwner;
		};
	}
}