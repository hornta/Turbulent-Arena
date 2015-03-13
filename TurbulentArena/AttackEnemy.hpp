#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class AttackEnemy : public BNode
		{
		public:
			AttackEnemy();

			EBNodeStatus Process();
		};
	}
}