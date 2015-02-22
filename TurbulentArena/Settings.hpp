//Settings.hpp

#pragma once
#include <string>
#include <SFML\Graphics.hpp>

namespace bjoernligan
{
	struct Settings
	{
		static sf::Vector2i m_xWindowSize;
		static std::string m_sSpritePath;

		static sf::Vector2i m_xSliderSize;
	};
}