//Keyboard.cpp

#include "stdafx.h"
#include "Keyboard.hpp"

namespace bjoernligan
{
	namespace input
	{
		Keyboard::Keyboard()
		{
			for (unsigned int i = 0; i < sf::Keyboard::KeyCount; i++)
			{
				m_axCurrentKeyState[i] = false;
				m_axPrevKeyState[i] = false;
			}
		}

		Keyboard::Ptr Keyboard::Create()
		{
			return Ptr(new Keyboard);
		}

		Keyboard::~Keyboard()
		{

		}

		bool Keyboard::IsDown(sf::Keyboard::Key p_xKey)
		{
			if (p_xKey >= 0 && p_xKey < sf::Keyboard::KeyCount)
				return m_axCurrentKeyState[p_xKey];

			return false;
		}

		bool Keyboard::IsDownOnce(sf::Keyboard::Key p_xKey)
		{
			return IsDown(p_xKey) && !GetPrevious(p_xKey);
		}

		bool Keyboard::GetPrevious(sf::Keyboard::Key p_xKey)
		{
			if (p_xKey >= 0 && p_xKey < sf::Keyboard::KeyCount)
				return m_axPrevKeyState[p_xKey];

			return false;
		}

		void Keyboard::SetCurrent(sf::Keyboard::Key p_xKey, bool p_bNewValue)
		{
			if (p_xKey >= 0 && p_xKey < sf::Keyboard::KeyCount)
				m_axCurrentKeyState[p_xKey] = p_bNewValue;
		}

		void Keyboard::SetPrevious(sf::Keyboard::Key p_xKey, bool p_bNewValue)
		{
			if (p_xKey >= 0 && p_xKey < sf::Keyboard::KeyCount)
				m_axPrevKeyState[p_xKey] = p_bNewValue;
		}

		void Keyboard::PostUpdate()
		{
			for (unsigned int i = 0; i < sf::Keyboard::KeyCount; i++)
				m_axPrevKeyState[i] = m_axCurrentKeyState[i];
		}
	}
}