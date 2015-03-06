//Agent.cpp

#include "stdafx.h"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "SteeringManager.hpp"
#include "ClanMember.hpp"
#include "Sense.hpp"

#define AGENT_SENSE_TIMER 0.3f
#define AGENT_DECIDE_TIMER 1.0f

namespace bjoernligan
{
	namespace ai
	{
		Agent::Agent(ClanMember* p_xOwner, Sense* sense)
			: m_xSenseTimer(AGENT_SENSE_TIMER)
			, m_xDecideTimer(AGENT_DECIDE_TIMER)
			, m_xOwner(p_xOwner)
<<<<<<< HEAD
		{
			m_Steering = std::make_unique<SteeringManager>();
=======
			, m_xBT(nullptr)
			, m_Steering(nullptr)
		{
>>>>>>> 557a866c28f9f0a331c36ebdc444a45779b7fb72
			m_xBT = std::make_unique<BehaviorTree>();
			m_senseData = std::make_unique<SenseData>(this, sense, 32.f);
			m_xSenseTimer.SetOneTimeMax(random::random(0.0f, AGENT_SENSE_TIMER));
			m_xDecideTimer.SetOneTimeMax(random::random(0.0f, AGENT_DECIDE_TIMER));
		}

		Agent::~Agent()
		{
		}

		void Agent::update(const float &p_fDeltaTime)
		{
			m_Steering->Update();

			m_xSenseTimer.Update(p_fDeltaTime);
			m_xDecideTimer.Update(p_fDeltaTime);

			if (m_xSenseTimer.Done())
			{
				m_xSenseTimer.Reset();
				sense();
			}
			if (m_xDecideTimer.Done())
			{
				m_xDecideTimer.Reset();
				decide();
			}

			//ADD ACT?
			Seek(m_xMoveTarget);
			UpdateSteering();
		}

		void Agent::sense()
		{
			m_senseData->update();
		}

		void Agent::decide()
		{
			m_xBT->Process();
		}

		SenseData* Agent::getSense() const
		{
			return m_senseData.get();
		}

		ClanMember* Agent::getOwner() const
		{
			return m_xOwner;
		}

		void Agent::onNotify(/*add parameters*/)
		{

		}

		BehaviorTree* Agent::getBehaviorTree()
		{
			return m_xBT.get();
		}

		void Agent::InitializeSteering(b2Body* p_CurrentBody, MovementStats* p_MovementStats)
		{
			m_Steering->Initialize();
			m_Steering->SetCurrentBody(p_CurrentBody, p_MovementStats->GetMaxVelocity(), p_MovementStats->GetSlowDownRadius());
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
			//m_xMoveTarget = sf::Vector2f(140.0f,140.0f);
			m_xMoveTarget = sf::Vector2f(random::random(64.0f, 1040.0f), random::random(64.0f, 1040.0f));
		}

		void Agent::MoveToTargetPos()
		{
			m_Steering->Seek(m_xMoveTarget);
		}

		bool Agent::AtMoveTarget()
		{
			return false;
		}

		bool Agent::canFindTarget()
		{
			if (m_senseData->getVisibleAgents().size() > 0)
			{
				return true;
			}
			else
				return false;
		}
	}
}