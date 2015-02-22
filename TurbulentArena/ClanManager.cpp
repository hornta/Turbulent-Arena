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

	void ClanManager::Update(const float &p_fDeltaTime)
	{
		for (uint32_t i = 0; i < m_clans.size(); ++i)
		{
			m_clans[i]->Update(p_fDeltaTime);
		}
	}
}