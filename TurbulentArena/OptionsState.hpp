//OptionsState.hpp

#pragma once
#include "GameState.hpp"

namespace bjoernligan
{
	namespace system
	{
		class AudioManager;
	}

	class UIManager;

	namespace input
	{
		class Keyboard;
		class Mouse;
	}

	class OptionsState : public GameState
	{
	public:
		OptionsState(const std::string &p_sName, const bool &p_bExclusive = false);

		void Enter();
		void Exit();
		bool Update(const float &p_fDeltaTime);
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		void Back(const bool &p_bButtonValue = true);

		system::AudioManager* m_xAudioManager;
		UIManager* m_xUIManager;
		input::Keyboard* m_xKeyboard;
	};
}