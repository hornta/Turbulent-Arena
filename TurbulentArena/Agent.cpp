//Agent.cpp

#include "stdafx.h"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "SteeringManager.hpp"
#include "Pathfinder.hpp"
#include "ClanMember.hpp"
#include "ServiceLocator.hpp"
#include "Utility.hpp"

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
			, m_Steering(nullptr)
			//, m_Pathfinder(nullptr)
			, m_Utility(nullptr)
		{
			m_xBT = std::make_unique<BehaviorTree>();

			m_xSenseTimer.SetOneTimeMax(random::random(0.0f, AGENT_SENSE_TIMER));
			m_xDecideTimer.SetOneTimeMax(random::random(0.0f, AGENT_DECIDE_TIMER));
			

			//Pathfinding stuff
			m_Utility = ServiceLocator<Utility>::GetService();
			//m_Pathfinder = ServiceLocator<Pathfinder>::GetService();
			//m_Pathfinder->getGrid();
		
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
			m_xSenseTimer.Update(p_fDeltaTime);
			m_xDecideTimer.Update(p_fDeltaTime);

			//SENSE
			if (m_xSenseTimer.Done())
			{
				m_xSenseTimer.Reset();
				Sense();
			}
			//DECIDE
			if (m_xDecideTimer.Done())
			{
				m_xDecideTimer.Reset();
				Decide();
			}
			//ACT
			Act();
		
		}

		void Agent::Sense()
		{

		}

		void Agent::Decide()
		{
			m_xBT->Process();
		}

		void Agent::Act()
		{
			/*if (!m_xPath.nodes.empty())
			{*/
				m_Steering->Seek(m_xMoveTarget);
				////LÄGG TILL NODE TA BORT NODE mellan steering
				//}
			m_Steering->Update();
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
		void Agent::InitializeSteering(b2Body* p_CurrentBody, MovementStats* p_MovementStats)
		{
			m_Steering = new SteeringManager();
			m_Steering->Initialize();
			m_Steering->SetCurrentBody(p_CurrentBody, p_MovementStats->GetMaxVelocity(), p_MovementStats->GetSlowDownRadius());
		}
		/*void Agent::Wander()
		{
		m_Steering->Wander();
		}*/
		/*void Agent::Seek(const sf::Vector2f& p_TargetPos)
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
		}*/

		//tomas BT-methods (bad solution)
		int32_t Agent::SensedEnemyCount()
		{
			return 0;
		}

		void Agent::ChooseWanderPos()
		{
			//m_xMoveTarget = sf::Vector2f(140.0f,140.0f);
			m_xMoveTarget = sf::Vector2f(random::random(64.0f, 1040.0f), random::random(64.0f, 1040.0f));
			//m_Pathfinder->setStart()
			//m_Pathfinder->setStart(m_Utility->ConvertVector_B2toSF(m_xOwner->getBody()->m_body->GetPosition()));

		}

		void Agent::MoveToTargetPos()
		{
			//insert pathfinding here
			//m_Pathfinder->findPath(m_CurrentPath, Pathfinder::Options());

			//m_Steering->Seek(m_xMoveTarget); // <- temporary
		}

		bool Agent::AtMoveTarget()
		{
			return false;
		}
	}
}