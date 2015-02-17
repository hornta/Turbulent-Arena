//DrawManager.hpp

#pragma once

#include <memory>
#include "SFML\Graphics.hpp"

class InputManager;

namespace bjoernligan
{
	namespace system
	{
		class DrawManager
		{
		private:
			DrawManager();
			DrawManager(const DrawManager&);
			DrawManager& operator=(const DrawManager&);

		public:
			typedef std::unique_ptr<DrawManager> Ptr;
			static Ptr Create();

			~DrawManager();

			bool Initialize();

			void ClearScr();
			void Draw(sf::Drawable* p_xDrawTarget);
			void Display();

			sf::Font* GetFont();

			sf::RenderWindow* m_xWindow;

		private:
			sf::Font m_xFont;
		};
	}
}