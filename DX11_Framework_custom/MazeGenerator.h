#pragma once
#include <stack>
#include <vector>
#include <assert.h>


constexpr uint8_t NORTHWALL = 0b0001;
constexpr uint8_t EASTWALL = 0b0010;
constexpr uint8_t SOUTHWALL = 0b0100;
constexpr uint8_t WESTWALL = 0b1000;

class MazeGenerator
{
	class Cell
	{
	public:
		Cell()
			:
			wall( NORTHWALL | EASTWALL | SOUTHWALL | WESTWALL ),
			visited( true )
		{}

		void RemoveWall( const uint8_t Direction )
		{
			wall = ( wall & ( ~Direction ) );
		}
		bool WasVisited()const
		{
			return visited;
		}
	private:
		uint8_t wall;
		bool visited;
	};
public:
	MazeGenerator();
	~MazeGenerator();

	bool Initialize( unsigned int MazeWidth, unsigned int MazeHeight );

private:
	// private member functions
	unsigned int getRandomDirection()const;
	std::pair<unsigned int, unsigned int> GetRandomNeighbor( const unsigned int Index )const;
private:
	// private member data
	std::stack<int> m_visitedCells;
	std::vector<Cell> m_cells;
	unsigned int m_mazeWidth, m_mazeHeight;
};

