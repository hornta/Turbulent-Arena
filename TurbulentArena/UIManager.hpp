//UIManager.hpp

#pragma once
#include <memory>
#include <vector>
#include <SFML\Graphics.hpp>
#include "UIBase.hpp"
#include "UISlider.hpp"
#include "DebugWindow.hpp"
#include <functional>

namespace bjoernligan
{
	class UISlider;

	class UIManager : public sf::Drawable
	{
	private:
		UIManager();
		UIManager(const UIManager&);
		UIManager& operator=(const UIManager&);

		struct UIText
		{
			sf::Text m_xText;
			std::string m_sLabel;
		};

	public:
		typedef std::unique_ptr<UIManager> Ptr;
		static Ptr Create();
		
		bool Initialize();

		void Clear();
		void Update(const float &p_fDeltaTime);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		template<class T>
		UIBase* AddElement(const std::string &p_sLabel, const float &p_fDepth)
		{
			uint32_t iPos = 0;
			auto itr = m_axElements.begin();
			while (itr != m_axElements.end())
			{
				if ((*itr)->m_fDepth > p_fDepth)
					++iPos;

				++itr;
			}

			m_axElements.insert(m_axElements.begin() + iPos, T::Create(p_sLabel, p_fDepth));
			return m_axElements[iPos].get();
		}

		void AddText(const std::string &p_sTextName, const std::string &p_sTextString, const std::string &p_sLabel, const sf::Vector2f &p_xPos, const uint32_t &p_iTextSize = 30U);
		void ChangeTextString(const std::string &p_sTextName, const std::string &p_sTextString);

		UISlider* AddSlider(const std::string &p_sLabel, const UISlider::SliderDef &p_xDefinition, const sf::Vector2f &p_xPos, const float &p_fDepth);
		void RemoveElementsByLabel(const std::string &p_sLabel);

		void setView(const sf::View& view);
		sf::View getView() const;
	private:
		sf::View m_view;
		std::vector<UIBase::Ptr> m_axElements;
		std::map< std::string, UIText > m_axTexts;
		uint32_t m_iSliderCount;

		DebugWindow::Ptr m_xDebugWindow;
	};
}