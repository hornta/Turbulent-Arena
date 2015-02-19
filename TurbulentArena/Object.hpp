//Object.hpp

#pragma once
#include <SFML\Graphics.hpp>

namespace bjoernligan
{
	class Object
	{
	public:
		Object();
		virtual ~Object();

		virtual void Update(const float &p_fDeltaTime);
		virtual void SetPos(const sf::Vector2f &p_xPos);

	protected:
		sf::Vector2f m_xPos;
	};
}