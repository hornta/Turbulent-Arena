//GameConditions.cpp

#include "stdafx.h"
#include "GameOverChecker.hpp"
#include "Clan.hpp"

namespace bjoernligan
{
	GameOverChecker::GameOverChecker()
	{

	}

	void GameOverChecker::AddClan(Clan* p_xClan)
	{
		m_axClans.push_back(p_xClan);
	}

	bool GameOverChecker::GameOver()
	{
		if (m_axClans.empty() || m_axClans.size() == 1)
			return false;

		uint32_t iClansAlive(0);
		for (uint32_t i = 0; i < m_axClans.size(); ++i)
		{
			if (m_axClans[i]->IsAlive())
				++iClansAlive;
		}

		return iClansAlive <= 1;
	}

	std::string GameOverChecker::GetWinningTeam()
	{
		for (uint32_t i = 0; i < m_axClans.size(); ++i)
		{
			if (m_axClans[i]->IsAlive())
				return m_axClans[i]->GetName();
		}

		return "";
	}
}