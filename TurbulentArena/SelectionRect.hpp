//SelectionRect.hpp

#pragma once

namespace bjoernligan
{
	namespace input
	{
		class Mouse;
	}

	class Clan;

	class SelectionRect : public sf::Drawable
	{
		enum EState : int8_t
		{
			Idle,
			Pressed,
		} m_eState;

	public:
		SelectionRect();

		void SetTargetClan(Clan* p_xTargetClan);

		void Update();

		void Clear();
		void SetOffset(const sf::Vector2f &p_xOffset);

		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		sf::RectangleShape m_xShape;
		std::vector<int32_t> m_aiSelectedAgentIndexes;
		Clan* m_xTargetClan;
		input::Mouse* m_xMouse;
		sf::Vector2f m_xStartPos;
		sf::Vector2f m_xOffset;
	};
}