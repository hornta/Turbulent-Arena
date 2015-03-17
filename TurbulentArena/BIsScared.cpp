#include "stdafx.h"
#include "BIsScared.hpp"
#include "Agent.hpp"
#include "ClanMember.hpp"
#include "Clan.hpp"
#include "ServiceLocator.hpp"
#include "ClanManager.hpp"

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

			if (!m_xAgent->getSense()->getVisibleEnemies().empty())
			{	
				//Axeman
				if (m_xAgent->getOwner()->GetClass() == ClanMember::EClassAxeman)
				{
					std::vector<SenseAgentData*> VisibleFriends = m_xAgent->getSense()->getVisibleFriends();
					std::vector<SenseAgentData*> VisibleEnemies = m_xAgent->getSense()->getVisibleEnemies();
					float FriendHP = m_xAgent->getOwner()->GetCombat()->GetHealthPercentage();
					for (unsigned int i = 0; i < VisibleFriends.size(); i++)
					{
						FriendHP += VisibleFriends[i]->m_agent->getOwner()->GetCombat()->GetHealthPercentage();
					}
					float EnemyHp = 0.0f;
					for (unsigned int i = 0; i < VisibleEnemies.size(); i++)
					{
						EnemyHp += VisibleEnemies[i]->m_agent->getOwner()->GetCombat()->GetHealthPercentage();
					}
					float diff = FriendHP / EnemyHp;
					float Brave = m_xAgent->getOwner()->GetCombat()->getBrave();
					if (Brave < 0.9f && m_xAgent->getOwner()->GetCombat()->GetHealthPercentage() < 0.1f)
					{
						return EBNodeStatus::Success;
					}
					if (Brave < 1.0f && diff < 0.3f)
						return EBNodeStatus::Success;
					else if (Brave < 0.9f && diff < 0.4f)
						return EBNodeStatus::Success;
					else if (Brave < 0.8f && diff < 0.5f)
						return EBNodeStatus::Success;
					else if (Brave < 0.7f && diff < 0.6f)
						return EBNodeStatus::Success;
					else if (Brave < 0.6f && diff < 0.7f)
						return EBNodeStatus::Success;
					else if (Brave < 0.5f && diff < 1.0f)
						return EBNodeStatus::Success;
					else if (Brave < 0.4f && diff < 1.6f)
						return EBNodeStatus::Success;
					else if (Brave < 0.3f && diff < 2.0f)
						return EBNodeStatus::Success;
					else if (Brave < 0.2f && diff < 3.0f)
						return EBNodeStatus::Success;
					else if (Brave < 0.1f && diff < 5.0f)
						return EBNodeStatus::Success;
				}
				//Scout
				else if (m_xAgent->getOwner()->GetClass() == ClanMember::EClassScout)
				{
					if (m_xAgent->getSense()->getVisibleFriends().size() < 4)
						return EBNodeStatus::Success;

					float fBrave = m_xAgent->getOwner()->GetCombat()->getBrave();
					float fHealthPercent = m_xAgent->getOwner()->GetCombat()->GetHealthPercentage();
					if (fBrave > 0.90f)
						return EBNodeStatus::Fail;
					if (fBrave < 0.90f && fHealthPercent < 0.25f)
						return EBNodeStatus::Success;
					if (fBrave < 0.60f && fHealthPercent < 0.5f)
						return EBNodeStatus::Success;
					if (fBrave < 0.30f && fHealthPercent < 0.75f)
						return EBNodeStatus::Success;
				}
			}
			return EBNodeStatus::Fail;
		}
	}
}
