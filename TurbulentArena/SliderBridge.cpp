//SliderBridge.cpp

#include "stdafx.h"
#include "SliderBridge.hpp"

namespace bjoernligan
{
	SliderBridge::SliderBridge(const std::string &p_sLabel)
		: m_sLabel(p_sLabel)
	{
		m_bNewValue = false;
	}

	SliderBridge::Ptr SliderBridge::Create(const std::string &p_sLabel)
	{
		return Ptr(new SliderBridge(p_sLabel));
	}

	SliderBridge::~SliderBridge()
	{

	}

	const std::string SliderBridge::GetLabel() const
	{
		return m_sLabel;
	}

	void SliderBridge::SetNewValue(const float &p_fNew)
	{
		m_fValue = p_fNew;
		m_bNewValue = true;
	}

	bool SliderBridge::HasNewValue()
	{
		return m_bNewValue;
	}

	float SliderBridge::GetNewValue()
	{
		m_bNewValue = false;
		return m_fValue;
	}
}