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
		m_clans.emplace_back(std::make_unique<Clan>(name));
		return m_clans.back().get();
	}
	
	std::size_t ClanManager::getAmount() const
	{
		return m_clans.size();
	}
}