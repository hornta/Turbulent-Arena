//ClanStats.cpp

#include "stdafx.h"
#include "ClanStats.hpp"
#include "ServiceLocator.hpp"
#include "DrawManager.hpp"
#include <iomanip>

namespace bjoernligan
{
	ClanStats::ClanStats(const bool &p_bActive)
		: m_bActive(p_bActive)
		, m_xBgRect(nullptr)
	{

	}

	bool ClanStats::Initialize(const std::vector<Clan*> &p_xClans)
	{
		system::DrawManager* xDrawManager = ServiceLocator<system::DrawManager>::GetService();

		if (!xDrawManager)
			return false;

		auto itr = p_xClans.begin();
		while (itr != p_xClans.end())
		{
			m_axClansWithText.push_back({ sf::Text("", *xDrawManager->GetFont(), 18), (*itr) });

			uint8_t iColorMod = 100;

			uint16_t r = (*itr)->getColor().r;
			(r < iColorMod) ? r += iColorMod : r = 255;
			uint16_t g = (*itr)->getColor().g;
			(g < iColorMod) ? g += iColorMod : g = 255;
			uint16_t b = (*itr)->getColor().b;
			(b < iColorMod) ? b += iColorMod : b = 255;

			m_axClansWithText.back().m_xText.setColor(sf::Color(sf::Uint8(r), sf::Uint8(g), sf::Uint8(b)));
			++itr;
		}

		m_xBgRect = std::unique_ptr<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f(300, p_xClans.size() * 32.0f)));
		m_xBgRect->setFillColor(sf::Color(80, 80, 80, 200));

		return true;
	}

	void ClanStats::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!m_bActive || !m_xBgRect)
			return;

		target.draw(*m_xBgRect, states);
		for (uint32_t i = 0; i < m_axClansWithText.size(); ++i)
		{
			target.draw(m_axClansWithText[i].m_xText, states);
		}
	}

	void ClanStats::Update()
	{
		if (!m_bActive)
			return;

		for (uint32_t i = 0; i < m_axClansWithText.size(); ++i)
		{
			m_axClansWithText[i].m_xText.setString(
				std::string(m_axClansWithText[i].m_xClan->GetName())
				+ " members alive: "
				+ std::to_string(m_axClansWithText[i].m_xClan->getMembers().size()));
		}
	}

	void ClanStats::SetActive(const bool &p_bActive)
	{
		m_bActive = p_bActive;
	}

	void ClanStats::SetPos(const float &p_x, const float &p_y)
	{
		SetPos(sf::Vector2f(p_x, p_y));
	}

	void ClanStats::SetPos(const sf::Vector2f &p_xPos)
	{
		for (uint32_t i = 0; i < m_axClansWithText.size(); ++i)
		{
			m_axClansWithText[i].m_xText.setPosition(p_xPos + sf::Vector2f(10, (32.0f * i) + 3));
		}
		if (m_xBgRect)
			m_xBgRect->setPosition(p_xPos);
	}
}