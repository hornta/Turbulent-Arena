#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class HasSomethingToReport : public BNode
		{
		public:
			HasSomethingToReport();

			EBNodeStatus Process();
		};
	}
}