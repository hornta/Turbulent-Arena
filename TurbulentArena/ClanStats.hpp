//ClanStats.hpp

#pragma once
#include "Clan.hpp"

namespace bjoernligan
{
	class Clan;

	class ClanStats : public sf::Drawable
	{
		struct ClanWithText
		{
			sf::Text m_xText;
			Clan* m_xClan;
		};

	public:
		ClanStats(const bool &p_bActive);

		bool Initialize(const std::vector<Clan*> &p_xClans);

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void Update();
		void SetActive(const bool &p_bActive);
		void SetPos(const float &p_x, const float &p_y);
		void SetPos(const sf::Vector2f &p_xPos);

	private:
		bool m_bActive;
		std::unique_ptr<sf::RectangleShape> m_xBgRect;
		std::vector<ClanWithText> m_axClansWithText;
	};
}