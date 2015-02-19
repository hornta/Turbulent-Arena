#include "stdafx.h"
#include "Pathfinder.hpp"

namespace bjoernligan
{

	Pathfinder::Pathfinder(int width, int height)
		: m_width(width),
		m_height(height),
		m_numNodes(m_width * m_height),
		m_start(nullptr),
		m_goal(nullptr),
		m_grid(width, height)
	{
	}

	Pathfinder::Pathfinder(const sf::Vector2i& size)
		: m_width(size.x),
		m_height(size.y),
		m_numNodes(m_width * m_height),
		m_start(nullptr),
		m_goal(nullptr),
		m_grid(size.x, size.y)
	{

	}

	void Pathfinder::setStart(int x, int y)
	{
		if (m_grid.isInside(x, y))
			m_start = static_cast<PathfinderGrid::PathfinderNode*>(m_grid.getNodeAt(x, y));
	}

	void Pathfinder::setStart(const Vector2i& position)
	{
		setStart(position.x, position.y);
	}

	void Pathfinder::setGoal(int x, int y)
	{
		if (m_grid.isInside(x, y))
			m_goal = static_cast<PathfinderGrid::PathfinderNode*>(m_grid.getNodeAt(x, y));
	}

	void Pathfinder::setGoal(const Vector2i& position)
	{
		setGoal(position.x, position.y);
	}

	PathfinderInfo::PathResult Pathfinder::findPath(Pathfinder::Path& path, const Options& options)
	{
		sf::Clock clock;
		PathfinderInfo::PathResult result;
		switch (options.algorithm)
		{
		case PathfinderInfo::ALGORITHM_ASTAR:
			result = astar(path, options);
			break;
		case PathfinderInfo::ALGORITHM_JPS:
			result = jps(path, options);
			break;
		default:
			result = PathfinderInfo::PathResult::PATHRESULT_FAILED;
			break;
		}
		path.time = clock.getElapsedTime();
		m_grid.resetNodes();
		return result;
	}

	PathfinderGrid& Pathfinder::getGrid()
	{
		return m_grid;
	}

	void Pathfinder::makePath(PathfinderGrid::PathfinderNode* n, Path& path, bool minimify = false)
	{
		PathfinderGrid::PathfinderNode* current = n;
		PathNode pathnode;
		path.currentNode = 0;
		path.nodes.clear();

		if (minimify)
		{
			pathnode.x = n->x;
			pathnode.y = n->y;
			path.nodes.push_back(pathnode);
		}

		while (true)
		{
			if (current == nullptr)
				break;
			pathnode.x = current->x;
			pathnode.y = current->y;

			if (minimify)
			{
				PathfinderGrid::PathfinderNode* parent = current->parent;
				if (parent != nullptr)
				{
					int prev_x = path.nodes.back().x;
					int prev_y = path.nodes.back().y;

					int next_x = parent->x;
					int next_y = parent->y;

					if (prev_x == current->x && prev_x == next_x && current->x == next_x ||
						prev_y == current->y && prev_y == next_y && current->y == next_y)
					{

					}
					else
						path.nodes.push_back(pathnode);
				}
			}

			else
				path.nodes.push_back(pathnode);
			current = current->parent;
		}

		path.length = path.nodes.size();
		path.reverse();
	}

	float Pathfinder::getHeuristic(int x0, int y0, int x1, int y1, PathfinderInfo::Heuristic heuristic)
	{
		if (heuristic == PathfinderInfo::HEURISTIC_MANHATTAN)
		{
			return static_cast<float>(abs(x1 - x0) + abs(y1 - y0)) * 1000.f;
		}
		else if (heuristic == PathfinderInfo::HEURISTIC_DIAGONAL)
		{
			float D = 1;
			float D2 = SQRT2;
			float d1 = static_cast<float>(std::abs(x1 - x0));
			float d2 = static_cast<float>(std::abs(y1 - y0));
			return (D * (d1 + d2)) + ((D2 - (2 * D)) * min(d1, d2));
		}
		else if (heuristic == PathfinderInfo::HEURISTIC_EUCLIDEAN)
		{
			return distanceBetweenPoints(static_cast<float>(x0), static_cast<float>(y0), static_cast<float>(x1), static_cast<float>(y1));
		}
		else if (heuristic == PathfinderInfo::HEURISTIC_DOODLE)
		{
			return distanceBetweenPoints(static_cast<float>(x0), static_cast<float>(x1), static_cast<float>(y0), static_cast<float>(y1));
		}
		else
			return 0.f;
	}

