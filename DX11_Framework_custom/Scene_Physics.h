#pragma once

#include "Includes.h"			// Used for std::vector and std::unique_ptr
#include "Actor_Light.h";		// Used for Actor and Actor_Light
#include "Actor_NPC.h"			// Used for Actor and Actor_NPC
#include "Actor_Stationary.h"
#include "Actor_Player.h"		// Used for Actor and Actor_Player
#include "Physics_NEW.h"		// Used for Physics
#include "ISubGame.h"			// Used for ISubGame
#include "Overlay.h"			// Used for Overlay

class Camera;
class D3DGraphics;
class Game;
class GameTimer;
class Graphics;
class Input;

class Scene_Physics: public ISubGame
{
public:
	Scene_Physics() = default;
	~Scene_Physics() = default;

	////////////////////////////////////
	// ISUBGAME FUNCTIONS //////////////
	////////////////////////////////////
	virtual void Initialize(
		Graphics *pGraphics, 
		Game *const pGame,
		Camera *const pCamera)override;

	// Use this to update actors, camera and anything else that needs updating.
	virtual void UpdateScene( Input &InputRef, Camera *const pCamera, 
							  const Physics& refPhysics, const GameTimer& refTimer )override;

	// Use RenderFrame to render the list of actors or other game objects
	virtual void RenderFrame( const GameView &GameViewRef )override; 

	void PoleForCollidedActors();

	void GetInput(Input& pInput, float time);

	bool CircleVsCircle( Actor * pActor1, Actor * pActor2 );

	bool CircleVsBox( Actor * pActor1, Actor * pActor2 );

	bool AABBvsAABB( Actor * pActor1, Actor * pActor2 );

	bool BoxVsBox( Actor * pActor1, Actor * pActor2 );

	bool Overlaps( Actor * pActor1, Actor * pActor2 );

private:
	void reset();
	
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
	Physics_NEW m_physics;  

	//////////////////////////////////////
	// ACTORS ////////////////////////////
	//////////////////////////////////////
	Camera* m_pCamera; // dynamic / fixed, different camera angles
	Actor_Stationary m_map;
	Actor_Player m_ball1;
	Actor_Player m_ball2;
	Actor_Player m_box1;
	Actor_Player m_box2;
	float m_radiusB1; 
	float m_radiusB2;
	int m_numBalls = 2;
	int m_numBoxes = 2;
	int m_numLights = 8;
	int m_lightsPerBall = m_numLights / m_numBalls;

	std::vector<std::unique_ptr<Actor>> m_actors_dynamic; // player, NPCs
	std::vector<std::unique_ptr<Actor>> m_actors_static; // trees, walls, scenery, etc
	std::vector<Actor*> m_pActorsMASTER;
	//////////////////////////////////////
	// COLLISION TESTING /////////////////
	//////////////////////////////////////	
	std::vector<Actor_Dynamic*> m_pActorsOverlapping;

	//////////////////////////////////////
	// LIGHTING //////////////////////////
	//////////////////////////////////////	
    std::vector<Actor_Light> m_spotLights; // manipulates lights
    std::vector<LightBufferType> m_lightSet; // passed to GameView
};

