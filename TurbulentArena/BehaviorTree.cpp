//BehaviorTree.cpp

#include "stdafx.h"
#include "BehaviorTree.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BehaviorTree::BehaviorTree()
		{
			m_xRoot = nullptr;
		}

		BehaviorTree::~BehaviorTree()
		{
			//nodes need a cleanup method to free allocated memory
		}

		void BehaviorTree::Process()
		{
			//m_xRoot->Process();
		}

		void BehaviorTree::SetRoot(BNode* p_xRoot)
		{
			m_xRoot = p_xRoot;
		}
	}
}