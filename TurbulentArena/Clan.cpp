#include "stdafx.h"
#include "Clan.hpp"

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

	void Clan::Update(const float &p_fDeltaTime)
	{
		for (std::size_t i = 0; i < m_clanMembers.size(); ++i)
		{
			m_clanMembers[i]->update(p_fDeltaTime);
		}
	}

	void Clan::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (std::size_t i = 0; i < m_clanMembers.size(); ++i)
		{
			target.draw(*m_clanMembers[i], states);
		}
	}

	sf::Color Clan::getColor() const
	{
		return m_color;
	}

	std::vector<ClanMember*> Clan::getMembers() const
	{
		std::vector<ClanMember*> members;
		for (std::size_t i = 0; i < m_clanMembers.size(); ++i)
		{
			members.push_back(m_clanMembers[i].get());
		}
		return members;
	}

	void Clan::SetSocial(const float &p_fNewValue)
	{
		p_fNewValue; // <-- for compile

		for (uint32_t k = 0; k < m_clanMembers.size(); ++k)
		{
			//m_members[k]->SetSocial(p_fNewValue);
		}
	}

	void Clan::SetBrave(const float &p_fNewValue)
	{
		p_fNewValue; // <-- for compile

		for (uint32_t k = 0; k < m_clanMembers.size(); ++k)
		{
			//m_members[k]->SetSocial(p_fNewValue);
		}
	}
	void Clan::SetAgression(const float &p_fNewValue)
	{
		p_fNewValue; // <-- for compile

		for (uint32_t k = 0; k < m_clanMembers.size(); ++k)
		{
			//m_members[k]->SetSocial(p_fNewValue);
		}
	}
}