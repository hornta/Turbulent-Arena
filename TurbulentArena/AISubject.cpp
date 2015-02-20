//AISubject.cpp

#include "stdafx.h"
#include "AISubject.hpp"
#include "AIObserver.hpp"

namespace bjoernligan
{
	namespace ai
	{
		
		AISubject::~AISubject()
		{

		}

		void AISubject::AddAIObserver(AIObserver* p_NewObserver)
		{
			m_AIObserverList.push_back(p_NewObserver);
		}
		void AISubject::RemoveAIObserver(AIObserver* p_Observer)
		{
			for (unsigned int i = 0; i < m_AIObserverList.size(); i++)
			{
				if (m_AIObserverList[i] == p_Observer)
				{
					m_AIObserverList.erase(m_AIObserverList.begin() + i);
					return;
				}
			}
		}
		void AISubject::Notify(const AIEvent &p_xEvent)
		{
			for (unsigned int i = 0; i < m_AIObserverList.size(); i++)
			{
				m_AIObserverList[i]->OnNotify(p_xEvent);
			}
		}
	}
}