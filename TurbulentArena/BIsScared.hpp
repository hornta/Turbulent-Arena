#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class BIsScared : public BNode
		{
		public:
			BIsScared();

			EBNodeStatus Process();
		};
	}
}