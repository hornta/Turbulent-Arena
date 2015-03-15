//Mood.cpp

#include "stdafx.h"
#include "Mood.hpp"

namespace bjoernligan
{
	namespace ai
	{
		void Mood::SetMood(const EMoodType &p_eMoodType, const float &p_fValue)
		{
			if (p_eMoodType == EMoodType::Count)
				return;
			m_fValues[p_eMoodType] = p_fValue;
		}

		float Mood::GetMoodValue(const EMoodType &p_eMoodType)
		{
			if (p_eMoodType == EMoodType::Count)
				return 0;
			return m_fValues[p_eMoodType];
		}
	}
}