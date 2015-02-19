//Agent.hpp

#pragma once

namespace bjoernligan
{
	namespace ai
	{
		class BehaviorTree;

		class Agent
		{
		public:
			Agent();
			virtual ~Agent();

			virtual void Update(const float &p_fDeltaTime);
			void SetBehaviorTree(BehaviorTree* p_xBT);

		protected:
			BehaviorTree* m_xBT;
		};
	}
}