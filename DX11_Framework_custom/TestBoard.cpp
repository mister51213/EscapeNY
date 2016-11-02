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

	const float xStart = static_cast< float >( rand() % m_width );
	const float yStart = static_cast< float >( m_height - 1 );
	const float xEnd = static_cast< float >( rand() % m_width );
	const float yEnd = 0.f + position.y;

	const UINT xStartCell = static_cast<UINT>( xStart + position.x );
	const UINT xEndCell = static_cast<UINT>( xEnd + position.x );
	const UINT yStartCell = static_cast<UINT>( yStart + position.y );
	const UINT yEndCell = static_cast<UINT>( yEnd );

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

DirectX::XMFLOAT3 TestBoard::GetBoundingBox( const DirectX::XMFLOAT3 & Position ) const
{
	auto cellExtent = m_cellSize * .5f;
	XMFLOAT3 bb;

	bb.x = Position.x + cellExtent.x;
	bb.z = Position.z + cellExtent.z;
	bb.y = 1.f;

	// Not uniform, so much search for cell at Position

	return DirectX::XMFLOAT3();
}

const std::vector<Actor>& TestBoard::GetTiles()const
{
	return m_pCells;
}
