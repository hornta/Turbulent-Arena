//GameplayClass.hpp

#pragma once
#include "MovingObject.hpp"
#include "CombatStats.hpp"

namespace bjoernligan
{
	class GameplayClass : public MovingObject
	{
	public:
		GameplayClass();
		virtual ~GameplayClass();

		virtual void Initiate() = 0;

		CombatStats m_xCombatStats;
	};
}