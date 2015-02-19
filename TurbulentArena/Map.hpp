#pragma once

#include "stdafx.h"

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

		class Tile
		{
			friend class Map;
		public:
			Tile(const sf::Vector2i& position, TileDefinition* tileDefinition);

			sf::Vector2i getPosition() const;
			bool isWalkable() const;
			sf::Vertex* getVertices();

		private:
			sf::Vertex* m_vertices;
			sf::Vector2i m_position;
			TileDefinition* m_tileDefinition;
		};

		class Layer
		{
			friend class Map;
		public:
			Layer(const sf::Vector2i& size, const std::string& name);
			~Layer();

			Tile* getTile(int x, int y);
			Tile* getTile(const sf::Vector2i& position);
		private:
			std::string m_name;
			Tile** m_tiles;
			sf::Vector2i m_size;
			sf::VertexArray m_vertices;
		};

		Map(const std::string& file);
		~Map();

		bool parseMap(const std::string& file);
		TileDefinition* getTileDefinition(char ID) const;
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		sf::Vector2i getSize() const;
		int getWidth() const;
		int getHeight() const;
		int getTileSize() const;
		Tile* getTopmostTile(int x, int y) const;
		Tile* getTopmostTile(const sf::Vector2i& position) const;

	private:
		bool beginsWith(const std::string& id, const std::vector<std::string>& parts);

	protected:
		sf::Vector2i m_size;
		int m_numLayers;
		int m_numTileDefinitions;
		int m_tileSize;
		Layer** m_layers;
		TileDefinition** m_tileDefinitions;
		sf::Texture m_texture;
	};
}