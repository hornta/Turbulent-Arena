//BehaviorTree.hpp

#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class BehaviorTree
		{
		public:
			BehaviorTree();

			void Break();
			void Process();

			template<class T>
			T* CreateRoot();

		private:
			std::unique_ptr<BNode> m_xRoot;
			BNode* m_xCurrentRunning;
		};

		template<class T>
		T* BehaviorTree::CreateRoot()
		{
			m_xRoot = std::make_unique<T>();
			return static_cast<T*>(m_xRoot.get());
		}
	}
}