#include "stdafx.h"
#include "BIsScared.hpp"
#include "Agent.hpp"
#include "ClanMember.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BIsScared::BIsScared()
		{

		}

		EBNodeStatus BIsScared::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;
			if (m_xAgent->getOwner()->GetCombat()->GetHealthPercentage() < 0.8f)
			{
				if (m_xAgent->getOwner()->GetMood()->GetMoodValue(ai::Mood::Brave) < 0.6f)
				{
					return EBNodeStatus::Success;
				}
			}
			//Maybe add a check to Success also if some Enemy is puruing agent.
			return EBNodeStatus::Fail;
		}
	}
}
