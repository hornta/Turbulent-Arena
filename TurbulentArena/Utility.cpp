//Utility.cpp

#include "stdafx.h"
#include "Utility.hpp"

namespace bjoernligan
{
	Utility::Utility()
	{
		m_fConversionBase = 32.0f;
		m_fRatio_B2toSF = 1.0f * m_fConversionBase;
		m_fRatio_SFtoB2 = 1.0f / m_fConversionBase;
	}
	
	Utility::~Utility()
	{

	}

	Utility::Ptr Utility::Create()
	{
		return Ptr(new Utility);
	}

	float Utility::ConvertFloat_SFtoB2(const float &p_fPFloat)
	{
		return p_fPFloat * m_fRatio_SFtoB2;
	}

	float Utility::ConvertFloat_B2toSF(const float &p_fMFloat)
	{
		return p_fMFloat * m_fRatio_B2toSF;
	}

	b2Vec2 Utility::ConvertVector_SFtoB2(const sf::Vector2f &p_xPVect)
	{
		b2Vec2 xMVect(1.0f, 1.0f);
		xMVect.x = ConvertFloat_SFtoB2(p_xPVect.x);
		xMVect.y = ConvertFloat_SFtoB2(p_xPVect.y);

		return xMVect;
	}

	sf::Vector2f Utility::ConvertVector_B2toSF(const b2Vec2 &p_xMVect)
	{
		sf::Vector2f xPVect(1.0f, 1.0f);
		xPVect.x = ConvertFloat_B2toSF(p_xMVect.x);
		xPVect.y = ConvertFloat_B2toSF(p_xMVect.y);

		return xPVect;
	}

	float Utility::AngleRadToDeg(const float &p_fRadians)
	{
		float fResult = static_cast<float>(p_fRadians * 180.0 / b2_pi);
		return fResult;
	}
}