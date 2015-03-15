#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class DetectEnemies : public BNode
		{
		public:
			DetectEnemies();

			EBNodeStatus Process();
		};
	}
}