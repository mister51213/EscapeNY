////////////////////////////////////////////////////////////////////////////////
//  TODO:   Later separate this class into GameWorldObjects(for gameplay purposes)
//  TODO:   and GameRenderObjects (for rendering purposes)
///////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>

// includes from Game.h
#include "Camera.h"
#include "Model.h"
#include "Graphics.h"
#include "Shader_Color.h"
#include "Texture.h"
#include "Shader_Texture.h"
#include "Overlay.h"

using namespace std;

// make a lightweight "skeletal" version of ModelSpecs analogous to actual Models
struct ModelSpecs
{
    XMFLOAT3 position, orientation, scale;
};

enum eModType{CUBE, CUBE_TEXTURED, PLANE, SPHERE, POLYGON};

class GameObjects
{
public:
    GameObjects(){}
    GameObjects( // initialize all pointers
        char numObjects, 
        Graphics* pGfx, 
        D3DGraphics* pD3D,
        std::shared_ptr<Camera> pCam,
        std::shared_ptr<Shader_Texture> pShader_Texture,
   	    std::shared_ptr<Texture> pStoneTexture,
        HWND WinHandle) 
    {
        m_pGfx = pGfx;
        m_pD3D = pD3D;
        m_objectCount = numObjects;
        m_pCam = pCam;
        m_pShader_Texture = pShader_Texture;
    	m_pStoneTexture = pStoneTexture;
        m_WinHandle = WinHandle;
    }

    // we return these Model pointers to the list so that we can access them again and modify them based on input.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Add an update function that updates position of every Model in the pointer list
    // TODO: based on positions in modSpecList; therefore move functions will only operate on modSpecList,
    // TODO: and then we call Update() function every frame to render each model to its new position.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::shared_ptr<Model> DrawModel(XMFLOAT3 offset, std::shared_ptr<Model> pMod)
    {
        PrimitiveFactory primMaker;

        std::shared_ptr<Model> pModel = pMod;
        /////////////////////
        // INITIALIZE MODEL
        /////////////////////
        //m_pModelTEST.reset(new Model_Textured(offset));
        //primMaker.CreateCube({ 0.f, 0.f, 0.f }, { 5.f, 20.f, 50.f });
        //m_pModelTEST->Initialize(primMaker, *m_pGfx);        
        
        // This version uses the passed pointer to render:
        pModel.reset(new Model_Textured(offset));
        primMaker.CreateCube({ 0.f, 0.f, 0.f }, { 5.f, 20.f, 50.f });
        pModel->Initialize(primMaker, *m_pGfx);  


        //////////////////////////////////
        // INITIALIZE TEXTURE SHADER
        //////////////////////////////////
        //m_pShader_Texture.reset(new Shader_Texture);        
        //m_pShader_Texture->Initialize(m_pD3D->GetDevice(), m_WinHandle, *m_pModelTEST);
        //m_pStoneTexture.reset(new Texture);
        //m_pStoneTexture->Initialize(*m_pGfx, L"Textures\\uncompressed_stone.tga");

        // This version uses the passed pointer to render:
        m_pShader_Texture.reset(new Shader_Texture);        
        m_pShader_Texture->Initialize(m_pD3D->GetDevice(), m_WinHandle, *pModel);
        m_pStoneTexture.reset(new Texture);
        m_pStoneTexture->Initialize(*m_pGfx, L"Textures\\uncompressed_stone.tga");

        ////////////////////////////////
        // RENDER MODEL using texture 
        ////////////////////////////////
        /*m_pShader_Texture->Render(
            m_pD3D->GetDeviceContext(),
            GetWorldMatrix(m_pModelTEST->m_Position, ConvertToRadians(m_pModelTEST->m_Orientation), m_pModelTEST->m_Scale),
            m_pCam->GetViewMatrix(),
            m_pCam->GetProjectionMatrix(),
            m_pStoneTexture->GetTextureView());
        m_pGfx->RenderModel(*m_pModelTEST);*/

        // This version uses the passed pointer to render:
        m_pShader_Texture->Render(
            m_pD3D->GetDeviceContext(),
            GetWorldMatrix(pModel->m_Position, ConvertToRadians(pModel->m_Orientation), m_pModelTEST->m_Scale),
            m_pCam->GetViewMatrix(),
            m_pCam->GetProjectionMatrix(),
            m_pStoneTexture->GetTextureView());
        m_pGfx->RenderModel(*pModel);

        // TODO: currently a dummy return value; implement it to use this pointer
        return pModel;
    }

private:
    void DrawAllModels()
    {
        for each (std::shared_ptr<Model> model in m_models)
        {
            DrawModel(model->m_Position, model);
        }
    }

    void PopulateModelList() {
        for (char i = 0; i < m_objectCount; i++)
        {
            m_models.push_back(ModelFactory(m_modSpecList[i], CUBE_TEXTURED));
        }
    }

    std::shared_ptr<Model> ModelFactory(ModelSpecs specs, eModType type)
    {
        bool result = true;
        std::shared_ptr<Model_Textured> pModel;
        pModel.reset(new Model_Textured(specs.position));
        //result = pModel != nullptr;
        //RETURN_MESSAGE_IF_FALSE(result, L"Could not allocate memory for Model.");

        PrimitiveFactory primMaker;
        primMaker.CreateCube({ 0.f, 0.f, 0.f }, { 5.f, 5.f, 5.f });
	    result = pModel->Initialize( primMaker, *m_pGfx );
	    //RETURN_IF_FALSE( result );

        return pModel;
    }

    bool InitShader()
    {
        bool result;
    // Initialize the texture shader object.
	m_pShader_Texture.reset( new Shader_Texture );
	result = m_pShader_Texture != nullptr;
	RETURN_MESSAGE_IF_FALSE( result, L"Could not allocate memory for Shader_Texture." );

	//result = m_pShader_Texture->Initialize( m_pD3D->GetDevice(), WinHandle, *m_models[0] );
	RETURN_IF_FALSE( result );

	m_pStoneTexture.reset( new Texture );
	RETURN_MESSAGE_IF_FALSE( m_pStoneTexture != nullptr, L"Could not allocate memory for Texture." );

	result = m_pStoneTexture->Initialize( *m_pGfx, L"Textures\\uncompressed_stone.tga" );
	RETURN_IF_FALSE( result );

    // TODO : Leaving overlay in main Game class for now until it is properly fixed.
	//result = m_Overlay.Initialize( *m_pGfx,ScreenWidth, ScreenHeight );
	//RETURN_IF_FALSE( result );

	return true;
    }


    private:

        HWND m_WinHandle;

        std::unique_ptr<Model_Textured> m_pModelTEST;

        std::shared_ptr<Shader_Texture> m_pShader_Texture;
	    std::shared_ptr<Texture> m_pStoneTexture;

        // store all game objects in a list for gameplay purposes
        vector<ModelSpecs> m_modSpecList;
        // list of actual models for rendering purposes
        vector<std::shared_ptr<Model>> m_models;
        char m_objectCount;

        // m_pGraphics and m_pDirect3D are created and passed to game without taking ownership
        Graphics* m_pGfx;
        D3DGraphics* m_pD3D;
        std::shared_ptr<Camera> m_pCam;
};