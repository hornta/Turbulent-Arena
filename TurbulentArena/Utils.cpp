#include "stdafx.h"
#include "Utils.hpp"

namespace bjoernligan
{
	float Box2D_Unit = 32.f;
	float toBox2D = 1.f / Box2D_Unit;
	float toPixel = 1.f * Box2D_Unit;

	float dotDegrees(const sf::Vector2f &p_xFirst, const sf::Vector2f &p_xSecond)
	{
		//return atan2(p_xSecond.y - p_xFirst.y, p_xSecond.x - p_xFirst.x);
		return atan2(p_xSecond.y, p_xSecond.y) - atan2(p_xFirst.y, p_xFirst.x);
		//return p_xFirst.x * p_xSecond.x + p_xFirst.y * p_xSecond.y;
	}

	float dotDegrees(const b2Vec2 &p_xFirst, const b2Vec2 &p_xSecond)
	{
		return p_xFirst.x * p_xSecond.x + p_xFirst.y * p_xSecond.y;
	}

	namespace random
	{
		std::random_device rd;
		std::mt19937 engine(rd());

		std::mt19937 getEngine()
		{
			return engine;
		}

		int random(int min, int max)
		{
			assert(min <= max);
			std::uniform_int_distribution<int> dist(min, max);
			return dist(engine);
		}

		float random(float min, float max)
		{
			assert(min <= max);
			std::uniform_real_distribution<float> dist(min, max);
			return dist(engine);
		}
	}
	
	float distanceBetweenPoints(float x0, float y0, float x1, float y1)
	{
		return sqrtf((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
	}

	float getDirectionFromPoints(float x0, float y0, float x1, float y1)
	{
		return atan2f(y1 - y0, x1 - x0);
	}
	std::vector<std::string> explode(const std::string& string, const std::string& delimeter)
	{
		std::vector<std::string> parts;
		std::size_t oldPos = 0;

		bool bRunning = true;
		while (bRunning)
		{
			std::size_t newPos = string.find(delimeter, oldPos);
			if (newPos == std::string::npos)
			{
				parts.push_back(string.substr(oldPos, newPos - oldPos));
				bRunning = false;
				continue;
			}
			parts.push_back(string.substr(oldPos, newPos - oldPos));
			oldPos = newPos + 1;
		}

		return parts;
	}

	bool string_to_bool(const std::string& string)
	{
		if (string == "0" || string.empty() || string == "false")
			return false;
		return true;
	}

	float floatToB2D(float f)
	{
		return f * toBox2D;
	}

	float floatToPix(float f)
	{
		return f * toPixel;
	}

	b2Vec2 vectorToB2D(const sf::Vector2f& v)
	{
		return b2Vec2(v.x * toBox2D, v.y * toBox2D);
	}

	sf::Vector2f vectorToPix(const b2Vec2& v)
	{
		return sf::Vector2f(v.x * toPixel, v.y * toPixel);
	}

	Grid::Grid(int width, int height)
		: m_width(width),
		m_height(height)
	{
	}

	Grid::~Grid()
	{
	}

	Grid::Node* Grid::getNodeAt(int x, int y) const
	{
		return m_nodes[x + y * m_width].get();
	}

	bool Grid::isInside(int x, int y) const
	{
		return (x >= 0 && x < m_width) && (y >= 0 && y < m_height);
	}

	std::vector<Grid::Node*> Grid::getNeighbors(Node* n, bool diagonals) const
	{
		std::vector<Node*> neighbors;

		int x = n->x;
		int y = n->y;

		// up
		if (isInside(x, y - 1))
			neighbors.push_back(m_nodes[x + (y - 1) * m_width].get());
		// down
		if (isInside(x, y + 1))
			neighbors.push_back(m_nodes[x + (y + 1) * m_width].get());
		// left
		if (isInside(x - 1, y))
			neighbors.push_back(m_nodes[(x - 1) + y * m_width].get());
		// down
		if (isInside(x + 1, y))
			neighbors.push_back(m_nodes[(x + 1) + y * m_width].get());

		if (diagonals)
		{
			// up left
			if (isInside(x - 1, y - 1))
				neighbors.push_back(m_nodes[(x - 1) + (y - 1) * m_width].get());
			// up right
			if (isInside(x + 1, y - 1))
				neighbors.push_back(m_nodes[(x + 1) + (y - 1) * m_width].get());
			// down left
			if (isInside(x - 1, y + 1))
				neighbors.push_back(m_nodes[(x - 1) + (y + 1) * m_width].get());
			// down right
			if (isInside(x + 1, y + 1))
				neighbors.push_back(m_nodes[(x + 1) + (y + 1) * m_width].get());
		}

		return neighbors;
	}


	Grid::Node::Node()
		: x(0),
		y(0)
	{

	}

	Grid::Node::Node(int x, int y)
		: x(x),
		y(y)
	{

	}

	PathfinderGrid::PathfinderGrid(int width, int height)
		: Grid(width, height)
	{
		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; ++x)
			{
				m_nodes.emplace_back(std::make_unique<PathfinderNode>(x, y, true));
			}
		}
	}

