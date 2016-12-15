/////////////////////////////////////////////////////////////////
// Class name: Actor
// Function: Parent class that holds all information related 
//           to gameplay for a specific object (location, etc.).
// Example:  MovingObject->Car->Mercedes
/////////////////////////////////////////////////////////////////
#pragma once

#include "Utilities.h"

// TEST: Going to try forward declaring as many things as I can get away with 
// to see if it improves compile times. Current unchanged compile Rebuild time: 86 secs
// After changes; forward declare what is able.  83 secs
/*
All the files that #include Actor.h directly or indirectly
Actor.cpp
Actor_Car.cpp
Actor_Dynamic.cpp
Actor_Light.cpp
Actor_NPC.cpp
Actor_Player.cpp
Actor_Player_Alt.cpp
Actor_Static.cpp
Algorithm_Spiral3D.cpp
Algorithm_Random.cpp
Algorithm_Maze.cpp
Algorithm_Grid3D.cpp
Board.cpp
Game.cpp					<- Indirectly (through GameView.h)
GameView.cpp
Main.cpp					<- Indirectly (through System.h)
Scene.cpp
Scene_Collision.cpp
Scene_FPS.cpp
Scene_Maze.cpp
System.cpp					<- Indirectly (through Game.h)
TestBoard.cpp
*/
namespace DirectX
{
	// Since XMFLOAT3 is in the DirectX namespace, have to forward declare as such
	struct XMFLOAT3;
}

class Input;

class Actor
{
public:
    Actor() = default;

    Actor(
        const ModelSpecs_W &worldSpecs,
        eTexture tex,
        const ModelSpecs_L &localSpecs,
        eModType modType = CUBE_TEXTURED,
    	std::shared_ptr<std::vector<std::vector<DirectX::XMFLOAT3>>> pNormals = NULL);
	// Parent classes need virtual destructors so the child destructor is called,
	// just like virtual functions.  Though the child destructors don't need
	// the override specified.  Child classes are automatically virtual, so
	// Actor_Dynamic and Actor_Static shouldn't need virtual destructors as long
	// as Actor has one.
	virtual ~Actor();
    ///////////////
    // ACCESSORS //
    ///////////////
	eModType GetModelType()const;
	eTexture GetTexIndex() const;
	const ModelSpecs_W &GetWorldSpecs() const;
	const ModelSpecs_L &GetLocalSpecs() const;
	PhysAttributes &GetAttributes();

	void SetWorldSpecs( const ModelSpecs_W& newSpecs );
	void SetPosition( const DirectX::XMFLOAT3& newPos );

	const DirectX::XMFLOAT3 &GetPosition()const;
	const DirectX::XMFLOAT3 &GetRotation()const;

	virtual void GetInput( Input& pInput, int randI = 0.f, float randF = 0.f );

protected:
    eModType m_modType;
    eTexture m_texIndex;

    ModelSpecs_W m_worldSpecs;
    ModelSpecs_L m_localSpecs;

public: //TODO: MAKE PROTECTED
	PhysAttributes m_attributes;
	PhysAttributes m_attributesPrev;

	AABB m_AABBox;
};