#include "MazeGenerator.h"
#include <random>


MazeGenerator::MazeGenerator()
{}


MazeGenerator::~MazeGenerator()
{}

bool MazeGenerator::Initialize( unsigned int MazeWidth, unsigned int MazeHeight )
{
	m_mazeWidth = MazeWidth;
	m_mazeHeight = MazeHeight;


}

unsigned int MazeGenerator::getRandomDirection() const
{
	std::mt19937 rng;
	std::uniform_int_distribution<unsigned int> distro( 0, 3 );	
	
	return distro( rng );
}

std::pair<unsigned int, unsigned int> MazeGenerator::GetRandomNeighbor( const unsigned int Index ) const
{
	unsigned x = Index % m_mazeWidth;
	unsigned y = Index / m_mazeWidth;
	unsigned random = getRandomDirection();
	std::pair<unsigned, unsigned> coords( -1, -1 );

	switch( static_cast<eCarDirection>( random ) )
	{
		case eCarDirection::FACESNORTH:
			if( !m_cells[ Index ].WasVisited() )
			{
				if( y - 1 < m_mazeHeight )
				{
					coords.first = x;
					coords.second = y - 1;
				}
			}
			break;
		case eCarDirection::FACESSOUTH:
			if( !m_cells[ Index ].WasVisited() )
			{
				if( y + 1 < m_mazeHeight )
				{
					coords.first = x;
					coords.second = y + 1;
				}
			}
			break;
		case eCarDirection::FACESEAST:
			if( !m_cells[ Index ].WasVisited() )
			{
				if( x + 1 < m_mazeWidth )
				{
					coords.first = x + 1;
					coords.second = y;
				}
			}
			break;
		case eCarDirection::FACESWEST:
			if( !m_cells[ Index ].WasVisited() )
			{
				if( x - 1 < m_mazeWidth )
				{
					coords.first = x - 1;
					coords.second = y;
				}
			}
			break;
	}
		
	return coords;
}
