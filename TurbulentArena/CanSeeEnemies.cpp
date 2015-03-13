#include "stdafx.h"
#include "CanSeeEnemies.h"
#include "Agent.hpp"

namespace bjoernligan
{
	namespace ai
	{
		CanSeeEnemies::CanSeeEnemies()
		{

		}

		EBNodeStatus CanSeeEnemies::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			if (m_xAgent->getSense()->getVisibleEnemies().empty())
			{
				return EBNodeStatus::Fail;
			}

			return EBNodeStatus::Success;
		}
	}
}
