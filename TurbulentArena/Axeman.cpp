//Axeman.cpp

#include "stdafx.h"
#include "Axeman.hpp"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "BNodesInclude.hpp"
#include "BFindTargetNode.hpp"

namespace bjoernligan
{
	Axeman::Axeman(ai::Sense* sense, const sf::Color &p_xTeamColor, Clan* p_xClan)
		: ClanMember(sense, p_xTeamColor, p_xClan)
	{

	}

	void Axeman::initiate()
	{
		m_xCombatStats.Initiate(3, 4);
		m_MovementStats.Initiate(sf::Vector2f(100, 100.0f), sf::Vector2f(300.0f, 300.0f));

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

	void Axeman::update(float deltatime)
	{
		ClanMember::update(deltatime);
	}
}