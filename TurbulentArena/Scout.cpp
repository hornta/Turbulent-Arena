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

	}

	void Scout::initiate()
	{
		m_xCombatStats.Initiate(1, 5);

		ai::BehaviorTree* xBT = new ai::BehaviorTree;

		ai::BSelectorNode* xSelector = new ai::BSelectorNode;
		xSelector->AttachAgent(m_xAgent);

		ai::BMoveToNode* xMoveTo = xSelector->AddChild<ai::BMoveToNode>();
		xMoveTo;
		m_xAgent->SetBehaviorTree(xBT);
		//Init Steering, with body, max velocity and slowdown radius
		m_xAgent->InitializeSteering(m_xPhysicsBody->m_body, b2Vec2(10,10), 10);
		//Sets the current velocity
		m_xPhysicsBody->m_body->SetLinearVelocity(b2Vec2(5, 0));
		
	}

	void Scout::update(float deltatime)
	{
		m_xAgent->Seek(b2Vec2(30.0f, 20.0f));
		//m_xAgent->Flee(b2Vec2(30.0f, 20.0f));
		m_xAgent->UpdateSteering();
		ClanMember::update(deltatime);
		deltatime;
	}
}