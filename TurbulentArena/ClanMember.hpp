#pragma once

#include "Object.hpp"
#include "CombatStats.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class Agent;
	}
	class Clan;
	namespace ai
	{
		class Sense;
	}
	class ClanMember : public Object, public sf::Drawable
	{
		friend class Clan;
	public:
		ClanMember(ai::Sense* sense);
		~ClanMember();

		virtual void initiate() = 0;
		virtual void update(float deltatime);
		void setBody(Physics::Body* body);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		sf::Sprite* getSprite();
		ai::Agent* getAgent() const;

	protected:
		ai::Agent* m_xAgent;
		sf::Vector2f m_MaxVelocity;
		CombatStats m_xCombatStats;
		Clan* m_clan;
		std::unique_ptr<sf::Sprite> m_sprite;
	};
}