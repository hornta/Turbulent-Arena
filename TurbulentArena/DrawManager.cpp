//DrawManager.cpp

#include "stdafx.h"
#include "Settings.hpp"
#include "DrawManager.hpp"
#include <string>

namespace bjoernligan
{
	namespace system
	{
		DrawManager::DrawManager()
		{
			m_xWindow = nullptr;
		}

		DrawManager::Ptr DrawManager::Create()
		{
			return Ptr(new DrawManager);
		}

		bool DrawManager::Initialize()
		{
			m_xWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(Settings::m_xWindowSize.x, Settings::m_xWindowSize.y), "Turbulent Arena");
			if (m_xWindow == nullptr)
			{
				return false;
			}

			m_xFont.loadFromFile("../data/fonts/arial.ttf");

			return true;
		}

		void DrawManager::ClearScr()
		{
			m_xWindow->clear(sf::Color(80, 80, 80));
		}

		void DrawManager::Draw(sf::Drawable* p_xDrawTarget)
		{
			if (!p_xDrawTarget)
				return;

			m_xWindow->draw(*p_xDrawTarget);
		}

		void DrawManager::Display()
		{
			m_xWindow->display();
		}

		sf::Font* DrawManager::GetFont()
		{
			return &m_xFont;
		}

		sf::RenderWindow* DrawManager::getWindow() const
		{
			return m_xWindow.get();
		}
	}
}