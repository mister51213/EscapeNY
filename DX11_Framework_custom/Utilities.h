#pragma once

#include "Includes.h"
// Files that include Utilties.h directly or indirectly 
/*
Actor.cpp
Actor_Car.cpp				<- Indirectly (through Actor.h)
Actor_Dynamic.cpp			<- Indirectly (through Actor.h)
Actor_Light.cpp				<- Indirectly (through Actor.h)
Actor_NPC.cpp				<- Indirectly (through Actor.h)
Actor_Player.cpp			<- Indirectly (through Actor.h)
Actor_Player_Alt.cpp		<- Indirectly (through Actor.h)
Actor_Static.cpp			<- Indirectly (through Actor.h)
Algorithm_Grid3D.cpp		<- Indirectly (through Actor.h)
Algorithm_Maze.cpp			<- Indirectly (through Actor.h)
Algorithm_Random.cpp		<- Indirectly (through Actor.h)
Algorithm_Spiral3D.cpp		<- Indirectly (through Actor.h)
Board.cpp					<- Indirectly (through Actor.h)
Camera.cpp
D2DGraphics.cpp
D3DGraphics.cpp
FontLoader.cpp
Game.cpp					<- Indirectly (through GameView.h)
GameView.cpp				<- Indirectly (through Actor.h)
Graphics.cpp
ImageLoader.cpp
Main.cpp					<- Indirectly (through System.h)
Model.cpp
Model_Textured.cpp			<- Indirectly (through Model.h)
Model_TexturedNM.cpp		<- Indirectly (through Model.h)
Overlay.cpp
Physics.cpp
PrimitiveFactory.cpp
Scene.cpp					<- Indirectly (through GameView.h)
Scene_Collision.cpp			<- Indirectly (through Actor.h)
Scene_FPS.cpp				<- Indirectly (through Actor.h)
Scene_Maze.cpp				<- Indirectly (through Actor.h)
Shader_NMap.cpp				<- Indirectly (through ShaderTEMPLATE.h)
System.cpp					<- Indirectly (through Game.h)
TestBoard.cpp				<- Indirectly (through Actor.h)
Texture.cpp
Utilities.cpp
Wic.cpp
*/
#pragma warning(disable:4067)
// Helper macros to decrease the amount of typing and space
#define RETURN_IF_FALSE(Result)\
if( !( Result ) ) \
{ \
	ShowCursor(TRUE);\
	return false; \
}\
else int a = 0
#define RETURN_MESSAGE_IF_FALSE(Result, Message)\
if(!(Result))\
{\
	ShowCursor(TRUE);\
	MessageBox(nullptr, Message, L"Error!", MB_OK);\
	return false;\
}\
else int a = 0
#define RETURN_IF_FAILED(Result) RETURN_IF_FALSE( SUCCEEDED( ( Result ) ) )

//////////////////////////////////////////////////////
//				GLOBAL CONSTANTS					//
//////////////////////////////////////////////////////
constexpr float g_tileWidth = 5.f;
constexpr float g_offset = g_tileWidth * .5f;


////////////////////////////////////////////////////
// Algorithms
////////////////////////////////////////////////////

void Sort( std::vector<DirectX::XMFLOAT3> &V );

//////////////////////////
/// GAME LOGIC RELATED ///
//////////////////////////
//enum eState
//{
//	Stationary, Falling, Moving, Move_PID
//};

////////////////////////
/// PHYSICS RELATED  ///
////////////////////////
// "downward" acceleration on the Y axis
constexpr float Gravity = -9.8f; // units are meters/second^2

struct PhysAttributes
{
	PhysAttributes() {}
	// initialize momentum and center of mass here
	PhysAttributes(DirectX::XMFLOAT3 cMass);

	DirectX::XMFLOAT3 velocLin = { 0.f, 0.f, 0.f }; // linear velocity
	DirectX::XMFLOAT3 velocAng = { 0.f, 0.f, 0.f }; // angular velocity

	DirectX::XMFLOAT3 accelLin = { 0.f, 0.f, 0.f };
	DirectX::XMFLOAT3 accelAng = { 0.f, 0.f, 0.f };

	DirectX::XMFLOAT3 momentumLin;
	DirectX::XMFLOAT3 momentumAng;

	DirectX::XMFLOAT3 centerOfMass = { 0.f, 0.f, 0.f };

	/// COEFFICIENTS ///
	float mass = 1.0f;
	float radius = 1.0f;
	float energy = 0.0f;
	float elasticity = 0.0f;
	// would need to calculate this for more realistic physics.
	float terminalV = 50.0f;

};

enum eForceType
{
	Push,
	Pull,
	Spin,
	Swerve
};

// It might be more efficient to store CENTER and EXTENT in Bounding box.
// This way you only have to update the CENTER each frame and calculate
// the sides of the box from the extents.  Currently, if you store all
// eight vertices of the bounding box, you'd have to update all eight each
// frame.  Even if you store just the min/max points of the box, that's 
// still two vertices instead of one to update.
struct AlignedAxisBoundingBox // JUST A BOX
{
	AlignedAxisBoundingBox() = default;
	AlignedAxisBoundingBox( const DirectX::XMFLOAT3 &Center, const DirectX::XMFLOAT3 &Extent );

	// Assumes that translation has already been applied
	bool Overlaps( const AlignedAxisBoundingBox &AABB )const;

	DirectX::XMFLOAT3 center, extent;
};

struct BoundingMesh // for detailed hit detection
{
	std::vector<DirectX::XMFLOAT3> vertices;
};

