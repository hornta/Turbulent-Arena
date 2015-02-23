//UIButton.cpp

#include "stdafx.h"
#include "UIButton.hpp"
#include "ServiceLocator.hpp"
#include "DrawManager.hpp"
#include "Mouse.hpp"

namespace bjoernligan
{
	sf::Color UIButton::m_xDefaultIdleColor = sf::Color(80, 80, 80, 128);
	sf::Color UIButton::m_xDefaultHoverColor = sf::Color(110, 110, 110, 128);
	sf::Color UIButton::m_xDefaultPressedColor = sf::Color(160, 160, 160, 128);

	UIButton::UIButton(sf::RenderWindow* p_xWindow, const float &p_fDepth)
		: UIBase(p_xWindow, p_fDepth)
		, m_bActive(false)
		, m_bNewValue(false)
		, m_eState(EButtonState::Idle)
		, m_xButtonRect(nullptr)
	{
		m_xButtonText.setFont(*ServiceLocator<system::DrawManager>::GetService()->GetFont());
		m_xButtonText.setCharacterSize(18);

		m_xMouse = ServiceLocator<input::Mouse>::GetService();
	}

	UIBase::Ptr UIButton::Create(sf::RenderWindow* p_xWindow, const float &p_fDepth)
	{
		return Ptr(new UIButton(p_xWindow, p_fDepth));
	}

	UIButton::~UIButton()
	{

	}

	void UIButton::Initialize(
		const std::string &p_sName,
		const sf::IntRect &p_xSize,
		const std::function<void(bool)> &p_xFunction,
		const sf::Color &p_xIdleColor,
		const sf::Color &p_xHoverColor,
		const sf::Color &p_xPressedColor)
	{
		m_xButtonText.setString(p_sName);

		m_xFunction = p_xFunction;

		m_xButtonRect = std::unique_ptr<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f((float)p_xSize.width, (float)p_xSize.height)));
		m_xButtonRect->setFillColor(p_xIdleColor);

		m_xIdleColor = p_xIdleColor;
		m_xHoverColor = p_xHoverColor;
		m_xPressedColor = p_xPressedColor;

		SetPos(sf::Vector2f((float)p_xSize.left, (float)p_xSize.top));
	}

	void UIButton::Update(const float &p_fDeltaTime)
	{
		UIBase::Update(p_fDeltaTime);

		if (!m_xMouse || !m_xButtonRect)
			return;

		if (m_xButtonRect->getGlobalBounds().contains((sf::Vector2f)m_xMouse->m_xPos))
		{
			if (!m_xMouse->IsDown(sf::Mouse::Button::Left) && m_eState == EButtonState::Pressed)
			{
				m_bActive = !m_bActive;
				m_eState = EButtonState::Idle;

				if (m_xFunction)
					m_xFunction(m_bActive);

				return;
			}

			if (m_eState != EButtonState::Pressed)
			{
				if (m_eState != EButtonState::Hover && !m_xMouse->IsDown(sf::Mouse::Button::Left))
				{
					m_eState = EButtonState::Hover;
					m_xButtonRect->setFillColor(m_xHoverColor);
				}
			}

			if (m_xMouse->IsDownOnce(sf::Mouse::Button::Left))
			{
				if (m_eState != EButtonState::Pressed)
				{
					m_eState = EButtonState::Pressed;
					m_xButtonRect->setFillColor(m_xPressedColor);
				}
			}

			return;
		}
		else
		{
			if (!m_xMouse->IsDown(sf::Mouse::Button::Left))
			{
				if (m_eState != EButtonState::Idle)
				{
					m_eState = EButtonState::Idle;
					m_xButtonRect->setFillColor(m_xIdleColor);
				}
			}
		}
		if (m_eState != EButtonState::Pressed)
		{
			if (m_eState != EButtonState::Idle)
			{
				m_eState = EButtonState::Idle;
				m_xButtonRect->setFillColor(m_xIdleColor);
			}
		}

	}

	void UIButton::SetPos(const sf::Vector2f &p_xPos)
	{
		if (!m_xButtonRect)
			return;

		m_xButtonRect->setPosition(p_xPos);

		m_xButtonText.setPosition(p_xPos);
	}

	void UIButton::Draw()
	{
		if (!m_xButtonRect)
			return;

		m_xWindow->draw(*m_xButtonRect.get());
		m_xWindow->draw(m_xButtonText);
	}

	const bool UIButton::GetActive() const
	{
		return m_bActive;
	}

	const bool UIButton::HasNewValue(const float &p_bResetNew)
	{
		if (p_bResetNew)
		{
			m_bNewValue = !m_bNewValue;
			return !m_bNewValue;
		}

		return m_bNewValue;
	}
}