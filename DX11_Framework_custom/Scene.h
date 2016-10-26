#pragma once

#include "Includes.h"
#include "Camera.h"
#include <vector>
#include "Actor_Light.h";
#include "Utilities.h"
#include "DXUtils.h"
#include "Texture.h"

using namespace std;

class Scene
{
public:
	Scene();
	~Scene();

public:
	vector<Actor> m_actors_dynamic; // player, NPCs *Use UpdateState() function
	vector<Actor> m_actors_static; // trees, walls, scenery, etc *SAME STATE all the time
	// TODO: look into how to animate static actors like this
	vector<Camera> m_cameras; // dynamic / fixed, different camera angles
	vector<Texture> m_texturePool;
	LightBufferType m_lightSet; // TODO: Make all lights controlled from one place
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
};

