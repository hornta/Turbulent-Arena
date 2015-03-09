#pragma once

#include "Physics.hpp"

namespace bjoernligan
{
	class Map : public sf::Drawable
	{
	public:
		enum Orientation
		{
			ORIENTATION_ORTHOGONAL,
			ORIENTATION_ISOMETRIC,
			ORIENTATION_STAGGERED
		};

		enum RenderOrder
		{
			RENDERORDER_RIGHT_DOWN,
			RENDERORDER_RIGHT_UP,
			RENDERORDER_LEFT_DOWN,
			RENDERORDER_LEFT_UP
		};

		struct Tileset
		{
			sf::Texture m_texture;
		};

		class Properties
		{
			friend class Map;
		public:
			std::string getProperty(const std::string& key);
			bool hasProperty(const std::string& key);

		protected:
			std::map<std::string, std::string> m_propertySet;
			void parseProperties(tinyxml2::XMLElement* propertiesNode);
		};

		struct TileInfo
		{
			std::array<sf::Vector2f, 4> m_textureCoordinates;
			Tileset* m_tileset;
			Properties m_properties;
		};

		class Tile : public Properties
		{
			friend class Map;
		public:

		private:
			TileInfo* m_tileInfo;
			sf::Vertex* m_vertices;
		};

		struct LayerSet
		{
			sf::VertexArray m_vertices;
			std::vector<std::unique_ptr<Tile>> m_tiles;
		};

		struct TileLayer
		{
			std::string m_name;
			sf::Vector2i m_size;
			std::map<Tileset*, std::unique_ptr<LayerSet>> m_layerSets;

			Tile* getTile(int x, int y);
		};

		struct Object : public Properties
		{
			virtual ~Object();
			int m_ID;
			sf::Vector2f m_position;
			std::vector<sf::Vector2f> m_points;
		};

		struct Polygon : public Object
		{
		};

		class ObjectGroup
		{
			friend class Map;
		public:
			std::vector<Object*> getObjects() const;
		private:
			std::vector<std::unique_ptr<Object>> m_objects;
			std::string m_name;
		};

		Map(const std::string& path);
		bool load(const std::string& file);

		TileLayer* getLayer(const std::string& name) const;
		ObjectGroup* getObjectGroup(const std::string& name) const;
			
		sf::Vector2i getSize() const;
		sf::Vector2i getTilePosition(const sf::Vector2f& position) const;
		int getWidth() const;
		int getHeight() const;
		sf::Vector2f getTileSize() const;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		bool GetRandomWalkableTile(const sf::Vector2i &p_xSearchStart, sf::Vector2i &p_xTarget, sf::Vector2i p_xSearchAreaSize);

	private:
		sf::Vector2i m_size;
		sf::Vector2f m_tileSize;
		std::string m_path;
		sf::Color m_backgroundColor;
		Orientation m_orientation;
		RenderOrder m_renderOrder;

		std::vector<std::unique_ptr<TileLayer>> m_tileLayers;
		std::vector<std::unique_ptr<ObjectGroup>> m_objectGroups;
		std::vector<std::unique_ptr<TileInfo>> m_tileInfo;
		std::vector<std::unique_ptr<Tileset>> m_tilesets;
	};
}