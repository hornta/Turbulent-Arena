#include "stdafx.h"
#include "HasSomethingToReport.h"
#include "Agent.hpp"
#include "Scout.hpp"

namespace bjoernligan
{
	namespace ai
	{
		HasSomethingToReport::HasSomethingToReport()
		{

		}

		EBNodeStatus HasSomethingToReport::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			Scout* scout = static_cast<Scout*>(m_xAgent->getOwner());

			bool toReport = false;
			for (std::size_t i = 0; i < scout->m_enlightendFriends.size(); ++i)
			{
				if (!scout->m_enlightendFriends[i]->enlightend)
				{
					toReport = true;
				}
			}

			if (toReport)
				return EBNodeStatus::Success;
			return EBNodeStatus::Fail;
		}
	}
}
