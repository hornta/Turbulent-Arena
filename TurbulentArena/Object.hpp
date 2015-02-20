//Object.hpp

#pragma once
#include <SFML\Graphics.hpp>
#include <Box2D\box2d.h>
#include "PhysicsBody.hpp"

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
		void SetPhysicsBody(PhysicsBody* p_xPhysicsBody);

	protected:
		sf::Vector2f m_xPos;

		//Box2D
		PhysicsBody* m_xPhysicsBody;
	};
}