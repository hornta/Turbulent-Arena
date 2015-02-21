#pragma once

#define PI 3.14159265358979323846f
#define SQRT2 1.41421356237f;
#define DEGREES(rad) rad * 180 / PI
#define RADIANS(deg) deg * PI / 180

namespace bjoernligan
{
	int random(int min, int max);
	float random(float min, float max);

	template <typename T>
	T max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	template <typename T>
	T min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template <typename T>
	T abs(const T& a)
	{
		return a < 0 ? -a : a;
	}

	template <typename T>
	T clamp(const T& v, const T& a, const T& b)
	{
		return v < a ? a : v > b ? b : v;
	}

	float distanceBetweenPoints(float x0, float y0, float x1, float y1);
	float getDirectionFromPoints(float x0, float y0, float x1, float y1);
	std::vector<std::string> explode(const std::string& string, const std::string& delimeter);
	bool string_to_bool(const std::string& string);

	float floatToB2D(float f);
	float floatToPix(float f);
	b2Vec2 vectorToB2D(const sf::Vector2f& v);
	sf::Vector2f vectorToPix(const b2Vec2& v);
	
	template<typename T>
	class BinaryHeap
	{
	public:
		struct Node
		{
			T data;
		};

		BinaryHeap();
		void insert(const T& data);
		void remove(const T& data);
		T pop();
		std::size_t size() const;
		void updateItem(const T& data);
	private:
		virtual bool score(std::shared_ptr<Node> a, std::shared_ptr<Node> b) const = 0;
		void rise(std::size_t currentIndex);
		void sink(std::size_t i);

	private:
		std::vector<std::shared_ptr<Node>> m_nodes;
	};

	template<typename T>
	class BinaryMaxHeap : public BinaryHeap<T>
	{
		bool score(std::shared_ptr<Node> a, std::shared_ptr<Node> b) const
		{
			return *a->data > *b->data;
		}
	};

	template<typename T>
	class BinaryMinHeap : public BinaryHeap<T>
	{
		bool score(std::shared_ptr<Node> a, std::shared_ptr<Node> b) const
		{
			return *a->data < *b->data;
		}
	};

	template<typename T>
	BinaryHeap<T>::BinaryHeap()
	{

	}

	template<typename T>
	void BinaryHeap<T>::insert(const T& data)
	{
		std::shared_ptr<Node> n = std::make_shared<Node>();
		n->data = data;
		m_nodes.push_back(n);
		rise(m_nodes.size() - 1);
	}

	template<typename T>
	T BinaryHeap<T>::pop()
	{
		// Store first element so we can return it later
		std::shared_ptr<Node> result = m_nodes[0];

		// Get element at the end of the array
		std::shared_ptr<Node> end = m_nodes.back();
		m_nodes.pop_back();

		// If the heap is not empty, put the end node at the
		// start, and let it sink down
		if (m_nodes.size() > 0)
		{
			m_nodes[0] = end;
			sink(0);
		}

		return result->data;
	}

	template<typename T>
	std::size_t BinaryHeap<T>::size() const
	{
		return m_nodes.size();
	}

	template<typename T>
	void BinaryHeap<T>::updateItem(const T& data)
	{
		int index = -1;
		for (std::size_t i = 0; i < m_nodes.size(); ++i)
		{
			if (m_nodes[i]->data == data)
			{
				index = i;
				break;
			}
		}

		if (index > -1)
		{
			rise(index);
		}
	}

	template<typename T>
	void BinaryHeap<T>::rise(std::size_t currentIndex)
	{
		// Fetch the node that has to be moved
		std::shared_ptr<Node> currentNode = m_nodes[currentIndex];

		// When index == 0, node can not rise further
		while (currentIndex > 0)
		{
			// Find parent index
			std::size_t parentIndex = ((currentIndex + 1) >> 1) - 1;
			std::shared_ptr<Node> parentNode = m_nodes[parentIndex];

			// case min-heap: if parent is larger, swap them
			if (score(currentNode, parentNode))
			{
				// Swap nodes
				m_nodes[parentIndex] = currentNode;
				m_nodes[currentIndex] = parentNode;

				// Update current index to its parent index
				currentIndex = parentIndex;
			}
			else
				break;
		}
	}

