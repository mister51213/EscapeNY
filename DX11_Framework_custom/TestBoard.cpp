#include "TestBoard.h"
#include "PrimitiveFactory.h"
#include "Model_Textured.h"

using namespace DirectX;

TestBoard::TestBoard()
	:
	Actor( { { 0.f, 0.f, 0.f },{ 90.f, 0.f, 0.f },{ 5.f, 5.f, 5.f } }, eTexture::Underwater3, ModelSpecs_L(), PLANE )
{
}


TestBoard::~TestBoard()
{
}

void TestBoard::Initialize( UINT Width, UINT Height, Graphics *const pGraphics )
{
	m_width = Width;
	m_height = Height;

	m_worldSpecs.position = { ( m_width * .5f ) * 5.f, 1.f, ( m_height * .5f ) * 5.f};
	m_worldSpecs.scale = { m_width * 5.f, 1.f, m_height * 5.f };

	m_startCell =	{ rand() % m_width, m_height - 1 };
	m_endCell =		{ rand() % m_width, 0 };
	
	m_cellSize = { 5.f, 1.f, 5.f };	
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
