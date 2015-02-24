//DebugWindow.cpp

#include "stdafx.h"
#include "DebugWindow.hpp"
#include "ServiceLocator.hpp"
#include "DrawManager.hpp"
#include <iomanip>

namespace bjoernligan
{
	DebugWindow::DebugWindow(const bool &p_bActive)
		: m_bActive(p_bActive)
	{
		m_xFps.setFont(*ServiceLocator<system::DrawManager>::GetService()->GetFont());
		m_xFps.setCharacterSize(18);

		m_xBgRect = std::unique_ptr<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f(128.0f, 32.0f)));
		m_xBgRect->setFillColor(sf::Color(80, 80, 80, 200));
	}

	DebugWindow::Ptr DebugWindow::Create(const bool &p_bActive)
	{
		return Ptr(new DebugWindow(p_bActive));
	}

	void DebugWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!m_bActive)
			return;

		target.draw(*m_xBgRect, states);
		target.draw(m_xFps, states);
	}

	void DebugWindow::Update(const float &p_fDeltaTime)
	{
		if (!m_bActive)
			return;

		/*m_xSStream.str("");
		m_xSStream << "fps: " << std::fixed << std::setprecision(5) << p_fDeltaTime;
		m_xFps.setString(m_xSStream.str());*/

		m_xFps.setString("fps: " + std::to_string(static_cast<int>(1.0f / p_fDeltaTime)));
	}

	void DebugWindow::SetActive(const bool &p_bActive)
	{
		m_bActive = p_bActive;
	}

	void DebugWindow::SetPos(const float &p_x, const float &p_y)
	{
		SetPos(sf::Vector2f(p_x, p_y));
	}

	void DebugWindow::SetPos(const sf::Vector2f &p_xPos)
	{
		m_xFps.setPosition(p_xPos);
		m_xBgRect->setPosition(p_xPos);
	}
}