#include "stdafx.h"
#include "ClanManager.hpp"
#include "Clan.hpp"

namespace bjoernligan
{
	ClanManager::ClanManager()
	{
	}

	ClanManager::~ClanManager()
	{
	}


	Clan* ClanManager::createClan(const std::string& name)
	{
		Clan* clan = new Clan(name);
		m_clans.push_back(clan);
		return clan;
	}
	
	std::size_t ClanManager::getAmount() const
	{
		return m_clans.size();
	}
}