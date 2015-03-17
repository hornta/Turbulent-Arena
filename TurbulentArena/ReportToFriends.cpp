#include "stdafx.h"
#include "ReportToFriends.h"
#include "Agent.hpp"
#include "Scout.hpp"

namespace bjoernligan
{
	namespace ai
	{
		ReportToFriends::ReportToFriends()
		{

		}

		EBNodeStatus ReportToFriends::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			Scout* scout = static_cast<Scout*>(m_xAgent->getOwner());

			std::vector<SenseAgentData*> xVisFriends = m_xAgent->getSense()->getVisibleFriends();
			
			//shaderforge
			//bool m_bCloseEnlightened(false);
			for (uint32_t i = 0; i < xVisFriends.size(); ++i)
			{
				for (uint32_t k = 0; k < scout->m_enlightendFriends.size(); ++k)
				{
					if (xVisFriends[i]->m_agent == scout->m_enlightendFriends[k]->agent->m_agent
						&& !scout->m_enlightendFriends[k]->enlightend)
					{
						m_xAgent->getOwner()->GetMovementStats().SetMaxVelocity(600);
						m_xAgent->getPathToVisibleTarget(scout->m_enlightendFriends[k]->agent.get());
						return EBNodeStatus::Success;
					}
				}
			}

			for (std::size_t i = 0; i < scout->m_enlightendFriends.size(); ++i)
			{
				if (!scout->m_enlightendFriends[i]->enlightend)
				{
					m_xAgent->getOwner()->GetMovementStats().SetMaxVelocity(600);
					m_xAgent->getPathToVisibleTarget(scout->m_enlightendFriends[i]->agent.get());
					break;
				}
			}

			return EBNodeStatus::Success;
		}
	}
}
