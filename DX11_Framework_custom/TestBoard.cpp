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

void TestBoard::SetCells( std::vector<Actor>&& pCells )
{
	m_pCells = std::move( pCells );
}

const std::vector<Actor>& TestBoard::GetTiles()const
{
	return m_pCells;
}
