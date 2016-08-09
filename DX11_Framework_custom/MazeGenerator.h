#pragma once
#include <stack>
#include <vector>
#include <assert.h>
#include "Algorithm.h"
#include "Board.h"


struct Coord
{
	Coord()
		:
		x(0),
		y(0)
	{
	}
	Coord( int X, int Y )
		:
		x( X ),
		y( Y )
	{
	}
	static Coord FromIndex( int Idx, int Width )
	{
		return Coord(
			Idx % Width,
			Idx / Width 
		);
	}
	int ToIndex( int Width )const
	{
		return ( x + ( y * Width ) );
	}
	int x, y;
};

struct Size
{
	Size() = default;
	Size( int Width, int Height )
		:
		width( Width ),
		height( Height )
	{
	}
	int Area()const
	{
		return width * height;
	}
	int width, height;
};


class MazeGenerator :public Algorithm
{
public:
	MazeGenerator();
	MazeGenerator( class Game *pGame );
	~MazeGenerator();

	void Initialize( int Width, int Height );

	vector<Actor> MakePattern( int numActors )override;
	void SetData()override
	{}

	void Generate( std::vector<Board::Cell>&Cells );
	Coord GetStart()const;
	Coord GetEnd()const;
// private member functions
private:
	std::vector<int> getAvailableNeighbors( const Coord &C, 
		const std::vector<Board::Cell>&Cells )const;
	int getRandomStart()const;
	int getRandomEnd()const;
	int getRandomNumber( unsigned Start, unsigned Range )const;
	int getRandomDirection()const;
	Coord getRandomNeighbor( const std::vector<int> &Available )const;
	void removeWalls( const Coord &CurCell, const Coord &NextCell,
		std::vector<Board::Cell>&Cells );
private:
	// private member data
	std::stack<class Board::Cell> m_visitedCells;
	Size m_size;
	Coord m_start, m_end;
};

class Algorithm_Maze :public Algorithm
{
public:
	Algorithm_Maze( class Game *pGame );
	vector<Actor> MakePattern( int numActors )override;
	void SetData()override
	{}
private:
	std::stack<DirectX::XMUINT2> m_visitedCells;

};