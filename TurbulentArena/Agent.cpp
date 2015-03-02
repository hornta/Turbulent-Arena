//Agent.cpp

#include "stdafx.h"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "SteeringManager.hpp"
#include "ClanMember.hpp"

#define AGENT_SENSE_TIMER 0.3f
#define AGENT_DECIDE_TIMER 1.0f

namespace bjoernligan
{
	namespace ai
	{
		Agent::Agent(ClanMember* p_xOwner)
			: m_xSenseTimer(AGENT_SENSE_TIMER)
			, m_xDecideTimer(AGENT_DECIDE_TIMER)
			, m_xOwner(p_xOwner)
			, m_xBT(nullptr)
		{
			m_Steering = new SteeringManager();

			m_xBT = std::make_unique<BehaviorTree>();

			m_xSenseTimer.SetOneTimeMax(random::random(0.0f, AGENT_SENSE_TIMER));
			m_xDecideTimer.SetOneTimeMax(random::random(0.0f, AGENT_DECIDE_TIMER));
		}

		Agent::~Agent()
		{
			if (m_Steering != nullptr)
			{
				delete m_Steering;
				m_Steering = nullptr;
			}
		}

		void Agent::Update(const float &p_fDeltaTime)
		{
			m_Steering->Update();

			m_xSenseTimer.Update(p_fDeltaTime);
			m_xDecideTimer.Update(p_fDeltaTime);

			if (m_xSenseTimer.Done())
			{
				m_xSenseTimer.Reset();
				Sense();
			}
			if (m_xDecideTimer.Done())
			{
				m_xDecideTimer.Reset();
				Decide();
			}
			//ADD ACT?
			UpdateSteering();
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

		BehaviorTree* Agent::GetBehaviorTree()
		{
			return m_xBT.get();
		}

		void Agent::setSenseRadius(float p_senseRadius)
		{
			m_senseRadius = p_senseRadius;
		}

		void Agent::setSenseVisibleArea(Visibility::Light* p_senseVisibleArea)
		{
			m_senseVisibleArea = p_senseVisibleArea;
		}
		void Agent::InitializeSteering(b2Body* p_CurrentBody, const sf::Vector2f& p_MaxVelocity, const float& p_SlowDownRadius)
		{
			m_Steering = new SteeringManager();
			m_Steering->Initialize();
			m_Steering->SetCurrentBody(p_CurrentBody, p_MaxVelocity, p_SlowDownRadius);
		}
		/*void Agent::Wander()
		{
		m_Steering->Wander();
		}*/
		void Agent::Seek(const sf::Vector2f& p_TargetPos)
		{
			m_Steering->Seek(p_TargetPos);
		}
		void Agent::Flee(const sf::Vector2f& p_TargetPos)
		{
			m_Steering->Flee(p_TargetPos);
		}
		void Agent::Pursuit(b2Body* p_TargetBody)
		{
			m_Steering->Pursuit(p_TargetBody);
		}
		void Agent::Evade(b2Body* p_TargetBody)
		{
			m_Steering->Evade(p_TargetBody);
		}
		void Agent::UpdateSteering()
		{
			m_Steering->Update();
		}

		//tomas BT-methods (bad solution)
		int32_t Agent::SensedEnemyCount()
		{
			return 0;
		}

		void Agent::ChooseWanderPos()
		{
			//m_xMoveTarget = sf::Vector2f(540.0f,540.0f);
			m_xMoveTarget = sf::Vector2f(random::random(64.0f, 540.0f), random::random(64.0f, 540.0f));
		}

		void Agent::MoveToTargetPos()
		{
			//insert pathfinding here
			
			m_Steering->Seek(m_xMoveTarget); // <- temporary
		}

		bool Agent::AtMoveTarget()
		{
			return false;
		}
	}
}