//Agent.cpp

#include "stdafx.h"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "ClanMember.hpp"
#include "ServiceLocator.hpp"
#include "Utility.hpp"
#include "Sense.hpp"
#include "Map.hpp"

#define AGENT_SENSE_DECIDE_TIMER 0.01f
#define AGENT_SENSE_RADIUS_DEFAULT 300.f
#define AGENT_SOCIAL_WANDER_TARGET 20.f
#define AGENT_STUCK_COOLDOWN 3.f

namespace bjoernligan
{
	namespace ai
	{
		Agent::Agent(ClanMember* p_xOwner, ai::Sense* sense)
			: m_xOwner(p_xOwner)
			, m_xBT(nullptr)
			, m_Steering(nullptr)
		{
			sense->addAgent(this);
			m_Steering = std::make_unique<SteeringManager>();
			m_xBT = std::make_unique<BehaviorTree>();
			m_senseData = std::make_unique<SenseData>(this, sense, 300.f);
			m_map = ServiceLocator<Map>::GetService();
		}

		void Agent::Update(const float &p_fDeltaTime)
		{
			p_fDeltaTime;
			//if (m_senseDecideTimer.getElapsedTime().asSeconds() >= AGENT_SENSE_DECIDE_TIMER)
			{
				m_senseDecideTimer.restart();
				Sense();
				Decide();
			}

			Act();
		}

		void Agent::Sense()
		{
			m_xCurrentMapPos = m_map->getTilePosition(getOwner()->getSprite()->getPosition());
			m_senseData->update();
		}

		void Agent::Decide()
		{
			m_xBT->Process();
		}

		void Agent::Act()
		{
			//Test code,
			//m_xOwner->GetMovementStats().SetMaxVelocity(sf::Vector2f(50.f, 50.f));
			MoveToTargetPos();
			m_Steering->Update();
		}

