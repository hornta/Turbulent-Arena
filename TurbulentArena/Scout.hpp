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
			ai::Agent* agent;
			sf::Vector2f position;
		};

		struct EnlightendFriend
		{
			EnlightendFriend(bool enlightend, ai::SenseAgentData* agentData);

			bool enlightend;
			ai::SenseAgentData* agentData;
		};

		std::vector<ReportData> m_toReport;
		std::vector<EnlightendFriend> m_enlightendFriends;

		Scout(ai::Sense* sense, const sf::Color &p_xTeamColor, Clan* p_xClan);

		void initiate();
		void update(float deltatime);
	};
}