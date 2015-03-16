#include "stdafx.h"
#include "ProcessIncomingReports.h"
#include "Agent.hpp"
#include "Axeman.hpp"

namespace bjoernligan
{
	namespace ai
	{
		ProcessIncomingReports::ProcessIncomingReports()
		{

		}

		EBNodeStatus ProcessIncomingReports::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			Axeman* axeman = static_cast<Axeman*>(m_xAgent->getOwner());
			if (axeman->processReports())
			{
				return EBNodeStatus::Success;
			}
			else
			{
				return EBNodeStatus::Fail;
			}
		}
	}
}