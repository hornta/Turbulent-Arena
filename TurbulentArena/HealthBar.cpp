//HealthBar.cpp

#include "stdafx.h"
#include "HealthBar.hpp"
#include "CombatStats.hpp"

namespace bjoernligan
{
	HealthBar::HealthBar(const sf::Vector2f &p_xOffset, const int32_t &p_iWidth, const sf::Color &p_xTeamColor, const sf::Color &p_xHealthColor, const sf::Color &p_xBgColor)
		: m_xCombatStats(nullptr)
		, m_xOffset(p_xOffset)
		, m_iMaxWidth(p_iWidth)
	{
		float fRectHeight = 8;

		m_xHealthRect.setFillColor(p_xHealthColor);
		m_xHealthRect.setSize(sf::Vector2f((float)p_iWidth, fRectHeight));
		m_xBgRect.setFillColor(p_xBgColor);
		m_xBgRect.setSize(sf::Vector2f((float)p_iWidth, fRectHeight));
		m_xBgRect.setOutlineThickness(3);
		m_xBgRect.setOutlineColor(p_xTeamColor);
	}

	void HealthBar::SetCombatStats(CombatStats* p_xCombatStats)
	{
		m_xCombatStats = p_xCombatStats;
	}

	void HealthBar::Update(const sf::Vector2f &p_xPos)
	{
		m_xHealthRect.setPosition(m_xOffset + p_xPos);
		m_xBgRect.setPosition(m_xOffset + p_xPos);

		if (!m_xCombatStats)
			return;

		m_xHealthRect.setSize(sf::Vector2f(m_xCombatStats->GetHealthPercentage() * m_iMaxWidth, m_xHealthRect.getSize().y));
	}

	void HealthBar::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		target.draw(m_xBgRect, states);
		target.draw(m_xHealthRect, states);
	}
}