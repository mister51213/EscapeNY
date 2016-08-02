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


class GameObjects
{
public:
    GameObjects() {}

    GameObjects(Model& mod)
    {
        for each (auto model in models)
        {
            *model = mod;
        }    
    }

    void CreateModels() {}




    bool InitializeAllModels()
    {
    bool result = true;
    ////////////////////////////////////////////////////////
    // Create Model objects
    ////////////////////////////////////////////////////////
    m_pModel.reset(new Model_Textured);
    result = m_pModel != nullptr;

    RETURN_MESSAGE_IF_FALSE(result, L"Could not allocate memory for Model.");
    m_pModel2.reset(new Model_Textured);
    result = m_pModel2 != nullptr;
    RETURN_MESSAGE_IF_FALSE(result, L"Could not allocate memory for Model.");

    ////////////////////////////////////////
    // Feed the Pimitive Maker w cube objects
    ////////////////////////////////////////
    PrimitiveMaker primMaker;
    primMaker.CreateCube({ 1.f, 1.f, 1.f },{ 1.f, 1.f, 1.f });
    PrimitiveMaker primMaker2;
    primMaker2.CreateCube({ 3.f, 3.f, 3.f },{ 1.f, 1.f, 1.f });

    // Initialize models
	result = m_pModel->Initialize( primMaker, *m_pGfx );
	RETURN_IF_FALSE( result );
	result = m_pModel2->Initialize( primMaker2, *m_pGfx );
	RETURN_IF_FALSE( result );


    return true;
    }

    bool RenderShapes()
    {
	// Render the model using the color shader.
	//bool result = m_pShader_Texture->Render(
	//	m_pDirect3D->GetDeviceContext(), 
	//	m_pModel->GetWorldMatrix(),
	//	m_pCamera->GetViewMatrix(),
	//	m_pCamera->GetProjectionMatrix(),
	//	m_pStoneTexture->GetTextureView() );

 //   // Render model 2 color
 //   	bool result2 = m_pShader_Texture->Render(
	//	m_pDirect3D->GetDeviceContext(), 
	//	m_pModel2->GetWorldMatrix(),
	//	m_pCamera->GetViewMatrix(),
	//	m_pCamera->GetProjectionMatrix(),
	//	m_pStoneTexture->GetTextureView() );    
 //   }


private:

    // TODO: Add these to models list   
    std::unique_ptr<Model_Textured> m_pModel3;

    // store all game objects in a list
    vector<Model*> models;
    
    // m_pGraphics and m_pDirect3D are created and passed to game without taking ownership
	Graphics *m_pGfx;
	D3DGraphics *m_pD3D;
};