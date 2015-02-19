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

			virtual void Update(const float &p_fDeltaTime);
			virtual void OnNotify(/*Add Parameters here*/);

			void SetBehaviorTree(BehaviorTree* p_xBT);

		protected:
			BehaviorTree* m_xBT;
		};
	}
}