//////////////////////
// GRAPHICS RELATED //
//////////////////////
/////////////////////////////
// CLAMP FUNCTION
/////////////////////////////
template<typename T>
static T Clamp( const T& x, const T& low, const T& high )
{
	return x < low ? low : ( x > high ? high : x );
}

////////////////////////////////////////////////////////////////
// STRUCTS CONTAINING MODEL INFO for gameplay and rendering
// TODO: make it global; see which is faster
////////////////////////////////////////////////////////////////
struct ModelSpecs_W
{
	DirectX::XMFLOAT3 position, orientation, scale;
};

struct ModelSpecs_L
{
	ModelSpecs_L(
		const DirectX::XMFLOAT3 &Center = { 0.f, 0.f, 0.f },
		const DirectX::XMFLOAT3 &Orientation = { 0.f, 0.f, 0.f },
		const DirectX::XMFLOAT3 &Size = { 1.f, 1.f, 1.f } );

	DirectX::XMFLOAT3 center;
	DirectX::XMFLOAT3 orientation;
	DirectX::XMFLOAT3 size;
};

// this will be initialized when actor is created.
// holds top left and bottom right corners of axis alligned bounding box

enum eModType
{
	CUBE,
	CUBE_TEXTURED,
	PLANE,
	SPHERE,
	POLYGON,
	CUSTOM_MESH,
	CUSTOM_MESH2,
	CUSTOM_MESH3,
	CUSTOM_MESH4,
	SOME_EDIFICE,
	CAR
};

enum CarType
{
	LAMBO,
	PORCHE,
	CORVETTE,
	BUG,
	THE_LOANA
};

enum eTexture
{
	AsphaltFresh = 0,
	waterSHALLOW = 1,
	AsphaltOld = 2,
	Water1 = 3,
	Underwater1 = 4,
	SharkSkin = 5,
	Aluminum = 6,
	Lava = 7,
	Lava2 = 8,
	Bush = 9,
	Energy = 10,
	Rock = 11
	//Water2 = 4,
	//Water3 = 5,
	//Underwater2 = 7,
	//Underwater3 = 8,
	//Underwater4 = 9,
	//Underwater5 = 10,
	//Underwater6 = 11,
	//Underwater7 = 12,
};

struct AABB
{
	AABB() = default;
	AABB(const ModelSpecs_W& specs, std::vector<DirectX::XMFLOAT3>* pNormMesh, eModType model = CUBE_TEXTURED);
	void ResetMinMax(DirectX::XMFLOAT3 position);

	DirectX::XMFLOAT3 m_center;
    DirectX::XMFLOAT3 m_extentHalf;
	DirectX::XMFLOAT3 m_min;
    DirectX::XMFLOAT3 m_max;
	eModType m_model;
	std::vector<DirectX::XMFLOAT3> m_Normals;
};

////////////////////////////////////////////////////////////////
// FUNCTION FOR GETTING WORLD MATRIX
// TODO: make it global; see which is faster
////////////////////////////////////////////////////////////////
DirectX::XMMATRIX GetWorldMatrix( const ModelSpecs_W &modSpecs );

struct CHAPTER
{
	// TODO: Add universal texture/model set.
	// Universal sets aren't applicable to specific chapters

	// Visual Theme
	eTexture textureset[ 5 ];
	eModType modelset[ 10 ];

	// Obstacle types
	bool hasMonstersEasy = false;
	bool hasMonstersMedium = false;
	bool hasMonstersHard = false;
	bool hasAgro = false;
	bool hasAgroDrunk = false;
};


// Returns center position in real world coordinates
// IE. center of tile
DirectX::XMFLOAT2 ConvertTileCoordinates( int X, int Y );


struct LEVEL_BLUEPRINT
{
	// Contains unique game play attributes and visual theme for each chapter.
	// Example; Levels  1-10 = Street theme(normal cars),
	//			Levels 11-20 = Mountain theme(agro cars),
	//			Levels 21-30 = Forest theme(monsters)
	CHAPTER *pActiveChapter = nullptr;

	// Incremented each level, defaults are for level 1
	int levelNumber = 1;
	int carCount = 15;
	int monsterCount = 0;
	int agroCount = 0;
	int agroDrunkCount = 0;

	// Map of car placements
	int carInitMap[ 10 ][ 10 ];
	

	static constexpr int carInitMap_example[ 10 ][10]
	{ 
		{ -1, -1, LAMBO, -1, -1, PORCHE, -1, -1, LAMBO, -1 },
		{ -1, -1, -1, -1, -1, PORCHE, -1, -1, LAMBO, -1 },
		{-1, -1, LAMBO, -1, -1, -1, -1, -1, LAMBO, -1 },
		{-1, -1, LAMBO, -1, -1, PORCHE, -1, -1, LAMBO, -1 },
		{-1, -1, -1, -1, -1, CORVETTE, -1, -1, -1, -1 },
		{-1, -1, LAMBO, -1, -1, PORCHE, -1, -1, LAMBO, -1 },
		{-1, -1, LAMBO, -1, -1, PORCHE, -1, -1, -1, -1 },
		{-1, -1, CORVETTE, -1, -1, PORCHE, -1, -1, LAMBO, -1 },
		{-1, -1, LAMBO, -1, -1, PORCHE, -1, -1, LAMBO, -1 },
		{-1, -1, LAMBO, -1, -1, CORVETTE, -1, -1, -1, -1 }
	};
};

