#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class GetPathToEnemy : public BNode
		{
		public:
			GetPathToEnemy();

			EBNodeStatus Process();
		};
	}
}