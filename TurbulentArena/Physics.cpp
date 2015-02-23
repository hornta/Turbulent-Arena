#include "stdafx.h"
#include "Physics.hpp"
#include "ContactListener.hpp"
#include "Box2DWorldDraw.h"

namespace bjoernligan
{
	Physics::Body::~Body()
	{
		m_world->DestroyBody(m_body);
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
		m_b2World->Step(deltatime, 10, 10);
	}

	void Physics::draw()
	{
		if (m_drawDebug)
		{
			m_b2World->DrawDebugData();
		}
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
		body->SetLinearDamping(0.6f);
		body->SetAngularDamping(0.6f);

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
		b->m_world = m_b2World.get();
		return b;
	}

	void Physics::construct(sf::RenderWindow* window)
	{
		m_b2World = std::make_unique<b2World>(b2Vec2(m_gravity.x, m_gravity.y));
		m_debugDraw = std::make_unique<Box2DWorldDraw>(window);
		m_xContactListener = std::make_unique<ContactListener>();
		m_debugDraw->SetFlags(b2Draw::e_jointBit | b2Draw::e_shapeBit);
		m_b2World->SetDebugDraw(m_debugDraw.get());
		m_b2World->SetContactListener(m_xContactListener.get());
	}
}