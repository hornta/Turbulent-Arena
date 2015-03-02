//GameStateManager.cpp

#include "stdafx.h"
#include "GameStateManager.hpp"
#include "DrawManager.hpp"
#include "ServiceLocator.hpp"

namespace bjoernligan
{
	namespace system
	{
		GameStateManager::GameStateManager()
			: m_xDrawManager(nullptr)
		{

		}

		bool GameStateManager::Initialize()
		{
			m_xDrawManager = ServiceLocator<DrawManager>::GetService();

			if (!m_xDrawManager)
				return false;

			return true;
		}

		GameStateManager::Ptr GameStateManager::Create()
		{
			return Ptr(new GameStateManager);
		}

		void GameStateManager::UpdateStates(const float &p_fDeltaTime)
		{
			if (m_axStates.size() > 0)
			{
				for (int i = m_axStates.size() - 1; i >= 0; --i)
				{
					if (m_axStates[i]->GetObscured())
					{
						m_axStates[i]->Reveal();
					}

					m_axStates[i]->Update(p_fDeltaTime);
					if (m_axStates[i]->GetExclusive())
					{
						ObscureStates(i);
						break;
					}
				}
			}
		}

		void GameStateManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
		{
			if (m_axStates.empty())
				return;

			for (unsigned int i = 0; i < m_axStates.size(); ++i)
			{
				m_axStates[i]->draw(target, states);
			}
		}

		void GameStateManager::PostUpdate()
		{
			if (m_axStates.empty())
				return;

			for (int i = m_axStates.size() - 1; i >= 0; --i)
			{
				if (m_axStates[i]->m_bDeleteMe)
				{
					RemoveStateByIndex(i);
				}
			}

		}

		void GameStateManager::PopState()
		{
			if (m_axStates.empty())
				return;

			m_axStates.back()->Exit();
			m_axStates.pop_back();
		}

		void GameStateManager::RemoveStateByName(const std::string p_sStateName)
		{
			for (unsigned int i = 0; i < m_axStates.size(); ++i)
			{
				if (m_axStates[i]->m_sName == p_sStateName)
				{
					RemoveStateByIndex(i);
					break;
				}
			}
		}

		void GameStateManager::RemoveStateByIndex(const int &p_iIndex)
		{
			m_axStates.erase(m_axStates.begin() + p_iIndex);
		}

		void GameStateManager::ObscureStates(const int &p_iStateIndex)
		{
			for (int i = p_iStateIndex - 1; i >= 0; --i)
			{
				if (!m_axStates[i]->GetObscured())
				{
					m_axStates[i]->Obscure();
				}
			}
		}
	}
}