#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class HelpFriend : public BNode
		{
		public:
			HelpFriend();

			EBNodeStatus Process();
		};
	}
}