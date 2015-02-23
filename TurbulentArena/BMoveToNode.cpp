//BMoveToNode.cpp

#include "stdafx.h"
#include "BMoveToNode.hpp"
#include "Agent.hpp"

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

		EBNodeStatus BMoveToNode::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			//m_xAgent->MoveToTarget();

			//get move target from agent
			//m_xTo = m_xAgent->GetMoveTarget()

			//if (agent is on the way)
			//return Running

			return EBNodeStatus::Success;
		}
	}
}