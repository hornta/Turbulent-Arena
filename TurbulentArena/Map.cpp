#include "stdafx.h"
#include "Map.hpp"

namespace bjoernligan
{
	// TILE DEFINITION
	Map::TileDefinition::TileDefinition(char ID, bool walkable, float u, float v, bool null)
		: m_ID(ID),
		m_walkable(walkable),
		m_uv(u, v),
		m_null(null)
	{

	}

	// OBJECT DEFINITION 
	Map::ObjectDefinition::ObjectDefinition(char ID, bool active, bool null)
		: m_ID(ID),
		m_null(null),
		m_active(active)
	{

	}

	// TILE
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

	void Map::Tile::setPhysicsBody(Physics::Body* body)
	{
		m_body = body;
	}

	// OBJECT
	Map::Object::Object(const sf::Vector2i& position, ObjectDefinition* objectDefinition)
		: m_position(position),
		m_objectDefinition(objectDefinition)
	{
		
	}

	sf::Vector2i Map::Object::getPosition() const
	{
		return m_position;
	}

	void Map::Object::setActive(bool value)
	{
		m_active = value;
	}

	bool Map::Object::isActive() const
	{
		return m_active;
	}

	// LAYER
	Map::TileLayer::TileLayer(const sf::Vector2i& size, const std::string& name)
		: m_name(name),
		m_size(size)
	{
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(m_size.x * m_size.y * 4);
	}

	Map::Tile* Map::TileLayer::getTile(int x, int y)
	{
		int index = x + y * m_size.x;
		return m_tiles[index].get();
	}

	Map::Tile* Map::TileLayer::getTile(const sf::Vector2i& position)
	{
		return getTile(position.x, position.y);
	}

	// OBJECT LAYER
	Map::ObjectLayer::ObjectLayer(const std::string& name)
		: m_name(name)
	{
	}

	Map::Object* Map::ObjectLayer::getActiveObject()
	{
		for (std::size_t i = 0; i < m_objects.size(); ++i)
		{
			if (m_objects[i]->isActive())
				return m_objects[i].get();
		}
		return nullptr;
	}

