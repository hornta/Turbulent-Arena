#include "stdafx.h"
#include "GetPathToEnemy.hpp"
#include "Agent.hpp"
#include "ClanMember.hpp"

namespace bjoernligan
{
	namespace ai
	{
		GetPathToEnemy::GetPathToEnemy()
		{

		}

		EBNodeStatus GetPathToEnemy::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			std::vector<SenseAgentData*> visibleEnemies = m_xAgent->getSense()->getVisibleEnemies();
			if (visibleEnemies.empty())
			{
				return EBNodeStatus::Fail;
			}
			m_xAgent->getOwner()->GetMovementStats().SetMaxVelocity(300.f);
			m_xAgent->getPathToVisibleTarget(visibleEnemies.front());
			return EBNodeStatus::Success;
		}

	}
}
