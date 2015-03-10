//Axeman.cpp

#include "stdafx.h"
#include "Scout.hpp"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "BNodesInclude.hpp"
#include "BFindTargetNode.hpp"

namespace bjoernligan
{
	Scout::Scout(ai::Sense* sense)
		: ClanMember(sense)
	{
	}

	void Scout::initiate()
	{
		m_xCombatStats.Initiate(1, 5);

		ai::BehaviorTree* xBT = m_xAgent->getBehaviorTree();
		m_MovementStats.Initiate(sf::Vector2f(300.0f, 300.0f), 0.1f, 2.0f);

		ai::BSequenceNode* xSequence = xBT->CreateRoot<ai::BSequenceNode>();
		xSequence->AttachAgent(m_xAgent);

		//xSequence->AddChild<ai::BFindTargetNode>()->AttachAgent(m_xAgent);
		xSequence->AddChild<ai::BSetWanderTarget>()->AttachAgent(m_xAgent);
	}

	void Scout::update(float deltatime)
	{
		//m_xAgent->Seek(sf::Vector2f(800.0f, 800.0f));
		//m_xAgent->Seek(sf::Vector2f(500.0f, 100.0f));
		//m_xAgent->Flee(sf::Vector2f(30.0f, 20.0f));
		//m_xAgent->UpdateSteering();
		ClanMember::update(deltatime);
	}
}