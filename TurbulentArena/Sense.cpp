#include "stdafx.h"
#include "Sense.hpp"
#include "Agent.hpp"
#include "ClanMember.hpp"
#include "Map.hpp"

struct sortAgentsByDistance
{
	bjoernligan::ai::Agent* relative;
	sortAgentsByDistance(bjoernligan::ai::Agent* relative) :
		relative(relative)
	{

	}

	bool operator()(std::unique_ptr<bjoernligan::ai::SenseAgentData>& a, std::unique_ptr<bjoernligan::ai::SenseAgentData>& b)
	{
		sf::Vector2f p0 = relative->getOwner()->getSprite()->getPosition();
		sf::Vector2f p1 = a->m_agent->getOwner()->getSprite()->getPosition();
		sf::Vector2f p2 = b->m_agent->getOwner()->getSprite()->getPosition();

		float d0 = bjoernligan::Vector2f::dist(bjoernligan::Vector2f(p0), bjoernligan::Vector2f(p1));
		float d1 = bjoernligan::Vector2f::dist(bjoernligan::Vector2f(p0), bjoernligan::Vector2f(p2));
		return d0 < d1;
	}
};


namespace bjoernligan
{
	namespace ai
	{
		SenseAgentData::SenseAgentData(Agent* agent) :
			m_agent(agent)
		{
			m_lastSeenPosition = agent->getOwner()->getSprite()->getPosition();
		}

		SenseData::SenseData(Agent* me, Sense* sense, float radius) :
			m_me(me),
			m_sense(sense),
			m_radius(radius)
		{
		}

		void SenseData::update()
		{
			m_visibleFriends = getVisibleFriendsInRadius(m_radius);
			m_visibleEnemies = getVisibleEnemiesInRadius(m_radius);

			std::sort(m_visibleFriends.begin(), m_visibleFriends.end(), sortAgentsByDistance(m_me));
			std::sort(m_visibleEnemies.begin(), m_visibleEnemies.end(), sortAgentsByDistance(m_me));
		}

		void SenseData::setRadius(float radius)
		{
			m_radius = radius;
		}

		float SenseData::getRadius() const
		{
			return m_radius;
		}

		std::vector<std::unique_ptr<SenseAgentData>> SenseData::getVisibleFriendsInRadius(float radius) const
		{
			std::vector<std::unique_ptr<SenseAgentData>> visibles;

			Physics* physics = ServiceLocator<Physics>::GetService();

			for (auto& agent : m_sense->getAgents())
			{
				if (agent == m_me)
					continue;

				sf::Vector2f agentPosition = agent->getOwner()->getSprite()->getPosition();
				sf::Vector2f myPosition = m_me->getOwner()->getSprite()->getPosition();
				float distance = Vector2f::dist(Vector2f(agentPosition), Vector2f(myPosition));

				if (distance <= radius)
				{
					Physics::RaycastResult result = physics->raycast(m_me->getOwner()->getSprite()->getPosition(), agent->getOwner()->getSprite()->getPosition());
					for (auto& object : result.bodies)
					{
						if (object->GetUserData() != NULL)
						{
							B2UserData* ud = static_cast<B2UserData*>(object->GetUserData());
							if (ud->type == B2UserData::CLANMEMBER)
							{
								ClanMemberUD* clanMemberUD = static_cast<ClanMemberUD*>(ud);
								if (m_me->getOwner()->IsFriend(clanMemberUD->clanMember))
								{
									visibles.emplace_back(std::make_unique<SenseAgentData>(clanMemberUD->clanMember->getAgent()));
								}
							}
						}
					}
				}
			}
			std::sort(visibles.begin(), visibles.end(), sortAgentsByDistance(m_me));

			return visibles;
		}

		std::vector<std::unique_ptr<SenseAgentData>> SenseData::getVisibleEnemiesInRadius(float radius) const
		{
			std::vector<std::unique_ptr<SenseAgentData>> visibles;

			Physics* physics = ServiceLocator<Physics>::GetService();

			for (auto& agent : m_sense->getAgents())
			{
				if (agent == m_me)
					continue;

				sf::Vector2f agentPosition = agent->getOwner()->getSprite()->getPosition();
				sf::Vector2f myPosition = m_me->getOwner()->getSprite()->getPosition();
				float distance = Vector2f::dist(Vector2f(agentPosition), Vector2f(myPosition));

				if (distance <= radius)
				{
					Physics::RaycastResult result = physics->raycast(m_me->getOwner()->getSprite()->getPosition(), agent->getOwner()->getSprite()->getPosition());
					for (auto& object : result.bodies)
					{
						if (object->GetUserData() != nullptr)
						{
							B2UserData* ud = static_cast<B2UserData*>(object->GetUserData());
							if (ud->type == B2UserData::CLANMEMBER)
							{
								ClanMemberUD* clanMemberUD = static_cast<ClanMemberUD*>(ud);
								if (clanMemberUD->clanMember != m_me->getOwner() && !m_me->getOwner()->IsFriend(clanMemberUD->clanMember))
								{
									visibles.emplace_back(std::make_unique<SenseAgentData>(clanMemberUD->clanMember->getAgent()));
								}
							}
						}
					}
				}
			}
			std::sort(visibles.begin(), visibles.end(), sortAgentsByDistance(m_me));

			return visibles;
		}

		std::vector<SenseAgentData*> SenseData::getVisibleEnemies() const
		{
			std::vector<SenseAgentData*> agents;
			for (std::size_t i = 0; i < m_visibleEnemies.size(); ++i)
			{
				agents.push_back(m_visibleEnemies[i].get()); 
			}
			return agents;
		}

		std::vector<SenseAgentData*> SenseData::getVisibleFriends() const
		{
			std::vector<SenseAgentData*> agents;
			for (std::size_t i = 0; i < m_visibleFriends.size(); ++i)
			{
				agents.push_back(m_visibleFriends[i].get());
			}
			return agents;
		}

		Sense::Sense()
		{
		}

		void Sense::addAgent(Agent* agent)
		{
			m_agents.push_back(agent);
		}

		void Sense::removeAgent(Agent* agent)
		{
			auto itr = m_agents.begin();
			while (itr != m_agents.end())
			{
				if ((*itr) == agent)
					itr = m_agents.erase(itr);
				else
					++itr;
			}
		}

		std::vector<Agent*>& Sense::getAgents()
		{
			return m_agents;
		}
	}
}