#pragma once

namespace bjoernligan
{
	class ContactListener;
	class Box2DWorldDraw;
	class Physics
	{
	public:
		struct Body
		{
			~Body();

			void setPosition(const sf::Vector2f& p_position);
			void setPosition(float x, float y);

			b2Body* m_body;
			b2World* m_world;
		};

		enum EB2ShapeType
		{
			NoShape,
			Box,
			Circle,
		};

		struct Params
		{
			b2BodyDef m_xBodyDef;
			b2FixtureDef m_xFixtureDef;
			EB2ShapeType m_eShapeType;
			union
			{
				float m_fCircleRadius;
				struct Vec2i
				{
					int32_t x;
					int32_t y;
				} m_xBox;
			} m_xShapeSize;
		};

		Physics(float gravity_x, float gravity_y, sf::RenderWindow* window);
		Physics(const sf::Vector2f& gravity, sf::RenderWindow* window);
		~Physics();

		void update(float deltatime);

		b2World* getWorld() const;
		Body* createBody(const Params& params);

	private:
		void construct(sf::RenderWindow* window);

	private:
		std::unique_ptr<b2World> m_b2World;
		std::unique_ptr<ContactListener> m_xContactListener;
		std::unique_ptr<Box2DWorldDraw> m_debugDraw;
		sf::Vector2f m_gravity;
		std::vector<std::unique_ptr<Body>> m_bodies;
	};
}