#include "MazeGenerator.h"
#include <random>
#include "Board.h"

MazeGenerator::MazeGenerator()
{}


MazeGenerator::~MazeGenerator()
{}

void MazeGenerator::Initialize( int Width, int Height )
{
	m_size = { Width, Height };
}

void MazeGenerator::Generate( std::vector<Board::Cell>&Cells )
{
	auto start = getRandomStart();
	auto end = getRandomEnd();
	Coord curCoord( Coord::FromIndex( start, m_size.width ) );

	auto &startCell = Cells[ start ];
	startCell.Visited();
	m_visitedCells.push( startCell );

	while( !m_visitedCells.empty() )
	{
		// Get the currently visited cell
		auto &curCell = m_visitedCells.top();
		
		// Get available list of neghbor coordinates
		auto available = getAvailableNeighbors( curCoord, Cells );
		// If empty, then dead end or done generating
		if( available.empty() )
		{
			m_visitedCells.pop();
			continue;
		}

		// Choose random neighbor from avaible list of neighbor coordinates
		auto neighbor = getRandomNeighbor( available );

		// Set as next cell
		auto &nextCell = Cells[ neighbor.ToIndex( m_size.width ) ];

		// Knock down walls of current cell and next cell
		removeWalls( curCoord, neighbor, Cells );

		// Push nextCell on stack
		m_visitedCells.push( nextCell );

		// Set current coordinates to neighbor coordinates
		curCoord = neighbor;
	}

}

Coord MazeGenerator::GetStart() const
{
	return m_start;
}

Coord MazeGenerator::GetEnd() const
{
	return m_end;
}

std::vector<int> MazeGenerator::getAvailableNeighbors( 
	const Coord & C, const std::vector<Board::Cell>&Cells ) const
{
	std::vector<int> available;
	if( C.y - 1 >= 0 )
	{
		int idx = Coord( C.x, C.y - 1 ).ToIndex( m_size.width );
		if( !Cells[ idx ].WasVisited() )
		{
			available.push_back( idx );
		}
	}
	if( C.y + 1 < m_size.height )
	{
		int idx = Coord( C.x, C.y + 1 ).ToIndex( m_size.width );
		if( !Cells[ idx ].WasVisited() )
		{
			available.push_back( idx );
		}
	}
	if( C.x - 1 >= 0 )
	{
		int idx = Coord( C.x - 1, C.y ).ToIndex( m_size.width );
		if( !Cells[ idx ].WasVisited() )
		{
			available.push_back( idx );
		}
	}
	if( C.y + 1 < m_size.width )
	{
		int idx = Coord( C.x + 1, C.y ).ToIndex( m_size.width );
		if( !Cells[ idx ].WasVisited() )
		{
			available.push_back( idx );
		}
	}

	return available;
}

int MazeGenerator::getRandomStart() const
{
	return getRandomNumber( ( m_size.height - 1 ) * m_size.width, m_size.width );
}

int MazeGenerator::getRandomEnd() const
{
	return getRandomNumber(0, m_size.width);
}

int MazeGenerator::getRandomNumber( unsigned Start, unsigned Range ) const
{
	std::mt19937 rng;
	std::uniform_int_distribution<unsigned int> distro( Start, Start + Range);
	return distro( rng );
}

int MazeGenerator::getRandomDirection() const
{	
	return getRandomNumber( 0, 3 );
}

Coord MazeGenerator::getRandomNeighbor( const std::vector<int> &Available ) const
{
	int idx = -1;
	if( Available.size() == 1 )
	{
		idx = Available[ 0 ];
	}
	else
	{
		idx = getRandomNumber( 0, Available.size() - 1 );
	}

	return Coord::FromIndex( idx, m_size.width );
}

void MazeGenerator::removeWalls( const Coord & CurCell, 
	const Coord & NextCell, std::vector<Board::Cell>&Cells )
{
	auto x = NextCell.x - CurCell.x;
	auto y = NextCell.y - CurCell.y;
	int curIdx = CurCell.ToIndex( m_size.width );
	int nextIdx = NextCell.ToIndex( m_size.width );

	if( x > 0 )
	{
		Cells[ curIdx ].RemoveWall( Board::EASTWALL );
		Cells[ nextIdx ].RemoveWall( Board::WESTWALL );
	}
	else if( x < 0 )
	{
		Cells[ curIdx ].RemoveWall( Board::WESTWALL );
		Cells[ nextIdx ].RemoveWall( Board::EASTWALL );
	}
	else
	{
		if( y > 0 )
		{
			Cells[ curIdx ].RemoveWall( Board::SOUTHWALL );
			Cells[ nextIdx ].RemoveWall( Board::NORTHWALL );
		}
		else
		{
			Cells[ curIdx ].RemoveWall( Board::NORTHWALL );
			Cells[ nextIdx ].RemoveWall( Board::SOUTHWALL );
		}
	}
}
