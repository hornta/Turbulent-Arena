//BSucceederNode.cpp

#include "stdafx.h"
#include "BSucceederNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BSucceederNode::BSucceederNode()
		{

		}

		BSucceederNode::~BSucceederNode()
		{

		}

		EBNodeStatus BSucceederNode::Process()
		{
			if (m_axChildren.size() != 1)
				return EBNodeStatus::Invalid;

			m_axChildren[0]->Process();
			return EBNodeStatus::Success;
		}
	}
}