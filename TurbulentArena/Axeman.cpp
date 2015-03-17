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
		m_eClass = EClass::EClassAxeman;

		m_xCombatStats.Initiate(1, 10);
		m_MovementStats.Initiate(0.0f);

		ai::BehaviorTree* xBT = m_xAgent->getBehaviorTree();
		xBT->AttachAgent(m_xAgent.get());

		ai::BSelectorNode* xRootSelector = xBT->CreateRoot<ai::BSelectorNode>();

		// level 2
		ai::BSequenceNode* combatSequence = xRootSelector->AddChild<ai::BSequenceNode>();
		ai::BSequenceNode* friendHelpSequence = xRootSelector->AddChild<ai::BSequenceNode>();
		ai::BSelectorNode* wanderSelector = xRootSelector->AddChild<ai::BSelectorNode>();

		// level 3
		combatSequence->AddChild<ai::CanSeeEnemies>();
		ai::BSelectorNode* FightOrFlightSel = combatSequence->AddChild<ai::BSelectorNode>();
		friendHelpSequence->AddChild<ai::BIsScared>();
		friendHelpSequence->AddChild<ai::HelpFriend>();
		wanderSelector->AddChild<ai::ProcessIncomingReports>();
		wanderSelector->AddChild<ai::BSetWanderTarget>();

		// level 4
		ai::BSequenceNode* FleeSeq = FightOrFlightSel->AddChild<ai::BSequenceNode>();
		ai::BSelectorNode* FightSel = FightOrFlightSel->AddChild<ai::BSelectorNode>();
		
		// level 5
		FleeSeq->AddChild<ai::BIsScared>();
		FleeSeq->AddChild<ai::BFleeFromEnemies>();
		ai::BSequenceNode* FightSeq = FightSel->AddChild<ai::BSequenceNode>();
		FightSel->AddChild<ai::GetPathToEnemy>();

		// level 6
		FightSeq->AddChild<ai::EnemyWithinRadius>();
		FightSeq->AddChild<ai::AttackEnemy>();
	}

	void Axeman::update(float deltatime)
	{
		ClanMember::update(deltatime);

		auto it = m_incomingReports.begin();
		while (it != m_incomingReports.end())
		{
			sf::Vector2f p0 = it->get()->position;
			sf::Vector2f p1 = m_sprite->getPosition();

			if (Vector2f::dist(Vector2f(p0), Vector2f(p1)) <= 64.f)
			{
				it = m_incomingReports.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	bool Axeman::processReports()
	{
		if (m_incomingReports.empty())
		{
			return false;
		}

		m_xAgent->getOwner()->GetMovementStats().SetMaxVelocity(220.f);
		m_xAgent->setWanderPos(m_incomingReports[0]->position);

		return true;
	}
}