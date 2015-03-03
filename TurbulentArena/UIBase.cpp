//UIBase.cpp

#include "stdafx.h"
#include "UIBase.hpp"

namespace bjoernligan
{
	UIBase::UIBase(const std::string &p_sLabel, const float &p_fDepth)
		: m_sLabel(p_sLabel)
		, m_fDepth(p_fDepth)
	{

	}

	UIBase::Ptr UIBase::Create(const std::string &p_sLabel, const float &p_fDepth)
	{
		return Ptr(new UIBase(p_sLabel, p_fDepth));
	}

	void UIBase::Update(const float &p_fDeltaTime)
	{
		p_fDeltaTime;
	}

	void UIBase::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (auto &sprite : m_axSprites)
		{
			target.draw(*sprite, states);
		}
	}

	void UIBase::SetPos(const sf::Vector2f &p_xPos)
	{
		m_xPos = p_xPos;

		if (m_axSprites.empty())
			return;

		auto itr = m_axSprites.begin();
		while (itr != m_axSprites.end())
		{
			(*itr)->setPosition(p_xPos);
			++itr;
		}
	}

	void UIBase::AddSprite(sf::Sprite* p_xSprite)
	{
		if (p_xSprite)
		{
			p_xSprite->setOrigin(p_xSprite->getLocalBounds().width / 2.0f, p_xSprite->getLocalBounds().height / 2.0f);
			m_axSprites.push_back(p_xSprite);
		}
	}
}