//UIManager.cpp

#include "stdafx.h"
#include "UIManager.hpp"
#include "ServiceLocator.hpp"
#include "SpriteManager.hpp"
#include "Settings.hpp"
#include <sstream>

namespace bjoernligan
{
	UIManager::UIManager()
	{
		m_view.setSize(sf::Vector2f(Settings::m_xWindowSize));
	}

	UIManager::Ptr UIManager::Create()
	{
		return Ptr(new UIManager());
	}

	void UIManager::Clear()
	{
		if (m_axElements.empty())
			return;

		m_iSliderCount = 0;

		auto itr = m_axElements.begin();
		while (itr != m_axElements.end())
		{
			itr = m_axElements.erase(itr);
		}
	}

	void UIManager::Update(const float &p_fDeltaTime)
	{
		if (m_axElements.empty())
			return;

		for (uint32_t i = 0; i < m_axElements.size(); ++i)
		{
			m_axElements[i]->Update(p_fDeltaTime);
		}
	}

	void UIManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (auto& element : m_axElements)
		{
			target.draw(*element, states);
		}
	}

	UISlider* UIManager::AddSlider(const std::string &p_sLabel, const UISlider::SliderDef &p_xDefinition, const sf::Vector2f &p_xPos, const float &p_fDepth)
	{
		++m_iSliderCount;

		UISlider* xSlider = static_cast<UISlider*>(AddElement<UISlider>(p_sLabel, p_fDepth));

		system::SpriteManager* xSpriteManager = ServiceLocator<system::SpriteManager>::GetService();

		std::stringstream xStream;
		xStream << "slider_" << p_xDefinition.m_sLabel << "_bar_" << m_iSliderCount;
		xSlider->AddSprite(xSpriteManager->LoadSprite("slider/slider_bar.png", xStream.str(), 0, 0, Settings::m_xSliderSize.x, Settings::m_xSliderSize.y));

		xStream.str("");
		xStream << "slider_" << p_xDefinition.m_sLabel << "_left_" << m_iSliderCount;
		xSlider->AddSprite(xSpriteManager->LoadSprite("slider/slider_left.png", xStream.str(), 0, 0, Settings::m_xSliderSize.x, Settings::m_xSliderSize.y));

		xStream.str("");
		xStream << "slider_" << p_xDefinition.m_sLabel << "_right_" << m_iSliderCount;
		xSlider->AddSprite(xSpriteManager->LoadSprite("slider/slider_right.png", xStream.str(), 0, 0, Settings::m_xSliderSize.x, Settings::m_xSliderSize.y));

		xStream.str("");
		xStream << "slider_" << p_xDefinition.m_sLabel << "_button_" << m_iSliderCount;
		xSlider->AddSprite(xSpriteManager->LoadSprite("slider/slider.png", xStream.str(), 0, 0, Settings::m_xSliderSize.x, Settings::m_xSliderSize.y));

		xSlider->Initialize(p_xDefinition);
		xSlider->SetPos(p_xPos);

		return xSlider;
	}

	void UIManager::RemoveElementsByLabel(const std::string &p_sLabel)
	{
		auto itr = m_axElements.begin();
		while (itr != m_axElements.end())
		{
			if ((*itr)->m_sLabel == p_sLabel)
			{
				itr = m_axElements.erase(itr);
				continue;
			}

			++itr;
		}
	}

	void UIManager::setView(const sf::View& view)
	{
		m_view = view;
	}

	sf::View UIManager::getView() const
	{
		return m_view;
	}
}