//ContactListener.hpp

#pragma once

#include <vector>
#include <Box2D\Box2D.h>

namespace bjoernligan
{
	class ContactListener : public b2ContactListener
	{
	public:
		void BeginContact(b2Contact* p_contact);
		void EndContact(b2Contact* p_contact);
	};
}