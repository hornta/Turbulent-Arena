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
			int friends = m_xAgent->getSense()->getVisibleFriends().size();
			int enemy = m_xAgent->getSense()->getVisibleEnemies().size();
			int diff = friends - enemy;

			float Brave = m_xAgent->getOwner()->GetCombat()->getBrave();
			if (Brave < 0.9f && m_xAgent->getOwner()->GetCombat()->GetHealthPercentage() < 0.1f)
			{
				return EBNodeStatus::Success;
			}
			if (Brave < 1.0f && diff < -4)
			{
				return EBNodeStatus::Success;
			}
			else if (Brave < 0.8f && diff < -2)
			{
				return EBNodeStatus::Success;
			}
			else if (Brave < 0.6f && diff < 0)
			{
				return EBNodeStatus::Success;
			}
			else if (Brave < 0.4f && diff < 2)
			{
				return EBNodeStatus::Success;
			}
			else if (Brave < 0.2f && diff < 4)
			{
				return EBNodeStatus::Success;
			}
			//Maybe add a check to Success also if some Enemy is puruing agent.
			return EBNodeStatus::Fail;
		}
	}
}