		void Agent::OnNotify(/*add parameters*/)
		{

		}
		SteeringManager* Agent::GetSteering() const
		{
			return m_Steering.get();
		}
		Pathfinder::Path * Agent::GetPath()
		{
			return &m_CurrentPath;
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

		void Agent::InitializeSteering(b2Body* p_CurrentBody, MovementStats &p_MovementStats)
		{
			m_Steering->Initialize();
			m_Steering->SetCurrentBody(p_CurrentBody, p_MovementStats);
		}

		int32_t Agent::SensedEnemyCount()
		{
			return 0;
		}

		void Agent::ChooseWanderPos()
		{
			if (m_CurrentPath.isDone())
			{
				m_stuckTimer.restart();
				sf::Vector2i originPos;
				sf::Vector2i target;
				Pathfinder* xPathFinder = ServiceLocator<Pathfinder>::GetService();

				sf::Vector2f searchArea;
				// Get nearest friend
				std::vector<SenseAgentData*> friends = m_senseData->getVisibleFriends();
				if (!friends.empty())
				{
					originPos = m_map->getTilePosition(friends[0]->m_agent->getOwner()->getSprite()->getPosition());
					searchArea.x = clamp((1.f - getOwner()->GetCombat()->getSocial()) * AGENT_SOCIAL_WANDER_TARGET, 3.f, AGENT_SOCIAL_WANDER_TARGET);
					searchArea.y = clamp((1.f - getOwner()->GetCombat()->getSocial()) * AGENT_SOCIAL_WANDER_TARGET, 3.f, AGENT_SOCIAL_WANDER_TARGET);
				}
				else
				{
					originPos = m_map->getTilePosition(m_xOwner->getSprite()->getPosition());
					searchArea.x = AGENT_SOCIAL_WANDER_TARGET;
					searchArea.y = AGENT_SOCIAL_WANDER_TARGET;
				}

				//xTargetPos = sf::Vector2i(random::random(0, xMap->getSize().x), random::random(0, xMap->getSize().y));
				if (m_map->GetRandomTopmostWalkableTile(originPos, target, sf::Vector2i(searchArea)))
				{
					Pathfinder::Options options;
					options.algorithm = PathfinderInfo::ALGORITHM_ASTAR;
					options.diagonal = PathfinderInfo::DIAGONAL_NO_OBSTACLES;
					options.heuristic = PathfinderInfo::HEURISTIC_DIAGONAL;
					xPathFinder->setStart(m_xCurrentMapPos);
					xPathFinder->setGoal(target);
					xPathFinder->findPath(m_CurrentPath, options);
				}
			}
		}

		void Agent::MoveToTargetPos()
		{
			if (!m_CurrentPath.isDone())
			{
				// Get the position to the current node in our path
				Vector2f target;
				target.x = m_CurrentPath.getCurrentNode()->x * m_map->getTileSize().x + m_map->getTileSize().x / 2;
				target.y = m_CurrentPath.getCurrentNode()->y * m_map->getTileSize().y + m_map->getTileSize().y / 2;
				Vector2f currentPosition = Vector2f(m_xOwner->getSprite()->getPosition());
				m_Steering->Seek(sf::Vector2f(target.x, target.y));

				if (target.dist(currentPosition) < 24)
				{
					++m_CurrentPath.currentNode;
					m_stuckTimer.restart();
				}
			}
			else
				//hard stop
				//m_xOwner->getBody()->m_body->SetLinearVelocity(b2Vec2(0, 0));
				//soft stop
				m_Steering->Arrival(sf::Vector2f(m_xOwner->getSprite()->getPosition()), 8.0f);

			// Check if they are stuck
			if (!m_CurrentPath.isDone())
			{
				// Has the agent been stuck for more than AGENT_STUCK_COOLDOWN seconds
				if (m_stuckTimer.getElapsedTime().asSeconds() >= AGENT_STUCK_COOLDOWN)
				{
					// Find new path for the poor agent
					m_CurrentPath.setDone();
					ChooseWanderPos();
				}
			}
		}
	/*	void Agent::FleeFromVisibleEnemies()
		{
			if (!m_senseData->getVisibleEnemies().empty())
			{
				for (unsigned int i = 0; i < m_senseData->getVisibleEnemies().size(); i++)
				{
					m_Steering->Flee(m_senseData->getVisibleEnemies()[i]->getOwner()->getSprite()->getPosition());
				}
			}
		}*/
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

		bool Agent::getPathToVisibleTarget(SenseAgentData* senseAgentData)
		{
			Map* map = ServiceLocator<Map>::GetService();
			Pathfinder* pathfinder = ServiceLocator<Pathfinder>::GetService();
			
			sf::Vector2i goalPosition;
			if (!m_CurrentPath.isDone())
			{
				Pathfinder::PathNode* node = &m_CurrentPath.nodes.back();
				goalPosition.x = node->x;
				goalPosition.y = node->y;
			}

			sf::Vector2i newGoalPosition = map->getTilePosition(senseAgentData->m_lastSeenPosition);

			if (goalPosition != newGoalPosition)
			{
				pathfinder->setStart(map->getTilePosition(m_xOwner->getSprite()->getPosition()));
				pathfinder->setGoal(newGoalPosition);

				Pathfinder::Options options;
				options.algorithm = PathfinderInfo::ALGORITHM_ASTAR;
				options.diagonal = PathfinderInfo::DIAGONAL_NO_OBSTACLES;
				options.heuristic = PathfinderInfo::HEURISTIC_DIAGONAL;

				PathfinderInfo::PathResult result = pathfinder->findPath(m_CurrentPath, options);
				if (result == PathfinderInfo::PATHRESULT_SUCCEEDED)
					return true;
				return false;
			}
			return false;
		}

		bool Agent::getPathToRandomVisibleTarget()
		{
			std::vector<SenseAgentData*> visibleAgents = m_senseData->getVisibleEnemies();
			std::size_t randomAgentIndex = random::random(0, visibleAgents.size() - 1);
			return getPathToVisibleTarget(visibleAgents[randomAgentIndex]);
		}

		bool Agent::IsEnemyWithinAttackRange()
		{
			std::vector<SenseAgentData*> enemies = m_senseData->getVisibleEnemies();
			for (SenseAgentData* agentData : enemies)
			{
				sf::Vector2f p0 = m_xOwner->getSprite()->getPosition();
				sf::Vector2f p1 = agentData->m_agent->getOwner()->getSprite()->getPosition();

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