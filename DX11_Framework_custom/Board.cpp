#include "Board.h"



Board::Board()
	:
	Actor( { { 0.f, 0.f, 0.f}, { 0.f, 0.f, 0.f }, { 1.f,1.f,1.f} }, { { 0.f, 0.f, 0.f },{ 9.f,9.f, 1.f } })
{
}


Board::~Board()
{
}

bool Board::Initialize( unsigned Width, unsigned Height )
{
	m_width = Width;
	m_height = Height;

	/*MazeGenerator maze;
	maze.Initialize( m_width, m_height );
	maze.Generate( m_cells );

	SetStartAndEnd( 
		maze.GetStart().ToIndex( m_width ), 
		maze.GetEnd().ToIndex( m_width ) );*/

	return true;
}

std::vector<Board::Cell>& Board::GetCellArray()
{
	return m_cells;
}

unsigned Board::GetWidth() const
{
	return m_width;
}

unsigned Board::GetHeight() const
{
	return m_height;
}

const std::shared_ptr<Model>& Board::GetModel() const
{
	return m_pModel;
}

void Board::SetStartAndEnd( unsigned StartCellIndex, unsigned EndCellIndex )
{
	m_start = StartCellIndex;
	m_end = EndCellIndex;
}

Board::Cell::Cell( const ModelSpecs_W & Specs )
	:
	m_wall( NORTHWALL | EASTWALL | SOUTHWALL | WESTWALL ),
	m_visited( false ),
	Actor( Specs )
{
}

void Board::Cell::Initialize( std::shared_ptr<Model> pModel )
{
}

const std::shared_ptr<Model>& Board::Cell::GetModel() const
{
	return m_pModel;
}

void Board::Cell::Visited()
{
	m_visited = true;
}

void Board::Cell::RemoveWall( const uint8_t Direction )
{
	m_wall = ( m_wall & ( ~Direction ) );
}

bool Board::Cell::WasVisited() const
{
	return m_visited;
}

uint8_t Board::Cell::GetWall() const
{
	return m_wall;
}
