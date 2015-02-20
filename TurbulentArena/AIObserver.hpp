//AIObserver.hpp

/*The class who will listen to changes from subject will inherit from this class*/

#pragma once

namespace bjoernligan
{
	namespace ai
	{
		class AIObserver
		{
		public:
			~AIObserver() {};
			virtual void OnNotify(/*add parameters*/) = 0;
		};
	}
}