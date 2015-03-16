#include "stdafx.h"
#include "EnemyWithinRadius.hpp"
#include "Agent.hpp"
#include "ClanMember.hpp"

namespace bjoernligan
{
	namespace ai
	{
		EnemyWithinRadius::EnemyWithinRadius()
		{

		}

		EBNodeStatus EnemyWithinRadius::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			if (m_xAgent->getOwner()->GetClass() == ClanMember::EClass::Scout)
			{

			}
			if (m_xAgent->IsEnemyWithinAttackRange())
			{
				return EBNodeStatus::Success;
			}
			else
				return EBNodeStatus::Fail;
		}

	}
}
