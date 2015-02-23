#pragma once
#include "ClanMember.hpp"

namespace bjoernligan
{
	class Axeman : public ClanMember
	{
	public:
		Axeman();

		void initiate();
		void update(float deltatime);
	};
}