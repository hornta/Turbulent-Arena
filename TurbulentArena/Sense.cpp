#include "stdafx.h"
#include "Sense.hpp"
#include "Visibility.hpp"
#include "Pathfinder.hpp"
#include "Agent.hpp"
#include "ClanMember.hpp"

namespace bjoernligan
{
	namespace ai
	{

		SenseData::SenseData(Agent* me, Sense* sense, float radius) :
			m_me(me),
			m_sense(sense),
			m_radius(radius)
		{
			m_visibilityArea = m_sense->getVisibility()->create(m_me->getOwner()->getSprite()->getPosition());
		}

		void SenseData::update()
		{
		}

		void SenseData::setRadius(float radius)
		{
			m_radius = radius;
		}

		float SenseData::getRadius() const
		{
			return m_radius;
		}

		std::vector<Agent*> SenseData::getVisibleAgents() const
		{
			std::vector<Agent*> agents;
			std::vector<Visibility::Triangle*> triangles = m_visibilityArea->getTriangles();

			for (std::size_t i = 0; i < triangles.size(); ++i)
			{

			}

			return agents;
		}

		bool SenseData::isVisible(Agent* agent, bool findPath) const
		{
			agent;
			findPath;
			return true;
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