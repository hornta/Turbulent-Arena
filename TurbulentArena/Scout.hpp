#pragma once
#include "ClanMember.hpp"

namespace bjoernligan
{
	class Scout : public ClanMember
	{
	public:
		Scout();

		void initiate();
		void update(float deltatime);
	};
}