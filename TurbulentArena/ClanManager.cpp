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

	Clan* ClanManager::createClan(const std::string& name, const sf::Color &p_xTeamColor)
	{
		m_clans.emplace_back(std::make_unique<Clan>(name, p_xTeamColor));
		return m_clans.back().get();
	}
	
	std::size_t ClanManager::getAmount() const
	{
		return m_clans.size();
	}

	void ClanManager::Update(const float &p_fDeltaTime)
	{
		for (std::size_t i = 0; i < m_clans.size(); ++i)
		{
			m_clans[i]->Update(p_fDeltaTime);
		}
	}

	void ClanManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (std::size_t i = 0; i < m_clans.size(); ++i)
		{
			target.draw(*m_clans[i].get(), states);
		}
	}

	std::vector<Clan*> ClanManager::getClans() const
	{
		std::vector<Clan*> clans;
		for (std::size_t i = 0; i < m_clans.size(); ++i)
		{
			clans.push_back(m_clans[i].get());
		}
		return clans;
	}
}