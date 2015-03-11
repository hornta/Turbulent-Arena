#pragma once
#include "ClanMember.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class Sense;
	}

	class Scout : public ClanMember
	{
	public:
		Scout(ai::Sense* sense, const sf::Color &p_xTeamColor);

		void initiate();
		void update(float deltatime);
	};
}