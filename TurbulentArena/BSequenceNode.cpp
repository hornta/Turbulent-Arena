//BSequenceNode.cpp

#include "stdafx.h"
#include "BSequenceNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BSequenceNode::BSequenceNode()
		{

		}

		BSequenceNode::~BSequenceNode()
		{

		}

		EBNodeStatus BSequenceNode::Process()
		{
			if (m_axChildren.empty())
				return EBNodeStatus::Fail;

			for (uint32_t i = 0; i < m_axChildren.size(); ++i)
			{
				//success: next child
				//fail: break loop and return fail
				//running: break loop and return running ?

				EBNodeStatus eRet = m_axChildren[i]->Process();
				if (eRet == EBNodeStatus::Success)
					continue;
				else if (eRet == EBNodeStatus::Running)
					return EBNodeStatus::Running;
				else if (eRet == EBNodeStatus::Fail)
					return EBNodeStatus::Fail;
				else if (eRet == EBNodeStatus::Invalid)
					return EBNodeStatus::Invalid;
			}

			return EBNodeStatus::Success;
		}
	}
}