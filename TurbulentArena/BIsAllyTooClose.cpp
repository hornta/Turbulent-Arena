#include "stdafx.h"
#include "BIsAllyTooClose.hpp"
#include "Agent.hpp"
#include "ClanMember.hpp"
#include "Utils.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BIsAllyTooClose::BIsAllyTooClose()
		{

		}
		//this leaf is used so we can evade our own allies to avoid deadlocks
		EBNodeStatus BIsAllyTooClose::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			if (!m_xAgent->getSense()->getVisibleFriends().empty())
			{
				sf::Vector2f AgentVect = m_xAgent->getOwner()->GetPos();
				//front element = closest friend
				sf::Vector2f AllyVect = m_xAgent->getSense()->getVisibleFriends().front()->m_agent->getOwner()->GetPos();
				float distance = distanceBetweenPoints(AgentVect.x, AgentVect.y, AllyVect.x, AllyVect.y);
				//if distance is less then sprite size * 1,5 then return success
				if (distance < (m_xAgent->getOwner()->getSprite()->getGlobalBounds().width * 1.5f) ||
					distance < (m_xAgent->getOwner()->getSprite()->getGlobalBounds().height * 1.5f))
				{
					return EBNodeStatus::Success;
				}
			}
			return EBNodeStatus::Fail;
		}
	}
}
