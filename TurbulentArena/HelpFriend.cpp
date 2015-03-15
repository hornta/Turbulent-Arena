#include "stdafx.h"
#include "HelpFriend.h"
#include "Agent.hpp"

namespace bjoernligan
{
	namespace ai
	{
		HelpFriend::HelpFriend()
		{

		}

		EBNodeStatus HelpFriend::Process()
		{
			if (!m_xAgent)
			{
				return EBNodeStatus::Invalid;
			}

			std::vector<SenseAgentData*> visibleFriends = m_xAgent->getSense()->getVisibleFriends();
			for (std::size_t i = 0; i < visibleFriends.size(); ++i)
			{
				std::vector<SenseAgentData*> friendsEnemies = visibleFriends[i]->m_agent->getSense()->getVisibleEnemies();

				if (friendsEnemies.empty())
				{
					continue;
				}
				
				m_xAgent->getPathToVisibleTarget(friendsEnemies.front());

				return EBNodeStatus::Success;
			}

			return EBNodeStatus::Fail;
		}
	}
}
