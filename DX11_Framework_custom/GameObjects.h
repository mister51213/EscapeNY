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

struct ModelSpecs
{
    XMFLOAT3 position, orientation, scale;
};

enum eModType{Cube, CubeTextured, Plane, Sphere, Polygon};

class GameObjects
{
public:
    GameObjects(){}

    GameObjects(char numObjects, Graphics* pGfx) 
    {
        m_pGfx = pGfx;
        m_objectCount = numObjects;
        for (char i = 0; i < m_objectCount; i++)
        {
            m_models.push_back(ModelFactory(m_modSpecList[i], CubeTextured));
        }
    }

private:
    std::shared_ptr<Model> ModelFactory(ModelSpecs specs, eModType type)
    {
        bool result = true;
        std::shared_ptr<Model_Textured> pModel;
        pModel.reset(new Model_Textured(specs.position));
        //result = pModel != nullptr;
        //RETURN_MESSAGE_IF_FALSE(result, L"Could not allocate memory for Model.");

        PrimitiveMaker primMaker;
        primMaker.CreateCube({ 0.f, 0.f, 0.f }, { 5.f, 5.f, 5.f });
	    result = pModel->Initialize( primMaker, *m_pGfx );
	    //RETURN_IF_FALSE( result );

        return pModel;
    }


    private:
        // store all game objects in a list for gameplay purposes
        vector<ModelSpecs> m_modSpecList;
        vector<std::shared_ptr<Model>> m_models;
        char m_objectCount;

        // m_pGraphics and m_pDirect3D are created and passed to game without taking ownership
        Graphics *m_pGfx;
        D3DGraphics *m_pD3D;
};