//CombatStats.cpp

#include "stdafx.h"
#include "CombatStats.hpp"

namespace bjoernligan
{
	sf::Clock* CombatStats::getAttackTimer()
	{
		return &m_attackTimer;
	}

	float CombatStats::GetAttackCooldown() const
	{
		return m_attackCooldown;
	}

	void CombatStats::SetAttackCooldown(const float &cooldown)
	{
		m_attackCooldown = cooldown;
	}

	void CombatStats::Initiate(const int32_t &p_iDamage, const int32_t &p_iHealth)
	{
		Initiate(ceilf((float)p_iDamage), ceilf((float)p_iHealth));
	}

	void CombatStats::Initiate(const float &p_fDamage, const float &p_fHealth)
	{
		m_fDamage = p_fDamage;
		m_fMaxHealth = m_fCurrentHealth = p_fHealth;
		m_social = m_brave = m_agression = .5f;
	}

	void CombatStats::TakeDamage(const float &p_iValue)
	{
		m_fCurrentHealth -= p_iValue;
		if (m_fCurrentHealth < 0)
			m_fCurrentHealth = 0;
	}

	float CombatStats::GetDamage()
	{
		return m_fDamage;
	}

	bool CombatStats::Alive()
	{
		return m_fCurrentHealth > 0;
	}

	float CombatStats::GetHealthPercentage()
	{
		return (float)m_fCurrentHealth / m_fMaxHealth;
	}

	void CombatStats::setSocial(const float &val)
	{
		m_social = val;
	}

	void CombatStats::setBrave(const float &val)
	{
		m_brave = val;
	}

	void CombatStats::setAgression(const float & val)
	{
		m_agression = val;
	}

	const float &CombatStats::getSocial() const
	{
		return m_social;
	}

	const float &CombatStats::getBrave() const
	{
		return m_brave;
	}

	const float &CombatStats::getAgression() const
	{
		return m_agression;
	}
}