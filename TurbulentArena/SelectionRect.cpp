//SelectionRect.cpp

#include "stdafx.h"
#include "SelectionRect.hpp"
#include "Mouse.hpp"

namespace bjoernligan
{
	SelectionRect::SelectionRect()
		: m_eState(EState::Idle)
	{
		m_xMouse = ServiceLocator<input::Mouse>::GetService();
		m_xShape.setFillColor(sf::Color(255, 255, 255, 100));
		m_xShape.setOutlineColor(sf::Color(0, 0, 0, 100));
		m_xShape.setOutlineThickness(2);
	}

	void SelectionRect::SetTargetClan(Clan* p_xTargetClan)
	{
		m_xTargetClan = p_xTargetClan;
	}

	void SelectionRect::Update()
	{
		if (m_eState == EState::Idle && m_xMouse->IsDownOnce(sf::Mouse::Left))
		{
			m_eState = EState::Pressed;
			m_xStartPos = sf::Vector2f(m_xMouse->m_xPos);
			m_xShape.setPosition(sf::Vector2f(m_xMouse->m_xPos));
			m_xShape.setSize(sf::Vector2f(0,0));
		}
		else if (m_eState == EState::Pressed)
		{
			m_xShape.setPosition(m_xStartPos + m_xOffset);
			m_xShape.setSize(m_xOffset + sf::Vector2f(m_xMouse->m_xPos) - m_xShape.getPosition());
			if (!m_xMouse->IsDown(sf::Mouse::Left))
				m_eState = EState::Idle;
		}
	}

	void SelectionRect::Clear()
	{
		m_aiSelectedAgentIndexes.clear();
	}

	void SelectionRect::SetOffset(const sf::Vector2f &p_xOffset)
	{
		m_xOffset = p_xOffset;
	}

	void SelectionRect::SetAgentIndexes(const std::vector<int32_t> &p_xIntVector)
	{
		m_aiSelectedAgentIndexes = p_xIntVector;
	}

	const std::vector<int32_t> &SelectionRect::GetSelectedAgentIndexes() const
	{
		return m_aiSelectedAgentIndexes;
	}

	void SelectionRect::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		if(m_eState == EState::Pressed)
			target.draw(m_xShape, states);
	}
}