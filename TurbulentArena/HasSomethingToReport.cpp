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

			if (scout->m_toReport.empty())
			{
				return EBNodeStatus::Fail;
			}

			return EBNodeStatus::Success;
		}

	}
}
