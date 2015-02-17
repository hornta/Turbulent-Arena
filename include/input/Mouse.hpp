//Mouse.hpp

#pragma once
#include <SFML\Window.hpp>
#include <memory>

namespace bjoernligan
{
	namespace input
	{
		class Mouse
		{
		private:
			Mouse();
			Mouse(const Mouse&);
			Mouse& operator=(const Mouse&);

		public:
			typedef std::unique_ptr<Mouse> Ptr;
			static Ptr Create();

			~Mouse();

			bool IsDown(sf::Mouse::Button p_xButton);
			bool IsDownOnce(sf::Mouse::Button p_xButton);

			bool GetPrevious(sf::Mouse::Button p_xButton);

			void SetCurrent(sf::Mouse::Button p_xButton, bool p_bNewValue);
			void SetPrevious(sf::Mouse::Button p_xButton, bool p_bNewValue);

			void PostUpdate();

			sf::Vector2i m_xPos;

		private:
			bool m_axCurrentButtonState[sf::Mouse::ButtonCount];
			bool m_axPrevButtonState[sf::Mouse::ButtonCount];
		};
	}
}