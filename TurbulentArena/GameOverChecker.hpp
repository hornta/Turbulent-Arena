//GameConditions.hpp

#pragma once

namespace bjoernligan
{
	class Clan;

	class GameOverChecker
	{
	public:
		GameOverChecker();

		void AddClan(Clan* p_xClan);

		bool GameOver();
		std::string GetWinningTeam();

	private:
		std::vector<Clan*> m_axClans;
	};
}