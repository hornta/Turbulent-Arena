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

		EBNodeStatus BMoveToNode::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			m_xAgent->MoveToTargetPos();

			//if (agent is on the way)
			//return Running
			if (!m_xAgent->AtMoveTarget())
				return EBNodeStatus::Running;

			return EBNodeStatus::Success;
		}
	}
}