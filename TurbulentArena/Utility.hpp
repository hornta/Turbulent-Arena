//Utility

#pragma once
#include <memory>
#include <SFML\Graphics.hpp>
#include <Box2D\box2d.h>

namespace bjoernligan
{
	class Utility
	{
	private:
		Utility();
		Utility(const Utility&);
		Utility& operator=(const Utility&);

	public:
		~Utility();

		typedef std::unique_ptr<Utility> Ptr;
		static Ptr Create();

		float ConvertFloat_SFtoB2(const float &p_fPFloat);
		float ConvertFloat_B2toSF(const float &p_fMFloat);
		b2Vec2 ConvertVector_SFtoB2(const sf::Vector2f &p_xPVect);
		sf::Vector2f ConvertVector_B2toSF(const b2Vec2 &p_xMVect);

		float AngleRadToDeg(const float &p_fRadians);

	private:
		float m_fConversionBase;
		float m_fRatio_B2toSF;
		float m_fRatio_SFtoB2;
	};
}