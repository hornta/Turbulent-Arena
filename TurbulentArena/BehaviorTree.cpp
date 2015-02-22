//BehaviorTree.cpp

#include "stdafx.h"
#include "BehaviorTree.hpp"
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BehaviorTree::BehaviorTree()
		{
			m_xRoot = nullptr;
			m_xCurrentRunning = nullptr;
		}

		void BehaviorTree::Break()
		{
			m_xCurrentRunning = nullptr;
		}

		void BehaviorTree::Process()
		{
			m_xRoot->Process();
		}

		void BehaviorTree::SetRoot(BNode* p_xRoot)
		{
			m_xRoot = p_xRoot;
		}
	}
}