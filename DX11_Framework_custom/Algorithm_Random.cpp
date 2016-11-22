#include "Algorithm_Random.h"
#include "Scene_Maze.h"
#include "MathUtils.h"

Algorithm_Random::Algorithm_Random( ISubGame*const pGame )
	:
	Algorithm( pGame )
{
}

vector<Actor> Algorithm_Random::MakePattern( int numActors )
{	
	// 0 is vertical, 1 is horizontal
	const unsigned vert = 0;
	const unsigned horiz = 1;

	std::vector<Actor> actors;
	const UINT carCount = numActors;

	UINT width = 0, height = 0;
	ModelSpecs_L lSpecs{};
	std::vector<eTileType> tileTypes;
	float cellSize = 0.f;
	{
		Scene_Maze *pGame = dynamic_cast<Scene_Maze *>( m_pGame );
		assert( pGame );

		const auto &board = pGame->GetBoard();
		width = board.GetWidth();
		height = board.GetHeight();

		const auto tileCount = width * height;

		const auto start = board.GetStartCellCoord();
		const auto end = board.GetEndCellCoord();
		const int startIndex = start.x + ( start.y * width );
		const int endIndex = end.x + ( end.y * width );

		for( int i = 0; i < tileCount; ++i )
		{
			tileTypes.push_back( eTileType::PATH );
		}
		tileTypes[ startIndex ] = WALL;
		tileTypes[ endIndex ] = WALL;

		const XMFLOAT3 xmCellSize = board.GetCellSize();
		lSpecs.size = xmCellSize * 0.8f;
		cellSize = xmCellSize.x;
	}
	
	for( int i = 0; i < carCount; ++i )
	{
		const auto dir = rand() % 2;

		auto hIdxList = std::vector<uint32_t>();
		auto vIdxList = std::vector<uint32_t>();

		for( int y = 0; y < height; ++y )
		{
			for( int x = 0; x < width; ++x )
			{
				const uint32_t jx = x + 1;
				const uint32_t ky = y + 1;
				const uint32_t i = x + ( y * width );
				const uint32_t j = jx + ( y * width );
				const uint32_t k = x + ( ky * width );

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

		const auto idx0 = [ & ]()
		{
			if ( dir == horiz )
			{
				if ( !hIdxList.empty() )
				{
					return hIdxList[ rand() % hIdxList.size() ];
				}
			}
			else
			{
				if ( !vIdxList.empty() )
				{
					return vIdxList[ rand() % vIdxList.size() ];
				}
			}

			return 0u;
		}( );
		const auto idx1 = [ & ]()
		{
			if ( dir == horiz )
			{
				if ( !hIdxList.empty() )
				{
					return idx0 + 1;
				}
			}
			else
			{
				if ( !vIdxList.empty() )
				{
					return idx0 + width;
				}
			}

			return 0u;
		}( );

		tileTypes[ idx0 ] = WALL;
		tileTypes[ idx1 ] = WALL;

		const auto x = static_cast<float>( idx0 % width ) * cellSize;
		const auto z = static_cast<float>( idx0 / width ) * cellSize;
		const auto pos = XMFLOAT3{ x, 1.f, z };

		const auto rot = XMFLOAT3{ 0.f, 0.f, 0.f };
		const auto scale = dir == horiz ? 
			XMFLOAT3( 2.F, 1.F, 1.F ) : XMFLOAT3( 1.f, 1.f, 2.f );

		const auto wSpecs = ModelSpecs_W{ pos, rot, scale };

		actors.push_back( Actor( wSpecs, Water1, lSpecs ) );
	}

	return actors;
}
