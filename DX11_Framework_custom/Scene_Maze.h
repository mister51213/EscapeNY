#pragma once

#include "Actor_Player_Alt.h"
#include "Actor_Light.h"
#include "DXUtils.h"
#include "Includes.h"
#include "ISubGame.h"
#include "Overlay.h"
#include "TestBoard.h"

class Camera;
class Game;
class GameTimer;
class GameView;
class Graphics;
class Input;


class Scene_Maze:
	public ISubGame
{
public:
	Scene_Maze() = default;
	~Scene_Maze()= default;
	Scene_Maze( Input *pInput );
	// Use Initialize to initialize anything that needs initializing.
	void Initialize( 
		Graphics *pGraphics,
		Game *const pGame,
		Camera *const pCamera,
		std::shared_ptr<std::vector<std::vector<DirectX::XMFLOAT3>>> pNormals) override;

	// Use UpdateFrame to update actors, camera and anything else that 
	// needs updating.
	void UpdateScene( Input &InputRef, Camera *const pCamera, const Physics& refPhysics, const GameTimer& refTimer) override;

	// Use RenderFrame to render the list of actors or other game objects
	void RenderFrame( const GameView &GameViewRef ) override;

	const TestBoard &GetBoard();

private:
	void UpdatePlayerState();
	void UpdateCarState();
	void UpdateMonsterState();

private: // Private methods
	void reset();

private: // Private members
	// Game, Graphics and Camera pointers are not owned by MazeGame, do not
	// delete
	Game *m_pGame;
	Graphics *m_pGraphics;
	Camera *m_pCamera;
	Input *m_pInput;

	Overlay m_Overlay;
	TestBoard m_board;

	Actor_Player_Alt m_player;
	std::vector<Actor*> m_pActorDrawList;

	std::vector<Actor_Light> m_pLights;
	SceneBufferType m_scene;

	bool m_endReached = false;
};

