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
		p_fDeltaTime;

		for (uint32_t i = 0; i < m_xBridges.size(); ++i)
		{
			if (m_xBridges[i]->HasNewValue())
			{
				float fValue = m_xBridges[i]->GetNewValue();

				if (m_xBridges[i]->GetLabel() == "Social")
				{
					for (uint32_t k = 0; k < m_clanMembers.size(); ++k)
					{
						//m_members[k]->SetSocial(fValue);
					}
				}
				else if (m_xBridges[i]->GetLabel() == "Brave")
				{
					for (uint32_t k = 0; k < m_clanMembers.size(); ++k)
					{
						//m_members[k]->SetBrave(fValue);
					}
				}
				else if (m_xBridges[i]->GetLabel() == "Agression")
				{
					for (uint32_t k = 0; k < m_clanMembers.size(); ++k)
					{
						//m_members[k]->SetAgression(fValue);
					}
				}

				//temp for compile:
				fValue;
			}
		}

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

	void Clan::AddSliderBridge(SliderBridge* p_xBridge)
	{
		m_xBridges.push_back(p_xBridge);
	}
}