#pragma once
#include "ISubGame.h";
#include "Algorithm_Grid3D.h"
#include "Algorithm_Spiral3D.h"

class Game_FPS:public ISubGame
{
public:
    virtual void Initialize(
        Graphics *pGraphics,
        class Game *const pGame,
        Camera *const pCamera) override;

	// Use UpdateFrame to update actors, camera and anything else that 
	// needs updating.
    virtual void UpdateFrame(const Input &InputRef, Camera *const pCamera) override;

	// Use RenderFrame to render the list of actors or other game objects
	virtual void RenderFrame( const GameView &GameViewRef ) override;

private:
    void reset();

private:
    Graphics *m_pGraphics; // m_pGraphics and m_pDirect3D are created and passed to game without taking ownership
	D3DGraphics *m_pDirect3D;
    Camera* m_pCamera; // Camera, Model and ColorShader are created in Game, so has ownership
    
    Overlay m_Overlay;
    Actor_Player m_player;

    vector<Actor_NPC> m_actorsSUB1; //* vector is destroyed before the list   
    vector<Actor_NPC> m_actorsSUB2;
    vector<Actor_NPC> m_actorsSUB3;
    vector<Actor_NPC> m_actorsSUB4;
    vector<Actor*> m_pActorsMASTER;

    Actor_NPC aTest1;
    Actor_NPC aTest2;
    Actor_NPC aTest3;
};