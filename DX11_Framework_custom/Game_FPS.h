#pragma once
#include "ISubGame.h";
#include "Algorithm_Grid3D.h"
#include "Algorithm_Spiral3D.h"
#include "Actor_Light.h"
#include "Light_Spot.h"
#include "Overlay.h"
#include "GameTimer.h"
//#include "Scene.h"

class Game_FPS:public ISubGame
{
public:
    virtual void Initialize(
        Graphics *pGraphics,
        class Game *const pGame,
        Camera *const pCamera) override;

	// Use UpdateFrame to update actors, camera and anything else that needs updating.
    virtual void UpdateScene(const Input &InputRef, Camera *const pCamera, const Physics& refPhysics, const GameTimer& refTimer) override;

	// Use RenderFrame to render the list of actors or other game objects
	virtual void RenderFrame( const GameView &GameViewRef ) override;

private:
    void reset();

    void LightingFX();
    //void VisualFX_Disco();

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

    // LIGHTING EFFECTS variables
    //float m_offset = .01f;
    //unique_ptr<ILightDirection> m_pLight;
    //unique_ptr<ILightPoint> m_pLightPT;
    //bool m_reverseL = false;

    // LIGHTING
    vector<Actor_Light> m_spotLights; // manipulates lights
    vector<LightBufferType> m_lightSet; // passed to GameView
    int m_numLights = 5;

	//////////////////////////////////////////////////////
	///////////////////// NEW CLASSES ////////////////////
	//////////////////////////////////////////////////////
	
	//Scene m_scene; (CHILDREN of these functions)
	// TODO: implement this so Actors are being passed to the physics engine
	// TODO: (currently, it is the OTHER way around)
	// TODO: remove physics member object from Actor.h line 74
	Physics m_physics;

    // TODO: Consolidate Timer, Physics, Scene, and GameView all into one GAME class
	// (now they are spread out between game_fps and game)
    
};