//Object.hpp

#pragma once
#include "Physics.hpp"

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

		virtual void update(float deltatime);
		virtual void setBody(Physics::Body* body);
		Physics::Body* getBody() const;

	protected:
		sf::Vector2f m_xPos;

		//Box2D
		Physics::Body* m_xPhysicsBody;
	};
}