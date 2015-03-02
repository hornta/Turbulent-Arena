#pragma once

namespace bjoernligan
{
	class ClanMember;
	class Clan : public sf::Drawable
	{
	public:
		Clan(const std::string& name);
		~Clan();

		template <typename T>
		ClanMember* createMember();

		void Update(const float &p_fDeltaTime);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		sf::Color getColor() const;
		std::vector<ClanMember*> getMembers() const;

		void SetSocial(const float &p_fNewValue);
		void SetBrave(const float &p_fNewValue);
		void SetAgression(const float &p_fNewValue);

	private:
		sf::Color m_color;
		std::string m_name;
		
		std::vector<std::unique_ptr<ClanMember>> m_clanMembers;
	};

	template <typename T>
	ClanMember* Clan::createMember()
	{
		m_clanMembers.emplace_back(std::make_unique<T>());
		m_clanMembers.back()->initiate();
		return m_clanMembers.back().get();
	}
}