	bool PathfinderGrid::isWalkableAt(int x, int y) const
	{
		return isInside(x, y) && static_cast<PathfinderNode*>(m_nodes[x + y * m_width].get())->walkable;
	}

	void PathfinderGrid::setWalkableAt(int x, int y, bool walkable)
	{
		static_cast<PathfinderNode*>(m_nodes[x + y * m_width].get())->walkable = walkable;
	}

	void PathfinderGrid::resetNodes()
	{
		for (int i = 0; i < m_width * m_height; ++i)
		{
			PathfinderNode* n = static_cast<PathfinderNode*>(m_nodes[i].get());
			n->closed = false;
			n->parent = nullptr;
			n->visisted = nullptr;
			n->f = 0.f;
			n->g = 0.f;
		}
	}

	std::vector<PathfinderGrid::PathfinderNode*> PathfinderGrid::getNeighbors(Node* n, PathfinderInfo::DiagonalMovement diagonalMovement, bool canCrossCorners) const
	{
		canCrossCorners;
		std::vector<PathfinderNode*> neighbors;

		int x = n->x;
		int y = n->y;

		bool s0 = false;
		bool s1 = false;
		bool s2 = false;
		bool s3 = false;

		bool d0 = false;
		bool d1 = false;
		bool d2 = false;
		bool d3 = false;

		// ↑
		if (isWalkableAt(x, y - 1))
		{
			neighbors.push_back(static_cast<PathfinderNode*>(m_nodes[x + (y - 1) * m_width].get()));
			s0 = true;
		}
		// →
		if (isWalkableAt(x + 1, y))
		{
			neighbors.push_back(static_cast<PathfinderNode*>(m_nodes[(x + 1) + y * m_width].get()));
			s1 = true;
		}
		// ↓
		if (isWalkableAt(x, y + 1))
		{
			neighbors.push_back(static_cast<PathfinderNode*>(m_nodes[x + (y + 1) * m_width].get()));
			s2 = true;
		}
		// ←
		if (isWalkableAt(x - 1, y))
		{
			neighbors.push_back(static_cast<PathfinderNode*>(m_nodes[(x - 1) + y * m_width].get()));
			s3 = true;
		}

		if (diagonalMovement == PathfinderInfo::DIAGONAL_NEVER)
			return neighbors;

		if (diagonalMovement == PathfinderInfo::DIAGONAL_NO_OBSTACLES)
		{
			d0 = s3 && s0;
			d1 = s0 && s1;
			d2 = s1 && s2;
			d3 = s2 && s3;
		}
		else if (diagonalMovement == PathfinderInfo::DIAGONAL_ONE_OBSTACLE)
		{
			d0 = s3 || s0;
			d1 = s0 || s1;
			d2 = s1 || s2;
			d3 = s2 || s3;
		}
		else if (diagonalMovement == PathfinderInfo::DIAGONAL_ALWAYS)
		{
			d0 = true;
			d1 = true;
			d2 = true;
			d3 = true;
		}
		else
		{
			std::cerr << "Incorrect value of diagonal" << std::endl;
		}

		// ↖
		if (d0 && isWalkableAt(x - 1, y - 1))
			neighbors.push_back(static_cast<PathfinderNode*>(m_nodes[(x - 1) + (y - 1) * m_width].get()));
		// ↗
		if (d1 && isWalkableAt(x + 1, y - 1))
			neighbors.push_back(static_cast<PathfinderNode*>(m_nodes[(x + 1) + (y - 1) * m_width].get()));
		// ↘
		if (d2 && isWalkableAt(x + 1, y + 1))
			neighbors.push_back(static_cast<PathfinderNode*>(m_nodes[(x + 1) + (y + 1) * m_width].get()));
		// ↙
		if (d3 && isWalkableAt(x - 1, y + 1))
			neighbors.push_back(static_cast<PathfinderNode*>(m_nodes[(x - 1) + (y + 1) * m_width].get()));

		return neighbors;
	}

	bool PathfinderGrid::PathfinderNode::operator<(const PathfinderNode& other) const
	{
		return f < other.f;
	}

	bool PathfinderGrid::PathfinderNode::operator>(const PathfinderNode& other) const
	{
		return f > other.f;
	}

	PathfinderGrid::PathfinderNode::PathfinderNode(int x, int y, bool walkable)
		: walkable(walkable),
		Node(x, y),
		visisted(false),
		closed(false),
		weight(1),
		g(0.f),
		f(0.f),
		parent(nullptr)
	{
	}
}