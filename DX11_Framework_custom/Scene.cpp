#include "Scene.h"

void Scene::Initialize( Graphics * pGraphics, Game * const pGame, Camera * const pCamera )
{}

void Scene::UpdateScene( const Input & InputRef, Camera * const pCamera, const Physics & refPhysics, const GameTimer & refTimer )
{}

void Scene::RenderFrame( const GameView & GameViewRef )
{}

const SceneBufferType & Scene::GetSceneData() const
{
	// TODO: insert return statement here
	return SceneBufferType();
}
