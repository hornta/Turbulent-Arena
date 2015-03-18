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

			template <typename T>
			T* AddChild();

		protected:
			std::vector<std::unique_ptr<BNode>> m_axChildren;
		};

		template <typename T>
		T* BCompositeNode::AddChild()
		{
			m_axChildren.emplace_back(std::make_unique<T>());
			T* xBack = static_cast<T*>(m_axChildren.back().get());
			xBack->AttachAgent(m_xAgent);
			return xBack;
		}
	}
}