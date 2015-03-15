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
				for (unsigned int i = 0; i < m_xAgent->getSense()->getVisibleEnemies().size(); i++)
				{
					m_xAgent->GetSteering()->Flee(m_xAgent->getSense()->getVisibleEnemies()[i]->getOwner()->getSprite()->getPosition());
				}
				return EBNodeStatus::Success;
			}
				// Play Run away sound from monty python?

			return EBNodeStatus::Fail;
		}
	}
}
