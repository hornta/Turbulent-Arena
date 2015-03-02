//GameState.cpp

#include "stdafx.h"
#include "GameState.hpp"
#include "ServiceLocator.hpp"
#include "GameStateManager.hpp"

namespace bjoernligan
{
	GameState::GameState(const std::string &p_sName, const bool &p_bExclusive)
		: m_xDrawManager(nullptr)
		, m_sName(p_sName)
		, m_bExclusive(p_bExclusive)
		, m_bDeleteMe(false)
		, m_bObscured(false)
	{
		m_xStateMngr = ServiceLocator<system::GameStateManager>::GetService();
	}

	bool GameState::GetExclusive()
	{
		return m_bExclusive;
	}

	bool GameState::GetObscured()
	{
		return m_bObscured;
	}

	void GameState::Obscure()
	{
		m_bObscured = true;
	}

	void GameState::Reveal()
	{
		m_bObscured = false;
	}
}