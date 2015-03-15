#include "stdafx.h"
#include "AttackEnemy.hpp"
#include "Agent.hpp"
#include "ClanMember.hpp"
#include "AudioManager.hpp"

namespace bjoernligan
{
	namespace ai
	{
		AttackEnemy::AttackEnemy()
		{

		}

		EBNodeStatus AttackEnemy::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			if (m_xAgent->IsEnemyWithinAttackRange() && m_xAgent->CanAttack())
			{
				// Get closest enemy
				Agent* agent = m_xAgent->getSense()->getVisibleEnemies().front()->m_agent;
				agent->getOwner()->GetCombat()->TakeDamage(m_xAgent->getOwner()->GetCombat()->GetDamage());

				// Play attack sound
				ServiceLocator<system::AudioManager>::GetService()->PlaySoundFromGroup("Battle");
			}

			return EBNodeStatus::Success;
		}
	}
}
