//DebugWindow.hpp

#pragma once

namespace bjoernligan
{
	class DebugWindow : sf::Drawable
	{
	public:
		DebugWindow();
		~DebugWindow();

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void Update(const float &p_fDeltaTime);

	private:
		sf::Text m_xFps;
		std::unique_ptr<sf::RectangleShape> m_xBgRect;
	};
}