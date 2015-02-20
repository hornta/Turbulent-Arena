//Agent.hpp

#pragma once
#include "AIObserver.hpp"

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

		protected:
			BehaviorTree* m_xBT;
		};
	}
}