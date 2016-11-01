#pragma once

#include "Includes.h"
#include "Camera.h"
#include "Actor_Light.h";
#include "Utilities.h"
#include "DXUtils.h"
#include "Texture.h"
#include "GameTimer.h"
#include "ISubGame.h"

/*
TODO: Keep graphics out of Scene and in GameView
TODO: Implement these
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
	// ISUBGAME FUNCTIONS //
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

	////////////////////////////////////
	// ISUBGAME FUNCTIONS //
	////////////////////////////////////	 

	// Retrieves the data for the shader to use in rendering.
	const SceneBufferType &GetSceneData()const;

public:

	std::vector<std::unique_ptr<Actor>> m_actors_dynamic; // player, NPCs
	std::vector<std::unique_ptr<Actor>> m_actors_static; // trees, walls, scenery, etc


	std::vector<Camera> m_cameras; // dynamic / fixed, different camera angles
	SceneBufferType m_lightSet; // TODO: Make all lights controlled from one place

};

