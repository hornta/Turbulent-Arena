#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class ProcessIncomingReports : public BNode
		{
		public:
			ProcessIncomingReports();

			EBNodeStatus Process();
		};
	}
}