//BCompositeNode.cpp

#include "stdafx.h"
#include "BCompositeNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BCompositeNode::BCompositeNode()
		{

		}

		BCompositeNode::~BCompositeNode()
		{

		}

		void BCompositeNode::AddChild(BNode* p_xNode)
		{
			m_axChildren.push_back(p_xNode);
		}
		
		void BCompositeNode::CleanUp()
		{
			if (m_axChildren.empty())
				return;

			auto itr = m_axChildren.begin();
			while (itr != m_axChildren.end())
			{
				if ((*itr))
				{
					(*itr)->CleanUp();

					delete (*itr);
					(*itr) = nullptr;
				}
				++itr;
			}
		}
	}
}