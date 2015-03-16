//CombatStats.hpp

#pragma once
#include <memory>

namespace bjoernligan
{
	struct CombatStats
	{
	private:
		float m_fDamage, m_fMaxHealth, m_fCurrentHealth;
		sf::Clock m_attackTimer;
		float m_attackCooldown;
		float m_social, m_brave, m_agression;

	public:
		sf::Clock* getAttackTimer();
		float GetAttackCooldown() const;
		void SetAttackCooldown(const float &cooldown);
		void Initiate(const int32_t &p_iDamage, const int32_t &p_iHealth);
		void Initiate(const float &p_fDamage, const float &p_fHealth);
		void TakeDamage(const float &p_iValue);
		float GetDamage();

		float GetHealthPercentage();
		bool Alive();

		void setSocial(const float &val);
		void setBrave(const float &val);
		void setAgression(const float &val);

		const float &getSocial() const;
		const float &getBrave() const;
		const float &getAgression() const;
	};
}