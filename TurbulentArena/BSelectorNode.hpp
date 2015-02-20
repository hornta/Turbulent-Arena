//BSelectorNode.hpp

#pragma once
#include "BCompositeNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class BSelectorNode : public BCompositeNode
		{
		public:
			BSelectorNode();
			virtual ~BSelectorNode();

			virtual EBNodeStatus Process();
		};
	}
}