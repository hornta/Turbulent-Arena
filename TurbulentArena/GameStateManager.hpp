//GameStateManager.hpp

#pragma once
#include "GameState.hpp"

namespace bjoernligan
{
	namespace system
	{
		class DrawManager;

		class GameStateManager : public sf::Drawable
		{
		private:
			GameStateManager();
			GameStateManager(const GameStateManager&);
			GameStateManager& operator=(const GameStateManager&);

		public:
			typedef std::unique_ptr<GameStateManager> Ptr;
			static Ptr Create();

			bool Initialize();
			void UpdateStates(const float &p_fDeltaTime);
			void draw(sf::RenderTarget &target, sf::RenderStates states) const;
			void PostUpdate();

			void PopState();
			void RemoveStateByName(const std::string p_sStateName);
			void RemoveStateByIndex(const int &p_iIndex);

			void ObscureStates(const int &p_iStateIndex);

			template<class T>
			T* CreateState(const std::string &p_sName, const bool &p_bExclusive = false)
			{
				m_axStates.emplace_back(std::make_unique<T>(p_sName, p_bExclusive));
				m_axStates.back()->Enter();
				return static_cast<T*>(m_axStates.back().get());
			};

		private:
			std::vector<std::unique_ptr<GameState>> m_axStates;
			system::DrawManager* m_xDrawManager;
		};
	}
}