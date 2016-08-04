////////////////////////////////////////////////////////////////////////////////
//  TODO:   Later separate this class into GameWorldObjects(for gameplay purposes)
//  TODO:   and GameRenderObjects (for rendering purposes)
///////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Camera.h"
#include "Model_Textured.h"
#include "Graphics.h"
#include "Shader_Color.h"
#include "Texture.h"
#include "Shader_Texture.h"
#include "Actor.h"

using namespace std;

// TODO: Make new ACTOR class; this holds the WORLD_SPECS and REFERENCES a model, then combines
// these two data types to allow itself to be rendered.

class GameWorld
{
public:
    GameWorld(){}
    GameWorld( // initialize all pointers
        char numObjects, 
        Graphics* pGfx, 
        D3DGraphics* pD3D,
        std::shared_ptr<Camera> pCam,
        HWND WinHandle) 
    {
        m_pGfx = pGfx;
        m_pD3D = pD3D;
        m_objectCount = numObjects;

        m_pCam = pCam;
        m_WinHandle = WinHandle;
    }

    // we return these Model pointers to the list so that we can access them again and modify them based on input.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Add an update function that updates position of every Model in the pointer list
    // TODO: based on positions in modSpecList; therefore move functions will only operate on modSpecList,
    // TODO: and then we call Update() function every frame to render each model to its new position.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
public:
// TODO: move these into game class
    void CreatModGrid()
    {
        ModelSpecs_W specs = { { 0.f, 0.f, 0.f }, { 0.f,0.f,0.f }, { 1.f,1.f,1.f } };
        for (int i = 0; i < m_objectCount; i++)
        {
            m_modSpecs_W.push_back(specs);
            specs.position.x += 3;
            specs.position.y += 3;
            specs.orientation.z += 10;
        }    
    }
    void MakeAllModels() {
        for (char i = 0; i < m_objectCount; i++){
            m_models.push_back(MakeModel(m_modSpecs_W[i])); 
        }
    }

    //TODO: Implement this functionality AFTER implementing Actor class.
    //void MakeAllGenericModels()
    //{
    //    // INITIALIZE ONE MODEL
    //    PrimitiveFactory primMaker;
    //    primMaker.CreateCube(localSpecs.center, localSpecs.size, localSpecs.orientation);        
    //    
    //    // MAKE MANY COPIES OF IT
    //    for (char i = 0; i < m_objectCount; i++) {
    //        m_models.push_back(MakeModel(m_modSpecs_W[i]));
    //    }
    //}

    //    std::shared_ptr<Model> MakeModelGeneric( // for generics
    //    ModelSpecs_W worldSpecs, 
    //    ModelSpecs_L localSpecs = { { 0.f, 0.f, 0.f }, { 5.f, 5.f, 5.f }, { 0.f, 0.f, 0.f } },
    //    eModType = CUBE_TEXTURED) 
    //{
    //    std::shared_ptr<Model_Textured> pModel;
    //    pModel.reset(new Model_Textured(worldSpecs.position));

	   // pModel->Initialize( primMaker, *m_pGfx );
    //    return pModel; 
    //}

    void UpdateView(vector<std::shared_ptr<Actor>> actors){
        for each (auto pActor in actors)
        {
            m_models.push_back(MakeModel(pActor->GetWorldSpecs));
        }
    }

    void DrawAllModels() {
        for each (const std::shared_ptr<Model>& model in m_models){
            DrawModel(model); 
        }
    }

    // INITIALIZE TEXTURE SHADER //
    void InitializeShader()
    {
        m_pShader_Texture.reset(new Shader_Texture);
        m_pShader_Texture->Initialize(m_pD3D->GetDevice(), m_WinHandle/*, *pMod*/);
        m_pStoneTexture.reset(new Texture);
        m_pStoneTexture->Initialize(*m_pGfx, L"Textures\\uncompressed_stone.tga");    
    }

    void InitializeGameObjectsSystem()
    {
        CreatModGrid();
        InitializeShader();
        MakeAllModels();
    }

private:
    std::shared_ptr<Model> MakeModel( // for custom models
        ModelSpecs_W worldSpecs, 
        ModelSpecs_L localSpecs = { { 0.f, 0.f, 0.f }, { 5.f, 5.f, 5.f }, { 0.f, 0.f, 0.f } },
        eModType = CUBE_TEXTURED) 
    {
        std::shared_ptr<Model_Textured> pModel;
        pModel.reset(new Model_Textured(worldSpecs.position));
        PrimitiveFactory primMaker;
        primMaker.CreateCube(localSpecs.center, localSpecs.size, localSpecs.orientation);
	    pModel->Initialize( primMaker, *m_pGfx );
        return pModel; 
    }

    void DrawModel( const std::shared_ptr<Model>& pMod ) 
    { 
        m_pShader_Texture->Render(
            m_pD3D->GetDeviceContext(),
            GetWorldMatrix( // TODO: take a ModelSpecs_W
                pMod->m_Position, 
                ConvertToRadians(pMod->m_Orientation), 
                pMod->m_Scale),
            m_pCam->GetViewMatrix(),
            m_pCam->GetProjectionMatrix(),
            m_pStoneTexture->GetTextureView());
        m_pGfx->RenderModel(*pMod);
    }
    private:
        HWND m_WinHandle;
        Graphics* m_pGfx;
        D3DGraphics* m_pD3D;
        std::shared_ptr<Camera> m_pCam;

        std::unique_ptr<Model_Textured> m_pModelTEST;
        std::shared_ptr<Shader_Texture> m_pShader_Texture;
	    std::shared_ptr<Texture> m_pStoneTexture;

        vector<ModelSpecs_W> m_modSpecs_W;// model specs list in WORLD SPACE
        vector<std::shared_ptr<Model>> m_models; // list of actual models for rendering purposes

        char m_objectCount;
};