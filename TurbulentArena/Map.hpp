#pragma once

#include "Physics.hpp"

namespace bjoernligan
{
	class Map : public sf::Drawable
	{
	public:
		class TileDefinition
		{
			friend class Map;
			friend class Tile;
		public:
			TileDefinition(char ID, bool walkable, float u, float v, bool null);

		private:
			char m_ID;
			bool m_walkable;
			bool m_null;
			sf::Vector2f m_uv;
		};

		class ObjectDefinition
		{
			friend class Map;
			friend class Object;

		public:
			ObjectDefinition(char ID, bool active, bool null);

		private:
			char m_ID;
			bool m_null;
			bool m_active;
		};

		class Tile
		{
			friend class Map;
		public:
			Tile(const sf::Vector2i& position, TileDefinition* tileDefinition);

			sf::Vector2i getPosition() const;
			bool isWalkable() const;
			sf::Vertex* getVertices();
			void setPhysicsBody(Physics::Body* body);

		private:
			sf::Vertex* m_vertices;
			sf::Vector2i m_position;
			TileDefinition* m_tileDefinition;
			Physics::Body* m_body;
		};

		class Object
		{
			friend class Map;
		public:
			Object(const sf::Vector2i& position, ObjectDefinition* objectDefinition);

			sf::Vector2i getPosition() const;
			void setActive(bool value);
			bool isActive() const;
		private:
			bool m_active;
			sf::Vector2i m_position;
			ObjectDefinition* m_objectDefinition;
		};

		class TileLayer
		{
			friend class Map;
		public:
			TileLayer(const sf::Vector2i& size, const std::string& name);

			Tile* getTile(int x, int y);
			Tile* getTile(const sf::Vector2i& position);
		private:
			std::string m_name;
			std::vector<std::unique_ptr<Tile>> m_tiles;
			sf::Vector2i m_size;
			sf::VertexArray m_vertices;
		};

		class ObjectLayer
		{
			friend class Map;
		public:
			ObjectLayer(const std::string& name);

			Object* getActiveObject();
		private:
			std::string m_name;
			std::vector<std::unique_ptr<Object>> m_objects;
		};

		Map(const std::string& file);

		bool parseMap(const std::string& file);
		TileDefinition* getTileDefinition(char ID) const;
		ObjectDefinition* getObjectDefinition(char ID) const;
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		sf::Vector2i getSize() const;
		int getWidth() const;
		int getHeight() const;
		int getTileSize() const;
		Tile* getTopmostTile(int x, int y) const;
		Tile* getTopmostTile(const sf::Vector2i& position) const;
		TileLayer* getTileLayer(const std::string& name);
		ObjectLayer* getObjectLayer(const std::string& name);

	private:
		bool beginsWith(const std::string& id, const std::vector<std::string>& parts);

	protected:
		sf::Vector2i m_size;
		int m_tileSize;
		std::vector<std::unique_ptr<TileLayer>> m_tileLayers;
		std::vector<std::unique_ptr<ObjectLayer>> m_objectLayers;
		std::vector<std::unique_ptr<TileDefinition>> m_tileDefinitions;
		std::vector<std::unique_ptr<ObjectDefinition>> m_objectDefinitions;
		sf::Texture m_texture;
	};
}