//Object.hpp

#pragma once
#include <SFML\Graphics.hpp>
#include <Box2D\box2d.h>

namespace bjoernligan
{
	class Object
	{
		friend class Engine;

	protected:
		enum EB2ShapeType
		{
			NoShape,
			Box,
			Circle,
		};

	public:
		Object();
		virtual ~Object();

		virtual void Update(const float &p_fDeltaTime);
		virtual void SetPos(const sf::Vector2f &p_xPos, const bool &p_bMoveBody);

		void CreateBody(const b2BodyType &p_eType);
		/*Sets shape as box*/
		virtual void CreateB2Shape(const sf::Vector2i &p_xSize);
		/*Sets shape as circle*/
		virtual void CreateB2Shape(const float &p_fRadius);

	//protected:
		void RemakeFixture();

		sf::Vector2f m_xPos;
		sf::Sprite m_sprite;

		//Box2D
		EB2ShapeType m_eB2ShapeType;
		union
		{
			b2PolygonShape* m_xPolyShape;
			b2CircleShape* m_xCircleShape;
		} m_xB2Shape;
		b2Body* m_xB2dBody;
		b2Fixture* m_xB2dFixture;
	};
}