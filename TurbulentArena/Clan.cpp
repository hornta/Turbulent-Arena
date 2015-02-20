#include "stdafx.h"
#include "Clan.hpp"
#include "Class.hpp"

#include "Scout.hpp"
#include "Axeman.hpp"

namespace bjoernligan
{
	Clan::Clan(const std::string& name)
		: m_name(name)
	{
	}

	Clan::~Clan()
	{
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