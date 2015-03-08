//BMoveToNode.hpp

#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class BFindTargetNode : public BNode
		{
		public:
			BFindTargetNode();

			EBNodeStatus Process();
		};
	}
}