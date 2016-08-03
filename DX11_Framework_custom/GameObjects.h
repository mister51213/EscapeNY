////////////////////////////////////////////////////////////////////////////////
//  TODO:
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

enum eModType{CUBE, CUBETEXTURED, PLANE, SPHERE, POLYGON};

class GameObjects
{
public:
    GameObjects(){}

    GameObjects(char numObjects, Graphics* pGfx) 
    {
        m_pGfx = pGfx;
        m_objectCount = numObjects;
    }

private:

    bool CreateSingleModel(XMFLOAT3 offset)
    {

        //// Create model 3 TEST    
        //m_pModelTEST.reset(new Model_Textured(offset));
        //result = m_pModelTEST != nullptr;
        //RETURN_MESSAGE_IF_FALSE(result, L"Could not allocate memory for Model.");
        //primMaker.CreateCube({ 0.f, 0.f, 0.f }, { 3.f, 5.f, 30.f });
        //result = m_pModelTEST->Initialize(primMaker, *m_pGraphics);
        //RETURN_IF_FALSE(result);

        //// Render model 3
        //bool result3 = m_pShader_Texture->Render(
        //    m_pDirect3D->GetDeviceContext(),
        //    GetWorldMatrix(m_pModelTEST->m_Position, ConvertToRadians(m_pModelTEST->m_Orientation), m_pModelTEST->m_Scale),
        //    m_pCamera->GetViewMatrix(),
        //    m_pCamera->GetProjectionMatrix(),
        //    m_pStoneTexture->GetTextureView());

        //m_pGraphics->RenderModel(*m_pModelTEST);
        //RETURN_IF_FALSE(result3);

    }

    void PopulateModelList() {
        for (char i = 0; i < m_objectCount; i++)
        {
            m_models.push_back(ModelFactory(m_modSpecList[i], CUBETEXTURED));
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
        std::unique_ptr<Model_Textured> m_pModelTEST;

        std::unique_ptr<Shader_Texture> m_pShader_Texture;
	    std::unique_ptr<Texture> m_pStoneTexture;

        // store all game objects in a list for gameplay purposes
        vector<ModelSpecs> m_modSpecList;
        // list of actual models for rendering purposes
        vector<std::shared_ptr<Model>> m_models;
        char m_objectCount;

        // m_pGraphics and m_pDirect3D are created and passed to game without taking ownership
        Graphics *m_pGfx;
        D3DGraphics *m_pD3D;
};