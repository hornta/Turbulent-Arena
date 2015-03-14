#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class BFleeFromEnemies : public BNode
		{
		public:
			BFleeFromEnemies();

			EBNodeStatus Process();
		};
	}
}