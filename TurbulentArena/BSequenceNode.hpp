//BSequenceNode.hpp

#pragma once
#include "BCompositeNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class BSequenceNode : public BCompositeNode
		{
		public:
			BSequenceNode();
			virtual ~BSequenceNode();

			virtual EBNodeStatus Process();
		};
	}
}