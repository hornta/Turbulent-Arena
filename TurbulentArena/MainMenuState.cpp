//MainMenuState.cpp

#include "stdafx.h"
#include "MainMenuState.hpp"
#include "UIManager.hpp"
#include "AudioManager.hpp"
#include "UIButton.hpp"
#include "Settings.hpp"
#include "ServiceLocator.hpp"
#include "DrawManager.hpp"
#include "GameStateManager.hpp"
#include "PlayState.hpp"
#include "Keyboard.hpp"
#include "OptionsState.hpp"

namespace bjoernligan
{
	MainMenuState::MainMenuState(const std::string &p_sName, const bool &p_bExclusive)
		: GameState(p_sName, p_bExclusive)
		, m_xUIManager(nullptr)
		, m_xKeyboard(nullptr)
		, m_xAudioManager(nullptr)
	{

	}

	void MainMenuState::Enter()
	{
		std::cout << "Entered MainMenuState." << std::endl;

		m_xUIManager = ServiceLocator<UIManager>::GetService();
		m_xKeyboard = ServiceLocator<input::Keyboard>::GetService();
		m_xAudioManager = ServiceLocator<system::AudioManager>::GetService();
		m_xAudioManager->PlayMusic("Menu");

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

		UIButton* xButton = static_cast<UIButton*>(m_xUIManager->AddElement<UIButton>("MainMenu", 1.0f));
		xButton->Initialize("Play", sf::IntRect((Settings::m_xWindowSize.x / 2) - xButtonSpacing.iButtonWidth / 2,
			xButtonSpacing.iStart, xButtonSpacing.iButtonWidth, xButtonSpacing.iButtonHeight),
			std::bind(&bjoernligan::MainMenuState::Play, this, std::placeholders::_1));

		xButton = static_cast<UIButton*>(m_xUIManager->AddElement<UIButton>("MainMenu", 1.0f));
		xButton->Initialize("Options", sf::IntRect((Settings::m_xWindowSize.x / 2) - xButtonSpacing.iButtonWidth / 2,
			xButtonSpacing.iStart + xButtonSpacing.iButtonHeight + xButtonSpacing.iSpacing,
			xButtonSpacing.iButtonWidth, xButtonSpacing.iButtonHeight),
			std::bind(&bjoernligan::MainMenuState::Options, this, std::placeholders::_1));

		xButton = static_cast<UIButton*>(m_xUIManager->AddElement<UIButton>("MainMenu", 1.0f));
		xButton->Initialize("Quit", sf::IntRect((Settings::m_xWindowSize.x / 2) - xButtonSpacing.iButtonWidth / 2,
			xButtonSpacing.iStart + (xButtonSpacing.iButtonHeight + xButtonSpacing.iSpacing) * 2,
			xButtonSpacing.iButtonWidth, xButtonSpacing.iButtonHeight),
			std::bind(&bjoernligan::MainMenuState::Quit, this, std::placeholders::_1));
	}

	void MainMenuState::Exit()
	{
		m_xUIManager->RemoveElementsByLabel("MainMenu");
	}

	bool MainMenuState::Update(const float &p_fDeltaTime)
	{
		p_fDeltaTime;
	
		if (m_xKeyboard->IsDownOnce(sf::Keyboard::Escape))
			m_xDrawManager->getWindow()->close();

		return true;
	}

	void MainMenuState::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		target, states;
	}

	void MainMenuState::Play(const bool &p_bButtonValue)
	{
		p_bButtonValue;
		m_bDeleteMe = true;
		m_xAudioManager->StopAllMusic();
		m_xStateMngr->CreateState<PlayState>("PlayState");
	}

	void MainMenuState::Options(const bool &p_bButtonValue)
	{
		p_bButtonValue;
		m_bDeleteMe = true;
		m_xStateMngr->CreateState<OptionsState>("OptionsState");
	}

	void MainMenuState::Quit(const bool &p_bButtonValue)
	{
		p_bButtonValue;
		m_bDeleteMe = true;
		m_xDrawManager->getWindow()->close();
	}
}