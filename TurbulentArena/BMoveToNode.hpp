//BMoveToNode.hpp

#pragma once
#include "BNode.hpp"
#include <SFML\Graphics.hpp>

namespace bjoernligan
{
	namespace ai
	{
		class BMoveToNode : public BNode
		{
		public:
			BMoveToNode();
			~BMoveToNode();

			EBNodeStatus Process();
		};
	}
}