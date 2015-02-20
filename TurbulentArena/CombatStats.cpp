//CombatStats.cpp

#include "stdafx.h"
#include "CombatStats.hpp"

namespace bjoernligan
{
	int32_t m_iDamage;
	int32_t m_iMaxHealth;
	int32_t m_iCurrentHealth;

	void CombatStats::Initiate(const int32_t &p_iDamage, const int32_t &p_iHealth)
	{
		m_iDamage = p_iDamage;
		m_iMaxHealth = m_iCurrentHealth = p_iHealth;
	}

	void CombatStats::TakeDamage(const int32_t &p_iValue)
	{
		m_iCurrentHealth -= p_iValue;
		if (m_iCurrentHealth < 0)
			m_iCurrentHealth = 0;
	}

	int32_t CombatStats::GetDamage()
	{
		return m_iDamage;
	}

	bool CombatStats::Alive()
	{
		return m_iCurrentHealth <= 0;
	}
}