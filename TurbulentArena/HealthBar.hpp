//HealthBar.hpp

#pragma once

namespace bjoernligan
{
	struct CombatStats;

	class HealthBar : public sf::Drawable
	{
	public:
		HealthBar(const sf::Vector2f &p_xOffset, const int32_t &p_iWidth, const sf::Color &p_xTeamColor, const sf::Color &p_xHealthColor = sf::Color(150, 255, 150), const sf::Color &p_xBgColor = sf::Color(100, 0, 0));

		void SetCombatStats(CombatStats* p_xCombatStats);
		void Update(const sf::Vector2f &p_xPos);

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		CombatStats* m_xCombatStats;
		int32_t m_iMaxWidth;
		sf::Vector2f m_xOffset;
		sf::RectangleShape m_xHealthRect;
		sf::RectangleShape m_xTeamRect;
		sf::RectangleShape m_xBgRect;
	};
}