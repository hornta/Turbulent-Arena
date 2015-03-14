#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class BIsAllyTooClose : public BNode
		{
		public:
			BIsAllyTooClose();

			EBNodeStatus Process();
		};
	}
}