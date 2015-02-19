//AISubject.hpp

/*The class who will notify changes will inherit from this class*/

#pragma once
#include <vector>

namespace bjoernligan
{
	namespace ai
	{
		class AIObserver;

		class AISubject
		{
		public:
			~AISubject();

			void AddAIObserver(AIObserver* p_NewObserver);
			void RemoveAIObserver(AIObserver* p_Observer);

		protected:
			void Notify(/*Add parameters here. remember to add them to observer also*/);

		private:
			std::vector<AIObserver*> m_AIObserverList;
		};
	}
}