#include "stdafx.h"
#include "BAvoidAlly.hpp"
#include "Agent.hpp"
#include "ClanMember.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BAvoidAlly::BAvoidAlly()
		{

		}

		EBNodeStatus BAvoidAlly::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;
			if (!m_xAgent->getSense()->getVisibleFriends().empty())
			{
				m_xAgent->GetSteering()->Evade(m_xAgent->getSense()->getVisibleFriends().front()->getOwner()->getBody()->m_body);
				return EBNodeStatus::Success;
			}
			return EBNodeStatus::Fail;
		}
	}
}
