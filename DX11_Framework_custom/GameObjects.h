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
struct ModelSpecs_W
{
    XMFLOAT3 position, orientation, scale;
};

struct ModelSpecs_L
{
    XMFLOAT3 center, size, orientation;
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
    
public:
    vector<std::shared_ptr<Model>> MakeAllModels(){/*TODO: IMPLEMENT*/}
    vector<std::shared_ptr<Model>> DrawAllModels(){/*TODO: IMPLEMENT*/}

//private:
    std::shared_ptr<Model> MakeModel( 
        ModelSpecs_W worldSpecs, 
        ModelSpecs_L localSpecs = { { 0.f, 0.f, 0.f }, { 5.f, 5.f, 5.f }, { 0.f, 0.f, 0.f } },
        eModType = CUBE_TEXTURED) 
    {
        std::shared_ptr<Model_Textured> pModel;
        // INITIALIZE MODEL //
        pModel.reset(new Model_Textured(worldSpecs.position));
        PrimitiveFactory primMaker;
        primMaker.CreateCube(localSpecs.center, localSpecs.size, localSpecs.orientation);
	    pModel->Initialize( primMaker, *m_pGfx );
        return pModel; 
    }

    void DrawModel( std::shared_ptr<Model> pMod ) 
    { 
        // INITIALIZE TEXTURE SHADER //
        m_pShader_Texture.reset(new Shader_Texture);        
        m_pShader_Texture->Initialize(m_pD3D->GetDevice(), m_WinHandle, *pMod);
        m_pStoneTexture.reset(new Texture);
        m_pStoneTexture->Initialize(*m_pGfx, L"Textures\\uncompressed_stone.tga");    
        // RENDER MODEL using texture //
        m_pShader_Texture->Render(
            m_pD3D->GetDeviceContext(),
            GetWorldMatrix(
                pMod->m_Position, 
                ConvertToRadians(pMod->m_Orientation), 
                pMod->m_Scale),
            m_pCam->GetViewMatrix(),
            m_pCam->GetProjectionMatrix(),
            m_pStoneTexture->GetTextureView());
        m_pGfx->RenderModel(*pMod);
        //return pModel;        
    }

private:
    //void PopulateModels() {
    //    for (char i = 0; i < m_objectCount; i++)
    //    {
    //        m_models.push_back(MakeModel(m_modSpecs_L[i], m_modSpecs_W[i], CUBE_TEXTURED));
    //        DrawModel(m_models[i]->m_Position);
    //    }
    //}
    //    void DrawAllModels()
    //{
    //    for each (std::shared_ptr<Model> model in m_models)
    //    {
    //        DrawModel(model->m_Position);
    //    }
    //}

 //   bool InitShader()
 //   {
 //       bool result;
 //   // Initialize the texture shader object.
	//m_pShader_Texture.reset( new Shader_Texture );
	//result = m_pShader_Texture != nullptr;
	//RETURN_MESSAGE_IF_FALSE( result, L"Could not allocate memory for Shader_Texture." );
	////result = m_pShader_Texture->Initialize( m_pD3D->GetDevice(), WinHandle, *m_models[0] );
	//RETURN_IF_FALSE( result );
	//m_pStoneTexture.reset( new Texture );
	//RETURN_MESSAGE_IF_FALSE( m_pStoneTexture != nullptr, L"Could not allocate memory for Texture." );
	//result = m_pStoneTexture->Initialize( *m_pGfx, L"Textures\\uncompressed_stone.tga" );
	//RETURN_IF_FALSE( result );
 // // TODO : Leaving overlay in main Game class for now until it is properly fixed.
	////result = m_Overlay.Initialize( *m_pGfx,ScreenWidth, ScreenHeight );
	////RETURN_IF_FALSE( result );
	//return true;
 //   }

    private:
        HWND m_WinHandle;
        Graphics* m_pGfx;
        D3DGraphics* m_pD3D;
        std::shared_ptr<Camera> m_pCam;

        std::unique_ptr<Model_Textured> m_pModelTEST;
        std::shared_ptr<Shader_Texture> m_pShader_Texture;
	    std::shared_ptr<Texture> m_pStoneTexture;

        vector<ModelSpecs_W> m_modSpecs_W;// model specs list in WORLD SPACE
        vector<ModelSpecs_L> m_modSpecs_L;// model specs list in LOCAL SPACE
        vector<std::shared_ptr<Model>> m_models; // list of actual models for rendering purposes

        char m_objectCount;
};