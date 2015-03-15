#pragma once

#include "Object.hpp"
#include "CombatStats.hpp"
#include "MovementStats.hpp"
#include "UserData.hpp"
#include "HealthBar.hpp"
#include "Agent.hpp"
#include "Mood.hpp"

namespace bjoernligan
{
	class Clan;
	namespace ai
	{
		class Sense;
	}
	class ClanMember : public Object, public sf::Drawable
	{
		friend class Clan;
	public:		
		enum EClass : uint8_t
		{
			Invalid,
			Axeman,
			Scout,
		};

		ClanMember(ai::Sense* sense, const sf::Color &p_xTeamColor, Clan* p_xClan);
		~ClanMember();

		virtual void initiate() = 0;
		virtual void update(float deltatime);
		void setBody(Physics::Body* body);
		void InitSteeringStats();

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void drawHpBar(sf::RenderTarget& target, sf::RenderStates states) const;
		sf::Sprite* getSprite();
		ai::Agent* getAgent() const;
		void drawPathfinder(bool value);
		bool IsFriend(ClanMember* p_xMember);
		Clan* GetClan();
		MovementStats& GetMovementStats();
		CombatStats* GetCombat();

		void SetMoodValue(const ai::Mood::EMoodType &p_eMoodType, const float &p_fValue);
		ai::Mood* GetMood();

		void SetSelection(const bool &p_bValue);

		const EClass &GetClass() const;

	protected:
		std::unique_ptr<ai::Agent> m_xAgent;
		MovementStats m_MovementStats;
		CombatStats m_xCombatStats;
		HealthBar m_xHealthBar;
		Clan* m_clan;
		std::unique_ptr<sf::Sprite> m_sprite;
		bool m_drawPathfinder;
		std::unique_ptr<B2UserData> m_userData;
		ai::Mood m_xMood;
		sf::RectangleShape m_xSelectionRect;
		EClass m_eClass;
	};
}