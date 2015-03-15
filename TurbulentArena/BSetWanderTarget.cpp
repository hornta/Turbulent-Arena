//BSetWanderTarget.cpp

#include "stdafx.h"
#include "BSetWanderTarget.hpp"
#include "Agent.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BSetWanderTarget::BSetWanderTarget()
		{

		}

		EBNodeStatus BSetWanderTarget::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;
			m_xAgent->ChooseWanderPos();

			return EBNodeStatus::Success;
		}
	}
}