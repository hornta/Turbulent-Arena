//Scout.hpp

#pragma once
#include "Class.hpp"

namespace bjoernligan
{
	class Scout : public Class
	{
	public:
		Scout();
		~Scout();

		virtual void Initiate();
		virtual void Update(const float &p_fDeltaTime);
	};
}