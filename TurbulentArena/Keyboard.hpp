//Keyboard.hpp

#pragma once
#include <SFML\Window.hpp>
#include <memory>

namespace bjoernligan
{
	namespace input
	{
		class Keyboard
		{
		private:
			Keyboard();
			Keyboard(const Keyboard&);
			Keyboard& operator=(const Keyboard&);

		public:
			typedef std::unique_ptr<Keyboard> Ptr;
			static Ptr Create();

			~Keyboard();

			bool IsDown(sf::Keyboard::Key p_xKey);
			bool IsDownOnce(sf::Keyboard::Key p_xKey);

			bool GetPrevious(sf::Keyboard::Key p_xKey);

			void SetCurrent(sf::Keyboard::Key p_xKey, bool p_bNewValue);
			void SetPrevious(sf::Keyboard::Key p_xKey, bool p_bNewValue);

			void PostUpdate();

		private:
			bool m_axCurrentKeyState[sf::Keyboard::KeyCount];
			bool m_axPrevKeyState[sf::Keyboard::KeyCount];
		};
	}
}