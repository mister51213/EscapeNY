#include "LightSet_Night.h"



LightSet_Night::LightSet_Night()
{
}


LightSet_Night::~LightSet_Night()
{
}

bool LightSet_Night::Initialize()
{
	// Set count and indices of lights
	m_directionalCount = 1;
	m_directionalIndex = 0;

	m_pointCount = 4;
	m_pointIndex = 1;

	// Check for count or returned index of -1 to determine if operations
	// will fail.
	m_spotCount = 0;
	m_spotIndex = -1;

	// Add light addresses to light list
	m_pLightList.reserve( m_directionalCount + m_pointCount + m_spotCount );
	m_pLightList.push_back( m_pMoonLight.get() );

	// unique_ptr array of abstract classes aren't allow, so to get around
	// this limitation, we'll use an iterator.
	
	// Get the first element address for iterator, and the address 
	// after the last element for end of range, increment
	// iterator while iterator address is less than end address
	for( ILight *pLightIter = m_pLanterns.get(),
		*pLightIterEnd = &m_pLanterns.get()[ m_pointCount ];
		pLightIter < pLightIterEnd; ++pLightIter )
	{
		m_pLightList.push_back( pLightIter );
	}
	
	return true;
}

int LightSet_Night::GetLightDirectionCount() const
{
	return m_directionalCount;
}

int LightSet_Night::GetLightPointCount() const
{
	return m_pointCount;
}

int LightSet_Night::GetLightSpotCount() const
{
	return m_spotCount;
}

ILightDirection * LightSet_Night::GetDirectionalLight( int Index ) const
{
	// Throw error if trying to index outside of light type count
	assert( Index < m_directionalCount );
	auto pLight = m_pLightList[ m_directionalIndex + Index ];
	return dynamic_cast<ILightDirection*>( pLight );
}

ILightPoint * LightSet_Night::GetPointLight( int Index ) const
{
	// Throw error if trying to index outside of light type count
	assert( Index < m_pointCount );
	auto pLight = m_pLightList[ m_pointIndex + Index ];
	return dynamic_cast<ILightPoint*>( pLight );
}

ILightSpot * LightSet_Night::GetSpotLight( int Index ) const
{
	return nullptr;
}

ILightDirection * LightSet_Night::GetDirectionalLights() const
{
	auto pLights = m_pLightList[ m_directionalIndex ]; 
	return dynamic_cast<ILightDirection*>( pLights );
}

ILightPoint * LightSet_Night::GetPointLights() const
{
	auto pLights = m_pLightList[ m_pointIndex ];
	return dynamic_cast<ILightPoint*>( pLights );
}

ILightSpot * LightSet_Night::GetSpotLights() const
{
	return nullptr;
}
