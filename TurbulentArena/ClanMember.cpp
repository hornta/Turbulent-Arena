#include "stdafx.h"
#include "ClanMember.hpp"

namespace bjoernligan
{
	ClanMember::ClanMember()
	{
		m_sprite = std::make_unique<sf::Sprite>();
	}

	void ClanMember::update(float deltatime)
	{
		Object::update(deltatime);
		m_sprite->setPosition(m_xPos);
	}

	void ClanMember::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(*m_sprite.get(), states);
	}

	sf::Sprite* ClanMember::getSprite()
	{
		return m_sprite.get();
	}
}