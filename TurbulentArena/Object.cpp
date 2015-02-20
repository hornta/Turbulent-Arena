//Object.cpp

#include "stdafx.h"
#include "Object.hpp"
#include "ServiceLocator.hpp"
#include "Map.hpp"
#include "Utility.hpp"
#include <Box2D\Box2D.h>

namespace bjoernligan
{
	Object::Object()
	{
		m_eB2ShapeType = EB2ShapeType::NoShape;
		m_xB2Shape.m_xCircleShape = nullptr;
		m_xB2dBody = nullptr;
		m_xB2dFixture = nullptr;
	}

	Object::~Object()
	{
		ServiceLocator<b2World>::GetService()->DestroyBody(m_xB2dBody);
		m_xB2dBody = nullptr;

		if (m_xB2Shape.m_xPolyShape)
		{
			delete m_xB2Shape.m_xPolyShape;
			m_xB2Shape.m_xPolyShape = nullptr;
		}
	}

	void Object::Update(const float &p_fDeltaTime)
	{
		p_fDeltaTime;

		Utility* xUtility = ServiceLocator<Utility>::GetService();

		if (m_xB2dBody)
		{
			m_sprite.setRotation(xUtility->AngleRadToDeg(m_xB2dBody->GetAngle()));
			SetPos(xUtility->ConvertVector_B2toSF(m_xB2dBody->GetPosition()), false);
		}
	}

	void Object::SetPos(const sf::Vector2f &p_xPos, const bool &p_bMoveBody)
	{
		m_xPos = p_xPos;

		if (p_bMoveBody && m_xB2dBody)
			m_xB2dBody->SetTransform(ServiceLocator<Utility>::GetService()->ConvertVector_SFtoB2(p_xPos), m_xB2dBody->GetAngle());
	}

	void Object::CreateBody(const b2BodyType &p_eType)
	{
		b2BodyDef xBodyDef;

		xBodyDef.type = p_eType;
		xBodyDef.gravityScale = 1.0f;
		xBodyDef.fixedRotation = true;
		xBodyDef.bullet = false;

		m_xB2dBody = ServiceLocator<b2World>::GetService()->CreateBody(&xBodyDef);
		m_xB2dBody->SetUserData(this);
		m_xB2dBody->SetLinearDamping(0.6f);
		m_xB2dBody->SetAngularDamping(0.6f);
	}

	void Object::CreateB2Shape(const sf::Vector2i &p_xSize)
	{
		m_eB2ShapeType = EB2ShapeType::Box;
		Utility* xUtility = ServiceLocator<Utility>::GetService();

		if (m_xB2Shape.m_xPolyShape)
		{
			delete m_xB2Shape.m_xPolyShape;
			m_xB2Shape.m_xPolyShape = nullptr;
		}

		m_xB2Shape.m_xPolyShape = new b2PolygonShape;
		m_xB2Shape.m_xPolyShape->SetAsBox(xUtility->ConvertFloat_SFtoB2(p_xSize.x / 2.0f) * 31.0f / 32.0f, xUtility->ConvertFloat_SFtoB2(p_xSize.y / 2.0f * 31.0f / 32.0f));

		RemakeFixture();
	}

	void Object::CreateB2Shape(const float &p_fRadius)
	{
		m_eB2ShapeType = EB2ShapeType::Circle;
		Utility* xUtility = ServiceLocator<Utility>::GetService();

		if (m_xB2Shape.m_xCircleShape)
		{
			delete m_xB2Shape.m_xCircleShape;
			m_xB2Shape.m_xCircleShape = nullptr;
		}
		m_xB2Shape.m_xCircleShape = new b2CircleShape;
		m_xB2Shape.m_xCircleShape->m_radius = xUtility->ConvertFloat_SFtoB2(p_fRadius);

		RemakeFixture();
	}

	void Object::RemakeFixture()
	{
		b2FixtureDef xFixtureDef;
		if (m_xB2dFixture)
		{
			xFixtureDef.friction = m_xB2dFixture->GetFriction();
			xFixtureDef.density = m_xB2dFixture->GetDensity();
			xFixtureDef.restitution = m_xB2dFixture->GetRestitution();
			xFixtureDef.isSensor = m_xB2dFixture->IsSensor();
			m_xB2dBody->DestroyFixture(m_xB2dFixture);
			m_xB2dFixture = nullptr;
		}
		else
		{
			xFixtureDef.friction = 1.0f;
			xFixtureDef.density = 1.0f;
			xFixtureDef.restitution = 1.0f;
			xFixtureDef.isSensor = false;
		}

		if (m_eB2ShapeType == EB2ShapeType::Box)
			xFixtureDef.shape = m_xB2Shape.m_xPolyShape;
		else if (m_eB2ShapeType == EB2ShapeType::Circle)
			xFixtureDef.shape = m_xB2Shape.m_xCircleShape;

		m_xB2dFixture = m_xB2dBody->CreateFixture(&xFixtureDef);
	}
}