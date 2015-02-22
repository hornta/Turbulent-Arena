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

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		sf::Color getColor() const;

	private:
		sf::Color m_color;
		std::string m_name;
		
		std::vector<std::unique_ptr<ClanMember>> m_clanMembers;
	};

	template <typename T>
	ClanMember* Clan::createMember()
	{
		m_clanMembers.emplace_back(std::make_unique<T>());
		return m_clanMembers.back().get();
	}
}