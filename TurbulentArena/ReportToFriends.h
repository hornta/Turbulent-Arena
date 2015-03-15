#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class ReportToFriends : public BNode
		{
		public:
			ReportToFriends();

			EBNodeStatus Process();
		};
	}
}