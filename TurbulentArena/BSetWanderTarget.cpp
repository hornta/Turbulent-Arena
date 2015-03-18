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

			//Set new Max velocity depending on injury
			float MoveSpeed = 100.f + 50.0f * m_xAgent->getOwner()->GetCombat()->GetHealthPercentage();
			m_xAgent->getOwner()->GetMovementStats().SetMaxVelocity(MoveSpeed);
			m_xAgent->ChooseWanderPos(true, 20);

			return EBNodeStatus::Success;
		}
	}
}