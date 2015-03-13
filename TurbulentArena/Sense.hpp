#pragma once

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
			Agent* m_me;
			std::vector<Agent*> m_visibleAgents;

		public:
			SenseData(Agent* me, Sense* sense, float radius);

			void update();
			void setRadius(float radius);
			float getRadius() const;
			bool isVisible(Agent* agent, bool findPath, Pathfinder::Path& path, Pathfinder::Options pathfinderOptions = Pathfinder::Options()) const;
			std::vector<Agent*> getVisibleAgents() const;
		};

		class Sense
		{
			std::vector<Agent*> m_agents;

		public:
			Sense();
			void addAgent(Agent* agent);
			void removeAgent(Agent* agent);
			std::vector<Agent*>& getAgents();
		};
	}
}