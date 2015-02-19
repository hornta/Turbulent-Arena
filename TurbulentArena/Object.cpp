//Object.cpp

#include "stdafx.h"
#include "Object.hpp"

namespace bjoernligan
{
	Object::Object()
	{

	}

	Object::~Object()
	{

	}

	void Object::Update(const float &p_fDeltaTime)
	{
		p_fDeltaTime;
	}

	void Object::SetPos(const sf::Vector2f &p_xPos)
	{
		m_xPos = p_xPos;
	}
}