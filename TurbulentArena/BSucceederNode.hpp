//BSucceederNode.hpp

#pragma once
#include "BCompositeNode.hpp"

/*
Requires one and only one child
*/

namespace bjoernligan
{
	namespace ai
	{
		class BSucceederNode : public BCompositeNode
		{
		public:
			BSucceederNode();
			virtual ~BSucceederNode();

			virtual EBNodeStatus Process();
		};
	}
}