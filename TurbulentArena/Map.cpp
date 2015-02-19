#include "stdafx.h"
#include "Map.hpp"

namespace bjoernligan
{
	/* TILE DEFINITION*/
	Map::TileDefinition::TileDefinition(char ID, bool walkable, float u, float v, bool null)
		: m_ID(ID),
		m_walkable(walkable),
		m_uv(u, v),
		m_null(null)
	{

	}

	/* TILE */
	Map::Tile::Tile(const sf::Vector2i& position, Map::TileDefinition* tileDefinition)
		: m_position(position),
		m_tileDefinition(tileDefinition)
	{

	}

	sf::Vector2i Map::Tile::getPosition() const
	{
		return m_position;
	}

	bool Map::Tile::isWalkable() const
	{
		return m_tileDefinition->m_walkable;
	}

	sf::Vertex* Map::Tile::getVertices()
	{
		return m_vertices;
	}

	/* LAYER */
	Map::Layer::Layer(const sf::Vector2i& size, const std::string& name)
		: m_name(name),
		m_size(size)
	{
		m_tiles = new Tile*[m_size.x * m_size.y];
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(m_size.x * m_size.y * 4);
	}

	Map::Layer::~Layer()
	{
		for (int i = 0; i < m_size.x * m_size.y; ++i)
		{
			delete m_tiles[i];
			m_tiles[i] = nullptr;
		}
		
		delete[] m_tiles;
		m_tiles = nullptr;
	}

	/* MAP */
	Map::Map(const std::string& file)
	{
		if (parseMap(file))
		{

		}
	}

	Map::~Map()
	{
		for (int i = 0; i < m_numTileDefinitions; ++i)
		{
			delete m_tileDefinitions[i];
			m_tileDefinitions[i] = nullptr;
		}

		delete[] m_tileDefinitions;
		m_tileDefinitions = nullptr;

		for (int i = 0; i < m_numLayers; ++i)
		{
			delete m_layers[i];
			m_layers[i] = nullptr;
		}

		delete[] m_layers;
		m_layers = nullptr;
	}

	bool Map::parseMap(const std::string& file)
	{
		std::ifstream ifs(file);
		if (!ifs.is_open())
		{
			std::cout << "Failed to parse map: Could not open file '" << file << "'" << std::endl;
			return false;
		}

		std::string line;
		std::vector<std::string> parts;
		int currentRow = 0;
		Layer* currentLayer = nullptr;

		while (std::getline(ifs, line))
		{
			parts = explode(line, " ");

			if (beginsWith("w", parts))
			{
				m_size.x = std::stoi(parts[1]);
			}
			else if (beginsWith("h", parts))
			{
				m_size.y = std::stoi(parts[1]);
			}
			else if (beginsWith("t", parts))
			{
				m_texture.loadFromFile(parts[1]);
			}
			else if (beginsWith("ts", parts))
			{
				m_tileSize = std::stof(parts[1]);
			}
			else if (beginsWith("ntd", parts))
			{
				m_tileDefinitions = new TileDefinition*[std::stoi(parts[1])];
			}
			else if (beginsWith("nl", parts))
			{
				m_layers = new Layer*[std::stoi(parts[1])];
			}
			else if (beginsWith("l", parts))
			{
				m_layers[m_numLayers++] = new Layer(m_size, parts[1]);
				currentRow = 0;
				currentLayer = m_layers[m_numLayers - 1];
			}
			else if (beginsWith("td", parts))
			{
				if (parts.size() == 2)
					m_tileDefinitions[m_numTileDefinitions++] = new TileDefinition(static_cast<char>((parts[1][0])), false, 0.f, 0.f, true);
				else
					m_tileDefinitions[m_numTileDefinitions++] = new TileDefinition(static_cast<char>((parts[1][0])), string_to_bool(parts[2]), std::stof(parts[3]), std::stof(parts[4]), false);
			}
			else if (beginsWith("d", parts))
			{
				for (int i = 0; i < parts[1].length() - 1; ++i)
				{
					char ID = static_cast<char>(parts[1][i]);
					TileDefinition* td = getTileDefinition(ID);
					if (td == nullptr || td->m_null)
					{
						continue;
					}

					int index = i + currentRow * m_size.x;
					Tile* tile = new Tile(sf::Vector2i(i, currentRow), td);
					tile->m_vertices = &currentLayer->m_vertices[index * 4];

					tile->m_vertices[0].position = sf::Vector2f(i * m_tileSize, currentRow * m_tileSize);
					tile->m_vertices[1].position = sf::Vector2f((i + 1) * m_tileSize, currentRow * m_tileSize);
					tile->m_vertices[2].position = sf::Vector2f((i + 1) * m_tileSize, (currentRow + 1) * m_tileSize);
					tile->m_vertices[3].position = sf::Vector2f(i * m_tileSize, (currentRow + 1) * m_tileSize);

					tile->m_vertices[0].texCoords = sf::Vector2f(td->m_uv.x, td->m_uv.y);
					tile->m_vertices[1].texCoords = sf::Vector2f(td->m_uv.x + m_tileSize, td->m_uv.y);
					tile->m_vertices[2].texCoords = sf::Vector2f(td->m_uv.x + m_tileSize, td->m_uv.y + m_tileSize);
					tile->m_vertices[3].texCoords = sf::Vector2f(td->m_uv.x, td->m_uv.y + m_tileSize);

					currentLayer->m_tiles[index] = tile;
				}
				++currentRow;
			}
		}

		return true;
	}

	Map::TileDefinition* Map::getTileDefinition(char ID) const
	{
		for (int i = 0; i < m_numTileDefinitions; ++i)
		{
			if (m_tileDefinitions[i]->m_ID == ID)
				return m_tileDefinitions[i];
		}
		return nullptr;
	}

	void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (int i = 0; i < m_numLayers; ++i)
		{
			states.texture = &m_texture;
			target.draw(m_layers[i]->m_vertices, states);
		}
	}

	bool Map::beginsWith(const std::string& id, const std::vector<std::string>& parts)
	{
		if (parts.size() > 0 && parts[0] == id)
			return true;
		return false;
	}
}