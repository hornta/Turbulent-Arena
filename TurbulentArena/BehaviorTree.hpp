//BehaviorTree.hpp

#pragma once

namespace bjoernligan
{
	namespace ai
	{
		class BNode;

		class BehaviorTree
		{
		public:
			BehaviorTree();
			~BehaviorTree();

			void Process();
			void SetRoot(BNode* p_xRoot);

		private:
			BNode* m_xRoot;
		};
	}
}