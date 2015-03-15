//BSetWanderTarget.cpp

#include "stdafx.h"
#include "BSetWanderTarget.hpp"
#include "Agent.hpp"
#include "ClanMember.hpp"

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

			m_xAgent->getOwner()->GetMovementStats().SetMaxVelocity(150.f);
			m_xAgent->ChooseWanderPos(true, 20);

			return EBNodeStatus::Success;
		}
	}
}