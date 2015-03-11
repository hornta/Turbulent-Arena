#pragma once
#include "ClanMember.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class Sense;
	}

	class Axeman : public ClanMember
	{
	public:
		Axeman(ai::Sense* sense, const sf::Color &p_xTeamColor);

		void initiate();
		void update(float deltatime);
	};
}