//Axeman.cpp

#include "stdafx.h"
#include "Scout.hpp"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "BNodesInclude.hpp"
#include "BFindTargetNode.hpp"

namespace bjoernligan
{
	Scout::Scout(ai::Sense* sense, const sf::Color &p_xTeamColor, Clan* p_xClan)
		: ClanMember(sense, p_xTeamColor, p_xClan)
	{
	}

	void Scout::initiate()
	{
		m_xCombatStats.Initiate(1, 5);
		m_MovementStats.Initiate(sf::Vector2f(100.0f, 100.0f));

		ai::BehaviorTree* xBT = m_xAgent->getBehaviorTree();

		ai::BSelectorNode* xRootSelector = xBT->CreateRoot<ai::BSelectorNode>();
		xRootSelector;

		ai::BSequenceNode* xCombatSequence = xRootSelector->AddChild<ai::BSequenceNode>();
		xCombatSequence->AttachAgent(m_xAgent.get());
		ai::BSequenceNode* xWanderSequence = xRootSelector->AddChild<ai::BSequenceNode>();
		xWanderSequence->AttachAgent(m_xAgent.get());

		xCombatSequence->AddChild<ai::BFindTargetNode>()->AttachAgent(m_xAgent.get());
		xWanderSequence->AddChild<ai::BSetWanderTarget>()->AttachAgent(m_xAgent.get());
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