//BCompositeNode.hpp

#pragma once
#include "BNode.hpp"
#include <vector>

namespace bjoernligan
{
	namespace ai
	{
		class BCompositeNode : public BNode
		{
		public:
			BCompositeNode();
			virtual ~BCompositeNode();

			virtual EBNodeStatus Process() = 0;
			void AddChild(BNode* p_xNode);

			virtual void CleanUp();

		protected:
			std::vector<BNode*> m_axChildren;
		};
	}
}