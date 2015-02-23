//UISlider.hpp

#pragma once
#include "UIBase.hpp"
#include "SliderBridge.hpp"
#include <functional>

namespace bjoernligan
{
	namespace input
	{
		class Mouse;
	};

	class UISlider : public UIBase
	{
	private:
		enum EStatus
		{
			Idle,
			Pressed,
		} m_eStatus;

		UISlider(sf::RenderWindow* p_xWindow, const float &p_fDepth);
		UISlider(const UISlider&);
		UISlider& operator=(const UISlider&);

	public:
		static Ptr Create(sf::RenderWindow* p_xWindow, const float &p_fDepth);

		~UISlider();
		
		void Initialize(const std::string &p_sLabel, const std::function<void(float)> &p_xFunction, const float &p_fWidth, const float &p_fMin, const float &p_fMax);
		void Update(const float &p_fDeltaTime);
		void Draw();
		void SetPos(const sf::Vector2f &p_xPos);

		bool NewValue();
		float GetValue(const bool &p_bResetBool = true);

		SliderBridge* GetBridge();

	private:
		float UISlider::GetAllowedX(float p_fX);

		sf::Text m_xLabelText;
		std::string m_sLabel;
		input::Mouse* m_xMouse;
		SliderBridge::Ptr m_xBridge;
		std::function<void(float)> m_xFunction;

		bool m_bNewValue;
		float m_fWidth, m_fCurrent, m_fMin, m_fMax;
	};
}