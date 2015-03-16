#pragma once
#include "ClanMember.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class Sense;
		class Agent;
	}

	class Scout : public ClanMember
	{
	public:
		struct ReportData
		{
			ReportData(ai::Agent* agent, const sf::Vector2f& position) :
				agent(agent),
				position(position)
			{

			}

			ai::Agent* agent;
			sf::Vector2f position;
		};

		struct EnlightendFriend
		{
			EnlightendFriend(bool enlightend);
			

			bool enlightend; 
			std::unique_ptr<ai::SenseAgentData> agent;
			std::vector<std::unique_ptr<ReportData>> toReport;
		};

		std::vector<ReportData> m_toReport;
		std::vector<std::unique_ptr<EnlightendFriend>> m_enlightendFriends;

		Scout(ai::Sense* sense, const sf::Color &p_xTeamColor, Clan* p_xClan);
		~Scout();

		void RemoveEnlightenedFriend(ai::Agent* p_xAgent);

		void initiate();
		void update(float deltatime);
	};
}