#include "TestBoard.h"
#include "PrimitiveFactory.h"
#include "Model_Textured.h"
#include "MathUtils.h"

using namespace DirectX;

TestBoard::TestBoard()
	:
	Actor( { { 0.f, 0.f, 0.f },{ 90.f, 0.f, 0.f },{ 1.f, 1.f, 1.f } }, eTexture::AsphaltOld, ModelSpecs_L(), PLANE )
{
}


TestBoard::~TestBoard()
{
}

void TestBoard::Initialize( UINT Width, UINT Height, Graphics *const pGraphics )
{
	m_width = Width;
	m_height = Height;

	const XMFLOAT2 position = ConvertTileCoordinates( m_width >> 1u, m_height >> 1u );
	m_worldSpecs.position = { -position.x, 1.f, position.y};
	m_worldSpecs.scale = 
	{ 
		static_cast<float>( m_width ), 1.f, 
		static_cast<float>( m_height ) 
	};

	const UINT xStartCell = rand() % m_width;
	const UINT xEndCell = rand() % m_width;
	const UINT yStartCell = m_height - 1;
	const UINT yEndCell = 0;

	m_startCell = { xStartCell, yStartCell };
	m_endCell   =	{ xEndCell, yEndCell };
	
	m_cellSize = { g_tileWidth, 1.f, g_tileWidth };
}

DirectX::XMUINT2 TestBoard::GetStartCellCoord() const
{
	return m_startCell;
}

DirectX::XMUINT2 TestBoard::GetEndCellCoord() const
{
	return m_endCell;
}

DirectX::XMFLOAT3 TestBoard::GetStartPosition() const
{
	XMFLOAT3 result{
		static_cast<float>( m_startCell.x ) * m_cellSize.x,
		1.f,
		static_cast<float>( m_startCell.y ) * m_cellSize.z
	};

	return result;	
}

DirectX::XMFLOAT3 TestBoard::GetEndPosition() const
{
	XMFLOAT3 result{
		static_cast<float>( m_endCell.x ) * m_cellSize.x,
		1.f,
		static_cast<float>( m_endCell.y ) * m_cellSize.z
	};
	
	return result;
}

UINT TestBoard::GetWidth() const
{
	return m_width;
}

UINT TestBoard::GetHeight() const
{
	return m_height;
}

DirectX::XMFLOAT3 TestBoard::GetCellSize() const
{
	return m_cellSize;
}

bool TestBoard::HasReachedEnd( const Actor &crActor ) const
{
	auto endCell = GetEndPosition();
	float cellLeft = endCell.x;
	float cellTop = endCell.z;
	float cellRight = endCell.x + m_cellSize.x;
	float cellBottom = endCell.z - m_cellSize.z;
	
	auto &position = crActor.GetPosition();

	if( ( ( position.x > cellLeft ) && ( position.x < cellRight ) ) &&
		( ( position.z > cellBottom ) && ( position.z < cellTop ) ) )
	{
		return true;
	}
	return false;
}

void TestBoard::SetCells( std::vector<Actor>&& pCells )
{
	m_pCells = std::move( pCells );
	m_cellSize = m_pCells[ 0 ].GetLocalSpecs().size;
}

AlignedAxisBoundingBox TestBoard::GetBoardBoundingBox()const
{
	const XMFLOAT3 boardExtent = ( m_cellSize * m_width ) * .5f;
	
	AlignedAxisBoundingBox bb( { 0.f, 0.f, 0.f }, boardExtent );
	
	return bb;
}

AlignedAxisBoundingBox TestBoard::GetCellBoundingBox( const DirectX::XMFLOAT3 & Position ) const
{
	const XMFLOAT3 cellExtent = m_cellSize * .5f;
	
	const int cellX = static_cast< int >( std::round( Position.x ) ) % m_width;
	const int cellY = static_cast< int >( std::round( Position.y ) ) / m_width;

	const XMFLOAT2 tileCenterCoordinates = ConvertTileCoordinates( cellX, cellY );
	const XMFLOAT3 tileCenter = 
	{ tileCenterCoordinates.x, 1.f, tileCenterCoordinates.y };
	
	return AlignedAxisBoundingBox( tileCenter, cellExtent );
}

const std::vector<Actor>& TestBoard::GetTiles()const
{
	return m_pCells;
}
