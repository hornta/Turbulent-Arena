//GameplayClass.cpp

#include "stdafx.h"
#include "Class.hpp"

namespace bjoernligan
{
	Class::Class()
	{

	}

	Class::~Class()
	{

	}

	void Class::setClan(Clan* p_clan)
	{
		m_clan = p_clan;
	}

	void Class::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_sprite, states);
	}

	sf::Sprite& Class::getSprite()
	{
		return m_sprite;
	}
}