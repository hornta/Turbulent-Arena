//AISubject.hpp

/*The object who will notify changes will inherit from this class*/

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
			void Notify(/*add parameters*/);

		private:
			std::vector<AIObserver*> m_AIObserverList;
		};
	}
}