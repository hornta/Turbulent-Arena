//Axeman.cpp

#include "stdafx.h"
#include "Axeman.hpp"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "BNodesInclude.hpp"

namespace bjoernligan
{
	Axeman::Axeman()
	{

	}

	void Axeman::initiate()
	{
		m_xCombatStats.Initiate(3, 4);

		ai::BehaviorTree* xBT = new ai::BehaviorTree;

		ai::BSelectorNode* xSelector = new ai::BSelectorNode;
		xSelector->AttachAgent(m_xAgent);

		ai::BMoveToNode* xMoveTo = xSelector->AddChild<ai::BMoveToNode>();
		xMoveTo;
		m_xAgent->SetBehaviorTree(xBT);
	}

	void Axeman::update(float deltatime)
	{
		deltatime;
	}
}