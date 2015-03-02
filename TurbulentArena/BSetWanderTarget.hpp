//BMoveToNode.hpp

#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class BSetWanderTarget : public BNode
		{
		public:
			BSetWanderTarget();

			EBNodeStatus Process();
		};
	}
}