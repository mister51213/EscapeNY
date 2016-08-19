#include "Game_FPS.h"

void Game_FPS::Initialize(Graphics *pGraphics, Game *const pGame, Camera *const pCamera) 
{
    // Take a copy of the graphics and direct3d pointers
	m_pGraphics = pGraphics;
	m_pDirect3D = pGraphics->GetDirect3D();
    m_pCamera = pCamera;

    m_Overlay.Initialize( *m_pGraphics );

	reset();
}

	// Use UpdateFrame to update actors, camera and anything else that needs updating.
void Game_FPS::UpdateScene(const Input &InputRef, Camera *const pCamera)
{
    int randInt = rand() % 3;
	// NOTE: If you need a float between 0.f and 10.f, shouldn't the line say:
    float randFloat = static_cast<float>(rand() % 100)* 0.1f; // get random float from 0~10.0

    /////////////////////
    // INPUT STUFF
    //////////////////////
    m_pCamera->GetInput(InputRef);
    m_pCamera->GetMouseInput(InputRef);
    m_player.GetInput(InputRef);
    for (auto& pActor : m_actorsSUB1)
    {
        pActor.GetInput(InputRef, randInt, randFloat);
    }

    randInt = rand() % 3;
    randFloat = (rand()%50)/10.0f; // get random float from 0~10.0
    for (auto& pActor : m_actorsSUB2)
    {
        pActor.GetInput(InputRef, randInt, randFloat);
    }
}

void Game_FPS::reset()
{
    m_actorsSUB1.clear();
    m_actorsSUB2.clear();
    m_actorsSUB3.clear();
    m_actorsSUB4.clear();
    m_pActorsMASTER.clear();
    //1
    m_player = Actor_Player(
    { { 0.f, 0.f, 0.f },
    { 0.f, 0.f, 0.f },
    { .5f, .5f, .5f } },
        eTexture::SharkSkin,
        ModelSpecs_L());
    //2
    ModelSpecs_W wSpecs1 = 
    { { 0.f, -1.f, 0.f },
      { 0.f,0.f,0.f },
      { 10.f, 8.f, 8.f } };
    aTest1 = Actor_NPC(wSpecs1, Water2, ModelSpecs_L(), CUSTOM_MESH);
    //3
    ModelSpecs_W wSpecs2 = 
    { { 20.f, -.5f, 20.f },
      { 0.f,0.f,0.f },
      { 0.5f, 0.5f, 0.5f } };
    aTest2 = Actor_NPC(wSpecs2, Water3, ModelSpecs_L(), CUSTOM_MESH2);
    //4
    ModelSpecs_W wSpecs3 = 
    { { -10.f, 0.f, -30.f },
      { 0.f,0.f,0.f },
      { 80.f, 1.f, 80.f } };
    aTest3 = Actor_NPC(wSpecs3, Underwater3, ModelSpecs_L(), CUBE_TEXTURED);

    // MAKE m_actorsSUB1 (ONE SUBSET OF ACTORS)
    const int numRows = 5, numColumns = 5, numZ = 5;
    const int numActors1 = numRows * numColumns * numZ;
    Algorithm_Grid3D alg;
    m_actorsSUB1 = alg.MakePatternNPC(numActors1);
    // MAKE m_actorsSUB2 (ONE SUBSET OF ACTORS)
    const int numActors2 = 100;
    Algorithm_Spiral3D alg2(this);
    //m_actorsSUB2 = makeActorSet(numActors2, &alg2);
    m_actorsSUB2 = alg2.MakePatternNPC(numActors2);

    m_pActorsMASTER.reserve(4 + numActors1 + numActors2);
    m_pActorsMASTER.push_back(&m_player);
    m_pActorsMASTER.push_back(&aTest1);
    m_pActorsMASTER.push_back(&aTest2);
    m_pActorsMASTER.push_back(&aTest3);
    for (int i = 0; i < numActors1; i++)
    {
        m_pActorsMASTER.push_back(&(m_actorsSUB1[i]));
    }
    for (int i = 0; i < numActors2; i++)
    {
        m_pActorsMASTER.push_back(&(m_actorsSUB2[i]));
    }
}

void Game_FPS::doVisualFX()
{
        // MOVE LIGHTING    
        if (!m_reverseL)
        {
            if (m_light.Direction.x < 1.f)
            {
                m_light.Direction.x += .01f;
                m_light.Direction.y += .03f;

                m_light.Color.y += .01f;
                m_light.Color.y += m_offset;
                m_offset += .0005f;
            }
            else
                m_reverseL = true;
        }
        else
        {
            if (m_light.Direction.x > -1.f)
            {
                m_light.Direction.x -= .01f;
                m_light.Direction.y -= .03f;

                m_light.Color.y -= .01f;
                m_light.Color.y -= m_offset;
                m_offset -= .0005f;
            }
            else
                m_reverseL = false;
        }
}

	// Use RenderFrame to render the list of actors or other game objects
void Game_FPS::RenderFrame(const GameView &GameViewRef)
{
    doVisualFX();
    GameViewRef.UpdateView(m_pActorsMASTER, m_light);
	//GameViewRef.UpdateView( m_pActorsMASTER, m_spotLight );
   	m_Overlay.Render( *m_pGraphics );
}

