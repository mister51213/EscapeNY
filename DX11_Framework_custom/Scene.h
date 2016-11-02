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

// TODO: Keep graphics out of Scene and in GameView
// TODO: Implement these
/*
	Map,
	List of Cars,
	Monsters,
	Player spawn point,
	Light Set (day or night?),
	Camera
*/

class Scene: public ISubGame
{
public:
	Scene() = default;
	~Scene() = default;

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
	Actor_Player m_player;

	std::vector<std::unique_ptr<Actor>> m_actors_dynamic; // player, NPCs
	std::vector<std::unique_ptr<Actor>> m_actors_static; // trees, walls, scenery, etc
	vector<Actor*> m_pActorsMASTER;

	//////////////////////////////////////
	// LIGHTING //////////////////////////
	//////////////////////////////////////	
	// TODO: Make all lights controlled from one place
    vector<Actor_Light> m_spotLights; // manipulates lights
    vector<LightBufferType> m_lightSet; // passed to GameView
    int m_numLights = 5;
};

