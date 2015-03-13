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
			std::vector<Agent*> m_visibleEnemies;
			std::vector<Agent*> m_visibleFriends;

		public:
			SenseData(Agent* me, Sense* sense, float radius);

			void update();
			void setRadius(float radius);
			float getRadius() const;
			std::vector<Agent*> getVisibleEnemies() const;
			std::vector<Agent*> getVisibleFriends() const;
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