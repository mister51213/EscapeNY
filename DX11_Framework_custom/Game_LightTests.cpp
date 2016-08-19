#include "Game_LightTests.h"



void Game_LightTests::Initialize( Graphics * pGraphics, Game * const pGame, Camera * const pCamera )
{
	m_pGraphics = pGraphics;
	m_pGame = pGame;
	m_pCamera = pCamera;

	m_pCamera->SetPosition( { -223.f, 203.f, -214.f} );
	m_pCamera->SetRotation( { 37.75f, 47.75f, 0.f } );
	ModelSpecs_W wSpecs3{};
	wSpecs3.scale = { 80.f, .1f, 80.f };

	m_plane = Actor_NPC( wSpecs3, FlatGray, ModelSpecs_L(), CUBE_TEXTURED );

	m_pActorDrawList.push_back( &m_plane );

	// Separate rotation angles
	XMFLOAT3 xRot( 90.f * g_radian, 0.f, 0.f );
	XMFLOAT3 zRot = { 0.f, 0.f, 30.f * g_radian };

	// Define forward vector
	XMVECTOR xmForward = DirectX::XMVectorSet( 0.f, 0.f, 1.f, 0.f );

	// Rotation forward vector around X axis
	XMVECTOR xmAxis = XMLoadFloat3( &xRot );
	XMMATRIX xmRot = XMMatrixRotationRollPitchYawFromVector( xmAxis );
	xmForward = DirectX::XMVector3TransformCoord( xmForward, xmRot );

	// Rotate transformed forward vector around the Z axis
	xmAxis = XMLoadFloat3( &zRot );
	xmRot = XMMatrixRotationRollPitchYawFromVector( xmAxis );
	XMVECTOR xmDirection = DirectX::XMVector3Transform( xmForward, xmRot );

	XMFLOAT3 direction{};
	XMStoreFloat3( &direction, xmDirection );

	float innerAngle = .4f;
	float offset = .1f;
	m_spotLight.SetColor( 1.f, 1.f, 1.f );
	m_spotLight.SetDirection( direction.x, direction.y, direction.z );
	m_spotLight.SetInnerConeAngle( innerAngle );
	m_spotLight.SetIntensity( 1.f );
	m_spotLight.SetOuterConeAngle( innerAngle + offset );
	m_spotLight.SetPosition( 15.f, 18.f, 10.f );



	m_animatedPendulum.MakePattern( { 0.f, 25.f, 10.f }, 10.f );
	//m_animatedLight.MakePattern( m_spotLight.GetPosition(), 4, 100.f, 10.f );
}

void Game_LightTests::UpdateScene( const Input & InputRef, Camera * const pCamera )
{
	//m_animatedPendulum.Update( &m_spotLight, 0.016f );
	//m_animatedLight.Update( &m_spotLight, 0.016f );
	//m_pCamera->GetInput( InputRef );
	//m_pCamera->GetMouseInput( InputRef );

}

void Game_LightTests::RenderFrame( const GameView & GameViewRef )
{
	GameViewRef.UpdateView( m_pActorDrawList, m_spotLight );
}
