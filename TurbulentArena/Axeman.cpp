//Axeman.cpp

#include "stdafx.h"
#include "Axeman.hpp"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "BNodesInclude.hpp"

namespace bjoernligan
{
	Axeman::Axeman(ai::Sense* sense)
		: ClanMember(sense)
	{

	}

	void Axeman::initiate()
	{
		m_xCombatStats.Initiate(3, 4);

		ai::BehaviorTree* xBT = m_xAgent->getBehaviorTree();

		ai::BSequenceNode* xSequence = xBT->CreateRoot<ai::BSequenceNode>();
		xSequence->AttachAgent(m_xAgent);

		xSequence->AddChild<ai::BSetWanderTarget>()->AttachAgent(m_xAgent);
		xSequence->AddChild<ai::BMoveToNode>()->AttachAgent(m_xAgent);
	}

	void Axeman::update(float deltatime)
	{
		ClanMember::update(deltatime);
	}
}