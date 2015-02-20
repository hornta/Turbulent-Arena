//BInverterNode.cpp

#include "stdafx.h"
#include "BInverterNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BInverterNode::BInverterNode()
		{

		}

		BInverterNode::~BInverterNode()
		{

		}

		EBNodeStatus BInverterNode::Process()
		{
			if (m_axChildren.size() != 1)
				return EBNodeStatus::Invalid;

			EBNodeStatus eRet = m_axChildren[0]->Process();
			if (eRet == EBNodeStatus::Fail)
				return EBNodeStatus::Success;
			if (eRet == EBNodeStatus::Success)
				return EBNodeStatus::Fail;
			if (eRet == EBNodeStatus::Running)
				return EBNodeStatus::Running;

			return EBNodeStatus::Invalid;
		}
	}
}