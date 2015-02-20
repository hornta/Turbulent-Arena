//PhysicsBody.hpp

#include "stdafx.h"
#include "PhysicsBody.hpp"
#include "ServiceLocator.hpp"
#include "Utility.hpp"

namespace bjoernligan
{
	PhysicsBody::PhysicsBody()
	{
		m_xB2dBody = nullptr;
	}

	PhysicsBody::~PhysicsBody()
	{
		if (m_xB2dBody)
		{
			ServiceLocator<b2World>::GetService()->DestroyBody(m_xB2dBody);
			m_xB2dBody = nullptr;
		}
	}

	void PhysicsBody::CreateBody(PhysicsParams p_xParams)
	{
		m_xB2dBody = ServiceLocator<b2World>::GetService()->CreateBody(&p_xParams.m_xBodyDef);
		m_xB2dBody->SetLinearDamping(0.6f);
		m_xB2dBody->SetAngularDamping(0.6f);

		Utility* xUtility = ServiceLocator<Utility>::GetService();

		b2PolygonShape xBoxShape;
		b2CircleShape xCircleShape;
		if (p_xParams.m_eShapeType == EB2ShapeType::Box)
		{
			xBoxShape.SetAsBox(
				xUtility->ConvertFloat_SFtoB2(p_xParams.m_xShapeSize.m_xBox.x / 2.0f) * 31.0f / 32.0f,
				xUtility->ConvertFloat_SFtoB2(p_xParams.m_xShapeSize.m_xBox.y / 2.0f * 31.0f / 32.0f));
			p_xParams.m_xFixtureDef.shape = &xBoxShape;
		}
		else if (p_xParams.m_eShapeType == EB2ShapeType::Circle)
		{
			xCircleShape.m_radius = xUtility->ConvertFloat_SFtoB2(p_xParams.m_xShapeSize.m_fCircleRadius);
			p_xParams.m_xFixtureDef.shape = &xCircleShape;
		}

		m_xB2dBody->CreateFixture(&p_xParams.m_xFixtureDef);
	}

	b2Body* PhysicsBody::GetBody()
	{
		return m_xB2dBody;
	}
}