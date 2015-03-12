//Timer.hpp

#pragma once

namespace bjoernligan
{
	struct Timer
	{
		Timer(const float &p_fMax)
		{
			m_fMax = p_fMax;
			m_fCurrent = m_fMax;
		}

		/*- also resets the timer*/
		void SetNewMax(const float &p_fNew)
		{
			m_fMax = p_fNew;
			Reset();
		};

		void SetOneTimeMax(const float &p_fNew)
		{
			m_fCurrent = p_fNew;
		};

		void Update(const float &p_fDeltaTime)
		{
			m_fCurrent -= p_fDeltaTime;
			if (m_fCurrent <= 0.0f)
			{
				m_fCurrent = 0.0f;
			}
		}

		void Reset()
		{
			m_fCurrent = m_fMax;
		}

		void ForceDone()
		{
			m_fCurrent = 0.0f;
		}

		bool Done()
		{
			return m_fCurrent <= 0;
		}

		float GetPercentageDone()
		{
			return m_fCurrent / m_fMax;
		}

		int32_t GetSecondsLeft()
		{
			return (int32_t)ceilf(m_fCurrent);
		}

	private:
		float m_fCurrent, m_fMax;
	};
}