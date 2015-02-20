//BMoveToNode.cpp

#include "stdafx.h"
#include "BMoveToNode.hpp"
#include "AIEvent.hpp"
#include "AIObserver.hpp"

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

			AIEvent xEvent;
			xEvent.m_eType = AIEventType::MoveTarget;
			xEvent.m_xContent.m_xVec2i.x = m_xTo.x;
			xEvent.m_xContent.m_xVec2i.x = m_xTo.y;

			m_xAgent->OnNotify(xEvent);
			
			//if (agent is on the way)
			//return Running

			return EBNodeStatus::Success;
		}
	}
}