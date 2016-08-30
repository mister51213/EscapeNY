#include "Game_FPS.h"
#include "Light_Diffuse.h"
#include "Light_Point.h"

void Game_FPS::Initialize(Graphics *pGraphics, Game *const pGame, Camera *const pCamera) 
{
    //srand( static_cast<unsigned int>( time( nullptr ) ) );

    // Take a copy of the graphics and direct3d pointers
	m_pGraphics = pGraphics;
	m_pDirect3D = pGraphics->GetDirect3D();
    m_pCamera = pCamera;

    m_Overlay.Initialize( *m_pGraphics );

    // INITIALIZE LIGHT
    m_spotLights.resize(m_numLights);
    for (Actor_Light& light: m_spotLights)
    {
		// Randomly create positions for new spot lights
		float x = static_cast<float>( rand() % 300 - 150 );
		float z = static_cast<float>( rand() % 300 - 150 );
		light.Initialize( { x, 150.f, z}, { 0.f, 0.f, 0.f } );

		// Randomly create colors for new spot lights
		float r = static_cast<float>(rand() % 100) * .01f;
		float g = static_cast<float>(rand() % 100) * .01f;
		float b = static_cast<float>(rand() % 100) * .01f;

		// Some functions aren't working with polymorphism unless you 
		// cast to concrete class instead of interface, which is fine
		// they are still ILight children, so can pass if needed to 
		// ILight * parameters.
		// This should probably be part of the Actor_LightSpot class or 
		// whatever we get around to naming it, so their is less of a chance
		// for casting to the incorrect type, which dynamic_cast "SHOULDN'T"
		// allow anyway.  It will return nullptr, if it can't cast to desired
		// light type.
		auto* pLight = dynamic_cast<Light_Spot*>( light.GetLight() );

		// Set the color for the newly created light.
		pLight->SetColor( r, g, b );
		pLight->SetConeAngle( 45.f );
    }

    m_lightSet.resize(m_numLights);

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
    // BEAR
    ModelSpecs_W wSpecs1 = 
    { { 0.f, -1.f, 0.f },
      { 0.f,0.f,0.f },
      { 5.f, 5.f, 5.f } };
    aTest1 = Actor_NPC(wSpecs1, AsphaltOld, ModelSpecs_L(), CUSTOM_MESH);
    // 
    ModelSpecs_W wSpecs2 = 
    { { 20.f, -.5f, 20.f },
      { 0.f,0.f,0.f },
      { 0.5f, 0.5f, 0.5f } };
    aTest2 = Actor_NPC(wSpecs2, Water3, ModelSpecs_L(), CUBE_TEXTURED);
    // GAME BOARD
    ModelSpecs_W wSpecs3 = 
    { { -10.f, 0.f, -30.f },
      { 0.f,0.f,0.f },
      { 80.f, 1.f, 80.f } };
	/*ModelSpecs_W wSpecs3{
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f },
		{ 1.f, 1.f, 1.f }
	};*/
    aTest3 = Actor_NPC( wSpecs3, Underwater3, ModelSpecs_L(), CUBE_TEXTURED);

    // MAKE m_actorsSUB1 (ONE SUBSET OF ACTORS)
    //const int numRows = 5, numColumns = 5, numZ = 5;
    //const int numActors1 = numRows * numColumns * numZ;
    //Algorithm_Grid3D alg;
    //m_actorsSUB1 = alg.MakePatternNPC(numActors1);
    //// MAKE m_actorsSUB2 (ONE SUBSET OF ACTORS)
    //const int numActors2 = 100;
    //Algorithm_Spiral3D alg2(this);
    ////m_actorsSUB2 = makeActorSet(numActors2, &alg2);
    //m_actorsSUB2 = alg2.MakePatternNPC(numActors2);

	auto numActors1 = m_actorsSUB1.size();
	auto numActors2 = m_actorsSUB2.size();
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

void Game_FPS::LightingFX()
{
    //VisualFX_Disco();
    
    // TODO: UPDATE SPOTLIGHT WORLD SPECS

    // TODO: manipulate the actors containing the lights
    // > extract the lights from the actors, and put them in a LIGHT SET
    // > PAss the light set to the GameView::Update for rendering    
}

//
//void Game_FPS::VisualFX_Disco() 
//{
//    XMFLOAT4 color = m_pLight->GetColor();
//    XMFLOAT3 direction = m_pLight->GetDirection();
//
//    // MOVE LIGHTING    
//    if (!m_reverseL)
//    {
//        if (direction.x < 1.f)
//        {
//            direction.x += .01f;
//            direction.y += .02f;
//            direction.z += .03f;
//
//            color.x += m_offset;
//            color.y -= m_offset;
//            color.z += m_offset;
//            m_offset += .001f;
//        }
//        else
//            m_reverseL = true;
//    }
//    else
//    {
//        if (direction.x > -1.f)
//        {
//            direction.x -= .01f;
//            direction.y -= .02f;
//            direction.z -= .03f;
//
//            color.x -= m_offset;
//            color.y += m_offset;
//            color.z -= m_offset;
//
//            m_offset -= .001f;
//        }
//        else
//            m_reverseL = false;
//    }
//
//    m_pLight->SetColor(color.x, color.y, color.z);
//    m_pLight->SetDirection(direction.x, direction.y, direction.z);
//}
//
	// Use RenderFrame to render the list of actors or other game objects

// TODO: ADD SCENE CLASS. Includes:
/*
    > Light set
    > Camera set
        (a collection of camera objects, each w their own
         settings [3rd, 1st person, difft angles, etc])
    >Actor set (difft NPCS, scenery, architecture, etc)
    >Texture set (difft rooms, difft terrains)
        
        ex.)
        > STATIC SCENE (only initialized in beginning)
        > DYNAMIC SCENE (updated every frame)

    > Then let it get updated every time
*/
    
void Game_FPS::RenderFrame(const GameView &GameViewRef)
{
    //LightingFX();
	SceneBufferType scene{};
	scene.ambientColor = { .1f, .1f, .1f, .1f };
	scene.lightCount = min( m_spotLights.size(), MAX_SHADER_LIGHTS );

    for (int i = 0; i < m_spotLights.size(); i++)
    {
        scene.lights[i] = m_spotLights[i].GetLight()->GetLightBufferType();
    }

    // TODO: make it take a whole VECTOR of LightBufferTypes
    // TODO: refactor the shader to have that array of 300 lights and
    // initialize them as needed

    GameViewRef.UpdateView(m_pActorsMASTER, scene);
   	m_Overlay.Render( *m_pGraphics );
}

