//Axeman.cpp

#include "stdafx.h"
#include "Axeman.hpp"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "BNodesInclude.hpp"

namespace bjoernligan
{
	Axeman::Axeman(ai::Sense* sense, const sf::Color &p_xTeamColor, Clan* p_xClan)
		: ClanMember(sense, p_xTeamColor, p_xClan)
	{

	}

	void Axeman::initiate()
	{
		m_xCombatStats.Initiate(3, 4);
		m_MovementStats.Initiate(sf::Vector2f(100, 100.0f));

		ai::BehaviorTree* xBT = m_xAgent->getBehaviorTree();

		ai::BSelectorNode* xRootSelector = xBT->CreateRoot<ai::BSelectorNode>();
		xRootSelector;

		// level 2
		ai::BSequenceNode* seq0 = xRootSelector->AddChild<ai::BSequenceNode>();
		seq0->AttachAgent(m_xAgent.get());
		xRootSelector->AddChild<ai::BSetWanderTarget>()->AttachAgent(m_xAgent.get());

		// level 3
		ai::BSelectorNode* sel0 = seq0->AddChild<ai::BSelectorNode>();
		seq0->AttachAgent(m_xAgent.get());
		ai::BSelectorNode* sel1 = seq0->AddChild<ai::BSelectorNode>();
		sel1->AttachAgent(m_xAgent.get());

		// level 4
		sel0->AddChild<ai::CanSeeEnemies>()->AttachAgent(m_xAgent.get());
		ai::BSequenceNode* seq1 = sel1->AddChild<ai::BSequenceNode>();
		seq1->AttachAgent(m_xAgent.get());
		sel1->AddChild<ai::GetPathToEnemy>()->AttachAgent(m_xAgent.get());

		// level 5
		seq1->AddChild<ai::EnemyWithinRadius>()->AttachAgent(m_xAgent.get());
		seq1->AddChild<ai::AttackEnemy>()->AttachAgent(m_xAgent.get());
	}

	void Axeman::update(float deltatime)
	{
		ClanMember::update(deltatime);
	}
}