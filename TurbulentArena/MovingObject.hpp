//MovingObject.hpp

#pragma once
#include "Object.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class Agent;
	}

	class MovingObject : public Object
	{
	public:
		MovingObject();
		virtual ~MovingObject();

		virtual void Update(const float &p_fDeltaTime);
		void SetVelocity(const sf::Vector2f &p_xVel);

	protected:
		sf::Vector2f m_xVelocity;
		ai::Agent* m_xAgent;
	};
}