//DebugWindow.hpp

#pragma once
#include <sstream>

namespace bjoernligan
{
	class DebugWindow : public sf::Drawable
	{
	private:
		DebugWindow(const bool &p_bActive);
		DebugWindow(const DebugWindow&);
		DebugWindow& operator=(const DebugWindow&);

	public:
		typedef std::unique_ptr<DebugWindow> Ptr;
		static Ptr Create(const bool &p_bActive = false);

		bool Initialize();

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void Update(const float &p_fDeltaTime);
		void SetActive(const bool &p_bActive);
		void SetPos(const float &p_x, const float &p_y);
		void SetPos(const sf::Vector2f &p_xPos);

	private:
		bool m_bActive;
		sf::Text m_xFps;
		std::stringstream m_xSStream;
		std::unique_ptr<sf::RectangleShape> m_xBgRect;
	};
}