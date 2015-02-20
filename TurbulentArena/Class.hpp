//GameplayClass.hpp

#pragma once
#include "CombatStats.hpp"
#include "Object.hpp"
#include "Agent.hpp"

namespace bjoernligan
{
	class Clan;
	class Class : public Object, public sf::Drawable
	{
	public:
		Class();
		virtual ~Class();

		virtual void Initiate() = 0;
		void setClan(Clan* p_clan);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		sf::Sprite& getSprite();

	protected:
		ai::Agent* m_xAgent;
		CombatStats m_xCombatStats;
		Clan* m_clan;
		sf::Sprite m_sprite;
	};
}