	template<typename T>
	void BinaryHeap<T>::sink(std::size_t i)
	{
		std::shared_ptr<Node> currentNode = m_nodes[i];

		while (true)
		{
			int swap = -1;
			std::size_t child1Index = 2 * i + 1;
			std::size_t child2Index = 2 * i + 2;

			if (child1Index < m_nodes.size())
			{
				std::shared_ptr<Node> child1Node = m_nodes[child1Index];
				if (score(child1Node, currentNode))
				{
					swap = child1Index;
				}
			}

			if (child2Index < m_nodes.size())
			{
				std::shared_ptr<Node> child2Node = m_nodes[child2Index];
				if (score(child2Node, (swap == -1) ? currentNode : m_nodes[child1Index]))
				{
					swap = child2Index;
				}
			}

			if (swap != -1)
			{
				m_nodes[i] = m_nodes[swap];
				m_nodes[swap] = currentNode;
				i = swap;
			}
			else
			{
				break;
			}
		}
	}

	struct PathfinderInfo
	{
		enum Heuristic
		{
			HEURISTIC_MANHATTAN,
			HEURISTIC_EUCLIDEAN,
			HEURISTIC_DOODLE,
			HEURISTIC_DIAGONAL,
			HEURISTIC_CHEBYSHEV
		};

		enum Algorithm
		{
			ALGORITHM_ASTAR,
			ALGORITHM_JPS
		};


		enum PathResult
		{
			PATHRESULT_SUCCEEDED,
			PATHRESULT_FAILED
		};

		enum DiagonalMovement
		{
			DIAGONAL_NEVER,
			DIAGONAL_NO_OBSTACLES,
			DIAGONAL_ONE_OBSTACLE,
			DIAGONAL_ALWAYS
		};
	};
	class Grid
	{
	public:
		struct Node
		{
			Node();
			Node(int x, int y);

			int x;
			int y;
		};

		Grid(int width, int height);
		~Grid();

		Node* getNodeAt(int x, int y) const;
		bool isInside(int x, int y) const;
		std::vector<Node*> getNeighbors(Node* n, bool diagonals = false) const;

	protected:
		Node** m_nodes;
		int m_width;
		int m_height;
	};

	class PathfinderGrid : public Grid
	{
	public:

		struct PathfinderNode : public Node
		{
			PathfinderNode();
			bool walkable;
			bool visisted;
			bool closed;
			int weight;
			float g;
			float f;
			PathfinderNode* parent;

			bool operator<(const PathfinderNode& other) const;
			bool operator>(const PathfinderNode& other) const;
		};

		PathfinderGrid(int width, int height);

		bool isWalkableAt(int x, int y) const;
		void setWalkableAt(int x, int y, bool walkable);
		void resetNodes();
		std::vector<PathfinderNode*> getNeighbors(Node* n, PathfinderInfo::DiagonalMovement diagonalMovement, bool canCrossCorners = false) const;
	};

	template <typename T>
	class Vector2
	{
	public:
		Vector2();
		Vector2(T x, T y);
		template <typename U>
		explicit Vector2(const Vector2<U>& v);
		template <typename U>
		explicit Vector2(const sf::Vector2<U>& v);

		static Vector2<T> sub(const Vector2<T>& v0, const Vector2<T>& v1);
		static Vector2<T> add(const Vector2<T>& v0, const Vector2<T>& v1);
		static float dist(const Vector2<T>& v0, const Vector2<T>& v1);

		Vector2<T>& reAssign(T newX, T newY);
		Vector2<T>& reAssign(const Vector2<T>& v);
		Vector2<T>& normalize();
		Vector2<T>& limit(float max);
		float dist(const Vector2<T>& v) const;
		float dist(T x, T y) const;
		float length() const;
		float angle() const;
		float angle(const Vector2<T>& v) const;
		Vector2<T>& mult(float scalar);
		Vector2<T>& divide(float scalar);
		Vector2<T>& sub(const Vector2<T>& v);
		Vector2<T>& add(const Vector2<T>& v);

		T x;
		T y;
	};

