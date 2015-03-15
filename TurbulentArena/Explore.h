#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class Explore : public BNode
		{
		public:
			Explore();

			EBNodeStatus Process();
		};
	}
}