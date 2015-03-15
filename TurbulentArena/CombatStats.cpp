//CombatStats.cpp

#include "stdafx.h"
#include "CombatStats.hpp"

namespace bjoernligan
{
	int32_t m_iDamage;
	int32_t m_iMaxHealth;
	int32_t m_iCurrentHealth;

	sf::Clock* CombatStats::getAttackTimer()
	{
		return &m_attackTimer;
	}

	float CombatStats::GetAttackCooldown() const
	{
		return m_attackCooldown;
	}

	void CombatStats::SetAttackCooldown(float cooldown)
	{
		m_attackCooldown = cooldown;
	}

	void CombatStats::Initiate(const int32_t &p_iDamage, const int32_t &p_iHealth)
	{
		m_iDamage = p_iDamage;
		m_iMaxHealth = m_iCurrentHealth = p_iHealth;
		m_social = m_brave = m_agression = .5f;
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
		return m_iCurrentHealth > 0;
	}

	float CombatStats::GetHealthPercentage()
	{
		return (float)m_iCurrentHealth / m_iMaxHealth;
	}

	void CombatStats::setSocial(float val)
	{
		m_social = val;
	}

	void CombatStats::setBrave(float val)
	{
		m_brave = val;
	}

	void CombatStats::setAgression(float val)
	{
		m_agression = val;
	}

	float CombatStats::getSocial() const
	{
		return m_social;
	}

	float CombatStats::getBrave() const
	{
		return m_brave;
	}

	float CombatStats::getAgression() const
	{
		return m_agression;
	}
}