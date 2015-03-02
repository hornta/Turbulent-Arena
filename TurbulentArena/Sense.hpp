#pragma once

#include "Visibility.hpp"

namespace bjoernligan
{
	class Pathfinder;

	namespace ai
	{
		class Sense;
		class Agent;

		class SenseData
		{
			float m_radius;
			Sense* m_sense;
			Visibility::Light* m_visibilityArea;
			Agent* m_me;
			std::vector<Agent*> m_visibleAgents;

		public:
			SenseData(Agent* me, Sense* sense, float radius);

			void update();
			void setRadius(float radius);
			float getRadius() const;
			std::vector<Agent*> getVisibleAgents() const;
			bool isVisible(Agent* agent, bool findPath = false) const;
		};

		class Sense
		{
			std::unique_ptr<Pathfinder> m_pathFinder;
			std::unique_ptr<Visibility> m_visiblity;
			std::vector<Agent*> m_agents;

		public:
			Sense(const sf::Vector2i& pathfinderSize);
			void addAgent(Agent* agent);

			void update(float dt);
			Pathfinder* getPathfinder() const;
			Visibility* getVisibility() const;
		};
	}
}