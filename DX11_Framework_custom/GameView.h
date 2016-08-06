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
    GameView( // initialize all pointers
        char numObjects,
        Graphics* pGfx,
        D3DGraphics* pD3D,
        std::shared_ptr<Camera> pCam,
        HWND WinHandle)
    {
        m_pGfx = pGfx;
        m_pD3D = pD3D;
        m_numModels = numObjects;
        m_pCam = pCam;
        m_WinHandle = WinHandle;
    }

    void InitTexturePool()
    {
        const int numTextures = 5;
        m_Textures.resize(numTextures);
        m_Textures[AsphaltFresh].Initialize(*m_pGfx, L"Textures\\fresh-black-asphalt-texture.jpg");
        m_Textures[AsphaltTga].Initialize(*m_pGfx, L"textures\\asphalt.tga");
        m_Textures[AsphaltOld].Initialize(*m_pGfx, L"textures\\old-asphalt-texture.jpg");
        m_Textures[Water].Initialize(*m_pGfx, L"textures\\water3.jpg");
        m_Textures[SharkSkin].Initialize(*m_pGfx, L"textures\\sharkskin5.jpg");
    }

    // INITIALIZE TEXTURE SHADER //
    void InitializeShader()
    {
        m_pShader_Texture.reset(new Shader_Texture);
        m_pShader_Texture->Initialize(m_pD3D->GetDevice(), m_WinHandle);
    }

    std::shared_ptr<Model> MakeModel( // for custom models
        ModelSpecs_L localSpecs = { { 0.f, 0.f, 0.f }, { 5.f, 5.f, 5.f }, { 0.f, 0.f, 0.f } },
        eModType = CUBE_TEXTURED)
    {
        std::shared_ptr<Model_Textured> pModel;
        pModel.reset(new Model_Textured);
        PrimitiveFactory primMaker;
        primMaker.CreateCube(localSpecs.center, localSpecs.size, localSpecs.orientation);
        pModel->Initialize(primMaker, *m_pGfx);
        return pModel;
    }

    void ModelAllActors(const vector<Actor*>& actors)
    {
        for each (auto pActor in actors)
        {
            m_models.push_back(MakeModel(pActor->GetLocalSpecs()));
        }
    }

    void InitializeGameObjectsSystem(const vector<Actor*>& actors)
    {
        InitTexturePool();
        InitializeShader();
        ModelAllActors(actors);
    }

    void DrawModel(const Actor& pActor /*ModelSpecs_W worldSpecs, const std::shared_ptr<Model>& pMod */)
    {
        m_pShader_Texture->Render(
            m_pD3D->GetDeviceContext(),
            GetWorldMatrix(pActor.GetWorldSpecs()),
            m_pCam->GetViewMatrix(),
            m_pCam->GetProjectionMatrix(),
            //pActor.GetTexture()->GetTextureView());
            (m_Textures[pActor.GetTexIndex()]).GetTextureView());
        m_pGfx->RenderModel(*(pActor.GetModel()));
    }

    /*DrawAllModels*/
    void UpdateView(const vector<Actor*>& actors) {
        for (int i = 0; i < m_numModels; i++)
        {
            DrawModel(*(actors[i]));
        }
    }

private:
    HWND m_WinHandle;
    Graphics* m_pGfx;
    D3DGraphics* m_pD3D;
    std::shared_ptr<Camera> m_pCam;

    //std::unique_ptr<Model_Textured> m_pModelTEST;
    std::shared_ptr<Shader_Texture> m_pShader_Texture;
    //std::shared_ptr<Texture> m_pStoneTexture;

    vector<ModelSpecs_W> m_modSpecs_W;// model specs list in WORLD SPACE
    vector<std::shared_ptr<Model>> m_models; // list of actual models for rendering purposes

    int m_numModels;
    vector<Texture> m_Textures;
};