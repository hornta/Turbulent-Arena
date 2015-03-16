#pragma once

#include "Pathfinder.hpp"


namespace bjoernligan
{

	namespace ai
	{
		class Sense;
		class Agent;

		struct SenseAgentData
		{
			SenseAgentData(Agent* agent);
			sf::Vector2f m_lastSeenPosition;
			Agent* m_agent;
		};

		class SenseData
		{
			float m_radius;
			Sense* m_sense;
			Agent* m_me;
			std::vector<std::unique_ptr<SenseAgentData>> m_visibleEnemies;
			std::vector<std::unique_ptr<SenseAgentData>> m_visibleFriends;

		public:
			SenseData(Agent* me, Sense* sense, float radius);

			void update();
			void setRadius(float radius);
			float getRadius() const;
			std::vector<std::unique_ptr<SenseAgentData>> getVisibleFriendsInRadius(float radius) const;
			std::vector<std::unique_ptr<SenseAgentData>> getVisibleEnemiesInRadius(float radius) const;
			std::vector<SenseAgentData*> getVisibleEnemies() const;
			std::vector<SenseAgentData*> getVisibleFriends() const;
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