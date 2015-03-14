//Mood.hpp

#pragma once

namespace bjoernligan
{
	namespace ai
	{
		struct Mood
		{
			enum EMoodType
			{
				Social,
				Brave,
				Agression,
				Count,
			};

			float m_fValues[EMoodType::Count];

			void SetMood(const EMoodType &p_eMoodType, const float &p_fValue);
			float GetMoodValue(const EMoodType &p_eMoodType);
		};
	}
}