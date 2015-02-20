//PhysicsBody.hpp

#pragma once
#include <Box2D\Box2D.h>

namespace bjoernligan
{
	enum EB2ShapeType
	{
		NoShape,
		Box,
		Circle,
	};

	struct PhysicsParams
	{
		b2BodyDef m_xBodyDef;
		b2FixtureDef m_xFixtureDef;
		EB2ShapeType m_eShapeType;
		union
		{
			float m_fCircleRadius;
			struct Vec2i
			{
				int32_t x;
				int32_t y;
			} m_xBox;
		} m_xShapeSize;
	};

	class PhysicsBody
	{
	public:
		PhysicsBody();
		~PhysicsBody();

		void CreateBody(PhysicsParams p_xParams);

		b2Body* GetBody();

	private:
		b2Body* m_xB2dBody;
	};
}