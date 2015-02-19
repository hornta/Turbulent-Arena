#include "stdafx.h"
#include "Utils.hpp"

namespace bjoernligan
{
	namespace
	{
		std::random_device rd;
		std::mt19937 engine(rd());
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

	Grid::Grid(int width, int height)
		: m_width(width),
		m_height(height)
	{
		m_nodes = nullptr;
	}

	Grid::~Grid()
	{
		for (int i = 0; i < m_width * m_height; ++i)
		{
			delete m_nodes[i];
			m_nodes[i] = nullptr;
		}

		delete[] m_nodes;
		m_nodes = nullptr;
	}


	Grid::Node* Grid::getNodeAt(int x, int y) const
	{
		return m_nodes[x + y * m_width];
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
			neighbors.push_back(m_nodes[x + (y - 1) * m_width]);
		// down
		if (isInside(x, y + 1))
			neighbors.push_back(m_nodes[x + (y + 1) * m_width]);
		// left
		if (isInside(x - 1, y))
			neighbors.push_back(m_nodes[(x - 1) + y * m_width]);
		// down
		if (isInside(x + 1, y))
			neighbors.push_back(m_nodes[(x + 1) + y * m_width]);

		if (diagonals)
		{
			// up left
			if (isInside(x - 1, y - 1))
				neighbors.push_back(m_nodes[(x - 1) + (y - 1) * m_width]);
			// up right
			if (isInside(x + 1, y - 1))
				neighbors.push_back(m_nodes[(x + 1) + (y - 1) * m_width]);
			// down left
			if (isInside(x - 1, y + 1))
				neighbors.push_back(m_nodes[(x - 1) + (y + 1) * m_width]);
			// down right
			if (isInside(x + 1, y + 1))
				neighbors.push_back(m_nodes[(x + 1) + (y + 1) * m_width]);
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
		m_nodes = new Node*[m_width * m_height];
		for (int x = 0; x < m_width; ++x)
		{
			for (int y = 0; y < m_height; ++y)
			{
				PathfinderNode* n = new PathfinderNode();
				n->x = x;
				n->y = y;
				n->walkable = true;
				m_nodes[x + y * m_width] = n;
			}
		}
	}

	bool PathfinderGrid::isWalkableAt(int x, int y) const
	{
		return isInside(x, y) && static_cast<PathfinderNode*>(m_nodes[x + y * m_width])->walkable;
	}

	void PathfinderGrid::setWalkableAt(int x, int y, bool walkable)
	{
		static_cast<PathfinderNode*>(m_nodes[x + y * m_width])->walkable = walkable;
	}

	void PathfinderGrid::resetNodes()
	{
		for (int i = 0; i < m_width * m_height; ++i)
		{
			PathfinderNode* n = static_cast<PathfinderNode*>(m_nodes[i]);
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
			neighbors.push_back(static_cast<PathfinderNode*>(m_nodes[x + (y - 1) * m_width]));
			s0 = true;
		}
		// →
		if (isWalkableAt(x + 1, y))
		{
			neighbors.push_back(static_cast<PathfinderNode*>(m_nodes[(x + 1) + y * m_width]));
			s1 = true;
		}
		// ↓
		if (isWalkableAt(x, y + 1))
		{
			neighbors.push_back(static_cast<PathfinderNode*>(m_nodes[x + (y + 1) * m_width]));
			s2 = true;
		}
		// ←
		if (isWalkableAt(x - 1, y))
		{
			neighbors.push_back(static_cast<PathfinderNode*>(m_nodes[(x - 1) + y * m_width]));
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
			neighbors.push_back(static_cast<PathfinderNode*>(m_nodes[(x - 1) + (y - 1) * m_width]));
		// ↗
		if (d1 && isWalkableAt(x + 1, y - 1))
			neighbors.push_back(static_cast<PathfinderNode*>(m_nodes[(x + 1) + (y - 1) * m_width]));
		// ↘
		if (d2 && isWalkableAt(x + 1, y + 1))
			neighbors.push_back(static_cast<PathfinderNode*>(m_nodes[(x + 1) + (y + 1) * m_width]));
		// ↙
		if (d3 && isWalkableAt(x - 1, y + 1))
			neighbors.push_back(static_cast<PathfinderNode*>(m_nodes[(x - 1) + (y + 1) * m_width]));

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

	PathfinderGrid::PathfinderNode::PathfinderNode()
		: walkable(false),
		visisted(false),
		closed(false),
		weight(1),
		g(0.f),
		f(0.f),
		parent(nullptr)
	{
	}
}