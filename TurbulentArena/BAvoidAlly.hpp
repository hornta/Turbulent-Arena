#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class BAvoidAlly : public BNode
		{
		public:
			BAvoidAlly();

			EBNodeStatus Process();
		};
	}
}