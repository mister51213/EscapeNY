#include "AnimatedLight_Square.h"
#include "Utilities.h"


AnimatedLight_Square::AnimatedLight_Square()
	:
	m_curIndex( 0 ),
	m_speed( 0.f )
{
}

void AnimatedLight_Square::MakePattern( 
	const DirectX::XMFLOAT3 & StartPosition, 
	UINT Count, 
	float Scale, 
	float Speed )
{
	m_speed = Speed;
	std::vector<DirectX::XMFLOAT3> points
	{
		{0.f, 0.f, 0.f},
		{1.f, 0.f, 0.f},
		{1.f, 0.f, 1.f},
		{0.f, 0.f, 1.f}
	};

	for( auto &point : points )
	{
		point = StartPosition + ( point * Scale );
	}

	m_points = std::move( points );
}

void AnimatedLight_Square::Update( ILightPoint * const pLight, float DetlaTime )
{
	const auto distCovered = XMVectorReplicate( m_speed * DetlaTime );
	const auto minRange = XMVectorReplicate( 0.5f );

	// get the next index, loop back to 0 if more than the number
	// of points in the list
	auto nextIndex = m_curIndex + 1;
	nextIndex %= m_points.size();

	// Get current position
	auto lightPosition = pLight->GetPosition();
	auto xmPosition = DirectX::XMLoadFloat3( &lightPosition );

	// Get next position
	auto xmNextPos = DirectX::XMLoadFloat3( &m_points[ nextIndex ] );

	// Calculate the distance to point
	auto distVector = xmNextPos - xmPosition;
	auto sqDistance = XMVector3LengthSq( distVector );
	auto recDistance = DirectX::XMVectorReciprocalSqrt( sqDistance );
	auto estDirection = recDistance * distVector;
	xmPosition += ( distCovered * estDirection );

	auto cmpResult = _mm_cmplt_ps( sqDistance, XMVector3LengthSq( minRange ) );
	auto isInRange = _mm_movemask_ps( cmpResult );

	m_curIndex = isInRange ? ++m_curIndex : m_curIndex;

	DirectX::XMFLOAT3 temp{};
	DirectX::XMStoreFloat3( &temp, xmPosition );
	pLight->SetPosition( temp.x, temp.y, temp.z );
}
