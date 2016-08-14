#include "Algorithm_Maze.h"
#include <random>
#include "Game_Maze.h"

using namespace DirectX;

Algorithm_Maze::Algorithm_Maze( ISubGame*const pGame )
	:
	Algorithm( pGame )
{
}

vector<Actor> Algorithm_Maze::MakePattern( int numActors )
{
	// Retrieve the board from the game object
	// TODO: Try casting to child class of ISubGame
	auto *pMazeGame = dynamic_cast<Game_Maze *>( m_pGame );

	// Crash with error if cast didn't work, otherwise continue creating
	// maze
	assert( pMazeGame != nullptr );

	const auto &board = pMazeGame->GetBoard();

	// Generate maze
	auto tileTypes = 
		Generate( board.GetWidth(), board.GetHeight(), board.GetStartCellCoord(), 
			board.GetEndCellCoord(), m_visitedCells );

	return CreateActorList( board.GetWidth(), board.GetHeight(), tileTypes );
}

std::vector<XMINT2> Algorithm_Maze::MakeNeighborList(
	const DirectX::XMUINT2 & CurCell, 
	UINT Width, UINT Height, 
	std::vector<eTileType>& TileTypes )
{
	// Get available list of neghbor coordinates
	XMINT2 neighbors[]
	{
		{ static_cast<int>( CurCell.x ) + 0, static_cast<int>( CurCell.y ) + 2 },
		{ static_cast<int>( CurCell.x ) + 2, static_cast<int>( CurCell.y ) + 0 },
		{ static_cast<int>( CurCell.x ) + 0, static_cast<int>( CurCell.y ) + -2 },
		{ static_cast<int>( CurCell.x ) + -2,static_cast<int>( CurCell.y ) + 0 }
	};

	// Weed out unavailabe cells
	std::vector<XMINT2> availableNeighborList;
	for( int i = 0; i < 4; ++i )
	{
		if( ( ( neighbors[ i ].x >= 0 ) && ( neighbors[ i ].x < Width ) ) &&
			( ( neighbors[ i ].y >= 0 ) && ( neighbors[ i ].y < Height ) ) )
		{
			auto idx = neighbors[ i ].x + ( neighbors[ i ].y * Width );
			if( TileTypes[ idx ] != PATH )
			{
				availableNeighborList.push_back( neighbors[ i ] );
			}
		}
	}

	return availableNeighborList;
}

UINT Algorithm_Maze::GetWallIndex( const DirectX::XMUINT2 & CurCell, const DirectX::XMUINT2 & NextCell, const UINT Width, const UINT Height, const UINT CurIdx )
{
	int xDiff = static_cast<int>( NextCell.x ) - static_cast<int>( CurCell.x );
	int yDiff = static_cast<int>( NextCell.y ) - static_cast<int>( CurCell.y );
	int widthInt = static_cast<int>( Width );
	int heightInt = static_cast<int>( Height );

	UINT wallIndex = -1;
	if( xDiff > 0 )
	{
		// Knock down wall to the east
		wallIndex = CurIdx + 1;
	}
	else if( xDiff < 0 )
	{
		// Knowck down wall to the west
		wallIndex = CurIdx - 1;
	}
	else  // If next X - current X == 0, then only Y has changed
	{
		if( yDiff < 0 )
		{
			// Knock down wall to the north
			wallIndex = CurIdx - Width;
		}
		else if( yDiff > 0 )
		{
			// Knock down wall to the south
			wallIndex = CurIdx + Width;
		}
	}

	return wallIndex;
}

std::vector<eTileType> Algorithm_Maze::Generate( const UINT Width, const UINT Height, const DirectX::XMUINT2 & StartCell, const DirectX::XMUINT2 & EndCell, std::stack<DirectX::XMUINT2>& VisitedCells )
{
	const UINT startIndex = StartCell.x + ( StartCell.y * Width );
	const UINT endIndex = EndCell.x + ( EndCell.y * Width );

	// Initialize the vector for storing eTileType types
	std::vector<eTileType> tileTypes( Width * Height );
	tileTypes[ startIndex ] = PATH;
	tileTypes[ endIndex ] = PATH;

	// Push the starting cell coordinates on the stack
	VisitedCells.push( StartCell );

	// Run until the stack is empty
	while( !VisitedCells.empty() )
	{
		// Set top of stack to current cell
		auto curCell = VisitedCells.top();
		auto curIdx = curCell.x + ( curCell.y * Width );
		tileTypes[ curIdx ] = PATH;

		// Get a list of available neighbors to current cell
		auto availableNeighborList =
			MakeNeighborList( curCell, Width, Height, tileTypes );

		// If empty, then dead end or done generating
		if( availableNeighborList.empty() )
		{
			// Set current cell in tileTypes to PATH and pop it off the
			// stack
			tileTypes[ curIdx ] = PATH;
			VisitedCells.pop();
			continue;
		}

		// Choose random neighbor from avaible list of neighbor coordinates
		auto nextIdx = rand() % availableNeighborList.size();

		// Set as next cell
		XMUINT2 nextCell(
			availableNeighborList[ nextIdx ].x,
			availableNeighborList[ nextIdx ].y
		);

		// Change the tile type to PATH between current cell and next cell
		const UINT wallIndex =
			GetWallIndex( curCell, nextCell, Width, Height, curIdx );
		tileTypes[ wallIndex ] = PATH;

		// Set next tile to PATH
		tileTypes[ nextIdx ] = PATH;

		// Push nextCell on stack
		VisitedCells.push( nextCell );
	}

	return tileTypes;
}

std::vector<Actor> Algorithm_Maze::CreateActorList( const UINT Width, const UINT Height, const std::vector<eTileType>& TileTypes )
{
	// Create offset vector so cells aren't created from 0,0,0; but instead
	// created from (-width, 2, -height) / 2 to ( width, 2, height) / 2
	XMFLOAT3 offset = XMFLOAT3( Width, 2.f, Height ) * .5f;

	// Create actor list where walls are
	vector<Actor> actorList;
	ModelSpecs_L lSpecs;
	for( int z = 0; z < Height; ++z )
	{
		for( int x = 0; x < Width; ++x )
		{
			int idx = x + ( z * Width );
			ModelSpecs_W specs{};
			if( TileTypes[ idx ] == WALL )
			{
				specs.position.x = (static_cast<float>( x ) * 5.f) - offset.x;
				specs.position.y = offset.y;
				specs.position.z = ( static_cast<float>( z ) * 5.f ) - offset.z;

				specs.scale = { 1.f, 1.f, 1.f };

				actorList.push_back( Actor( specs, AsphaltOld, lSpecs ) );
			}
		}
	}

	return actorList;
}
