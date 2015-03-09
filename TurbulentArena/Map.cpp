#include "stdafx.h"
#include "Map.hpp"

namespace bjoernligan
{
	std::string Map::Properties::getProperty(const std::string& key)
	{
		assert(m_propertySet.find(key) != m_propertySet.end());
		return m_propertySet[key];
	}

	bool Map::Properties::hasProperty(const std::string& key)
	{
		auto it = m_propertySet.find(key);
		if (it != m_propertySet.end())
			return true;
		return false;
	}

	void Map::Properties::parseProperties(tinyxml2::XMLElement* propertiesNode)
	{
		if (propertiesNode != nullptr)
		{
			tinyxml2::XMLElement* p = propertiesNode->FirstChildElement("property");
			while (p != nullptr)
			{
				m_propertySet.insert(std::make_pair(p->Attribute("name"), p->Attribute("value")));
				p = p->NextSiblingElement("property");
			}
		}
	}

	sf::Vector2i Map::Tile::getPosition() const
	{
		float tileWidth = m_vertices[1].position.x - m_vertices[0].position.x;
		float tileHeight = m_vertices[3].position.y - m_vertices[0].position.y;

		return sf::Vector2i(static_cast<int>(m_vertices[0].position.x / tileWidth), static_cast<int>(m_vertices[0].position.y / tileHeight));
	}

	Map::Tile* Map::TileLayer::getTile(int x, int y)
	{
		Tile* tile = nullptr;
		for (auto it = m_layerSets.begin(); it != m_layerSets.end(); ++it)
		{
			tile = it->second->m_tiles[x + y * m_size.x].get();
			if (tile != nullptr)
			{
				return tile;
			}
		}

		return nullptr;
	}

	Map::Object::~Object()
	{

	}

	std::vector<Map::Object*> Map::ObjectGroup::getObjects() const
	{
		std::vector<Map::Object*> objects;
		for (std::size_t i = 0; i < m_objects.size(); ++i)
		{
			objects.push_back(m_objects[i].get());
		}
		return objects;
	}

	Map::Map(const std::string& path)
	{
		m_path = path;
	}

