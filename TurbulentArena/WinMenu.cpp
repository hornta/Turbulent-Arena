//MainMenuState.cpp

#include "stdafx.h"
#include "WinMenu.hpp"
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
#include "Settings.hpp"

namespace bjoernligan
{
	WinMenu::WinMenu(const std::string &p_sName, const bool &p_bExclusive)
		: GameState(p_sName, true)
		, m_xUIManager(nullptr)
		, m_xKeyboard(nullptr)
		, m_xAudioManager(nullptr)
	{
		p_bExclusive;
	}

	void WinMenu::Enter()
	{
		std::cout << "Entered WinState." << std::endl;

		m_xUIManager = ServiceLocator<UIManager>::GetService();
		m_xKeyboard = ServiceLocator<input::Keyboard>::GetService();
		m_xAudioManager = ServiceLocator<system::AudioManager>::GetService();

		m_xAudioManager->PlayMusic("Victory");

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

		UIButton* xButton = static_cast<UIButton*>(m_xUIManager->AddElement<UIButton>("WinMenu", 1.0f));
		xButton->Initialize("Okay", sf::IntRect((Settings::m_xWindowSize.x / 2) - xButtonSpacing.iButtonWidth / 2,
			xButtonSpacing.iStart, xButtonSpacing.iButtonWidth, xButtonSpacing.iButtonHeight),
			std::bind(&bjoernligan::WinMenu::Okay, this, std::placeholders::_1));
		xButton->SetPos(sf::Vector2f(Settings::m_xWindowSize / 2) + sf::Vector2f(0, (float)xButtonSpacing.iButtonHeight + xButtonSpacing.iSpacing));
		
		m_xMenuBg.setSize(sf::Vector2f(Settings::m_xWindowSize / 2));
		m_xMenuBg.setOrigin(sf::Vector2f(Settings::m_xWindowSize / 4));
		m_xMenuBg.setPosition(sf::Vector2f(Settings::m_xWindowSize / 2));
		m_xMenuBg.setFillColor(sf::Color(150,150,150,100));
		m_xMenuBg.setOutlineThickness(1);
		m_xMenuBg.setOutlineColor((sf::Color(0, 0, 0, 100)));
	}

	void WinMenu::Exit()
	{
		m_xUIManager->RemoveElementsByLabel("WinMenu");
		m_xUIManager->AddText("GameOver", "Returning to main menu in: x seconds", "PlayState", sf::Vector2f(Settings::m_xWindowSize / 2));
	}

	bool WinMenu::Update(const float &p_fDeltaTime)
	{
		p_fDeltaTime;

		if (m_xKeyboard->IsDownOnce(sf::Keyboard::Escape))
			m_bDeleteMe = true;

		return true;
	}

	void WinMenu::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		m_xDrawManager->getWindow()->setView(m_xUIManager->getView());
		target.draw(m_xMenuBg, states);
	}

	void WinMenu::SetWinningTeam(const std::string &p_sWinningTeam)
	{
		std::string sWinbox("The winning clan is: ");
		sWinbox += p_sWinningTeam;
		m_xUIManager->AddText("Winning", sWinbox, "WinMenu", sf::Vector2f((float)Settings::m_xWindowSize.x / 2, (float)Settings::m_xWindowSize.y / 3));
	}

	void WinMenu::Okay(const bool &p_bButtonValue)
	{
		p_bButtonValue;
		m_bDeleteMe = true;
	}
}