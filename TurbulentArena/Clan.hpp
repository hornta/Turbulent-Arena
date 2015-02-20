#pragma once

namespace bjoernligan
{
	class Class;
	class Clan : public sf::Drawable
	{
	public:
		Clan(const sf::Color& p_color);
		~Clan();

		void addMember(Class* p_member);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		sf::Color getColor() const;

	private:
		sf::Color m_color;
		std::vector<Class*> m_members;
	};
}