	bool Map::load(const std::string& file)
	{
		tinyxml2::XMLDocument document;
		tinyxml2::XMLError fileLoaded = document.LoadFile(std::string(m_path + file).c_str());

		if (fileLoaded != tinyxml2::XML_SUCCESS)
		{
			std::cout << "Failed to load map: Could not open file '" << file << "'" << std::endl;
			return false;
		}

		tinyxml2::XMLElement* mapNode = document.FirstChildElement("map");

		m_size.x = std::stoi(mapNode->Attribute("width"));
		m_size.y = std::stoi(mapNode->Attribute("height"));
		m_tileSize.x = std::stof(mapNode->Attribute("tilewidth"));
		m_tileSize.y = std::stof(mapNode->Attribute("tileheight"));

		// PARSE TILESETS
		tinyxml2::XMLElement* tilesetNode = mapNode->FirstChildElement("tileset");
		m_tileInfo.emplace_back(std::make_unique<TileInfo>());

		sf::Vector2f tileSize;
		while (tilesetNode != nullptr)
		{
			tileSize.x = tilesetNode->FloatAttribute("tilewidth");
			tileSize.y = tilesetNode->FloatAttribute("tileheight");

			tinyxml2::XMLElement* imageNode = tilesetNode->FirstChildElement("image");
			m_tilesets.emplace_back(std::make_unique<Tileset>());
			Tileset* tileset = m_tilesets.back().get();

			tileset->m_texture.loadFromFile(m_path + imageNode->Attribute("source"));

			int columns = static_cast<int>(static_cast<float>(tileset->m_texture.getSize().x) / tileSize.x);
			int rows = static_cast<int>(static_cast<float>(tileset->m_texture.getSize().y) / tileSize.y);

			for (int y = 0; y < rows; ++y)
			{
				for (int x = 0; x < columns; ++x)
				{
					m_tileInfo.emplace_back(std::make_unique<TileInfo>());
					TileInfo* tileInfo = m_tileInfo.back().get();

					tileInfo->m_tileset = tileset;
					tileInfo->m_textureCoordinates[0].x = x * tileSize.x;
					tileInfo->m_textureCoordinates[0].y = y * tileSize.y;
					tileInfo->m_textureCoordinates[1].x = (x + 1) * tileSize.x;
					tileInfo->m_textureCoordinates[1].y = y * tileSize.y;
					tileInfo->m_textureCoordinates[2].x = (x + 1) * tileSize.x;
					tileInfo->m_textureCoordinates[2].y = (y + 1) * tileSize.y;
					tileInfo->m_textureCoordinates[3].x = x * tileSize.x;
					tileInfo->m_textureCoordinates[3].y = (y + 1) * tileSize.y;
				}
			}

			tilesetNode = tilesetNode->NextSiblingElement("tileset");
		}

		// LAYERS
		tinyxml2::XMLElement* currentNode = mapNode->FirstChildElement();
		while (currentNode != nullptr)
		{
			std::string elementName = currentNode->Name();

			if (elementName == "layer")
			{
				m_tileLayers.emplace_back(std::make_unique<TileLayer>());
				TileLayer* tileLayer = m_tileLayers.back().get();

				for (std::size_t i = 0; i < m_tilesets.size(); ++i)
				{
					tileLayer->m_layerSets.insert(std::make_pair(m_tilesets[i].get(), std::make_unique<LayerSet>()));
					LayerSet* layerSet = tileLayer->m_layerSets[m_tilesets[i].get()].get();
					layerSet->m_vertices.resize(m_size.x * m_size.y * 4);
					layerSet->m_vertices.setPrimitiveType(sf::Quads);
				}

				tileLayer->m_name = currentNode->Attribute("name");
				tileLayer->m_size.x = currentNode->IntAttribute("width");
				tileLayer->m_size.y = currentNode->IntAttribute("height");

				tinyxml2::XMLElement* dataNode = currentNode->FirstChildElement("data");
				tinyxml2::XMLElement* tileNode = dataNode->FirstChildElement("tile");

				int x = 0;
				int y = 0;
				int gid = 0;
				while (tileNode != nullptr)
				{
					gid = tileNode->IntAttribute("gid");

					auto layerSetIt = tileLayer->m_layerSets.begin();
					while (layerSetIt != tileLayer->m_layerSets.end())
					{
						if (gid != 0)
						{
							Tileset* id = m_tileInfo[gid]->m_tileset;
							LayerSet* layerSet = tileLayer->m_layerSets.find(id)->second.get();
							layerSet->m_tiles.emplace_back(std::make_unique<Tile>());
							Tile* tile = layerSet->m_tiles.back().get();

							tile->m_vertices = &layerSet->m_vertices[(x + y * m_size.x) * 4];
							tile->m_vertices[0].texCoords = m_tileInfo[gid]->m_textureCoordinates[0] + sf::Vector2f(0.5f, 0.5f);
							tile->m_vertices[1].texCoords = m_tileInfo[gid]->m_textureCoordinates[1] + sf::Vector2f(-0.5f, 0.5f);
							tile->m_vertices[2].texCoords = m_tileInfo[gid]->m_textureCoordinates[2] + sf::Vector2f(-0.5f, -0.5f);
							tile->m_vertices[3].texCoords = m_tileInfo[gid]->m_textureCoordinates[3] + sf::Vector2f(0.5f, -0.5f);

							tile->m_vertices[0].position = sf::Vector2f(x * m_tileSize.x, y * m_tileSize.y);
							tile->m_vertices[1].position = sf::Vector2f((x + 1) * m_tileSize.x, y * m_tileSize.y);
							tile->m_vertices[2].position = sf::Vector2f((x + 1) * m_tileSize.x, (y + 1) * m_tileSize.y);
							tile->m_vertices[3].position = sf::Vector2f(x * m_tileSize.x, (y + 1) * m_tileSize.y);

							auto layerSetIt2 = tileLayer->m_layerSets.begin();
							while (layerSetIt2 != tileLayer->m_layerSets.end())
							{
								if (layerSetIt2 != layerSetIt)
								{
									layerSetIt2->second->m_tiles.emplace_back();
								}
								++layerSetIt2;
							}
						}
						else
						{
							layerSetIt->second->m_tiles.emplace_back();
						}

						++layerSetIt;
					}

					tileNode = tileNode->NextSiblingElement("tile");
					++x;
					if (x == m_size.x)
					{
						x = 0;
						++y;
					}
				}
			}
			else if (elementName == "objectgroup")
			{
				m_objectGroups.emplace_back(std::make_unique<ObjectGroup>());
				ObjectGroup* objectGroup = m_objectGroups.back().get();
				objectGroup->m_name = currentNode->Attribute("name");

				tinyxml2::XMLElement* objectNode = currentNode->FirstChildElement("object");
				while (objectNode != nullptr)
				{
					int ID = objectNode->IntAttribute("id");
					sf::Vector2f position(objectNode->FloatAttribute("x"), objectNode->FloatAttribute("y"));

					tinyxml2::XMLElement* shapeNode = objectNode->FirstChildElement();
					std::string shapeName = shapeNode->Name();
					if (shapeName == "polyline" || shapeName == "polygon")
					{
						objectGroup->m_objects.emplace_back(std::make_unique<Polygon>());
						Polygon* polygon = static_cast<Polygon*>(objectGroup->m_objects.back().get());
						polygon->m_ID = ID;
						polygon->m_position = position;
						std::vector<std::string> stringPoints = explode(shapeNode->Attribute("points"), " ");
						for (std::size_t i = 0; i < stringPoints.size(); ++i)
						{
							std::vector<std::string> stringPointsParts = explode(stringPoints[i], ",");
							polygon->m_points.push_back(sf::Vector2f(position.x + std::stof(stringPointsParts[0]), position.y + std::stof(stringPointsParts[1])));
						}
					}
					else
					{
						objectGroup->m_objects.emplace_back(std::make_unique<Object>());
						Object* object = objectGroup->m_objects.back().get();
						object->m_ID = ID;
						object->m_position = position;
					}

					Object* object = objectGroup->m_objects.back().get();
					object->parseProperties(objectNode->FirstChildElement("properties"));

					objectNode = objectNode->NextSiblingElement("object");
				}
			}
			currentNode = currentNode->NextSiblingElement();
		}

		return true;
	}

