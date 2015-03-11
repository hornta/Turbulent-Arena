//CombatStats.hpp

#pragma once
#include <memory>

namespace bjoernligan
{
	struct CombatStats
	{
	private:
		int32_t m_iDamage;
		int32_t m_iMaxHealth;
		int32_t m_iCurrentHealth;

	public:
		void Initiate(const int32_t &p_iDamage, const int32_t &p_iHealth);
		void TakeDamage(const int32_t &p_iValue);
		int32_t GetDamage();
		bool Alive();
		float GetHealthPercentage();
	};
}