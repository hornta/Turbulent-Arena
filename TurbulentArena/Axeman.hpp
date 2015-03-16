#pragma once
#include "ClanMember.hpp"
#include "Scout.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class Sense;
	}

	class Axeman : public ClanMember
	{
	public:
		std::vector<std::unique_ptr<Scout::ReportData>> m_incomingReports;

		Axeman(ai::Sense* sense, const sf::Color &p_xTeamColor, Clan* p_xClan);

		void initiate();
		void update(float deltatime);
		bool processReports();
	};
}