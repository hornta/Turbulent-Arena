//PlayState.hpp

#pragma once
#include "GameState.hpp"
#include "Pathfinder.hpp"
#include "GameOverChecker.hpp"
#include "Timer.hpp"
#include "SelectionRect.hpp"
#include "ClanStats.hpp"

namespace bjoernligan
{
	class UIManager;
	class Utility;
	class DebugWindow;
	class Map;
	class ClanManager;
	class Physics;
	class UIBase;
	namespace ai
	{
		class Sense;
	}

	namespace input
	{
		class Keyboard;
		class Mouse;
	}

	namespace system
	{
		class AudioManager;
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
		void GameOverCheck(const float &p_fDeltaTime);
		void updateCamera(const float &p_fDeltaTime);
		void SetDebugMode(const bool &p_bValue);
		void ToggleDebugPathfinder(bool value);
		void SetScrollSpeed(const float &p_fNewSpeed);
		void SetGameSpeed(const float &p_fNewSpeed);
		void SetProgramOptionsActive(const bool &p_bValue);
		void SetDebugOptionsActive(const bool &p_bValue);

		void ToggleGamePaused();

		//from engine
		system::SpriteManager* m_xSpriteManager;
		system::AudioManager* m_xAudioManager;
		UIManager* m_xUIManager;
		input::Mouse* m_xMouse;
		input::Keyboard* m_xKeyboard;
		Utility* m_xUtility;
		DebugWindow* m_xDebugWindow;
		SelectionRect m_xSelectionRect;

		//play state specific
		std::unique_ptr<ClanStats> m_xClanStats;
		std::unique_ptr<Map> m_map;
		std::unique_ptr<ai::Sense> m_sense;
		std::unique_ptr<Physics> m_physics;
		std::unique_ptr<Pathfinder> m_pathFinder;
		std::unique_ptr<ClanManager> m_clanManager;
		Clan* m_xPlayerClan;

		float m_fScrollSpeed;
		sf::View m_view;
		sf::Vector2f m_lastRightClick;

		bool m_debugPathfinder = false;

		GameOverChecker m_xGameOverChecker;
		Timer m_xGameOverTimer;
		bool m_bGameOver;

		bool m_bGamePaused;
		float m_fGameSpeed;

		std::vector<UIBase*> m_axDebugOptions;
		std::vector<UIBase*> m_axProgramOptions;
	};
}