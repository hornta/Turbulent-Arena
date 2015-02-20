//Axeman.hpp

#pragma once
#include "GameplayClass.hpp"

namespace bjoernligan
{
	class Axeman : public GameplayClass
	{
	public:
		Axeman();
		~Axeman();

		virtual void Initiate();
		virtual void Update(const float &p_fDeltaTime);
	};
}