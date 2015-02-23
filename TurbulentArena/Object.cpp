//Object.cpp

#include "stdafx.h"
#include "Object.hpp"

namespace bjoernligan
{
	Object::Object()
	{
		m_xPhysicsBody = nullptr;
	}

	Object::~Object()
	{
	}

	void Object::update(float deltatime)
	{
		deltatime;

		if (m_xPhysicsBody && m_xPhysicsBody->m_body)
		{
			m_xPos = vectorToPix(m_xPhysicsBody->m_body->GetPosition());
		}
	}

	void Object::setBody(Physics::Body* body)
	{
		m_xPhysicsBody = body;
		if (m_xPhysicsBody)
			m_xPhysicsBody->m_body->SetUserData(this);
	}

	Physics::Body* Object::getBody() const
	{
		return m_xPhysicsBody;
	}

}