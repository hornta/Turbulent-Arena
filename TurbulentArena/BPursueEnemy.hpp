#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class BPursueEnemy : public BNode
		{
		public:
			BPursueEnemy();
			
			EBNodeStatus Process();
		};
	}
}