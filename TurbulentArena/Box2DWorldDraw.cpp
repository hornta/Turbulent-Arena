#include "stdafx.h"
#include <SFML\Graphics.hpp>

#include "Box2DWorldDraw.h"
#include "ServiceLocator.hpp"
#include "Utility.hpp"

namespace bjoernligan
{
	Box2DWorldDraw::Box2DWorldDraw(sf::RenderWindow* window)
	{
		m_window = window;
	}

	Box2DWorldDraw::~Box2DWorldDraw()
	{

	}

	void Box2DWorldDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		sf::ConvexShape polygon(vertexCount);
		sf::Vector2f center;
		for (int i = 0; i < vertexCount; i++)
		{
			//polygon.setPoint(i, SFMLDraw::B2VecToSFVec(vertices[i]));
			sf::Vector2f transformedVec = ServiceLocator<bjoernligan::Utility>::GetService()->ConvertVector_B2toSF(vertices[i]);
			polygon.setPoint(i, sf::Vector2f(std::floor(transformedVec.x), std::floor(transformedVec.y))); // flooring the coords to fix distorted lines on flat surfaces
		}																								   // they still show up though.. but less frequently
		polygon.setOutlineThickness(-1.f);
		polygon.setFillColor(sf::Color::Transparent);
		polygon.setOutlineColor(Box2DWorldDraw::GLColorToSFML(color));

		m_window->draw(polygon);
	}
	void Box2DWorldDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		sf::ConvexShape polygon(vertexCount);
		for (int i = 0; i < vertexCount; i++)
		{
			//polygon.setPoint(i, SFMLDraw::B2VecToSFVec(vertices[i]));
			sf::Vector2f transformedVec = ServiceLocator<bjoernligan::Utility>::GetService()->ConvertVector_B2toSF(vertices[i]);
			polygon.setPoint(i, sf::Vector2f(std::floor(transformedVec.x), std::floor(transformedVec.y))); // flooring the coords to fix distorted lines on flat surfaces
		}																								   // they still show up though.. but less frequently
		polygon.setOutlineThickness(-1.f);
		polygon.setFillColor(Box2DWorldDraw::GLColorToSFML(color, 60));
		polygon.setOutlineColor(Box2DWorldDraw::GLColorToSFML(color));

		m_window->draw(polygon);
	}
	void Box2DWorldDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
		sf::CircleShape circle(ServiceLocator<bjoernligan::Utility>::GetService()->ConvertFloat_B2toSF(radius));
		circle.setOrigin(ServiceLocator<bjoernligan::Utility>::GetService()->ConvertFloat_B2toSF(radius), ServiceLocator<bjoernligan::Utility>::GetService()->ConvertFloat_B2toSF(radius));
		circle.setPosition(ServiceLocator<bjoernligan::Utility>::GetService()->ConvertVector_B2toSF(center));
		circle.setFillColor(sf::Color::Transparent);
		circle.setOutlineThickness(-1.f);
		circle.setOutlineColor(Box2DWorldDraw::GLColorToSFML(color));

		m_window->draw(circle);
	}
	void Box2DWorldDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		sf::CircleShape circle(ServiceLocator<bjoernligan::Utility>::GetService()->ConvertFloat_B2toSF(radius));
		circle.setOrigin(ServiceLocator<bjoernligan::Utility>::GetService()->ConvertFloat_B2toSF(radius), ServiceLocator<bjoernligan::Utility>::GetService()->ConvertFloat_B2toSF(radius));
		circle.setPosition(ServiceLocator<bjoernligan::Utility>::GetService()->ConvertVector_B2toSF(center));
		circle.setFillColor(Box2DWorldDraw::GLColorToSFML(color, 60));
		circle.setOutlineThickness(1.f);
		circle.setOutlineColor(Box2DWorldDraw::GLColorToSFML(color));

		b2Vec2 endPoint = center + radius * axis;
		sf::Vertex line[2] =
		{
			sf::Vertex(ServiceLocator<bjoernligan::Utility>::GetService()->ConvertVector_B2toSF(center), Box2DWorldDraw::GLColorToSFML(color)),
			sf::Vertex(ServiceLocator<bjoernligan::Utility>::GetService()->ConvertVector_B2toSF(endPoint), Box2DWorldDraw::GLColorToSFML(color)),
		};

		m_window->draw(circle);
		m_window->draw(line, 2, sf::Lines);
	}
	void Box2DWorldDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(ServiceLocator<bjoernligan::Utility>::GetService()->ConvertVector_B2toSF(p1), Box2DWorldDraw::GLColorToSFML(color)),
			sf::Vertex(ServiceLocator<bjoernligan::Utility>::GetService()->ConvertVector_B2toSF(p2), Box2DWorldDraw::GLColorToSFML(color))
		};

		m_window->draw(line, 2, sf::Lines);
	}
	void Box2DWorldDraw::DrawTransform(const b2Transform& xf)
	{
		float lineLength = 0.4f;

		b2Vec2 xAxis = xf.p + lineLength * xf.q.GetXAxis();
		sf::Vertex redLine[] =
		{
			sf::Vertex(ServiceLocator<bjoernligan::Utility>::GetService()->ConvertVector_B2toSF(xf.p), sf::Color::Red),
			sf::Vertex(ServiceLocator<bjoernligan::Utility>::GetService()->ConvertVector_B2toSF(xAxis), sf::Color::Red)
		};

		b2Vec2 yAxis = xf.p + lineLength * xf.q.GetYAxis();
		sf::Vertex greenLine[] =
		{
			sf::Vertex(ServiceLocator<bjoernligan::Utility>::GetService()->ConvertVector_B2toSF(xf.p), sf::Color::Green),
			sf::Vertex(ServiceLocator<bjoernligan::Utility>::GetService()->ConvertVector_B2toSF(yAxis), sf::Color::Green)
		};

		m_window->draw(redLine, 2, sf::Lines);
		m_window->draw(greenLine, 2, sf::Lines);
	}
}