	PathfinderInfo::PathResult Pathfinder::astar(Path& path, const Options& options)
	{
		BinaryMinHeap<PathfinderGrid::PathfinderNode*> openList;

		m_start->g = 0.f;
		m_start->f = 0.f;
		openList.insert(m_start);
		while (openList.size() != 0)
		{
			PathfinderGrid::PathfinderNode* currentNode = openList.pop();
			currentNode->closed = true;

			if (currentNode == m_goal)
			{
				makePath(currentNode, path, options.minimizePath);
				return PathfinderInfo::PATHRESULT_SUCCEEDED;
			}

			std::vector<PathfinderGrid::PathfinderNode*> neighbors = m_grid.getNeighbors(currentNode, options.diagonal);
			for (unsigned int i = 0; i < neighbors.size(); ++i)
			{
				PathfinderGrid::PathfinderNode* neighbor = neighbors[i];

				if (neighbor->closed)
					continue;

				// new G is equal to current G + distance between current and neighbor
				float newG = currentNode->g + getHeuristic(neighbor->x, neighbor->y, currentNode->x, currentNode->y, options.heuristic);
				bool beenVisisted = neighbor->visisted;

				// if node hasn't been visisted before or 
				// can be reached with smaller cost from the current node
				if (!beenVisisted || newG < neighbor->g)
				{
					neighbor->parent = currentNode;
					neighbor->g = newG;
					neighbor->f = neighbor->g + getHeuristic(neighbor->x, neighbor->y, m_goal->x, m_goal->y, options.heuristic);

					if (!beenVisisted)
					{
						neighbor->visisted = true;
						openList.insert(neighbor);
					}
					else
					{
						// the neighbor can be reached with smaller cost
						// since F has been updated, update position in open list
						openList.updateItem(neighbor);
					}
				}
			}
		}
		return PathfinderInfo::PATHRESULT_FAILED;
	}

	PathfinderInfo::PathResult Pathfinder::jps(Path& path, const Options& options)
	{
		BinaryMinHeap<PathfinderGrid::PathfinderNode*> openList;
		openList.insert(m_start);

		while (openList.size() != 0)
		{
			PathfinderGrid::PathfinderNode* currentNode = openList.pop();
			currentNode->closed = true;

			if (currentNode == m_goal)
			{
				makePath(currentNode, path);
				return PathfinderInfo::PATHRESULT_SUCCEEDED;
			}

			JPS_identifySuccessors(currentNode, openList, options);
		}
		return PathfinderInfo::PATHRESULT_FAILED;
	}

	void Pathfinder::JPS_identifySuccessors(PathfinderGrid::PathfinderNode* node, BinaryMinHeap<PathfinderGrid::PathfinderNode*>& openList, const Pathfinder::Options& options)
	{
		std::vector<PathfinderGrid::PathfinderNode*> neighbors = JPS_getNeighbors(node, options);
		PathfinderGrid::PathfinderNode* jumpPoint = nullptr;
		int px = node->x;
		int py = node->y;
		int x = 0;
		int y = 0;
		int jx = 0;
		int jy = 0;

		for (std::size_t i = 0; i < neighbors.size(); ++i)
		{
			PathfinderGrid::PathfinderNode* n = neighbors[i];
			x = n->x;
			y = n->y;
			jumpPoint = JPS_jump(x, y, px, py);
			if (jumpPoint != nullptr)
			{
				jx = jumpPoint->x;
				jy = jumpPoint->y;

				if (jumpPoint->closed)
					continue;

				float distance = getHeuristic(x, y, jx, jy, options.heuristic);
				float gScore = node->g + distance;

				if (!jumpPoint->visisted || gScore < jumpPoint->g)
				{
					jumpPoint->g = gScore;
					jumpPoint->f = jumpPoint->g + getHeuristic(x, y, m_goal->x, m_goal->y, options.heuristic);
					jumpPoint->parent = node;

					if (!jumpPoint->visisted)
					{
						jumpPoint->visisted = true;
						openList.insert(jumpPoint);
					}
					else
					{
						openList.updateItem(jumpPoint);
					}
				}
			}
		}
	}

