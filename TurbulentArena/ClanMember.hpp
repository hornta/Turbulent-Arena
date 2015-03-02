#pragma once

#include "Object.hpp"
#include "CombatStats.hpp"
#include "MovementStats.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class Agent;
	}
	class Clan;
	class ClanMember : public Object, public sf::Drawable
	{
		friend class Clan;
	public:
		ClanMember();
		~ClanMember();

		virtual void initiate() = 0;
		virtual void update(float deltatime);
		void setBody(Physics::Body* body);
		void InitSteeringStats();

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		sf::Sprite* getSprite();

	protected:
		ai::Agent* m_xAgent;
		MovementStats m_MovementStats;
		CombatStats m_xCombatStats;
		Clan* m_clan;
		std::unique_ptr<sf::Sprite> m_sprite;
	};
}