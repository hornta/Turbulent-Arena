#include "stdafx.h"
#include "DetectEnemies.h"
#include "Agent.hpp"
#include "Scout.hpp"

namespace bjoernligan
{
	namespace ai
	{
		DetectEnemies::DetectEnemies()
		{

		}

		EBNodeStatus DetectEnemies::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			std::vector<SenseAgentData*> enemies = m_xAgent->getSense()->getVisibleEnemies();
			if (enemies.empty()) return EBNodeStatus::Fail;

			Scout* scout = static_cast<Scout*>(m_xAgent->getOwner());
			for (std::size_t i = 0; i < enemies.size(); ++i)
			{
				bool found = false;

				// Check if we havent seen this enemy before
				for (std::size_t k = 0; k < scout->m_toReport.size(); k++)
				{
					// We have seen this person before.
					// Update position
					if (enemies[i]->m_agent == scout->m_toReport[k].agent)
					{
						found = true;
						scout->m_toReport[k].position = enemies[i]->m_agent->getOwner()->getSprite()->getPosition();
					}
				}

				if (!found)
					scout->m_toReport.push_back({enemies[i]->m_agent, enemies[i]->m_agent->getOwner()->getSprite()->getPosition()});
			}

			return EBNodeStatus::Success;
		}
	}
}
