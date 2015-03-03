//OptionsState.cpp

#include "stdafx.h"
#include "OptionsState.hpp"
#include "UIManager.hpp"
#include "UIButton.hpp"
#include "Settings.hpp"
#include "ServiceLocator.hpp"
#include "DrawManager.hpp"
#include "GameStateManager.hpp"
#include "PlayState.hpp"
#include "Keyboard.hpp"
#include "MainMenuState.hpp"
#include "AudioManager.hpp"

namespace bjoernligan
{
	OptionsState::OptionsState(const std::string &p_sName, const bool &p_bExclusive)
		: GameState(p_sName, p_bExclusive)
		, m_xUIManager(nullptr)
		, m_xKeyboard(nullptr)
		, m_xAudioManager(nullptr)
	{

	}

	void OptionsState::Enter()
	{
		std::cout << "Entered OptionsState." << std::endl;

		m_xUIManager = ServiceLocator<UIManager>::GetService();
		m_xKeyboard = ServiceLocator<input::Keyboard>::GetService();
		m_xAudioManager = ServiceLocator<system::AudioManager>::GetService();

		struct ButtonSpacing
		{
			int32_t iStart;
			int32_t iButtonHeight;
			int32_t iButtonWidth;
			int32_t iSpacing;
		} xButtonSpacing =
		{
			96, //iStart
			64, //iButtonHeight
			200,//iButtonWidth
			5,  //iSpacing
		};
		
		UISlider::SliderDef xDef;
		xDef.m_sLabel = "Master volume";
		xDef.m_xFunction = std::bind(&bjoernligan::system::AudioManager::ChangeMasterVolume, m_xAudioManager, std::placeholders::_1);
		xDef.m_fWidth = 240.0f;
		xDef.m_fMin = 0.0f;
		xDef.m_fMax = 1.0f;
		xDef.m_fCurrent = m_xAudioManager->GetMasterVolume();
		xDef.m_bContinous = true;

		m_xUIManager->AddSlider("OptionsState", xDef, sf::Vector2f(((float)Settings::m_xWindowSize.x / 2.0f) - 240.0f / 2.0f, (float)xButtonSpacing.iStart), 1.0f);

		xDef.m_sLabel = "Music volume";
		xDef.m_xFunction = std::bind(&bjoernligan::system::AudioManager::ChangeMusicVolume, m_xAudioManager, std::placeholders::_1);
		xDef.m_fWidth = 240.0f;
		xDef.m_fMin = 0.0f;
		xDef.m_fMax = 100.0f;
		xDef.m_fCurrent = (float)m_xAudioManager->GetMusicVolume();

		m_xUIManager->AddSlider("OptionsState", xDef, sf::Vector2f(((float)Settings::m_xWindowSize.x / 2.0f) - 240.0f / 2.0f, (float)xButtonSpacing.iStart + 100), 1.0f);

		xDef.m_sLabel = "Sound effects volume";
		xDef.m_xFunction = std::bind(&bjoernligan::system::AudioManager::ChangeSoundVolume, m_xAudioManager, std::placeholders::_1);
		xDef.m_fWidth = 240.0f;
		xDef.m_fMin = 0.0f;
		xDef.m_fMax = 100.0f;
		xDef.m_fCurrent = (float)m_xAudioManager->GetSoundVolume();

		m_xUIManager->AddSlider("OptionsState", xDef, sf::Vector2f(((float)Settings::m_xWindowSize.x / 2.0f) - 240.0f / 2.0f, (float)xButtonSpacing.iStart + 200), 1.0f);

		UIButton* xButton = static_cast<UIButton*>(m_xUIManager->AddElement<UIButton>("OptionsState", 1.0f));
		xButton->Initialize("Back", sf::IntRect((Settings::m_xWindowSize.x / 2) - xButtonSpacing.iButtonWidth / 2,
			xButtonSpacing.iStart + 300, xButtonSpacing.iButtonWidth, xButtonSpacing.iButtonHeight),
			std::bind(&bjoernligan::OptionsState::Back, this, std::placeholders::_1));
	}

	void OptionsState::Exit()
	{
		m_xUIManager->RemoveElementsByLabel("OptionsState");
	}

	bool OptionsState::Update(const float &p_fDeltaTime)
	{
		p_fDeltaTime;

		if (m_xKeyboard->IsDownOnce(sf::Keyboard::Escape))
			Back();

		return true;
	}

	void OptionsState::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		target, states;
	}

	void OptionsState::Back(const bool &p_bButtonValue)
	{
		p_bButtonValue;
		m_bDeleteMe = true;
		m_xStateMngr->CreateState<MainMenuState>("MainMenu");
	}
}