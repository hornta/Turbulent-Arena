//BehaviorTree.hpp

#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class Agent;

		class BehaviorTree
		{
		public:
			BehaviorTree();

			void Break();
			void Process();

			template<class T>
			T* CreateRoot();

			void AttachAgent(Agent* p_xAgent);

		private:
			std::unique_ptr<BNode> m_xRoot;
			BNode* m_xCurrentRunning;
			Agent* m_xAgent;
		};

		template<class T>
		T* BehaviorTree::CreateRoot()
		{
			m_xRoot = std::make_unique<T>();
			m_xRoot->AttachAgent(m_xAgent);
			return static_cast<T*>(m_xRoot.get());
		}
	}
}