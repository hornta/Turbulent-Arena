#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class BWalkToTarget : public BNode
		{
		public:
			BWalkToTarget();

			EBNodeStatus Process();
		};
	}
}