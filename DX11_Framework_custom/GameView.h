////////////////////////////////////////////////////////////////////////////////
//  TODO:   Later separate this class into GameWorldObjects(for gameplay purposes)
//  TODO:   and GameRenderObjects (for rendering purposes)
///////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Camera.h"
#include "Model_Textured.h"
#include "Model.h"
#include "Graphics.h"
#include "Shader_Color.h"
#include "Texture.h"
#include "Shader_Texture.h"
#include "Actor.h"
#include <string>

using namespace std;

// TODO: Make new ACTOR class; this holds the WORLD_SPECS and REFERENCES a model, then combines
// these two data types to allow itself to be rendered.

class GameView
{
public:
    GameView() {}
    GameView( 
        Graphics* pGfx,
        D3DGraphics* pD3D,
        std::shared_ptr<Camera> pCam)
    {
        m_pGfx = pGfx;
        m_pD3D = pD3D;
        m_pCam = pCam;
    }

    void InitTexturePool()
    {
        const int numTextures = 5;
        m_Textures.resize(numTextures);
        m_Textures[AsphaltFresh].Initialize(*m_pGfx, L"Textures\\fresh-black-asphalt-texture.jpg");
        m_Textures[AsphaltTGA].Initialize(*m_pGfx, L"Textures\\asphalt.tga");
        m_Textures[AsphaltOld].Initialize(*m_pGfx, L"Textures\\old-asphalt-texture.jpg");
        m_Textures[Water].Initialize(*m_pGfx, L"Textures\\water3.jpg");
        m_Textures[SharkSkin].Initialize(*m_pGfx, L"Textures\\sharkskin1.jpg");
    }

    // INITIALIZE TEXTURE SHADER //
    void InitializeShader()
    {
        m_pShader_Texture.reset(new Shader_Texture);
        m_pShader_Texture->Initialize(m_pD3D->GetDevice(), m_WinHandle);
    }

    // MAKE MODEL
	std::shared_ptr<Model>
		MakeModel(
			ModelSpecs_L localSpecs = {
				{ 0.f, 0.f, 0.f },				// Center
				{ 5.f, 5.f, 5.f },				// Size
				{ 0.f, 0.f, 0.f } },			// Orientation
			eModType = CUBE_TEXTURED )
	{
        std::shared_ptr<Model_Textured> pModel;
        pModel.reset(new Model_Textured);
        PrimitiveFactory primMaker;
        // TODO: Implement it so it can draw difft shapes based on eModType
        primMaker.CreateCube(localSpecs);
        pModel->Initialize(primMaker, *m_pGfx);
        return pModel;
    }

    void ModelAllActors(const vector<Actor*>& actors)
    {
        for (int i = 0; i < actors.size(); i++)
        {
            actors[i]->SetModel(MakeModel(actors[i]->GetLocalSpecs()));
        }
    }

    void InitializeGameObjectsSystem(const vector<Actor*>& actors)
    {
        InitTexturePool();
        InitializeShader();
        ModelAllActors(actors);
    }

    void DrawModel(const Actor& actor)
    {
        m_pShader_Texture->Render(
            m_pD3D->GetDeviceContext(),
            GetWorldMatrix(actor.GetWorldSpecs()),
            m_pCam->GetViewMatrix(),
            m_pCam->GetProjectionMatrix(),
            (m_Textures[actor.GetTexIndex()]).GetTextureView());

		m_pGfx->RenderModel(*(actor.GetModel()));
    }

    void UpdateView(const vector<Actor*>& actors) {
        for (int i = 0; i < actors.size(); i++)
        {
            DrawModel(*actors[ i ] );
        }
    }

private:
    Graphics* m_pGfx;
    D3DGraphics* m_pD3D;

    std::shared_ptr<Camera> m_pCam;

    std::unique_ptr<Model_Textured> m_pModelTEST;
    std::shared_ptr<Shader_Texture> m_pShader_Texture;

    vector<Texture> m_Textures;
};