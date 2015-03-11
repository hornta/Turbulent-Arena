#pragma once

#include "Object.hpp"
#include "CombatStats.hpp"
#include "MovementStats.hpp"
#include "UserData.hpp"

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
		void InitSteeringStats();

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		sf::Sprite* getSprite();
		ai::Agent* getAgent() const;
		void drawPathfinder(bool value);

	protected:
		ai::Agent* m_xAgent;
		MovementStats m_MovementStats;
		CombatStats m_xCombatStats;
		Clan* m_clan;
		std::unique_ptr<sf::Sprite> m_sprite;
		bool m_drawPathfinder;
		std::unique_ptr<B2UserData> m_userData;
	};
}