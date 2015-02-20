//BSelectorNode.cpp

#include "stdafx.h"
#include "BSelectorNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BSelectorNode::BSelectorNode()
		{

		}

		BSelectorNode::~BSelectorNode()
		{

		}

		EBNodeStatus BSelectorNode::Process()
		{
			if (m_axChildren.empty())
				return EBNodeStatus::Fail;

			for (uint32_t i = 0; i < m_axChildren.size(); ++i)
			{
				//success: break loop and return success
				//fail: next child
				//running: break loop and return running ?

				EBNodeStatus eRet = m_axChildren[i]->Process();
				if (eRet == EBNodeStatus::Success)
					return EBNodeStatus::Success;
				else if (eRet == EBNodeStatus::Running)
					return EBNodeStatus::Running;
				else if (eRet == EBNodeStatus::Fail)
					continue;
				else if (eRet == EBNodeStatus::Invalid)
					return EBNodeStatus::Invalid;
			}

			return EBNodeStatus::Fail;
		}
	}
}
