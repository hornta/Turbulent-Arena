#pragma once

#include "Mood.hpp"

namespace bjoernligan
{
	class ClanMember;
	
	namespace ai
	{
		class Sense;
	}

	class Clan : public sf::Drawable
	{
	public:
		Clan(const std::string& name, const sf::Color &p_xTeamColor);

		template <typename T>
		ClanMember* createMember(ai::Sense* sense);

		void Update(const float &p_fDeltaTime);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void drawHpBars(sf::RenderTarget& target, sf::RenderStates states) const;
		sf::Color getColor() const;
		std::vector<ClanMember*> getMembers() const;
		bool IsAlive();

		void SetSocial(const float &p_fNewValue);
		void SetBrave(const float &p_fNewValue);
		void SetAgression(const float &p_fNewValue);
		void SetMoodValues(const ai::Mood::EMoodType &p_eMoodType, const float &p_fNewValue);

		const std::string &GetName() const;

		void SelectAgentsInRect(const sf::FloatRect &p_xShape);

		//temporary
		void DamageRandomMember(const float &p_iAmount);

	private:
		sf::Color m_color;
		std::string m_name;
		
		std::vector<std::unique_ptr<ClanMember>> m_clanMembers;
		std::vector<ClanMember*> m_selectedAgents;
	};

	template <typename T>
	ClanMember* Clan::createMember(ai::Sense* sense)
	{
		m_clanMembers.emplace_back(std::make_unique<T>(sense, m_color, this));
		m_clanMembers.back()->initiate();
		return m_clanMembers.back().get();
	}
}