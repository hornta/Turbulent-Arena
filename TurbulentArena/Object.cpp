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

		if (m_xPhysicsBody && m_xPhysicsBody->GetBody())
			SetPos(xUtility->ConvertVector_B2toSF(m_xPhysicsBody->GetBody()->GetPosition()), false);
	}


	void Object::SetPhysicsBody(PhysicsBody* p_xPhysicsBody)
	{
		m_xPhysicsBody = p_xPhysicsBody;
		if (m_xPhysicsBody)
			m_xPhysicsBody->GetBody()->SetUserData(this);
	}

	void Object::SetPos(const sf::Vector2f &p_xPos, const bool &p_bMoveBody)
	{
		m_xPos = p_xPos;

		if (p_bMoveBody && m_xPhysicsBody && m_xPhysicsBody->GetBody())
			m_xPhysicsBody->GetBody()->SetTransform(ServiceLocator<Utility>::GetService()->ConvertVector_SFtoB2(p_xPos), m_xPhysicsBody->GetBody()->GetAngle());
	}
}