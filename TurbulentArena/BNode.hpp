//BNode.hpp

#pragma once

namespace bjoernligan
{
	namespace ai
	{
		class Agent;

		enum EBNodeStatus
		{
			Invalid,
			Fail,
			Success,
			Running,
		};

		class BNode
		{
		public:
			BNode();
			virtual ~BNode();

			virtual EBNodeStatus Process() = 0;

			void AttachAgent(Agent* p_xAgent);

			//virtual void CleanUp();

		protected:
			Agent* m_xAgent;
		};
	}
}