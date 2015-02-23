//Agent.hpp

#pragma once
#include "AIObserver.hpp"
#include "Visibility.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class BehaviorTree;

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

			//tomas BT-methods (bad solution)
			bool EnemyClose();
			void ChooseEnemyTarget();
			void MoveToTargetPos();


		protected:
			BehaviorTree* m_xBT;
			float m_senseRadius;
			Visibility::Light* m_senseVisibleArea;

			//tomas BT-variables (bad solution)

		};
	}
}