	template <typename T>
	Vector2<T> operator -(const Vector2<T>& right);
	template <typename T>
	Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right);
	template <typename T>
	Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right);
	template <typename T>
	Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right);
	template <typename T>
	Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right);
	template <typename T>
	Vector2<T> operator *(const Vector2<T>& left, T right);
	template <typename T>
	Vector2<T> operator *(T left, const Vector2<T>& right);
	template <typename T>
	Vector2<T>& operator *=(Vector2<T>& left, T right);
	template <typename T>
	Vector2<T> operator /(const Vector2<T>& left, T right);
	template <typename T>
	Vector2<T>& operator /=(Vector2<T>& left, T right);
	template <typename T>
	bool operator ==(const Vector2<T>& left, const Vector2<T>& right);
	template <typename T>
	bool operator !=(const Vector2<T>& left, const Vector2<T>& right);

	template <typename T>
	Vector2<T>::Vector2() :
		x(0),
		y(0)
	{

	}

	template <typename T>
	Vector2<T>::Vector2(T x, T y) :
		x(x),
		y(y)
	{

	}

	template <typename T>
	template <typename U>
	Vector2<T>::Vector2(const Vector2<U>& v) :
		x(static_cast<T>(v.x)),
		y(static_cast<T>(v.y))
	{

	}

	template <typename T>
	template <typename U>
	Vector2<T>::Vector2(const sf::Vector2<U>& v) :
		x(static_cast<T>(v.x)),
		y(static_cast<T>(v.y))
	{

	}

	template <typename T>
	Vector2<T> Vector2<T>::sub(const Vector2<T>& v0, const Vector2<T>& v1)
	{
		return v0 - v1;
	}

	template <typename T>
	Vector2<T> Vector2<T>::add(const Vector2<T>& v0, const Vector2<T>& v1)
	{
		return v0 + v1;
	}

	template <typename T>
	float Vector2<T>::dist(const Vector2<T>& v0, const Vector2<T>& v1)
	{
		return v0.dist(v1);
	}

	template <typename T>
	Vector2<T>& Vector2<T>::reAssign(T newX, T newY)
	{
		x = newX;
		y = newY;
		return *this;
	}

	template <typename T>
	Vector2<T>& Vector2<T>::reAssign(const Vector2<T>& v)
	{
		reAssign(v.x, v.y);
		return *this;
	}

	template <typename T>
	Vector2<T>& Vector2<T>::normalize()
	{
		float len = length();
		if (len < std::numeric_limits<float>::epsilon())
		{
			x = 0;
			y = 0;
			return *this;
		}
		else
		{
			float invLen = 1.f / len;
			x *= invLen;
			y *= invLen;
			return *this;
		}
	}

	template <typename T>
	Vector2<T>& Vector2<T>::limit(float max)
	{
		if (length() > max)
			reAssign(normalize() * max);
		return *this;
	}

	template <typename T>
	float Vector2<T>::dist(const Vector2<T>& v) const
	{
		return sqrt((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y));
	}

	template <typename T>
	float Vector2<T>::dist(T x, T y) const
	{
		return dist(Vector2<T>(x, y));
	}

	template <typename T>
	float Vector2<T>::length() const
	{
		return sqrt(x * x + y * y);
	}

	template <typename T>
	float Vector2<T>::angle() const
	{
		return atan2(y, x);
	}

	template <typename T>
	float Vector2<T>::angle(const Vector2<T>& v) const
	{
		return atan2(v.y - y, v.x - x);
	}

	template <typename T>
	Vector2<T>& Vector2<T>::mult(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	template <typename T>
	Vector2<T>& Vector2<T>::divide(float scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	template <typename T>
	Vector2<T>& Vector2<T>::sub(const Vector2<T>& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	template <typename T>
	Vector2<T>& Vector2<T>::add(const Vector2<T>& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	template <typename T>
	Vector2<T> operator -(const Vector2<T>& right)
	{
		return Vector2<T>(-right.x, -right.y);
	}

	template <typename T>
	Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right)
	{
		left.x += right.x;
		left.y += right.y;
		return left;
	}

	template <typename T>
	Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right)
	{
		left.x -= right.x;
		left.y -= right.y;
		return left;
	}

	template <typename T>
	Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right)
	{
		return Vector2<T>(left.x + right.x, left.y + right.y);
	}

	template <typename T>
	Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right)
	{
		return Vector2<T>(left.x - right.x, left.y - right.y);
	}

	template <typename T>
	Vector2<T> operator *(const Vector2<T>& left, T right)
	{
		return Vector2<T>(left.x * right, left.y * right);
	}

	template <typename T>
	Vector2<T> operator *(T left, const Vector2<T>& right)
	{
		return Vector2<T>(right.x * left, right.y * left);
	}

	template <typename T>
	Vector2<T>& operator *=(Vector2<T>& left, T right)
	{
		left.x *= right;
		left.y *= right;
		return left;
	}

	template <typename T>
	Vector2<T> operator /(const Vector2<T>& left, T right)
	{
		return Vector2<T>(left.x / right, left.y / right);
	}

	template <typename T>
	Vector2<T>& operator /=(Vector2<T>& left, T right)
	{
		left.x /= right;
		left.y /= right;
		return left;
	}

	template <typename T>
	bool operator ==(const Vector2<T>& left, const Vector2<T>& right)
	{
		return (left.x == right.x) && (left.y == right.y);
	}

	template <typename T>
	bool operator !=(const Vector2<T>& left, const Vector2<T>& right)
	{
		return (left.x != right.x) || (left.y != right.y);
	}


	typedef Vector2<int> Vector2i;
	typedef Vector2<float> Vector2f;

}