	/* MAP */
	Map::Map(const std::string& file)
	{
		if (parseMap(file))
		{

		}
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
		TileLayer* currentTileLayer = nullptr;
		ObjectLayer* currentObjectLayer = nullptr;

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
				m_tileSize = (int)std::stof(parts[1]);
			}
			else if (beginsWith("tl", parts))
			{
				m_tileLayers.emplace_back(std::make_unique<TileLayer>(m_size, parts[1]));
				currentRow = 0;
				currentTileLayer = m_tileLayers.back().get();
			}
			else if (beginsWith("ol", parts))
			{
				m_objectLayers.emplace_back(std::make_unique<ObjectLayer>(parts[1]));
				currentRow = 0;
				currentObjectLayer = m_objectLayers.back().get();
			}
			else if (beginsWith("td", parts))
			{
				if (parts.size() == 2)
					m_tileDefinitions.emplace_back(std::make_unique<TileDefinition>(static_cast<char>(parts[1][0]), false, 0.f, 0.f, true));
				else
					m_tileDefinitions.emplace_back(std::make_unique<TileDefinition>(static_cast<char>(parts[1][0]), string_to_bool(parts[2]), std::stof(parts[3]), std::stof(parts[4]), false));
			}
			else if (beginsWith("od", parts))
			{
				if (parts.size() == 2)
					m_objectDefinitions.emplace_back(std::make_unique<ObjectDefinition>(static_cast<char>(parts[1][0]), false, true));
				else
					m_objectDefinitions.emplace_back(std::make_unique<ObjectDefinition>(static_cast<char>(parts[1][0]), string_to_bool(parts[2]), false));
			}
			else if (beginsWith("d", parts))
			{
				for (uint32_t i = 0; i < parts[1].length(); ++i)
				{
					char ID = static_cast<char>(parts[1][i]);

					TileDefinition* td = getTileDefinition(ID);
					if (td == nullptr || td->m_null)
					{
						currentTileLayer->m_tiles.push_back(nullptr);
						continue;
					}

					int index = i + currentRow * m_size.x;
					currentTileLayer->m_tiles.emplace_back(std::make_unique<Tile>(sf::Vector2i(i, currentRow), td));
					Tile* tile = currentTileLayer->m_tiles.back().get();

					tile->m_vertices = &currentTileLayer->m_vertices[index * 4];

					tile->m_vertices[0].position = sf::Vector2f((float)i * m_tileSize, (float)currentRow * m_tileSize);
					tile->m_vertices[1].position = sf::Vector2f((float)(i + 1) * m_tileSize, (float)currentRow * m_tileSize);
					tile->m_vertices[2].position = sf::Vector2f((float)(i + 1) * m_tileSize, (float)(currentRow + 1) * m_tileSize);
					tile->m_vertices[3].position = sf::Vector2f((float)i * m_tileSize, (float)(currentRow + 1) * m_tileSize);

					tile->m_vertices[0].texCoords = sf::Vector2f(td->m_uv.x, td->m_uv.y);
					tile->m_vertices[1].texCoords = sf::Vector2f(td->m_uv.x + m_tileSize, td->m_uv.y);
					tile->m_vertices[2].texCoords = sf::Vector2f(td->m_uv.x + m_tileSize, td->m_uv.y + m_tileSize);
					tile->m_vertices[3].texCoords = sf::Vector2f(td->m_uv.x, td->m_uv.y + m_tileSize);
				}
				++currentRow;
			}
			else if (beginsWith("o", parts))
			{
				for (uint32_t i = 0; i < parts[1].length(); ++i)
				{
					char ID = static_cast<char>(parts[1][i]);
					ObjectDefinition* od = getObjectDefinition(ID);
					if (od == nullptr || od->m_null)
					{
						continue;
					}

					currentObjectLayer->m_objects.emplace_back(std::make_unique<Object>(sf::Vector2i(i, currentRow), od));
					Object* object = currentObjectLayer->m_objects.back().get();
					object->setActive(od->m_active);
				}
				++currentRow;
			}
		}

		return true;
	}

	Map::TileDefinition* Map::getTileDefinition(char ID) const
	{
		for (std::size_t i = 0; i < m_tileDefinitions.size(); ++i)
		{
			if (m_tileDefinitions[i]->m_ID == ID)
				return m_tileDefinitions[i].get();
		}
		return nullptr;
	}

	Map::ObjectDefinition* Map::getObjectDefinition(char ID) const
	{
		for (std::size_t i = 0; i < m_objectDefinitions.size(); ++i)
		{
			if (m_objectDefinitions[i]->m_ID == ID)
				return m_objectDefinitions[i].get();
		}
		return nullptr;
	}

	void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (std::size_t i = 0; i < m_tileLayers.size(); ++i)
		{
			states.texture = &m_texture;
			target.draw(m_tileLayers[i]->m_vertices, states);
		}
	}

	sf::Vector2i Map::getSize() const
	{
		return m_size;
	}

	int Map::getWidth() const
	{
		return m_size.x;
	}

	int Map::getHeight() const
	{
		return m_size.y;
	}

	int Map::getTileSize() const
	{
		return m_tileSize;
	}

	Map::Tile* Map::getTopmostTile(int x, int y) const
	{
		Tile* tmp = nullptr;
		for (std::size_t i = m_tileLayers.size() - 1; i >= 0; --i)
		{
			tmp = m_tileLayers[i]->getTile(x, y);
			if (tmp != nullptr)
			{
				return tmp;
			}
		}
		return tmp;
	}

	Map::Tile* Map::getTopmostTile(const sf::Vector2i& position) const
	{
		return getTopmostTile(position.x, position.y);
	}

	Map::TileLayer* Map::getTileLayer(const std::string& name)
	{
		for (std::size_t i = 0; i < m_tileLayers.size(); ++i)
		{
			if (m_tileLayers[i]->m_name == name)
			{
				return m_tileLayers[i].get();
			}
		}
		return nullptr;
	}

	Map::ObjectLayer* Map::getObjectLayer(const std::string& name)
	{
		for (std::size_t i = 0; i < m_objectLayers.size(); ++i)
		{
			if (m_tileLayers[i]->m_name == name)
			{
				return m_objectLayers[i].get();
			}
		}
		return nullptr;
	}

	bool Map::beginsWith(const std::string& id, const std::vector<std::string>& parts)
	{
		if (parts.size() > 0 && parts[0] == id)
			return true;
		return false;
	}
}