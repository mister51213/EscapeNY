#pragma once

#include "Includes.h"
#include "Camera.h"
#include "Actor_Light.h";
#include "Utilities.h"
#include "DXUtils.h"
#include "Texture.h"
#include "GameTimer.h"

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	// Entry point into the scene object.  Each Scene child should implement this 
	// function.  It's purpose is to handle calling the update functions or all 
	// objects in the scene; Player, Cars, Sun, Environment like debris or birds,
	// Ocean wave animations, Sun position, etc.
	virtual void Update( GameTimer &Timer ) = 0;

	// Retrieves the data for the shader to use in rendering.
	const SceneBufferType &GetSceneData()const;
public:
	// TODO: Make Actor_Static and Actor_Dynamic parent classes
	// Dynamic and Static actors should be classes of those types, not just a 
	// list of actors that have those names.  Probably won't want to go this route
	// since we don't want a vector of Actor base objects.  Player and NPCs will
	// be of different types.
	std::vector<std::unique_ptr<Actor>> m_actors_dynamic; // player, NPCs
	std::vector<std::unique_ptr<Actor>> m_actors_static; // trees, walls, scenery, etc

	// TODO: look into how to animate dynamic actors like this
	// For polymorphic behavior with vectors, you'll need to have a vector
	// of pointers, preferrably a vector of unique_ptrs of Camera_Base type
	// or whatever we name the parent camera class
	std::vector<Camera> m_cameras; // dynamic / fixed, different camera angles
	std::vector<Texture> m_texturePool;
	std::vector<Model> m_modelPool;

	SceneBufferType m_lightSet; // TODO: Make all lights controlled from one place
	// TODO: Right now there are the following light related types:
	/*
		1. SceneBufferType
		2. LightBufferType
		3. ILight interface
		4. FOUR Child interfaces of ILight
		5. THREE Light classes
		6. Actor_Light
		> Can these be consolidated?
	*/
	/*
		NOTE: This may need some thought as each type has it's own purpose
		
		SceneBufferType: 
			Used for shader purposes, is used to pass ALL light data to the 
			shader(s).
		
		LightBufferType: 
			Used for one light instance, is used to supply shader with light 
			attributes including which light type is being used.
		
		ILight and it's children: 
			Just Interfaces, so no data is being manipulated.  Their sole 
			purpose is to provide a structure for the implementation of the 
			lights.
		
		Light_*: 
			classes are the implementations of the ILight_* interfaces
		
		Actor_Light: 
			Was suppose to represent an actual object that represents a 
			physical light source, such as a flash-light or a candle.
	*/

	/* 
	NOTE:
		The LightBufferType is used in the Light_* implementations,
		to consolidate with Actor_Light_*, you'd have to sync the Actor_Light
		implementation data with the LightBufferType structs of the Light_*
		implementations, or the other way around sync Lights with Actor_Lights.

		The SceneBufferType will be retrieved from a Scene object.
		
		The LightBufferTypes are stored in Light_Base and all Light_* children
		inherit it, so that data is directly changed by each Light_* child.

		The only problem is the Actor_Light, because it has it's own, but similar
		attributes; position and orientation		
	*/

	/*
		Thinking out loud:
		First thought: 
			Make Actor an interface, but then we'd run into the 
			same problems we had with the lights, where the compiler wouldn't know 
			which interface you were implementing; ILight_* functions or Actor 
			functions.

		Second thought:
			Just give some Actors lights, they would sync their position/orientation
			with the lights themselves; Street lamps won't need to constantly update
			the position nor orientation, head/tail lights only need to sync
			positions, etc.
	*/
};

