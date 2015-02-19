//Mouse.cpp

#include "stdafx.h"
#include "Mouse.hpp"

namespace bjoernligan
{
	namespace input
	{
		Mouse::Mouse()
		{
			for (unsigned int i = 0; i < sf::Mouse::ButtonCount; i++)
			{
				m_axCurrentButtonState[i] = false;
				m_axPrevButtonState[i] = false;
			}
		}

		Mouse::Ptr Mouse::Create()
		{
			return Ptr(new Mouse);
		}

		Mouse::~Mouse()
		{

		}

		bool Mouse::IsDown(sf::Mouse::Button p_xButton)
		{
			if (p_xButton >= 0 && p_xButton < sf::Mouse::ButtonCount)
				return m_axCurrentButtonState[p_xButton];

			return false;
		}

		bool Mouse::IsDownOnce(sf::Mouse::Button p_xButton)
		{
			return IsDown(p_xButton) && !GetPrevious(p_xButton);
		}

		bool Mouse::GetPrevious(sf::Mouse::Button p_xButton)
		{
			if (p_xButton >= 0 && p_xButton < sf::Mouse::ButtonCount)
				return m_axPrevButtonState[p_xButton];

			return false;
		}

		void Mouse::SetCurrent(sf::Mouse::Button p_xButton, bool p_bNewValue)
		{
			if (p_xButton >= 0 && p_xButton < sf::Mouse::ButtonCount)
				m_axCurrentButtonState[p_xButton] = p_bNewValue;
		}

		void Mouse::SetPrevious(sf::Mouse::Button p_xButton, bool p_bNewValue)
		{
			if (p_xButton >= 0 && p_xButton < sf::Mouse::ButtonCount)
				m_axPrevButtonState[p_xButton] = p_bNewValue;
		}

		void Mouse::PostUpdate()
		{
			for (unsigned int i = 0; i < sf::Mouse::ButtonCount; i++)
				m_axPrevButtonState[i] = m_axCurrentButtonState[i];
		}
	}
}