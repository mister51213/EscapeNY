#include "TestBoard.h"

using namespace DirectX;

TestBoard::TestBoard()
	:
	Actor( { { 0.f, 0.f, 0.f },{ 0.f, 0.f, 0.f },{ 10.f, 10.f, 10.f } } )
{
}


TestBoard::~TestBoard()
{
}

void TestBoard::Initialize( UINT Width, UINT Height )
{
	m_width = Width;
	m_height = Height;

	m_startCell =	{ rand() % m_width, m_height - 1 };
	m_endCell =		{ rand() % m_width, 0 };
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
	float multiplier = m_pCells[ 0 ].GetLocalSpecs().size.x;
	XMFLOAT3 res;
	res.x = static_cast<float>(m_startCell.x) * multiplier;
	res.y = 1.f;
	res.z = static_cast<float>(m_startCell.y) * multiplier;
	return res;
}

DirectX::XMFLOAT3 TestBoard::GetEndPosition() const
{
	float multiplier = m_pCells[ 0 ].GetLocalSpecs().size.x;
	XMFLOAT3 res;
	res.x = static_cast<float>( m_endCell.x ) * multiplier;
	res.y = 1.f;
	res.z = static_cast<float>( m_endCell.y ) * multiplier;
	return res;
}

UINT TestBoard::GetWidth() const
{
	return m_width;
}

UINT TestBoard::GetHeight() const
{
	return m_height;
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
