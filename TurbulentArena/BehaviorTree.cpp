//BehaviorTree.cpp

#include "stdafx.h"
#include "BehaviorTree.hpp"
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BehaviorTree::BehaviorTree()
			: m_xRoot(nullptr)
			, m_xCurrentRunning(nullptr)
		{

		}

		void BehaviorTree::Break()
		{
			m_xCurrentRunning = nullptr;
		}

		void BehaviorTree::Process()
		{
			if (m_xRoot)
				m_xRoot->Process();
		}
	}
}