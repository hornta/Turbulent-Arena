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

			for (std::size_t i = 0; i < scout->m_enlightendFriends.size(); ++i)
			{
				if (!scout->m_enlightendFriends[i]->enlightend)
				{
					m_xAgent->getPathToVisibleTarget(scout->m_enlightendFriends[i]->agent.get());
					break;
				}
			}

			return EBNodeStatus::Success;
		}
	}
}
