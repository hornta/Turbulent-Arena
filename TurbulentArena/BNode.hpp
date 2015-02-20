//BNode.hpp

#pragma once

namespace bjoernligan
{
	namespace ai
	{
		class AIObserver;

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

			void AttachAgent(AIObserver* p_xAgent);

			virtual void CleanUp();

		protected:
			AIObserver* m_xAgent;
		};
	}
}