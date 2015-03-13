#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class BDamageTarget : public BNode
		{
		public:
			BDamageTarget();

			EBNodeStatus Process();
		};
	}
}