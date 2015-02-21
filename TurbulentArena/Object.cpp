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
		m_xPhysicsBody = nullptr;
	}

	Object::~Object()
	{
		delete m_xPhysicsBody;
		m_xPhysicsBody = nullptr;
	}

	void Object::Update(const float &p_fDeltaTime)
	{
		p_fDeltaTime;

		Utility* xUtility = ServiceLocator<Utility>::GetService();

		if (m_xPhysicsBody && m_xPhysicsBody->m_body)
			SetPos(xUtility->ConvertVector_B2toSF(m_xPhysicsBody->m_body->GetPosition()), false);
	}


	void Object::SetPhysicsBody(Physics::Body* p_xPhysicsBody)
	{
		m_xPhysicsBody = p_xPhysicsBody;
		if (m_xPhysicsBody)
			m_xPhysicsBody->m_body->SetUserData(this);
	}

	void Object::SetPos(const sf::Vector2f &p_xPos, const bool &p_bMoveBody)
	{
		m_xPos = p_xPos;

		if (p_bMoveBody && m_xPhysicsBody && m_xPhysicsBody->m_body)
			m_xPhysicsBody->m_body->SetTransform(ServiceLocator<Utility>::GetService()->ConvertVector_SFtoB2(p_xPos), m_xPhysicsBody->m_body->GetAngle());
	}
}