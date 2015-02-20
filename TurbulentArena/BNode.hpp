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

			void SetParent(BNode* p_xParent);
			void AttachAgent(AIObserver* p_xAgent);

			virtual void CleanUp();

		protected:
			BNode* m_xParent;
			AIObserver* m_xAgent;
		};
	}
}