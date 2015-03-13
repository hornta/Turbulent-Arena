#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class CanSeeEnemies : public BNode
		{
		public:
			CanSeeEnemies();

			EBNodeStatus Process();
		};
	}
}