#include "stdafx.h"
#include "BFleeFromEnemies.hpp"
#include "Agent.hpp"
#include "ClanMember.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BFleeFromEnemies::BFleeFromEnemies()
		{

		}

		EBNodeStatus BFleeFromEnemies::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			if (!m_xAgent->getSense()->getVisibleEnemies().empty())
			{
				m_xAgent->GetPath()->setDone();
				float fFleeSpeed = 150.f + 200.0f * m_xAgent->getOwner()->GetCombat()->GetHealthPercentage();
				m_xAgent->getOwner()->GetMovementStats().SetMaxVelocity(fFleeSpeed);
				m_xAgent->GetSteering()->Flee(m_xAgent->getSense()->getVisibleEnemies().front()->m_agent->getOwner()->getSprite()->getPosition());
				m_xAgent->GetSteering()->Update();
				return EBNodeStatus::Success;
			}
				// Play Run away sound from monty python?
			return EBNodeStatus::Fail;
		}
	}
}
