//Agent.cpp

#include "stdafx.h"
#include "Agent.hpp"

namespace bjoernligan
{
	namespace ai
	{
		Agent::Agent()
		{
			m_xBT = nullptr;
		}

		Agent::~Agent()
		{

		}

		void Agent::Update(const float &p_fDeltaTime)
		{
			p_fDeltaTime;
		}
		void Agent::OnNotify(/*Add Parameters here*/)
		{
			//Add stuff here, remember to add subject who send notifications.
		}
		void Agent::SetBehaviorTree(BehaviorTree* p_xBT)
		{
			m_xBT = p_xBT;
		}
	}
}