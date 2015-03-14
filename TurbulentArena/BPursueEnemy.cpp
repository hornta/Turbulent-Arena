#include "stdafx.h"
#include "BPursueEnemy.hpp"
#include "Agent.hpp"
#include "ClanMember.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BPursueEnemy::BPursueEnemy()
		{

		}

		EBNodeStatus BPursueEnemy::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			if (!m_xAgent->getSense()->getVisibleEnemies().empty())
			{
				//at this time you only pursue the closest enemy.
				//maybe add so you pursue enemies that flee or that are weak.
				m_xAgent->GetSteering()->Pursuit(m_xAgent->getSense()->getVisibleEnemies().front()->getOwner()->getBody()->m_body);
				return EBNodeStatus::Success;
			}

			return EBNodeStatus::Fail;
		}
	}
}
