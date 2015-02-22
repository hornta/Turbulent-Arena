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

	Axeman::~Axeman()
	{

	}

	void Axeman::Initiate()
	{
		m_xCombatStats.Initiate(1, 5);
		
		//create behaviortree
		ai::BehaviorTree* xBT = new ai::BehaviorTree;

		ai::BSelectorNode* xSelector = new ai::BSelectorNode;
		xSelector->AttachAgent(m_xAgent);

		ai::BMoveToNode* xMoveTo = xSelector->AddChild<ai::BMoveToNode>();
		xMoveTo;
		m_xAgent->SetBehaviorTree(xBT);
	}

	void Axeman::Update(const float &p_fDeltaTime)
	{
		p_fDeltaTime;
	}
}