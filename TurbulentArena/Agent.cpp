//Agent.cpp

#include "stdafx.h"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "ClanMember.hpp"
#include "ServiceLocator.hpp"
#include "Utility.hpp"
#include "Sense.hpp"
#include "Map.hpp"

#define AGENT_SENSE_TIMER 0.3f
#define AGENT_DECIDE_TIMER 1.0f

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
			m_Steering = std::make_unique<SteeringManager>();
			m_xBT = std::make_unique<BehaviorTree>();
			m_senseData = std::make_unique<SenseData>(this, sense, 32.f);
			m_xSenseTimer.SetOneTimeMax(random::random(0.0f, AGENT_SENSE_TIMER));
			m_xDecideTimer.SetOneTimeMax(random::random(0.0f, AGENT_DECIDE_TIMER));
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
			// Trace current path
			if (!m_CurrentPath.isDone())
			{
				// Get the position to the current node in our path
				Vector2f target;
				target.x = m_CurrentPath.getCurrentNode()->x * m_map->getTileSize().x + m_map->getTileSize().x / 2;
				target.y = m_CurrentPath.getCurrentNode()->y * m_map->getTileSize().y + m_map->getTileSize().y / 2;

				Vector2f currentPosition = Vector2f(m_xOwner->getSprite()->getPosition());
				float fTargetDist = target.dist(currentPosition);

				if (fTargetDist < 32)
				{
					++m_CurrentPath.currentNode;
				}
				//if (fTargetDist < 64 && m_CurrentPath.getNextNode())
				//{
				//	Vector2f nexttarget;
				//	nexttarget.x = m_CurrentPath.getNextNode()->x * m_map->getTileSize().x + m_map->getTileSize().x / 2;
				//	nexttarget.y = m_CurrentPath.getNextNode()->y * m_map->getTileSize().y + m_map->getTileSize().y / 2;
				//	//m_Steering->Seek(sf::Vector2f(nexttarget.x, nexttarget.y));
				//}
				if (!m_CurrentPath.getNextNode())
				{
					m_Steering->Arrival(sf::Vector2f(target.x, target.y), 8.0f);
				}
				else
				{
					m_Steering->Seek(sf::Vector2f(target.x, target.y));
				}
			}
			else
				m_Steering->Seek(sf::Vector2f(m_xOwner->getSprite()->getPosition()));

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
			m_Steering->SetCurrentBody(p_CurrentBody, p_MovementStats->GetMaxVelocity());
		}

		//tomas BT-methods (bad solution)
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

		bool Agent::canFindTarget()
		{
			if (m_senseData->getVisibleAgents().size() > 0)
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
			std::vector<Agent*> visibleAgents = m_senseData->getVisibleAgents();
			std::size_t randomAgentIndex = random::random(0, visibleAgents.size() - 1);
			return getPathToVisibleTarget(visibleAgents[randomAgentIndex]);
		}
	}
}