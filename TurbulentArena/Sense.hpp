#pragma once

#include "Visibility.hpp"
#include "Pathfinder.hpp"

namespace bjoernligan
{
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
			std::vector<Agent*> getVisibleAgents() const;

		public:
			SenseData(Agent* me, Sense* sense, float radius);

			void update();
			void setRadius(float radius);
			float getRadius() const;
			bool isVisible(Agent* agent, bool findPath, Pathfinder::Path& path, Pathfinder::Options pathfinderOptions = Pathfinder::Options()) const;
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
			std::vector<Agent*>& getAgents();
			Pathfinder* getPathfinder() const;
			Visibility* getVisibility() const;
		};
	}
}