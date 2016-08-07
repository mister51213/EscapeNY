#include "MazeGenerator.h"
#include <random>
#include "Board.h"
#include "Game.h"
#include "TestBoard.h"

using namespace DirectX;
MazeGenerator::MazeGenerator()
{}

MazeGenerator::MazeGenerator( Game * pGame )
	:
	Algorithm(pGame)
{
}


MazeGenerator::~MazeGenerator()
{}

void MazeGenerator::Initialize( int Width, int Height )
{
	m_size = { Width, Height };
}

vector<Actor> MazeGenerator::MakePattern( int numActors )
{
	std::vector<Actor> actorList;
	/*auto &board = m_pGame->GetBoard();
	auto &cellList = board.GetCellArray();
	auto width = board.GetWidth();
	auto height = board.GetHeight();
	
	for( int iz = 0; iz < height; ++iz )
	{
		for( int ix = 0; ix < width; ++ix )
		{
			int idx = ix + ( iz * width );

			ModelSpecs_W specs;
			specs.position.x = ix;
			specs.position.y = 1.f;
			specs.position.z = iz;

			cellList.push_back( Board::Cell( specs ) );
		}
	}
	Initialize( width, height );
	Generate( cellList );

	board.SetStartAndEnd( 
		m_start.ToIndex( m_size.width ),
		m_end.ToIndex( m_size.width ) );
*/
	return actorList;
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

Algorithm_Maze::Algorithm_Maze( Game * pGame )
	:
	Algorithm(pGame)
{
}

vector<Actor> Algorithm_Maze::MakePattern( int numActors )
{
	// Setup the algorithm
	auto board = m_pGame->GetBoard();	
	
	auto startCell = board.GetStartCellCoord();
	auto endCell = board.GetEndCellCoord();
	auto width = board.GetWidth();
	auto height = board.GetHeight();

	std::vector<eTileType> tileTypes( width * height );

	const UINT startIndex = startCell.x + ( startCell.y * width );
	const UINT endIndex = endCell.x + ( endCell.y * width );
	
	tileTypes[ startIndex ] = PATH;
	tileTypes[ endIndex ] = PATH;
	
	m_visitedCells.push( startCell );

	while( !m_visitedCells.empty() )
	{
		// Set top of stack to current cell
		auto curCell = m_visitedCells.top();
		auto curIdx = curCell.x + ( curCell.y * width );
		tileTypes[ curIdx ] = PATH;

		// Get available list of neghbor coordinates
		XMINT2 neighbors[]
		{ 
			{ static_cast<int>( curCell.x ) + 0, static_cast<int>( curCell.y ) + 2 },
			{ static_cast<int>( curCell.x ) + 2, static_cast<int>( curCell.y ) + 0 },
			{ static_cast<int>( curCell.x ) + 0, static_cast<int>( curCell.y ) + -2 },
			{ static_cast<int>( curCell.x ) + -2,static_cast<int>( curCell.y ) + 0 }
		};

		std::vector<XMINT2> availableNeighborList;
		for( int i = 0; i < 4; ++i )
		{
			if( ( ( neighbors[ i ].x >= 0 ) && ( neighbors[ i ].x < width ) ) &&
				( ( neighbors[ i ].y >= 0 ) && ( neighbors[ i ].y < height ) ) )
			{
				auto idx = neighbors[ i ].x + ( neighbors[ i ].y * width );
				if( tileTypes[ idx ] != PATH )
				{
					availableNeighborList.push_back( neighbors[ i ] );
				}
			}
		}

		// If empty, then dead end or done generating
		if( availableNeighborList.empty() )
		{			
			tileTypes[ curIdx ] = PATH;
			
			m_visitedCells.pop();
			continue;
		}

		// Choose random neighbor from avaible list of neighbor coordinates
		auto nextIdx = rand() % availableNeighborList.size();

		// Set as next cell
		XMUINT2 nextCell(
			availableNeighborList[ nextIdx ].x,
			availableNeighborList[ nextIdx ].y
		);

		// Knock down wall between current cell and next cell
		// Unsigned int has no negative numbers, so result of subtraction
		// is either going to be between 0 and 2^32 - 1.  If greater than
		// height, then the result would have been negative.
		int xDiff = static_cast<int>( nextCell.x ) - static_cast<int>( curCell.x );
		int yDiff = static_cast<int>( nextCell.y ) - static_cast<int>( curCell.y );
		int widthInt = static_cast<int>( width );
		int heightInt = static_cast<int>( height );

		if( xDiff > 0 )
		{
			// Knock down wall to the east
			tileTypes[ curIdx + 1 ] = PATH;
		}
		else if( xDiff < 0)
		{
			// Knowck down wall to the west
			tileTypes[ curIdx - 1 ] = PATH;
		}
		else  // If next X - current X == 0, then only Y has changed
		{
			if( yDiff < 0 )
			{
				// Knock down wall to the north
				tileTypes[ curIdx - width ] = PATH;
			}
			else if( yDiff > 0 )
			{
				// Knock down wall to the south
				tileTypes[ curIdx + width ] = PATH;
			}
		}
		tileTypes[ nextIdx ] = PATH;

		// Push nextCell on stack
		m_visitedCells.push( nextCell );
	}

	// Create actor list where walls are
	vector<Actor> actorList;
	for( int z = 0; z < height; ++z )
	{
		for( int x = 0; x < width; ++x )
		{
			int idx = x + ( z * width );
			ModelSpecs_W specs{};
			if( tileTypes[ idx ] == WALL )
			{
				specs.position.x = static_cast<float>( x ) * 5.f;
				specs.position.y = 1.f;
				specs.position.z = static_cast<float>( z ) * 5.f;

				specs.scale = { 1.f, 1.f, 1.f };

				actorList.push_back( Actor( specs ) );
			}
		}
	}

	return actorList;
}
