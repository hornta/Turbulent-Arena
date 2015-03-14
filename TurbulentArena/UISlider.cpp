//UISlider.cpp

#include "stdafx.h"
#include "UISlider.hpp"
#include "ServiceLocator.hpp"
#include "DrawManager.hpp"
#include "AudioManager.hpp"
#include "Mouse.hpp"
#include "Settings.hpp"
#include <sstream>
#include <iomanip>

namespace bjoernligan
{
	UISlider::UISlider(const std::string &p_sLabel, const float &p_fDepth)
		: UIBase(p_sLabel, p_fDepth)
		, m_eStatus(EStatus::Idle)
		, m_fCurrent(0.0f)
		, m_fWidth(0.0f)
		, m_fMin(0.0f)
		, m_fMax(0.0f)
	{
		m_xText.setFont(*ServiceLocator<system::DrawManager>::GetService()->GetFont());
		m_xText.setCharacterSize(18);

		m_xMouse = ServiceLocator<input::Mouse>::GetService();
	}

	UIBase::Ptr UISlider::Create(const std::string &p_sLabel, const float &p_fDepth)
	{
		return Ptr(new UISlider(p_sLabel, p_fDepth));
	}

	void UISlider::Initialize(const SliderDef &p_xDefinition)
	{
		m_sTextString = p_xDefinition.m_sTextString;
		m_xFunction = std::move(p_xDefinition.m_xFunction);
		m_fWidth = p_xDefinition.m_fWidth;
		m_fMin = p_xDefinition.m_fMin;
		m_fMax = p_xDefinition.m_fMax;
		m_fCurrent = p_xDefinition.m_fCurrent / (m_fMax - m_fMin);
		m_bContinous = p_xDefinition.m_bContinous;

		std::stringstream xStream;
		xStream << m_sTextString << ": " << std::fixed << std::setprecision(1) << GetValue();
		m_xText.setString(xStream.str());

		if (m_axSprites.size() >= 4 && m_axSprites[3])
		{
			m_axSprites[0]->setScale(sf::Vector2f((((m_fWidth + Settings::m_xSliderSize.x) / Settings::m_xSliderSize.x) * Settings::m_xSliderSize.x) / Settings::m_xSliderSize.x, 1.0f));
		}
	}

	void UISlider::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		UIBase::draw(target, states);
		target.draw(m_xText, states);
	}

	bool UISlider::Update(const float &p_fDeltaTime)
	{
		UIBase::Update(p_fDeltaTime);

		if (!m_xMouse || m_axSprites.size() != 4U || !m_axSprites[3] || !m_axSprites[0])
			return false;

		if (m_eStatus == EStatus::Idle)
		{
			if (m_xMouse->IsDownOnce(sf::Mouse::Button::Left) && m_axSprites[0]->getGlobalBounds().contains(sf::Vector2f(m_xMouse->m_xPos)))
			{
				m_eStatus = EStatus::Pressed;
				ServiceLocator<system::AudioManager>::GetService()->PlaySoundClip("Button2");
				return true;
			}
		}
		if (m_eStatus == EStatus::Pressed)
		{
			if (!m_xMouse->IsDown(sf::Mouse::Button::Left))
			{
				m_eStatus = EStatus::Idle;

				ServiceLocator<system::AudioManager>::GetService()->PlaySoundClip("Button1");

				m_xFunction(GetValue());
				return true;
			}

			m_axSprites[3]->setPosition(sf::Vector2f(GetAllowedX((float)m_xMouse->m_xPos.x), m_xPos.y));
			m_fCurrent = (m_axSprites[3]->getPosition().x - m_xPos.x) / m_fWidth;

			if (m_bContinous)
				m_xFunction(GetValue());

			std::stringstream xStream;
			xStream << m_sTextString << ": " << std::fixed << std::setprecision(1) << GetValue();
			m_xText.setString(xStream.str());

			return true;
		}
		return false;
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

			m_xText.setPosition(sf::Vector2f(p_xPos.x - Settings::m_xSliderSize.x, p_xPos.y - Settings::m_xSliderSize.y * 1.5f));
		}
	}

	float UISlider::GetValue()
	{
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