//BNode.cpp

#include "stdafx.h"
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BNode::BNode()
		{
			m_xParent = nullptr;
			m_xAgent = nullptr;
		}

		BNode::~BNode()
		{

		}

		void BNode::SetParent(BNode* p_xParent)
		{
			m_xParent = p_xParent;
		}

		void BNode::AttachAgent(AIObserver* p_xAgent)
		{
			m_xAgent = p_xAgent;
		}

		void BNode::CleanUp()
		{

		}
	}
}