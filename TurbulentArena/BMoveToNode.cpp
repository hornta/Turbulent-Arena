//BMoveToNode.cpp

#include "stdafx.h"
#include "BMoveToNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BMoveToNode::BMoveToNode()
		{

		}

		BMoveToNode::~BMoveToNode()
		{

		}

		void BMoveToNode::SetTo(const sf::Vector2i &p_xTo)
		{
			m_xTo = p_xTo;
		}

		EBNodeStatus BMoveToNode::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			//get move target from agent
			//m_xTo = m_xAgent->GetMoveTarget()

			//if (agent is on the way)
			//return Running

			return EBNodeStatus::Success;
		}
	}
}