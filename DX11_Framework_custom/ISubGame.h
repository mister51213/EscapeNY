#pragma once

class Camera;
class Game;
class GameTimer;
class GameView;
class Graphics;
class Input;
class Physics;

class ISubGame
{
public:
	// Use Initialize to initialize anything that needs initializing.
	virtual void Initialize(
		Graphics *pGraphics, 
		Game *const pGame,
		Camera *const pCamera,
		std::shared_ptr<std::vector<std::vector<DirectX::XMFLOAT3>>> pNormals) = 0;

	// Use this to update actors, camera and anything else that needs updating.
	virtual void UpdateScene( Input &InputRef, 
							  Camera *const pCamera, 
							  const Physics& refPhysics, 
							  const GameTimer& refTimer ) = 0; // TODO: add const to end of physics functions

	// Use RenderFrame to render the list of actors or other game objects
	virtual void RenderFrame( const GameView &GameViewRef ) = 0;

};
