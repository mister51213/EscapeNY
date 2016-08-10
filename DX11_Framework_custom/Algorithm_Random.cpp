#include "Algorithm_Random.h"
#include "MazeGame.h"


Algorithm_Random::Algorithm_Random( ISubGame*const pGame )
	:
	Algorithm( pGame )
{
}


Algorithm_Random::~Algorithm_Random()
{
}

vector<Actor> Algorithm_Random::MakePattern( int numActors )
{	
	// 0 is vertical, 1 is horizontal
	const unsigned vert = 0;
	const unsigned horiz = 1;

	const UINT carCount = numActors;

	MazeGame *pGame = dynamic_cast<MazeGame *>( m_pGame );
	assert( pGame );

	const auto &board = pGame->GetBoard();
	const auto &width = board.GetWidth();
	const auto &height = board.GetHeight();
	const UINT tileCount = width * height;
	auto start = board.GetStartCellCoord();
	auto end = board.GetEndCellCoord();
	const int startIndex = start.x + ( start.y * width );
	const int endIndex = end.x + ( end.y * width );

	std::vector<Actor> actors;
	const auto &cellSize = board.GetLocalSpecs().size.x;

	ModelSpecs_L lSpecs{};
	lSpecs.size = XMFLOAT3( cellSize, cellSize, cellSize ) * 0.8f;

	std::vector<eTileType> tileTypes;
	for( int i = 0; i < tileCount; ++i )
	{
		tileTypes.push_back( eTileType::PATH );
	}
	tileTypes[ startIndex ] = WALL;
	tileTypes[ endIndex ] = WALL;

	for( int i = 0; i < carCount; ++i )
	{
		auto dir = rand() % 2;

		auto hIdxList = std::vector<uint32_t>();
		auto vIdxList = std::vector<uint32_t>();

		for( int y = 0; y < height; ++y )
		{
			for( int x = 0; x < width; ++x )
			{
				uint32_t jx = x + 1;
				uint32_t ky = y + 1;
				uint32_t i = x + ( y * width );
				uint32_t j = jx + ( y * width );
				uint32_t k = x + ( ky * width );

				if( dir == horiz )
				{
					if( jx < width )
					{
						if( ( tileTypes[ i ] == PATH ) && ( tileTypes[ j ] == PATH ) )
						{
							hIdxList.push_back( i );
						}
					}
				}
				else
				{
					if( ky < height )
					{
						if( ( tileTypes[ i ] == PATH ) && ( tileTypes[ j ] == PATH ) )
						{
							vIdxList.push_back( i );
						}
					}
				}
			}
		}

		int idx0 = 0, idx1 = 0;
		/*std::vector<uint32_t> *list[2];
		list[ 0 ] = &hIdxList;
		list[ 1 ] = &vIdxList;
		int randIndex = list[ dir ]->size();
		const int offset = dir == 0 ? 1 : width;

		if( !list[ dir ]->empty() )
		{
			idx0 = list[ dir ]->at( randIndex );
			idx1 = idx0 + offset;
		}*/
		if( dir == horiz )
		{
			if( !hIdxList.empty() )
			{
				idx0 = hIdxList[ rand() % hIdxList.size() ];
				idx1 = idx0 + 1;
			}
		}
		else
		{
			if( !vIdxList.empty() )
			{
				idx0 = vIdxList[ rand() % vIdxList.size() ];
				idx1 = idx0 + width;
			}
		}

		tileTypes[ idx0 ] = WALL;
		tileTypes[ idx1 ] = WALL;

		ModelSpecs_W wSpecs{};
		if( dir == horiz )
		{
			wSpecs.scale = { 2.f, 1.f, 1.f };
		}
		else
		{
			wSpecs.scale = { 1.f, 1.f, 2.f };
		}

		float x = ( idx0 % width ) * cellSize;
		float z = ( idx0 / width ) * cellSize;
		wSpecs.position = { x, 1.f, z };

		actors.push_back( Actor( wSpecs, Water, lSpecs ) );
	}

	return actors;
}
