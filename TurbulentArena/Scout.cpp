//Axeman.cpp

#include "stdafx.h"
#include "Scout.hpp"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "BNodesInclude.hpp"
#include "BFindTargetNode.hpp"
#include "Clan.hpp"

namespace bjoernligan
{
	Scout::EnlightendFriend::EnlightendFriend(bool enlightend, ai::SenseAgentData* agentData) :
		enlightend(enlightend),
		agentData(agentData)
	{

	}

	Scout::Scout(ai::Sense* sense, const sf::Color &p_xTeamColor, Clan* p_xClan)
		: ClanMember(sense, p_xTeamColor, p_xClan)
	{
	}

	void Scout::initiate()
	{
		m_xCombatStats.Initiate(1, 5);
		m_MovementStats.Initiate(sf::Vector2f(550, 550.0f));

		ai::BehaviorTree* xBT = m_xAgent->getBehaviorTree();

		ai::BSelectorNode* xRootSelector = xBT->CreateRoot<ai::BSelectorNode>();
		xRootSelector;

		// level 2
		ai::BSequenceNode* reportSequence = xRootSelector->AddChild<ai::BSequenceNode>();
		xRootSelector->AddChild<ai::DetectEnemies>()->AttachAgent(m_xAgent.get());
		xRootSelector->AddChild<ai::Explore>()->AttachAgent(m_xAgent.get());

		// level 3
		reportSequence->AddChild<ai::HasSomethingToReport>()->AttachAgent(m_xAgent.get());
		reportSequence->AddChild<ai::ReportToFriends>()->AttachAgent(m_xAgent.get());
	}

	void Scout::update(float deltatime)
	{
		ClanMember::update(deltatime);
	}
}