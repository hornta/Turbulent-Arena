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
			void setPosition(const sf::Vector2f& p_position);
			void setPosition(float x, float y);

			b2Body* m_body;
		};

		enum EB2ShapeType
		{
			NoShape,
			Box,
			Circle,
		};

		struct RaycastResult
		{
			std::vector<b2Body*> bodies;

			void reset()
			{
				bodies.clear();
			}
		};

		class Raycast : b2RayCastCallback
		{
			friend Physics;
			RaycastResult m_result;
			b2World* m_world;

		public:
			Raycast();
			void cast(const b2Vec2& start, const b2Vec2& end);
			void cast(const b2Vec2& start, float direction, float distance);
			float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
		};


		struct Params
		{
			b2BodyDef m_xBodyDef;
			b2FixtureDef m_xFixtureDef;
			EB2ShapeType m_eShapeType;
			union
			{
				float m_fCircleRadius;
				struct Vec2f
				{
					float x;
					float y;
				} m_xBox;
			} m_xShapeSize;
		};



		Physics(float gravity_x, float gravity_y, sf::RenderWindow* window);
		Physics(const sf::Vector2f& gravity, sf::RenderWindow* window);
		~Physics();

		void update(float deltatime);
		void draw();
		RaycastResult raycast(const sf::Vector2f& start, const sf::Vector2f& end);
		RaycastResult raycast(const sf::Vector2f& start, float direction, float distance);
		void setDebug(bool value);

		b2World* getWorld() const;
		Body* createBody(const Params& params);
		void destroyBody(Body* p_xBody);

	private:
		void construct(sf::RenderWindow* window);

	private:
		bool m_drawDebug = false;
		std::unique_ptr<b2World> m_b2World;
		std::unique_ptr<ContactListener> m_xContactListener;
		std::unique_ptr<Box2DWorldDraw> m_debugDraw;
		sf::Vector2f m_gravity;
		Raycast m_raycast;
		std::vector<std::unique_ptr<Body>> m_bodies;
	};
}