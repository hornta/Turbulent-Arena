#include "stdafx.h"
#include "ReportToFriends.h"
#include "Agent.hpp"

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

			//m_xAgent->getPathToVisibleTarget();

			return EBNodeStatus::Success;
		}
	}
}
