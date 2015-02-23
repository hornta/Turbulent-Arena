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

		ai::BehaviorTree* xBT = new ai::BehaviorTree;

		ai::BSelectorNode* xSelector = new ai::BSelectorNode;
		xSelector->AttachAgent(m_xAgent);

		ai::BMoveToNode* xMoveTo = xSelector->AddChild<ai::BMoveToNode>();
		xMoveTo;
		m_xAgent->SetBehaviorTree(xBT);
	}

	void Scout::update(float deltatime)
	{
		ClanMember::update(deltatime);
		deltatime;
	}
}