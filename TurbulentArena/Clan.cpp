#include "stdafx.h"
#include "Clan.hpp"
#include "Class.hpp"

namespace bjoernligan
{
	Clan::Clan(const sf::Color& p_color)
		: m_color(p_color)
	{
	}


	Clan::~Clan()
	{
	}

	void Clan::addMember(Class* p_member)
	{
		p_member->setClan(this);
		m_members.push_back(p_member);
	}

	void Clan::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (std::size_t i = 0; i < m_members.size(); ++i)
		{
			target.draw(*m_members[i], states);
		}
	}

	sf::Color Clan::getColor() const
	{
		return m_color;
	}
}