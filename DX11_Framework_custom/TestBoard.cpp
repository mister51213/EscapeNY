#include "TestBoard.h"



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

	m_tileTypes.resize( m_width * m_height );

	m_startCell =	{ rand() % m_width, m_height - 1 };
	m_endCell =		{ rand() % m_width, 0 };
}

DirectX::XMUINT2 TestBoard::GetStartPosition() const
{
	return m_startCell;
}

DirectX::XMUINT2 TestBoard::GetEndPosition() const
{
	return m_endCell;
}

UINT TestBoard::GetWidth() const
{
	return m_width;
}

UINT TestBoard::GetHeight() const
{
	return m_height;
}

std::vector<eTileType>& TestBoard::GetTiles()
{
	return m_tileTypes;
}
