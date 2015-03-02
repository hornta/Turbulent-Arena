//GameState.hpp

#pragma once

namespace bjoernligan
{
	namespace system
	{
		class GameStateManager;
		class DrawManager;
	}

	class GameState : public sf::Drawable
	{
		friend system::GameStateManager;

	public:
		GameState(const std::string &p_sName, const bool &p_bExclusive = false);

		virtual void Enter() = 0;
		virtual void Exit() = 0;
		virtual bool Update(const float &p_fDeltaTime) = 0;
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;

		bool GetExclusive();
		bool GetObscured();
		virtual void Obscure();
		virtual void Reveal();

	protected:
		std::string m_sName;
		bool m_bDeleteMe;

		system::GameStateManager* m_xStateMngr;
		system::DrawManager* m_xDrawManager;

		bool m_bObscured;
		bool m_bExclusive;
	};
}