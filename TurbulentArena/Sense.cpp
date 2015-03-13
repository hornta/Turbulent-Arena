#include "stdafx.h"
#include "Sense.hpp"
#include "Agent.hpp"
#include "ClanMember.hpp"
#include "Map.hpp"

namespace bjoernligan
{
	namespace ai
	{
		float sign(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3)
		{
			return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
		}

		bool pointInTriangle(const sf::Vector2f& pt, const sf::Vector2f& v1, const sf::Vector2f& v2, const sf::Vector2f& v3)
		{
			bool b1, b2, b3;

			b1 = sign(pt, v1, v2) < 0.0f;
			b2 = sign(pt, v2, v3) < 0.0f;
			b3 = sign(pt, v3, v1) < 0.0f;

			return ((b1 == b2) && (b2 == b3));
		}

		std::vector<Agent*> SenseData::getVisibleAgents() const
		{
			return m_visibleAgents;
		}

		SenseData::SenseData(Agent* me, Sense* sense, float radius) :
			m_me(me),
			m_sense(sense),
			m_radius(radius)
		{
		}

		void SenseData::update()
		{
			m_visibleAgents.clear();

			Physics* physics = ServiceLocator<Physics>::GetService();

			// First check all agents are in our sense range
			for (auto& agent : m_sense->getAgents())
			{
				// Dont check against myself
				if (agent == m_me)
					continue;

				// Calculate distance from me and other agent
				sf::Vector2f agentPosition = agent->getOwner()->getSprite()->getPosition();
				sf::Vector2f myPosition = m_me->getOwner()->getSprite()->getPosition();
				float distance = Vector2f::dist(Vector2f(agentPosition), Vector2f(myPosition));

				// Is the agent in our sense range?
				if (distance <= m_radius)
				{
					// Perform a raycasting to check if we REALLY can see the other agent
					Physics::RaycastResult result = physics->raycast(m_me->getOwner()->getSprite()->getPosition(), agent->getOwner()->getSprite()->getPosition());
					for (auto& object : result.bodies)
					{
						if (object->GetUserData() != NULL)
						{
							B2UserData* ud = static_cast<B2UserData*>(object->GetUserData());
							if (ud->type == B2UserData::CLANMEMBER)
							{
								ClanMemberUD* clanMemberUD = static_cast<ClanMemberUD*>(ud);
								if (!m_me->getOwner()->IsFriend(clanMemberUD->clanMember))
								{
									m_visibleAgents.push_back(clanMemberUD->clanMember->getAgent());
								}
							}
						}
					}
				}
			}

			LOG(INFO) << "Enemies detected: " << m_visibleAgents.size();
		}

		void SenseData::setRadius(float radius)
		{
			m_radius = radius;
		}

		float SenseData::getRadius() const
		{
			return m_radius;
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