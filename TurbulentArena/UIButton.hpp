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
		
		UIButton(sf::RenderWindow* p_xWindow, const float &p_fDepth);
		UIButton(const UIButton&);
		UIButton& operator=(const UIButton&);

	public:
		static Ptr Create(sf::RenderWindow* p_xWindow, const float &p_fDepth);

		~UIButton();

		void Initialize(
			const std::string &p_sName,
			const sf::IntRect &p_xSize,
			const std::function<void(bool)> &p_xFunction,
			const sf::Color &p_xIdleColor = m_xDefaultIdleColor,
			const sf::Color &p_xHoverColor = m_xDefaultHoverColor,
			const sf::Color &p_xPressedColor = m_xDefaultPressedColor);

		void Update(const float &p_fDeltaTime);
		void SetPos(const sf::Vector2f &p_xPos);
		void Draw();

		const bool HasNewValue(const float &p_bResetNew);
		const bool GetActive() const;

	private:
		bool m_bNewValue;
		bool m_bActive;
		std::function<void(bool)> m_xFunction;
		sf::Text m_xButtonText;
		input::Mouse* m_xMouse;
		std::unique_ptr<sf::RectangleShape> m_xButtonRect;

		sf::Color m_xIdleColor, m_xHoverColor, m_xPressedColor;
	};
}