//Agent.cpp

#include "stdafx.h"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "SteeringManager.hpp"

namespace bjoernligan
{
	namespace ai
	{
		Agent::Agent()
		{
			m_xBT = nullptr;
			m_Steering = new SteeringManager();
		}

		Agent::~Agent()
		{
			delete m_Steering;
		}

		void Agent::Sense()
		{

		}

		void Agent::Decide()
		{
			m_xBT->Process();
		}

		void Agent::OnNotify(/*add parameters*/)
		{

		}

		void Agent::SetBehaviorTree(BehaviorTree* p_xBT)
		{
			m_xBT = p_xBT;
		}

		void Agent::setSenseRadius(float p_senseRadius)
		{
			m_senseRadius = p_senseRadius;
		}

		void Agent::setSenseVisibleArea(Visibility::Light* p_senseVisibleArea)
		{
			m_senseVisibleArea = p_senseVisibleArea;
		}
		void Agent::InitializeSteering(b2Body* p_CurrentBody)
		{
			m_Steering->Initialize();
			m_Steering->SetCurrentBody(p_CurrentBody);
		}
		void Agent::Wander()
		{
			m_Steering->Wander();
		}
		void Agent::Seek(sf::Vector2f p_TargetPos)
		{
			m_Steering->Seek(p_TargetPos);
		}
		void Agent::Flee(b2Body* p_TargetBody)
		{
			m_Steering->Flee(p_TargetBody);
		}
		void Agent::Follow(b2Body* p_TargetBody)
		{
			m_Steering->Follow(p_TargetBody);
		}
		void Agent::UpdateSteering()
		{
			m_Steering->Update();
		}
	}
}