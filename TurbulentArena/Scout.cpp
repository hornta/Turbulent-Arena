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
		m_MaxVelocity = sf::Vector2f(10.0f, 10.0f);
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
		//m_xAgent->Seek(sf::Vector2f(800.0f, 800.0f));
		//m_xAgent->Flee(sf::Vector2f(30.0f, 20.0f));
		//m_xAgent->UpdateSteering();
		ClanMember::update(deltatime);
	}
}