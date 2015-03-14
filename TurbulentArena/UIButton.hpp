//UIButton.hpp

#pragma once
#include "UIBase.hpp"
#include <functional>

namespace bjoernligan
{
	namespace input
	{
		class Mouse;
	}

	class UIButton : public UIBase
	{
	private:
		static sf::Color m_xDefaultIdleColor;
		static sf::Color m_xDefaultHoverColor;
		static sf::Color m_xDefaultPressedColor;

		enum EButtonState
		{
			Idle,
			Hover,
			Pressed,
		} m_eState;
		
		UIButton(const std::string &p_sLabel, const float &p_fDepth);
		UIButton(const UIButton&);
		UIButton& operator=(const UIButton&);

	public:
		static Ptr Create(const std::string &p_sLabel, const float &p_fDepth);

		void Initialize(
			const std::string &p_sName,
			const sf::IntRect &p_xSize,
			std::function<void(bool)> p_xFunction,
			const sf::Color &p_xIdleColor = m_xDefaultIdleColor,
			const sf::Color &p_xHoverColor = m_xDefaultHoverColor,
			const sf::Color &p_xPressedColor = m_xDefaultPressedColor);

		bool Update(const float &p_fDeltaTime);
		void SetPos(const sf::Vector2f &p_xPos);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		const bool HasNewValue(const float &p_bResetNew);
		const bool GetActive() const;

	private:
		bool m_bNewValue;
		bool m_bActive;
		bool m_bFunction;
		std::function<void(bool)> m_xFunction;
		sf::Text m_xButtonText;
		input::Mouse* m_xMouse;
		std::unique_ptr<sf::RectangleShape> m_xButtonRect;

		sf::Color m_xIdleColor, m_xHoverColor, m_xPressedColor;
	};
}