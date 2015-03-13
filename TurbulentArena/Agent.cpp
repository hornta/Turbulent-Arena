//Agent.cpp

#include "stdafx.h"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "ClanMember.hpp"
#include "ServiceLocator.hpp"
#include "Utility.hpp"
#include "Sense.hpp"
#include "Map.hpp"

#define AGENT_SENSE_TIMER 0.1f
#define AGENT_DECIDE_TIMER 0.1f

namespace bjoernligan
{
	namespace ai
	{
		Agent::Agent(ClanMember* p_xOwner, ai::Sense* sense)
			: m_xSenseTimer(AGENT_SENSE_TIMER)
			, m_xDecideTimer(AGENT_DECIDE_TIMER)
			, m_xOwner(p_xOwner)
			, m_xBT(nullptr)
			, m_Steering(nullptr)
			//, m_Pathfinder(nullptr)
			, m_Utility(nullptr)
		{
			sense->addAgent(this);
			m_Steering = std::make_unique<SteeringManager>();
			m_xBT = std::make_unique<BehaviorTree>();
			m_senseData = std::make_unique<SenseData>(this, sense, 300.f);
			/*m_xSenseTimer.SetOneTimeMax(random::random(0.0f, AGENT_SENSE_TIMER));
			m_xDecideTimer.SetOneTimeMax(random::random(0.0f, AGENT_DECIDE_TIMER));*/
			m_map = ServiceLocator<Map>::GetService();

			//Pathfinding stuff
			m_Utility = ServiceLocator<Utility>::GetService();
			//m_Pathfinder = ServiceLocator<Pathfinder>::GetService();
			//m_Pathfinder->getGrid();

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

			m_senseData->update();
		}

		void Agent::Decide()
		{
			m_xBT->Process();
		}

		void Agent::Act()
		{
			MoveToTargetPos(true);
			//Add other stuff here?, 
			//evade target
			//attack target/do damage?
			m_Steering->Update();
		}

		void Agent::OnNotify(/*add parameters*/)
		{

		}

		SenseData* Agent::getSense() const
		{
			return m_senseData.get();
		}

		ClanMember* Agent::getOwner() const
		{
			return m_xOwner;
		}

		BehaviorTree* Agent::getBehaviorTree()
		{
			return m_xBT.get();
		}

		void Agent::InitializeSteering(b2Body* p_CurrentBody, MovementStats* p_MovementStats)
		{
			m_Steering->Initialize();
			m_Steering->SetCurrentBody(p_CurrentBody, p_MovementStats->GetMaxWalkVelocity(), p_MovementStats->GetMaxRunVelocity());
		}

		int32_t Agent::SensedEnemyCount()
		{
			return 0;
		}

		void Agent::ChooseWanderPos()
		{
			if (m_CurrentPath.isDone())
			{
				sf::Vector2i xTargetPos;
				Map* xMap = ServiceLocator<Map>::GetService();
				Pathfinder* xPathFinder = ServiceLocator<Pathfinder>::GetService();

				m_xCurrentMapPos = xMap->getTilePosition(m_xOwner->getSprite()->getPosition());

				xTargetPos = sf::Vector2i(random::random(0, xMap->getSize().x), random::random(0, xMap->getSize().y));
				if (xMap->GetRandomTopmostWalkableTile(m_xCurrentMapPos, xTargetPos, sf::Vector2i(20, 20)))
				{
					xPathFinder->setStart(m_xCurrentMapPos);
					xPathFinder->setGoal(xTargetPos);
					xPathFinder->findPath(m_CurrentPath);
				}
			}
		}

		void Agent::MoveToTargetPos(bool p_Run)
		{
			if (!m_CurrentPath.isDone())
			{
				// Get the position to the current node in our path
				Vector2f target;
				target.x = m_CurrentPath.getCurrentNode()->x * m_map->getTileSize().x + m_map->getTileSize().x / 2;
				target.y = m_CurrentPath.getCurrentNode()->y * m_map->getTileSize().y + m_map->getTileSize().y / 2;
				Vector2f currentPosition = Vector2f(m_xOwner->getSprite()->getPosition());
				if (!m_CurrentPath.getNextNode())
					m_Steering->Arrival(sf::Vector2f(target.x, target.y), p_Run, 2.0f);
				else
					m_Steering->Seek(sf::Vector2f(target.x, target.y), p_Run);
	
				if (target.dist(currentPosition) < 24)
					++m_CurrentPath.currentNode;
			}
			else
				//hard stop
				//m_xOwner->getBody()->m_body->SetLinearVelocity(b2Vec2(0, 0));
				//soft stop
				m_Steering->Seek(sf::Vector2f(m_xOwner->getSprite()->getPosition()),p_Run);
		}
		bool Agent::AtMoveTarget()
		{
			return false;
		}

		bool Agent::canFindTarget()
		{
			if (m_senseData->getVisibleEnemies().size() > 0)
			{
				return true;
			}
			else
				return false;
		}

		bool Agent::getPathToVisibleTarget(Agent* agent)
		{
			Map* map = ServiceLocator<Map>::GetService();
			Pathfinder* pathfinder = ServiceLocator<Pathfinder>::GetService();
			pathfinder->setStart(map->getTilePosition(m_xOwner->getSprite()->getPosition()));
			pathfinder->setGoal(map->getTilePosition(agent->getOwner()->getSprite()->getPosition()));

			Pathfinder::Options options;
			options.algorithm = PathfinderInfo::ALGORITHM_ASTAR;
			options.diagonal = PathfinderInfo::DIAGONAL_NO_OBSTACLES;
			options.heuristic = PathfinderInfo::HEURISTIC_DIAGONAL;

			PathfinderInfo::PathResult result = pathfinder->findPath(m_CurrentPath, options);
			if (result == PathfinderInfo::PATHRESULT_SUCCEEDED)
				return true;
			return false;
		}

		bool Agent::getPathToRandomVisibleTarget()
		{
			std::vector<Agent*> visibleAgents = m_senseData->getVisibleEnemies();
			std::size_t randomAgentIndex = random::random(0, visibleAgents.size() - 1);
			return getPathToVisibleTarget(visibleAgents[randomAgentIndex]);
		}

		bool Agent::IsEnemyWithinAttackRange()
		{
			// TWEAK ATTACK RANGE SO IT BECOMES GOOD!!!!!!!!!!!!!!!!!!!!!
			std::vector<Agent*> enemies = m_senseData->getVisibleEnemies();
			for (Agent* agent : enemies)
			{
				sf::Vector2f p0 = m_xOwner->getSprite()->getPosition();
				sf::Vector2f p1 = agent->getOwner()->getSprite()->getPosition();

				float distance = Vector2f::dist(Vector2f(p0), Vector2f(p1));
				if (distance <= 64.f)
				{
					m_CurrentPath.setDone();
					return true;
				}
			}

			return false;
		}

		bool Agent::CanAttack() const
		{
			// Make the attack timer range a little bit random
			sf::Clock* attackTimer = m_xOwner->GetCombat()->getAttackTimer();
			if (attackTimer->getElapsedTime().asSeconds() >= m_xOwner->GetCombat()->GetAttackCooldown())
			{
				m_xOwner->GetCombat()->SetAttackCooldown(random::random(1.f - 0.05f, 1.f + 0.05f));
				attackTimer->restart();
				return true;
			}
			return false;
		}
	}
}