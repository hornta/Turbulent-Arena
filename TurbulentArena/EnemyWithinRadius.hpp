#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class EnemyWithinRadius : public BNode
		{
		public:
			EnemyWithinRadius();

			EBNodeStatus Process();
		};
	}
}