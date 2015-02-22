//UISlider.cpp

#include "stdafx.h"
#include "UISlider.hpp"
#include "ServiceLocator.hpp"
#include "DrawManager.hpp"
#include "Mouse.hpp"
#include "Settings.hpp"
#include <sstream>
#include <iomanip>

namespace bjoernligan
{
	UISlider::UISlider(sf::RenderWindow* p_xWindow, const float &p_fDepth)
		: UIBase(p_xWindow, p_fDepth)
	{
		m_eStatus = EStatus::Idle;

		m_xLabelText.setFont(*ServiceLocator<system::DrawManager>::GetService()->GetFont());
		m_xLabelText.setCharacterSize(18);

		m_xMouse = ServiceLocator<input::Mouse>::GetService();

		m_fWidth = m_fCurrent = m_fMin = m_fMax = 0.0f;
		m_bNewValue = false;
	}

	UIBase::Ptr UISlider::Create(sf::RenderWindow* p_xWindow, const float &p_fDepth)
	{
		return Ptr(new UISlider(p_xWindow, p_fDepth));
	}

	UISlider::~UISlider()
	{

	}

	void UISlider::Initialize(const std::string &p_sLabel, const float &p_fWidth, const float &p_fMin, const float &p_fMax)
	{
		m_sLabel = p_sLabel;
		m_xLabelText.setString(p_sLabel);
		m_fWidth = p_fWidth;
		m_fMin = p_fMin;
		m_fMax = p_fMax;
		
		if (m_axSprites.size() >= 4 && m_axSprites[3])
		{
			m_axSprites[0]->setScale(sf::Vector2f((((p_fWidth + Settings::m_xSliderSize.x * 2) / Settings::m_xSliderSize.x) * Settings::m_xSliderSize.x) / Settings::m_xSliderSize.x, 1.0f));
		}
	}

	void UISlider::Update(const float &p_fDeltaTime)
	{
		UIBase::Update(p_fDeltaTime);

		if (!m_xMouse || m_axSprites.size() != 4U || !m_axSprites[3] || !m_axSprites[0])
			return;

		if (m_eStatus == EStatus::Idle)
		{
			if (m_xMouse->IsDownOnce(sf::Mouse::Button::Left) && m_axSprites[0]->getGlobalBounds().contains(sf::Vector2f(m_xMouse->m_xPos)))
			{
				m_eStatus = EStatus::Pressed;
			}
		}
		if (m_eStatus == EStatus::Pressed)
		{
			if (!m_xMouse->IsDown(sf::Mouse::Button::Left))
			{
				m_eStatus = EStatus::Idle;
				m_bNewValue = true;
				return;
			}

			m_axSprites[3]->setPosition(sf::Vector2f(GetAllowedX((float)m_xMouse->m_xPos.x), m_xPos.y));
			m_fCurrent = (m_axSprites[3]->getPosition().x - m_xPos.x) / m_fWidth;

			std::stringstream xStream;
			xStream << m_sLabel << ": " << std::fixed << std::setprecision(1) << GetValue(false);
			m_xLabelText.setString(xStream.str());
		}
	}

	void UISlider::Draw()
	{
		UIBase::Draw();
		m_xWindow->draw(m_xLabelText);
	}

	void UISlider::SetPos(const sf::Vector2f &p_xPos)
	{
		m_xPos = p_xPos;
		if (m_axSprites.size() >= 4)
		{
			if (m_axSprites[0])
				m_axSprites[0]->setPosition(sf::Vector2f(p_xPos.x + m_fWidth / 2.0f, p_xPos.y));
			if (m_axSprites[1])
				m_axSprites[1]->setPosition(sf::Vector2f(p_xPos.x - Settings::m_xSliderSize.x, p_xPos.y));
			if (m_axSprites[2])
				m_axSprites[2]->setPosition(sf::Vector2f(p_xPos.x + m_fWidth + Settings::m_xSliderSize.x, p_xPos.y));
			if (m_axSprites[3])
				m_axSprites[3]->setPosition(sf::Vector2f(p_xPos.x + m_fCurrent * m_fWidth, p_xPos.y));

			m_xLabelText.setPosition(sf::Vector2f(p_xPos.x - Settings::m_xSliderSize.x, p_xPos.y - Settings::m_xSliderSize.y * 1.5f));
		}
	}

	bool UISlider::NewValue()
	{
		return m_bNewValue;
	}

	float UISlider::GetValue(const bool &p_bResetBool)
	{
		if (p_bResetBool)
			m_bNewValue = false;

		return m_fMin + (m_fMax - m_fMin) * m_fCurrent;
	}

	float UISlider::GetAllowedX(float p_fX)
	{
		if (p_fX < m_xPos.x)
			p_fX = m_xPos.x;
		if (p_fX > m_xPos.x + m_fWidth)
			p_fX = m_xPos.x + m_fWidth;

		return p_fX;
	}
}