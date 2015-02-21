#pragma once

#include "Clan.hpp"

namespace bjoernligan
{
	class ClanManager
	{
	public:
		ClanManager();
		~ClanManager();

		Clan* createClan(const std::string& name);
		std::size_t getAmount() const;

	private:
		std::vector<std::unique_ptr<Clan>> m_clans;
	};
}