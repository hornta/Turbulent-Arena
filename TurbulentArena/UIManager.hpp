//UIManager.hpp

#pragma once
#include <memory>
#include <vector>
#include <SFML\Graphics.hpp>
#include "UIBase.hpp"

namespace bjoernligan
{
	class UISlider;

	class UIManager
	{
	private:
		UIManager();
		UIManager(const UIManager&);
		UIManager& operator=(const UIManager&);

	public:
		typedef std::unique_ptr<UIManager> Ptr;
		static Ptr Create();

		~UIManager();

		bool Initialize(sf::RenderWindow* p_xWindow);
		void Update(const float &p_fDeltaTime);
		void DrawElements();

		template<class T>
		UIBase* AddElement(const float &p_fDepth)
		{
			++m_iElementCount;

			uint32_t iPos = 0;
			auto itr = m_axElements.begin();
			while (itr != m_axElements.end())
			{
				if ((*itr)->m_fDepth > p_fDepth)
					++iPos;

				++itr;
			}

			m_axElements.insert(m_axElements.begin() + iPos, T::Create(m_xWindow, p_fDepth));
			return m_axElements[iPos].get();
		}

		UISlider* AddSlider(const std::string &p_sLabel, const float &p_fDepth, const sf::Vector2f &p_xPos, const float &p_fWidth, const float &p_fMin, const float &p_fMax);

	private:
		sf::RenderWindow* m_xWindow;
		std::vector<UIBase::Ptr> m_axElements;
		uint32_t m_iElementCount;
	};
}