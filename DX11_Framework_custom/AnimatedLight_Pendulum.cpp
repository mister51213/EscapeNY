#include "AnimatedLight_Pendulum.h"

void AnimatedLight_Pendulum::MakePattern( const DirectX::XMFLOAT3 & PivotPosition, float Length )
{
	m_pivotPosition = PivotPosition;
	m_length = Length;
	m_velocity = { 0.f, 0.f, 0.f };
}

void AnimatedLight_Pendulum::Update( ILightSpot * pLight, float DeltaTime )
{
	auto lightPos = pLight->GetPosition();
	
	float gravity = 9.8f * pow( DeltaTime, 2 );
	
	auto accel = XMFLOAT3( 0.f, -gravity, 0.f );
	auto dist = m_pivotPosition - lightPos;
	float len = Magnitude( dist );
	auto pullVector = dist / len;

	if( len >= m_length )
	{
		lightPos = m_pivotPosition + ( -pullVector * m_length );
	}
	accel += pullVector;
	m_velocity += accel;
	lightPos += m_velocity;
	
	
	pLight->SetPosition( lightPos.x, lightPos.y, lightPos.z );
	pLight->SetDirection( -pullVector.x, -pullVector.y, -pullVector.z );
	int a = 0;
}
