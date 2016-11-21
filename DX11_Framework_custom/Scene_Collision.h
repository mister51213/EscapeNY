#pragma once

#include "Includes.h"
#include "Camera.h"
#include "Actor_Light.h";
#include "Utilities.h"
#include "DXUtils.h"
#include "Texture.h"
#include "GameTimer.h"
#include "ISubGame.h"
#include "Overlay.h"

class Scene_Collision: public ISubGame
{
public:
	Scene_Collision() = default;
	~Scene_Collision() = default;

	////////////////////////////////////
	// ISUBGAME FUNCTIONS //////////////
	////////////////////////////////////
	virtual void Initialize(
		Graphics *pGraphics, 
		class Game *const pGame,
		Camera *const pCamera);

	// Use this to update actors, camera and anything else that needs updating.
	virtual void UpdateScene( const Input &InputRef, Camera *const pCamera, 
							  const Physics& refPhysics, const GameTimer& refTimer );

	// Use RenderFrame to render the list of actors or other game objects
	virtual void RenderFrame( const GameView &GameViewRef ); 

	void CheckCollisions();

	bool Overlaps( Actor * pActor1, Actor * pActor2 );

private:
	void reset();
	
	//TODO: IMPLEMENT THIS
	// Retrieves the data for the shader to use in rendering.
	const SceneBufferType &GetSceneData()const;

private:
	//////////////////////////////////////
	// OVERHEAD //////////////////////////
	//////////////////////////////////////
	Graphics *m_pGraphics;
	D3DGraphics *m_pDirect3D;
    Overlay m_Overlay;

	//////////////////////////////////////
	// PHYSICS ///////////////////////////
	//////////////////////////////////////
	Physics m_physics;  

	//////////////////////////////////////
	// ACTORS ////////////////////////////
	//////////////////////////////////////
	Camera* m_pCamera; // dynamic / fixed, different camera angles

	Actor_NPC m_map;

	// TODO: Issue with sharing sphere radius info between actor (for collision) 
	// TODO: and gameview (for drawing)
	// FOR TESTING PURPOSES
	// Just calculate radius here same way as PrimitiveFactory.CreateSphereNM()
	// in order to keep it consistent w physics and drawing

	Actor_Player m_ball1;
	float m_radiusB1; 

	Actor_Player m_ball2;
	float m_radiusB2;

	std::vector<std::unique_ptr<Actor>> m_actors_dynamic; // player, NPCs
	std::vector<std::unique_ptr<Actor>> m_actors_static; // trees, walls, scenery, etc
	vector<Actor*> m_pActorsMASTER;
	
	//////////////////////////////////////
	// COLLISION TESTING /////////////////
	//////////////////////////////////////	
	vector<Actor_Dynamic*> m_pActorsOverlapping;
	// TODO: Check every actor every frame, whichever are found to overlap,
	// add them to the above list of overlapping actors.
	// TODO: remember to clear the list after they no longer overlap!

	//////////////////////////////////////
	// LIGHTING //////////////////////////
	//////////////////////////////////////	
	// TODO: Make all lights controlled from one place
    vector<Actor_Light> m_spotLights; // manipulates lights
    vector<LightBufferType> m_lightSet; // passed to GameView

	int m_numBalls = 2;
	int m_numLights = 8;
	int m_lightsPerBall = m_numLights / m_numBalls;
};
