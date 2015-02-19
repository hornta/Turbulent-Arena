//Object.hpp

#pragma once
#include <SFML\Graphics.hpp>
#include <Box2D\box2d.h>

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
		sf::Sprite m_sprite;

		//Box2D
		b2CircleShape * m_xB2dCircleShape;
		b2Body* m_xB2dBody;
		b2Fixture* m_xB2dFixture;
	};
}