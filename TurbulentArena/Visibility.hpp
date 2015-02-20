#pragma once

namespace bjoernligan
{
	class Visibility : public sf::Drawable
	{
	public:
		class Segment;
		class EndPoint
		{
		public:
			EndPoint();

			sf::Vector2f getPosition() const;
			float getAngle() const;
			Segment* getSegment() const;
			bool getBegin() const;

			void setPosition(const sf::Vector2f& position);
			void setAngle(float angle);
			void setSegment(Segment* segment);
			void setBegin(bool begin);


		private:
			sf::Vector2f m_position;
			float m_angle;
			Segment* m_segment;
			bool m_begin;
		};

		class Segment
		{
		public:
			Segment();

			EndPoint* getEndPointA() const;
			EndPoint* getEndPointB() const;
			float getD() const;

			void setD(float d);
			void setEndPointA(EndPoint* a);
			void setEndPointB(EndPoint* b);
		private:
			EndPoint* m_a;
			EndPoint* m_b;
			float m_d;
		};

		class Triangle
		{
		public:
			Triangle(const sf::Vector2f& p0, const sf::Vector2f& p1, const sf::Vector2f& p2);

			sf::Vector2f getPoint0() const;
			sf::Vector2f getPoint1() const;
			sf::Vector2f getPoint2() const;
		private:
			sf::Vector2f m_p0;
			sf::Vector2f m_p1;
			sf::Vector2f m_p2;
		};

		class PrepareSegment
		{
		public:
			PrepareSegment(const sf::Vector2f& a, const sf::Vector2f& b);

			sf::Vector2f getA() const;
			sf::Vector2f getB() const;

		private:
			sf::Vector2f m_a;
			sf::Vector2f m_b;
		};

		class Light : public sf::Drawable, public sf::Transformable
		{
		public:
			Light(const sf::Color& color);
			~Light();

			void setTexture(sf::Texture* texture);
			void setColor(const sf::Color color);
			void updateSegments();
			void sortEndPoints();
			void draw(sf::RenderTarget& target, sf::RenderStates states) const;
			void addSegment(const PrepareSegment& segment);
			void addTriangle(float angle1, float angle2, Segment* segment);
			std::vector<EndPoint*>& getEndPoints();

		private:
			std::vector<EndPoint*> m_endPoints;
			std::vector<Segment*> m_segments;
			sf::VertexArray m_vertices;
			sf::Color m_color;
			sf::Texture* m_texture;
		};

		Visibility();
		~Visibility();

		void update();
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		Light* create(const sf::Vector2f& position, const sf::Color& color = sf::Color::White);
		void addSegment(const sf::Vector2f& p0, const sf::Vector2f& p1);
	private:
		std::vector<Light*> m_lights;
		std::vector<PrepareSegment> m_segments;
	};
}