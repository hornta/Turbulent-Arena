//AIEvent.hpp

#pragma once
#include <memory>
#include <SFML\Graphics.hpp>

namespace bjoernligan
{
	namespace ai
	{
		enum AIEventType
		{
			MoveTarget,
		};

		struct AIEvent
		{
			AIEventType m_eType;

			union
			{
				struct Vec2i
				{
					int32_t x, y;
				} m_xVec2i;
				/*struct Vec2f
				{
					float x, y;
				} m_xVec2f;*/
			} m_xContent;
		};
	}
}