	std::vector<PathfinderGrid::PathfinderNode*> Pathfinder::JPS_getNeighbors(PathfinderGrid::PathfinderNode* n, const Options& options)
	{
		PathfinderGrid::PathfinderNode* parent = n->parent;
		int dx = 0;
		int dy = 0;
		int x = n->x;
		int y = n->y;
		int px = 0;
		int py = 0;
		std::vector<PathfinderGrid::PathfinderNode*> neighbors;

		if (n->parent == nullptr)
		{
			neighbors = m_grid.getNeighbors(n, options.diagonal);
		}
		else
		{
			px = parent->x;
			py = parent->y;

			dx = (x - px) / max(abs(x - px), 1);
			dy = (y - py) / max(abs(y - py), 1);

			// search diagonally
			if (dx != 0 && dy != 0)
			{
				if (m_grid.isWalkableAt(x, y + dy))
					neighbors.push_back(static_cast<PathfinderGrid::PathfinderNode*>(m_grid.getNodeAt(x, y + dy)));

				if (m_grid.isWalkableAt(x + dx, y))
					neighbors.push_back(static_cast<PathfinderGrid::PathfinderNode*>(m_grid.getNodeAt(x + dx, y)));

				if (m_grid.isWalkableAt(x + dx, y) && m_grid.isWalkableAt(x, y + dy))
					neighbors.push_back(static_cast<PathfinderGrid::PathfinderNode*>(m_grid.getNodeAt(x + dx, y + dy)));
			}
			// search horizontally / verically
			else
			{
				bool isNextWalkable;
				if (dx != 0)
				{
					isNextWalkable = m_grid.isWalkableAt(x + dx, y);
					bool isTopWalkable = m_grid.isWalkableAt(x, y + 1);
					bool isBottomWalkable = m_grid.isWalkableAt(x, y - 1);

					if (isNextWalkable)
					{
						neighbors.push_back(static_cast<PathfinderGrid::PathfinderNode*>(m_grid.getNodeAt(x + dx, y)));
						if (isTopWalkable)
							neighbors.push_back(static_cast<PathfinderGrid::PathfinderNode*>(m_grid.getNodeAt(x + dx, y + 1)));
						if (isBottomWalkable)
							neighbors.push_back(static_cast<PathfinderGrid::PathfinderNode*>(m_grid.getNodeAt(x + dx, y - 1)));
					}

					if (isTopWalkable)
						neighbors.push_back(static_cast<PathfinderGrid::PathfinderNode*>(m_grid.getNodeAt(x, y + 1)));
					if (isBottomWalkable)
						neighbors.push_back(static_cast<PathfinderGrid::PathfinderNode*>(m_grid.getNodeAt(x, y - 1)));
				}
				else if (dy != 0)
				{
					isNextWalkable = m_grid.isWalkableAt(x, y + dy);
					bool isRightWalkable = m_grid.isWalkableAt(x + 1, y);
					bool isLeftWalkable = m_grid.isWalkableAt(x - 1, y);

					if (isNextWalkable)
					{
						neighbors.push_back(static_cast<PathfinderGrid::PathfinderNode*>(m_grid.getNodeAt(x, y + dy)));
						if (isRightWalkable)
							neighbors.push_back(static_cast<PathfinderGrid::PathfinderNode*>(m_grid.getNodeAt(x + 1, y + dy)));
						if (isLeftWalkable)
							neighbors.push_back(static_cast<PathfinderGrid::PathfinderNode*>(m_grid.getNodeAt(x - 1, y + dy)));
					}

					if (isRightWalkable)
						neighbors.push_back(static_cast<PathfinderGrid::PathfinderNode*>(m_grid.getNodeAt(x + 1, y)));
					if (isLeftWalkable)
						neighbors.push_back(static_cast<PathfinderGrid::PathfinderNode*>(m_grid.getNodeAt(x - 1, y)));
				}
			}
		}
		return neighbors;
	}

	PathfinderGrid::PathfinderNode* Pathfinder::JPS_jump(int x, int y, int px, int py)
	{
		int dx = x - px;
		int dy = y - py;
		PathfinderGrid::PathfinderNode* current = static_cast<PathfinderGrid::PathfinderNode*>(m_grid.getNodeAt(x, y));

		if (!m_grid.isWalkableAt(x, y))
			return nullptr;

		if (current == m_goal)
			return current;

		// check for forced neighbors along the diagonal
		if (dx != 0 && dy != 0)
		{
			/*if ((m_grid.isWalkableAt(x - dx, y + dy) && !m_grid.isWalkableAt(x - dx, y)) ||
			(m_grid.isWalkableAt(x + dy, y - dy) && !m_grid.isWalkableAt(x, y - dy)))
			return current;
			*/
			// when moving diagonally, check for vertical/horizontal jump points
			if (JPS_jump(x + dx, y, x, y) != nullptr || JPS_jump(x, y + dy, x, y) != nullptr)
				return current;
		}
		// horizontally/vertically
		else
		{
			if (dx != 0)
			{
				if ((m_grid.isWalkableAt(x, y - 1) && !m_grid.isWalkableAt(x - dx, y - 1)) ||
					(m_grid.isWalkableAt(x, y + 1) && !m_grid.isWalkableAt(x - dx, y + 1)))
					return current;
			}
			else if (dy != 0)
			{
				if ((m_grid.isWalkableAt(x - 1, y) && !m_grid.isWalkableAt(x - 1, y - dy)) ||
					(m_grid.isWalkableAt(x + 1, y) && !m_grid.isWalkableAt(x + 1, y - dy)))
					return current;
			}
		}

		// moving diagonally must make sure on of the vertical/horizontal neighbors is open to allow the path
		if (m_grid.isWalkableAt(x + dx, y) || m_grid.isWalkableAt(x, y + dy))
			return JPS_jump(x + dx, y + dy, x, y);
		return nullptr;
	}
}