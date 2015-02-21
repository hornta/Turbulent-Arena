//UIBase.cpp

#include "stdafx.h"
#include "UIBase.hpp"

namespace bjoernligan
{
	UIBase::UIBase(sf::RenderWindow* p_xWindow, const float &p_fDepth)
	{
		m_xWindow = p_xWindow;
		m_fDepth = p_fDepth;
	}

	UIBase::Ptr UIBase::Create(sf::RenderWindow* p_xWindow, const float &p_fDepth)
	{
		return Ptr(new UIBase(p_xWindow, p_fDepth));
	}

	UIBase::~UIBase()
	{

	}

	void UIBase::Update(const float &p_fDeltaTime)
	{
		p_fDeltaTime;
	}

	void UIBase::Draw()
	{
		if (m_axSprites.empty())
			return;

		auto itr = m_axSprites.begin();
		while (itr != m_axSprites.end())
		{
			if ((*itr))
				m_xWindow->draw(*(*itr));

			++itr;
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