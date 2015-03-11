#pragma once

namespace bjoernligan
{
	class Clan;
	class ClanManager : public sf::Drawable
	{
	public:
		ClanManager();
		~ClanManager();

		Clan* createClan(const std::string& name, const sf::Color &p_xTeamColor);
		std::size_t getAmount() const;

		void Update(const float &p_fDeltaTime);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		std::vector<Clan*> getClans() const;
	private:
		std::vector<std::unique_ptr<Clan>> m_clans;
	};
}