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
		Scout(ai::Sense* sense);

		void initiate();
		void update(float deltatime);
	};
}