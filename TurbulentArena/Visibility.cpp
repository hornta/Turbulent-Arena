#include "stdafx.h"
#include "Visibility.hpp"

bool sortEndPoints(const std::unique_ptr<bjoernligan::Visibility::EndPoint>& a, const std::unique_ptr<bjoernligan::Visibility::EndPoint>& b)
{
	if (a->getAngle() > b->getAngle())
		return false;
	if (a->getAngle() < b->getAngle())
		return true;

	if (!a->getBegin() && b->getBegin())
		return false;
	if (a->getBegin() && !b->getBegin())
		return true;
	return false;
}

namespace bjoernligan
{
	bool leftOf(Visibility::Segment *s, const sf::Vector2f& p)
	{
		sf::Vector2f a = s->getEndPointA()->getPosition();
		sf::Vector2f b = s->getEndPointB()->getPosition();
		float cross = (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
		return cross < 0;
	}

	sf::Vector2f interpolate(const sf::Vector2f& p, const sf::Vector2f& q, float f)
	{
		return sf::Vector2f(p.x * (1.f - f) + q.x * f, p.y * (1.f - f) + q.y * f);
	}

	sf::Vector2f linesIntersection(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3, const sf::Vector2f& p4)
	{
		float s = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x))
			/ ((p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y));
		return sf::Vector2f(p1.x + s * (p2.x - p1.x), p1.y + s * (p2.y - p1.y));
	}

	bool segment_in_front_of(Visibility::Segment *s0, Visibility::Segment *s1, const sf::Vector2f& relativeTo)
	{
		sf::Vector2f a = s1->getEndPointA()->getPosition();
		sf::Vector2f b = s1->getEndPointB()->getPosition();

		bool A1 = leftOf(s0, interpolate(sf::Vector2f(a.x, a.y), sf::Vector2f(b.x, b.y), 0.01f));
		bool A2 = leftOf(s0, interpolate(sf::Vector2f(b.x, b.y), sf::Vector2f(a.x, a.y), 0.01f));
		bool A3 = leftOf(s0, relativeTo);

		a = s0->getEndPointA()->getPosition();
		b = s0->getEndPointB()->getPosition();
		bool B1 = leftOf(s1, interpolate(sf::Vector2f(a.x, a.y), sf::Vector2f(b.x, b.y), 0.01f));
		bool B2 = leftOf(s1, interpolate(sf::Vector2f(b.x, b.y), sf::Vector2f(a.x, a.y), 0.01f));
		bool B3 = leftOf(s1, relativeTo);

		if (B1 == B2 && B2 != B3) return true;
		if (A1 == A2 && A2 == A3) return true;
		if (A1 == A2 && A2 != A3) return false;
		if (B1 == B2 && B2 == B3) return false;
		return false;
	}

	Visibility::EndPoint::EndPoint()
	{

	}

	sf::Vector2f Visibility::EndPoint::getPosition() const
	{
		return m_position;
	}

	float Visibility::EndPoint::getAngle() const
	{
		return m_angle;
	}

	Visibility::Segment* Visibility::EndPoint::getSegment() const
	{
		return m_segment;
	}

	bool Visibility::EndPoint::getBegin() const
	{
		return m_begin;
	}

	void Visibility::EndPoint::setPosition(const sf::Vector2f& position)
	{
		m_position = position;
	}

	void Visibility::EndPoint::setAngle(float angle)
	{
		m_angle = angle;
	}

	void Visibility::EndPoint::setSegment(Segment* segment)
	{
		m_segment = segment;
	}

	void Visibility::EndPoint::setBegin(bool begin)
	{
		m_begin = begin;
	}

	Visibility::Segment::Segment()
	{

	}

	Visibility::EndPoint* Visibility::Segment::getEndPointA() const
	{
		return m_a;
	}

	Visibility::EndPoint* Visibility::Segment::getEndPointB() const
	{
		return m_b;
	}

	float Visibility::Segment::getD() const
	{
		return m_d;
	}

	void Visibility::Segment::setD(float d)
	{
		m_d = d;
	}

	void Visibility::Segment::setEndPointA(EndPoint* a)
	{
		m_a = a;
	}

	void Visibility::Segment::setEndPointB(EndPoint* b)
	{
		m_b = b;
	}

	Visibility::Triangle::Triangle(const sf::Vector2f& p0, const sf::Vector2f& p1, const sf::Vector2f& p2)
		: m_p0(p0),
		m_p1(p1),
		m_p2(p2)
	{

	}

	sf::Vector2f Visibility::Triangle::getPoint0() const
	{
		return m_p0;
	}

	sf::Vector2f Visibility::Triangle::getPoint1() const
	{
		return m_p1;
	}

	sf::Vector2f Visibility::Triangle::getPoint2() const
	{
		return m_p2;
	}


	Visibility::PrepareSegment::PrepareSegment(const sf::Vector2f& a, const sf::Vector2f& b)
		: m_a(a),
		m_b(b)
	{

	}

	sf::Vector2f Visibility::PrepareSegment::getA() const
	{
		return m_a;
	}

	sf::Vector2f Visibility::PrepareSegment::getB() const
	{
		return m_b;
	}

	/* Light */
	Visibility::Light::Light(const sf::Color& color)
		: m_color(color)
	{
		m_vertices.setPrimitiveType(sf::PrimitiveType::TrianglesFan);
	}

	void Visibility::Light::setColor(const sf::Color color)
	{
		m_color = color;
	}

	void Visibility::Light::updateSegments()
	{
		sf::Vector2f position = getPosition();
		m_triangles.clear();
		m_vertices.clear();
		m_vertices.append(sf::Vertex(position, m_color, getOrigin()));

		for (std::size_t i = 0; i < m_segments.size(); ++i)
		{
			Segment* segment = m_segments[i].get();
			sf::Vector2f p0 = segment->getEndPointA()->getPosition();
			sf::Vector2f p1 = segment->getEndPointB()->getPosition();

			float dx = 0.5f * (p0.x + p1.x) - position.x;
			float dy = 0.5f * (p0.y + p1.y) - position.y;

			segment->setD(dx * dx + dy * dy);

			segment->getEndPointA()->setAngle(atan2f(p0.y - position.y, p0.x - position.x));
			segment->getEndPointB()->setAngle(atan2f(p1.y - position.y, p1.x - position.x));

			float dAngle = segment->getEndPointB()->getAngle() - segment->getEndPointA()->getAngle();
			if (dAngle <= -PI)
				dAngle += 2.f * PI;
			if (dAngle > PI)
				dAngle -= 2.f * PI;

			segment->getEndPointA()->setBegin(dAngle > 0.f);
			segment->getEndPointB()->setBegin(!segment->getEndPointA()->getBegin());
		}
	}

	void Visibility::Light::sortEndPoints()
	{
		std::sort(m_endPoints.begin(), m_endPoints.end(), ::sortEndPoints);
	}

	void Visibility::Light::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.blendMode = sf::BlendAdd;
		target.draw(m_vertices, states);
	}

	void Visibility::Light::addSegment(const PrepareSegment& segment)
	{
		m_segments.emplace_back(std::make_unique<Segment>());
		Segment* s = m_segments.back().get();

		m_endPoints.emplace_back(std::make_unique<EndPoint>());
		EndPoint* ep1 = m_endPoints.back().get();
		m_endPoints.emplace_back(std::make_unique<EndPoint>());
		EndPoint* ep2 = m_endPoints.back().get();

		ep1->setBegin(false);
		ep1->setPosition(segment.getA());
		ep1->setSegment(s);

		ep2->setBegin(false);
		ep2->setPosition(segment.getB());
		ep2->setSegment(s);

		s->setEndPointA(ep1);
		s->setEndPointB(ep2);
		s->setD(0.f);
	}

	std::vector<std::unique_ptr<Visibility::EndPoint>>& Visibility::Light::getEndPoints()
	{
		return m_endPoints;
	}

	std::vector<Visibility::Triangle*> Visibility::Light::getTriangles() const
	{
		std::vector<Visibility::Triangle*> triangles;
		for (std::size_t i = 0; i < m_triangles.size(); ++i)
		{
			triangles.push_back(m_triangles[i].get());
		}
		return triangles;
	}

	void Visibility::Light::addTriangle(float angle1, float angle2, Segment* segment)
	{
		sf::Vector2f p1 = getPosition();
		sf::Vector2f p2(p1.x + cosf(angle1), p1.y + sinf(angle1));
		sf::Vector2f p3;
		sf::Vector2f p4;

		if (segment != nullptr)
		{
			p3 = segment->getEndPointA()->getPosition();
			p4 = segment->getEndPointB()->getPosition();
		}
		else
		{
			p3.x = p1.x + cosf(angle1) * 500.f;
			p3.y = p1.y + sinf(angle1) * 500.f;
			p4.x = p1.x + cosf(angle2) * 500.f;
			p4.y = p1.y + sinf(angle2) * 500.f;
		}

		sf::Vector2f begin = linesIntersection(p3, p4, p1, p2);
		p2.x = p1.x + cosf(angle2);
		p2.y = p1.y + sinf(angle2);
		sf::Vector2f end = linesIntersection(p3, p4, p1, p2);

		m_vertices.append(sf::Vertex(begin, m_color));
		m_vertices.append(sf::Vertex(end, m_color));

		m_triangles.emplace_back(std::make_unique<Triangle>(getPosition(), begin, end));
	}

	Visibility::Visibility()
	{
	}

	void Visibility::update()
	{
		for (std::size_t i = 0; i < m_lights.size(); ++i)
		{
			Light* light = m_lights[i].get();

			light->updateSegments();
			light->sortEndPoints();

			std::vector<Segment*> open;
			float startingAngle = 0.f;

			for (unsigned int k = 0; k < 2; ++k)
			{
				std::vector<std::unique_ptr<EndPoint>>& endPoints = light->getEndPoints();
				for (std::size_t m = 0; m < endPoints.size(); ++m)
				{
					EndPoint* p = endPoints[m].get();
					Segment* current_old = open.empty() ? nullptr : open.front();

					if (p->getBegin())
					{
						auto it = open.begin();
						while (it != open.end() && segment_in_front_of(p->getSegment(), (*it), light->getPosition()))
						{
							++it;
						}

						if (it == open.end())
							open.push_back(p->getSegment());
						else
							open.insert(it, p->getSegment());
					}
					else
					{
						auto it = std::find(open.begin(), open.end(), p->getSegment());
						if (it != open.end())
						{
							open.erase(it);
						}
					}

					Segment* current_new = open.empty() ? nullptr : open.front();
					if (current_old != current_new)
					{
						if (k == 1)
						{
							light->addTriangle(startingAngle, p->getAngle(), current_old);
						}
						startingAngle = p->getAngle();
					}
				}
			}
		}
	}

	void Visibility::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (std::size_t i = 0; i < m_lights.size(); ++i)
		{
			target.draw(*m_lights[i], states);
		}
	}

	Visibility::Light* Visibility::create(const sf::Vector2f& position, const sf::Color& color)
	{
		m_lights.emplace_back(std::make_unique<Light>(color));
		Light* light = m_lights.back().get();
		light->setPosition(position);

		for (std::size_t i = 0; i < m_segments.size(); ++i)
		{
			light->addSegment(m_segments[i]);
		}

		return light;
	}

	void Visibility::addSegment(const sf::Vector2f& p0, const sf::Vector2f& p1)
	{
		m_segments.emplace_back(p0, p1);
	}
}