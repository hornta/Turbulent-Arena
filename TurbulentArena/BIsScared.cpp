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

			if (!m_xAgent->getSense()->getVisibleEnemies().empty())
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
				
				/*int friends = m_xAgent->getSense()->getVisibleFriendsInRadius(150.f).size();
				int enemy = m_xAgent->getSense()->getVisibleEnemiesInRadius(150.f).size();
				int diff = friends - enemy;*/

				float Brave = m_xAgent->getOwner()->GetCombat()->getBrave();
				if (Brave < 0.9f && m_xAgent->getOwner()->GetCombat()->GetHealthPercentage() < 0.1f)
				{
					return EBNodeStatus::Success;
				}
				if (Brave < 1.0f && diff < 0.3f)
				{
					return EBNodeStatus::Success;
				}
				else if (Brave < 0.8f && diff < 0.5f)
				{
					return EBNodeStatus::Success;
				}
				else if (Brave < 0.6f && diff < 0.6f)
				{
					//if (m_xAgent->getOwner()->GetCombat()->GetHealthPercentage() > 0.9f)
					//if (diff < 1.8f && diff > 0.2f)
					/*	return EBNodeStatus::Fail;
					else if*/ 
						return EBNodeStatus::Success;
				}
				else if (Brave < 0.4f && diff < 2.0f)
				{
					return EBNodeStatus::Success;
				}
				else if (Brave < 0.2f && diff < 3.0f)
				{
					return EBNodeStatus::Success;
				}
			}
			//Maybe add a check to Success also if some Enemy is puruing agent.
			return EBNodeStatus::Fail;
		}
	}
}
