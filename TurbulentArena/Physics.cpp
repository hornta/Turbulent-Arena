#include "stdafx.h"
#include "Physics.hpp"
#include "ContactListener.hpp"
#include "Box2DWorldDraw.h"

namespace bjoernligan
{
	Physics::Raycast::Raycast()
	{

	}

	void Physics::Raycast::cast(const b2Vec2& start, const b2Vec2& end)
	{
		float x_end = end.x - start.x;
		float y_end = end.y - start.y;
		float wanted_angle = atan2f(y_end, x_end);
		float distance = distanceBetweenPoints(start.x, start.y, end.x, end.y);

		cast(start, wanted_angle, distance);
	}

	void Physics::Raycast::cast(const b2Vec2& start, float direction, float distance)
	{
		float end_x = start.x + cosf(direction) * distance;
		float end_y = start.y + sinf(direction) * distance;

		
		m_world->RayCast(this, start, b2Vec2(end_x, end_y));
	}

	float32 Physics::Raycast::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
	{
		m_result.bodies.push_back(fixture->GetBody());
		normal;
		point;
		return fraction;
	}
	void Physics::Body::setPosition(const sf::Vector2f& p_position)
	{
		if (m_body == nullptr)
			return;
		m_body->SetTransform(vectorToB2D(p_position), m_body->GetAngle());
	}
	
	void Physics::Body::setPosition(float x, float y)
	{
		setPosition(sf::Vector2f(x, y));
	}

	Physics::Physics(float gravity_x, float gravity_y, sf::RenderWindow* window)
		: m_gravity(sf::Vector2f(gravity_x, gravity_y))
	{
		construct(window);
	}

	Physics::Physics(const sf::Vector2f& gravity, sf::RenderWindow* window)
		: m_gravity(gravity)
	{
		construct(window);
	}

	Physics::~Physics()
	{

	}

	void Physics::update(float deltatime)
	{
		m_b2World->Step(deltatime, 8, 3);
	}

	void Physics::draw()
	{
		if (m_drawDebug)
		{
			m_b2World->DrawDebugData();
		}
	}

	Physics::RaycastResult Physics::raycast(const sf::Vector2f& start, const sf::Vector2f& end)
	{
		m_raycast.m_result.reset();
		m_raycast.cast(vectorToB2D(start), vectorToB2D(end));
		return m_raycast.m_result;
	}

	Physics::RaycastResult Physics::raycast(const sf::Vector2f& start, float direction, float distance)
	{
		m_raycast.m_result.reset();
		m_raycast.cast(vectorToB2D(start), direction, floatToB2D(distance));
		return m_raycast.m_result;
	}

	void Physics::setDebug(bool value)
	{
		m_drawDebug = value;
	}

	b2World* Physics::getWorld() const
	{
		return m_b2World.get();
	}

	Physics::Body* Physics::createBody(const Params& params)
	{
		b2Body* body = m_b2World->CreateBody(&params.m_xBodyDef);
		body->SetLinearDamping(params.m_xBodyDef.linearDamping);
		body->SetAngularDamping(params.m_xBodyDef.angularDamping);
		body->SetFixedRotation(params.m_xBodyDef.fixedRotation);

		b2PolygonShape xBoxShape;
		b2CircleShape xCircleShape;
		b2FixtureDef fixtureDef(params.m_xFixtureDef);
		if (params.m_eShapeType == EB2ShapeType::Box)
		{
			xBoxShape.SetAsBox(
				floatToB2D(params.m_xShapeSize.m_xBox.x / 2.0f),
				floatToB2D(params.m_xShapeSize.m_xBox.y / 2.0f));
			fixtureDef.shape = &xBoxShape;
		}
		else if (params.m_eShapeType == EB2ShapeType::Circle)
		{
			xCircleShape.m_radius = floatToB2D(params.m_xShapeSize.m_fCircleRadius);
			fixtureDef.shape = &xCircleShape;
		}
		body->CreateFixture(&fixtureDef);

		m_bodies.emplace_back(std::make_unique<Body>());
		Body* b = m_bodies.back().get();
		b->m_body = body;
		return b;
	}

	void Physics::destroyBody(Body* p_xBody)
	{
		if (m_bodies.empty())
			return;

		for (uint32_t i = m_bodies.size() - 1; i >= 0; --i)
		{
			if (m_bodies[i].get() == p_xBody)
			{
				m_b2World->DestroyBody(m_bodies[i].get()->m_body);
				m_bodies.erase(m_bodies.begin() + i);
				return;
			}
		}
	}

	void Physics::construct(sf::RenderWindow* window)
	{
		m_b2World = std::make_unique<b2World>(b2Vec2(m_gravity.x, m_gravity.y));
		m_debugDraw = std::make_unique<Box2DWorldDraw>(window);
		m_xContactListener = std::make_unique<ContactListener>();
		m_debugDraw->SetFlags(b2Draw::e_jointBit | b2Draw::e_shapeBit);
		m_b2World->SetDebugDraw(m_debugDraw.get());
		m_b2World->SetContactListener(m_xContactListener.get());
		m_raycast.m_world = m_b2World.get();
	}
}