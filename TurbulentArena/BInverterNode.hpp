//BInverterNode.hpp

#pragma once
#include "BCompositeNode.hpp"

/*
Requires one and only one child
*/

namespace bjoernligan
{
	namespace ai
	{
		class BInverterNode : public BCompositeNode
		{
		public:
			BInverterNode();
			virtual ~BInverterNode();

			virtual EBNodeStatus Process();
		};
	}
}