#include "stdafx.h"
#include "Sense.hpp"
#include "Visibility.hpp"
#include "Pathfinder.hpp"
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
			std::vector<Agent*> visibleAgents;
			std::vector<Agent*> agents = m_sense->getAgents();

			std::vector<Visibility::Triangle*> triangles = m_visibilityArea->getTriangles();

			for (std::size_t k = 0; k < agents.size(); ++k)
			{
				for (std::size_t i = 0; i < triangles.size(); ++i)
				{
					sf::Vector2f agentPos = agents[k]->getOwner()->getSprite()->getPosition();

					if (pointInTriangle(agentPos, triangles[i]->getPoint0(), triangles[i]->getPoint1(), triangles[i]->getPoint2()))
					{
						visibleAgents.push_back(agents[k]);
						break;
					}
				}
			}

			return agents;
		}

		SenseData::SenseData(Agent* me, Sense* sense, float radius) :
			m_me(me),
			m_sense(sense),
			m_radius(radius)
		{
			m_visibilityArea = m_sense->getVisibility()->create(m_me->getOwner()->getSprite()->getPosition());
		}

		void SenseData::update()
		{
			m_visibleAgents = getVisibleAgents();
		}

		void SenseData::setRadius(float radius)
		{
			m_radius = radius;
		}

		float SenseData::getRadius() const
		{
			return m_radius;
		}

		bool SenseData::isVisible(Agent* agent, bool findPath, Pathfinder::Path& path, Pathfinder::Options pathfinderOptions) const
		{
			Map* map = ServiceLocator<Map>::GetService();
			for (std::size_t i = 0; i < m_visibleAgents.size(); ++i)
			{
				if (agent == m_visibleAgents[i])
				{
					if (findPath)
					{
						m_sense->getPathfinder()->setStart(Vector2i(map->getTilePosition(m_me->getOwner()->getSprite()->getPosition())));
						m_sense->getPathfinder()->setGoal(Vector2i(map->getTilePosition(agent->getOwner()->getSprite()->getPosition())));
						m_sense->getPathfinder()->findPath(path, pathfinderOptions);
					}
					return true;
				}
			}
			return false;
		}

		Sense::Sense(const sf::Vector2i& pathfinderSize)
		{
			m_pathFinder = std::make_unique<Pathfinder>(pathfinderSize);
			m_visiblity = std::make_unique<Visibility>();
		}

		void Sense::addAgent(Agent* agent)
		{
			m_agents.push_back(agent);
		}

		void Sense::update(float dt)
		{
			dt;
			m_visiblity->update();
		}

		std::vector<Agent*>& Sense::getAgents()
		{
			return m_agents;
		}

		Pathfinder* Sense::getPathfinder() const
		{
			return m_pathFinder.get();
		}

		Visibility* Sense::getVisibility() const
		{
			return m_visiblity.get();
		}
	}
}