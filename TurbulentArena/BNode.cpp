//BNode.cpp

#include "stdafx.h"
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BNode::BNode()
		{
			m_xAgent = nullptr;
		}

		BNode::~BNode()
		{

		}

		void BNode::AttachAgent(Agent* p_xAgent)
		{
			m_xAgent = p_xAgent;
		}

		void BNode::CleanUp()
		{

		}
	}
}