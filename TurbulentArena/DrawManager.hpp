//DrawManager.hpp

#pragma once

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

			bool Initialize();

			void ClearScr();
			void Draw(sf::Drawable* p_xDrawTarget);
			void Display();

			sf::Font* GetFont();
			sf::RenderWindow* getWindow() const;

		private:
			std::unique_ptr<sf::RenderWindow> m_xWindow;
			sf::Font m_xFont;
		};
	}
}