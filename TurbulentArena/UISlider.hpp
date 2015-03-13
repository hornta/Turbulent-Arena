//UISlider.hpp

#pragma once
#include "UIBase.hpp"
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

		UISlider(const std::string &p_sLabel, const float &p_fDepth);
		UISlider(const UISlider&);
		UISlider& operator=(const UISlider&);

	public:
		static Ptr Create(const std::string &p_sLabel, const float &p_fDepth);
		
		struct SliderDef
		{
			std::string m_sTextString;
			std::function<void(float)> m_xFunction;
			float m_fCurrent, m_fWidth, m_fMin, m_fMax;
			bool m_bContinous;
		};

		void Initialize(const SliderDef &p_xDefinition);
		void Update(const float &p_fDeltaTime);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void SetPos(const sf::Vector2f &p_xPos);

		float GetValue();

	private:
		float UISlider::GetAllowedX(float p_fX);

		sf::Text m_xText;
		std::string m_sTextString;
		input::Mouse* m_xMouse;
		std::function<void(float)> m_xFunction;

		bool m_bContinous;
		float m_fWidth, m_fCurrent, m_fMin, m_fMax;
	};
}