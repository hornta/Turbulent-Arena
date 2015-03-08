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
		Axeman(ai::Sense* sense);

		void initiate();
		void update(float deltatime);
	};
}