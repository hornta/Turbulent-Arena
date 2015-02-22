#include "stdafx.h"
#include "ClanMember.hpp"

namespace bjoernligan
{
	ClanMember::ClanMember()
	{

	}

	ClanMember::~ClanMember()
	{

	}

	void ClanMember::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_sprite, states);
	}

	sf::Sprite& ClanMember::getSprite()
	{
		return m_sprite;
	}
}