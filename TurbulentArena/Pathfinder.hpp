#pragma once

namespace bjoernligan
{
	class Pathfinder
	{
	public:

		struct Options
		{
			Options()
				:minimizePath(false),
				canCrossCorners(false),
				diagonal(PathfinderInfo::DIAGONAL_NEVER),
				heuristic(PathfinderInfo::HEURISTIC_MANHATTAN),
				algorithm(PathfinderInfo::ALGORITHM_ASTAR)
			{

			}
			bool canCrossCorners;
			bool minimizePath;
			PathfinderInfo::DiagonalMovement diagonal;
			PathfinderInfo::Heuristic heuristic;
			PathfinderInfo::Algorithm algorithm;
		};

		struct PathNode
		{
			int x;
			int y;
		};

		class Path
		{
			friend class Pathfinder;
		public:
			Path()
				: currentNode(0)
			{

			}

			std::size_t length;
			std::size_t currentNode;
			sf::Time time;
			std::vector<PathNode> nodes;

		private:
			void reverse()
			{
				std::reverse(nodes.begin(), nodes.end());
			}
		};

		Pathfinder(int width, int height);
		Pathfinder(const sf::Vector2i& size);

		void setStart(int x, int y);
		void setStart(const sf::Vector2i& position);
		void setStart(const Vector2i& position);
		void setGoal(int x, int y);
		void setGoal(const sf::Vector2i& position);
		void setGoal(const Vector2i& position);
		PathfinderInfo::PathResult findPath(Path& path, const Options& options = Options());
		PathfinderGrid& getGrid();
	private:
		void makePath(PathfinderGrid::PathfinderNode* n, Path& path, bool minimify);
		float getHeuristic(int x1, int y1, int x2, int y2, PathfinderInfo::Heuristic heuristic = PathfinderInfo::HEURISTIC_MANHATTAN);

		PathfinderInfo::PathResult astar(Path& path, const Options& options);
		PathfinderInfo::PathResult jps(Path& path, const Options& options);

		void JPS_identifySuccessors(PathfinderGrid::PathfinderNode* n, BinaryMinHeap<PathfinderGrid::PathfinderNode*>& openList, const Pathfinder::Options& options);
		std::vector<PathfinderGrid::PathfinderNode*> JPS_getNeighbors(PathfinderGrid::PathfinderNode* n, const Options& options);
		PathfinderGrid::PathfinderNode* JPS_jump(int x, int y, int px, int py);
		unsigned int m_width;
		unsigned int m_height;
		unsigned int m_numNodes;
		PathfinderGrid::PathfinderNode* m_start;
		PathfinderGrid::PathfinderNode* m_goal;
		PathfinderGrid m_grid;
	};

}
