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
	}

	UIManager::Ptr UIManager::Create()
	{
		return Ptr(new UIManager());
	}

	void UIManager::Update(const float &p_fDeltaTime)
	{
		if (m_axElements.empty())
			return;

		auto itr = m_axElements.begin();
		while (itr != m_axElements.end())
		{
			(*itr)->Update(p_fDeltaTime);
			++itr;
		}
	}

	void UIManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (auto& element : m_axElements)
		{
			target.draw(*element, states);
		}
	}

	UISlider* UIManager::AddSlider(const UISlider::SliderDef &p_xDefinition, const sf::Vector2f &p_xPos, const float &p_fDepth)
	{
		UISlider* xSlider = static_cast<UISlider*>(AddElement<UISlider>(p_fDepth));
		
		system::SpriteManager* xSpriteManager = ServiceLocator<system::SpriteManager>::GetService();

		std::stringstream xStream;
		xStream << "slider_" << p_xDefinition.m_sLabel << "_bar_" << m_iElementCount;
		xSlider->AddSprite(xSpriteManager->LoadSprite("slider/slider_bar.png", xStream.str(), 0, 0, Settings::m_xSliderSize.x, Settings::m_xSliderSize.y));

		xStream.str("");
		xStream << "slider_" << p_xDefinition.m_sLabel << "_left_" << m_iElementCount;
		xSlider->AddSprite(xSpriteManager->LoadSprite("slider/slider_left.png", xStream.str(), 0, 0, Settings::m_xSliderSize.x, Settings::m_xSliderSize.y));

		xStream.str("");
		xStream << "slider_" << p_xDefinition.m_sLabel << "_right_" << m_iElementCount;
		xSlider->AddSprite(xSpriteManager->LoadSprite("slider/slider_right.png", xStream.str(), 0, 0, Settings::m_xSliderSize.x, Settings::m_xSliderSize.y));

		xStream.str("");
		xStream << "slider_" << p_xDefinition.m_sLabel << "_button_" << m_iElementCount;
		xSlider->AddSprite(xSpriteManager->LoadSprite("slider/slider.png", xStream.str(), 0, 0, Settings::m_xSliderSize.x, Settings::m_xSliderSize.y));

		xSlider->Initialize(p_xDefinition);
		xSlider->SetPos(p_xPos);

		return xSlider;
	}
}