	Map::TileLayer* Map::getLayer(const std::string& name) const
	{
		for (std::size_t i = 0; i < m_tileLayers.size(); ++i)
		{
			if (m_tileLayers[i]->m_name == name)
				return m_tileLayers[i].get();
		}
		return nullptr;
	}

	Map::Tile* Map::getTopmostTileAt(int x, int y)
	{
		Tile* currentTile = nullptr;
		for (std::size_t i = m_tileLayers.size() - 1; i >= 0; --i)
		{
			currentTile = m_tileLayers[i]->getTile(x, y);

			if (currentTile != nullptr)
			{
				return currentTile;
			}
		}

		return nullptr;
	}

	Map::Tile* Map::getTopmostTileAt(const sf::Vector2i& position)
	{
		return getTopmostTileAt(position.x, position.y);
	}

	Map::ObjectGroup* Map::getObjectGroup(const std::string& name) const
	{
		for (std::size_t i = 0; i < m_objectGroups.size(); ++i)
		{
			if (m_objectGroups[i]->m_name == name)
				return m_objectGroups[i].get();
		}
		return nullptr;
	}

	sf::Vector2i Map::getSize() const
	{
		return m_size;
	}

	sf::Vector2i Map::getTilePosition(const sf::Vector2f& position) const
	{
		return sf::Vector2i(static_cast<int>(position.x / m_tileSize.x), static_cast<int>(position.y / m_tileSize.y));
	}

	int Map::getWidth() const
	{
		return m_size.x;
	}

	int Map::getHeight() const
	{
		return m_size.y;
	}

	sf::Vector2f Map::getTileSize() const
	{
		return m_tileSize;
	}

	void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (std::size_t i = 0; i < m_tileLayers.size(); ++i)
		{
			TileLayer* tileLayer = m_tileLayers[i].get();
			for (auto& ls : tileLayer->m_layerSets)
			{
				states.texture = &ls.first->m_texture;
				target.draw(ls.second.get()->m_vertices, states);
			}
		}
	}

	bool Map::GetRandomTopmostWalkableTile(const sf::Vector2i &p_xSearchStart, sf::Vector2i &p_xTarget, sf::Vector2i p_xSearchAreaSize)
	{
		sf::Vector2i xStart(sf::Vector2i(p_xSearchStart.x - p_xSearchAreaSize.x / 2, p_xSearchStart.y - p_xSearchAreaSize.y / 2));
		if (xStart.x < 0)
			xStart.x = 0;
		if (xStart.y < 0)
			xStart.y = 0;

		sf::Vector2i xEnd(sf::Vector2i(p_xSearchStart.x + p_xSearchAreaSize.x / 2, p_xSearchStart.y + p_xSearchAreaSize.y / 2));

		if (xEnd.x > m_size.x)
			xEnd.x = m_size.x - p_xSearchStart.x;
		if (xEnd.y > m_size.y)
			xEnd.y = m_size.y - p_xSearchStart.y;

		std::vector<Tile*> xAvailableTiles;

		for (int32_t x = xStart.x; x < xEnd.x; ++x)
		{
			for (int32_t y = xStart.y; y < xEnd.y; ++y)
			{
				Tile* xTile = getTopmostTileAt(x, y);
				if (xTile && xTile->hasProperty("walkable"))
					xAvailableTiles.push_back(xTile);
			}
		}

		if (!xAvailableTiles.empty())
		{
			p_xTarget = xAvailableTiles[random::random(0, xAvailableTiles.size() - 1)]->getPosition();
			return true;
		}

		return false;
	}
}