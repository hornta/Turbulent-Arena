//PlayState.hpp

#pragma once
#include "GameState.hpp"
#include "Pathfinder.hpp"
#include "Visibility.hpp"

namespace bjoernligan
{
	class UIManager;
	class Utility;
	class DebugWindow;
	class Map;
	class ClanManager;
	class Physics;

	namespace input
	{
		class Keyboard;
		class Mouse;
	}

	namespace system
	{
		class SpriteManager;
	}

	class PlayState : public GameState
	{
	public:
		PlayState(const std::string &p_sName, const bool &p_bExclusive = false);

		void Enter();
		void Exit();
		bool Update(const float &p_fDeltaTime);
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		void updateCamera(const float &p_fDeltaTime);
		void SetDebugMode(const bool &p_bValue);
		void SetScrollSpeed(const float &p_fNewSpeed);

		//from engine
		system::SpriteManager* m_xSpriteManager;
		UIManager* m_xUIManager;
		input::Mouse* m_xMouse;
		input::Keyboard* m_xKeyboard;
		Utility* m_xUtility;
		DebugWindow* m_xDebugWindow;

		//play state specific
		std::unique_ptr<Map> m_map;
		std::unique_ptr<ClanManager> m_clanManager;
		std::unique_ptr<Physics> m_physics;
		std::unique_ptr<Pathfinder> m_pathFinder;
		std::unique_ptr<Visibility> m_visibility;

		float m_fScrollSpeed;
		sf::View m_view;
		sf::Vector2f m_lastRightClick;
	};
}