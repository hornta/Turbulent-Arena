//Axeman.cpp

#include "stdafx.h"
#include "Scout.hpp"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "BNodesInclude.hpp"

namespace bjoernligan
{
	Scout::Scout()
	{

	}

	void Scout::initiate()
	{
		m_xCombatStats.Initiate(1, 5);

		ai::BehaviorTree* xBT = m_xAgent->GetBehaviorTree();

		ai::BSequenceNode* xSequence = xBT->CreateRoot<ai::BSequenceNode>();
		xSequence->AttachAgent(m_xAgent);

		xSequence->AddChild<ai::BSetWanderTarget>()->AttachAgent(m_xAgent);
		xSequence->AddChild<ai::BMoveToNode>()->AttachAgent(m_xAgent);
	}

	void Scout::update(float deltatime)
	{
		ClanMember::update(deltatime);
	}
}