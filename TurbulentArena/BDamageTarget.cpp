#include "stdafx.h"
#include "BDamageTarget.hpp"
#include "Agent.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BDamageTarget::BDamageTarget()
		{

		}

		EBNodeStatus BDamageTarget::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			if (m_xAgent->canFindTarget())
			{
				// Make agent run here
				return EBNodeStatus::Success;
			}
			else
				return EBNodeStatus::Fail;
		}

	}
}
