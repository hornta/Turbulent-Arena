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

			void SetTo(const sf::Vector2i &p_xTo);

			EBNodeStatus Process();

		private:
			sf::Vector2i m_xTo;
		};
	}
}