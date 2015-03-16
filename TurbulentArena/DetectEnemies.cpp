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

				for (std::size_t k = 0; k < scout->m_enlightendFriends.size(); k++)
				{
					for (std::size_t l = 0; l < scout->m_enlightendFriends[k]->toReport.size(); ++l)
					{
						if (scout->m_enlightendFriends[k]->toReport[l]->agent == enemies[i]->m_agent)
						{
							scout->m_enlightendFriends[k]->toReport[l]->position = enemies[i]->m_agent->getOwner()->getSprite()->getPosition();
							scout->m_enlightendFriends[k]->enlightend = false;
							found = true;
						}
					}
				}

				if (!found)
				{
					for (std::size_t k = 0; k < scout->m_enlightendFriends.size(); k++)
					{
						scout->m_enlightendFriends[k]->toReport.emplace_back(std::make_unique<Scout::ReportData>(enemies[i]->m_agent, enemies[i]->m_agent->getOwner()->getSprite()->getPosition()));
						scout->m_enlightendFriends[k]->enlightend = false;
					}
				}
			}

			return EBNodeStatus::Success;
		}
	}
}
