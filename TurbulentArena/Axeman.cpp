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
		m_eClass = EClass::Axeman;

		m_xCombatStats.Initiate(1, 10);
		m_MovementStats.Initiate(sf::Vector2f(300, 300.0f));

		ai::BehaviorTree* xBT = m_xAgent->getBehaviorTree();

		ai::BSelectorNode* xRootSelector = xBT->CreateRoot<ai::BSelectorNode>();
		xRootSelector;

		// level 2
		ai::BSequenceNode* combatSequence = xRootSelector->AddChild<ai::BSequenceNode>();
		ai::BSequenceNode* friendHelpSequence = xRootSelector->AddChild<ai::BSequenceNode>();
		xRootSelector->AddChild<ai::BSetWanderTarget>()->AttachAgent(m_xAgent.get());

		// level 3
		ai::BSelectorNode* sel0 = combatSequence->AddChild<ai::BSelectorNode>();
		ai::BSelectorNode* FightOrFlightSel = combatSequence->AddChild<ai::BSelectorNode>();
		friendHelpSequence->AddChild<ai::BIsScared>()->AttachAgent(m_xAgent.get());
		friendHelpSequence->AddChild<ai::HelpFriend>()->AttachAgent(m_xAgent.get());

		// level 4
		sel0->AddChild<ai::CanSeeEnemies>()->AttachAgent(m_xAgent.get());
		ai::BSequenceNode* FleeSeq = FightOrFlightSel->AddChild<ai::BSequenceNode>();
		ai::BSelectorNode* FightSel = FightOrFlightSel->AddChild<ai::BSelectorNode>();
		
		// level 5
		FleeSeq->AddChild<ai::BIsScared>()->AttachAgent(m_xAgent.get());
		FleeSeq->AddChild<ai::BFleeFromEnemies>()->AttachAgent(m_xAgent.get());
		ai::BSequenceNode* FightSeq = FightSel->AddChild<ai::BSequenceNode>();
		FightSel->AddChild<ai::GetPathToEnemy>()->AttachAgent(m_xAgent.get());

		// level 6
		FightSeq->AddChild<ai::EnemyWithinRadius>()->AttachAgent(m_xAgent.get());
		FightSeq->AddChild<ai::AttackEnemy>()->AttachAgent(m_xAgent.get());
	}

	void Axeman::update(float deltatime)
	{
		ClanMember::update(deltatime);
	}
}