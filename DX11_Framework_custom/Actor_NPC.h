#pragma once
#include "Actor.h"
#include <time.h>

class Actor_NPC : public Actor
{
public:
	Actor_NPC() = default;

	Actor_NPC(
		const ModelSpecs_W &worldSpecs,
		eTexture tex,
		const ModelSpecs_L &localSpecs,
		eModType mod = CUBE_TEXTURED );

public:
	// Leaving these functions here for you to test with, 
	// please remember to put in CPP files when done.
    void GetInput(const Input& pInput, int randI, float randF) override
    {
        //srand( static_cast<unsigned int>( time( nullptr ) ) );
        //int dieRoll = rand() % 3;
        //float randNum = (rand()%50)/10.0f; // get random float from 0~10.0
        if (pInput.IsKeyDown(VK_RIGHT))
        {
            //if(randI == 0)
            //Move({ randF, randF, randF });
            //else
            Move({ .5f, 0.f, 0.f });
        }
        if (pInput.IsKeyDown(VK_LEFT))
        {
            //if(randI == 1)
            //    Move({ -randF, -randF, -randF });
            //else
            Move({ -.5f, 0.f, 0.f });
        }

        if (pInput.IsKeyDown(VK_UP))
        {
            //if(randI == 2)
            //Move({ randF, randF, randF });
            //else
            Move({ 0.f, 0.f, .5f });
        }
        if (pInput.IsKeyDown(VK_DOWN))
        {
            //if(randI == 3)
            //Move({ -randF, -randF, -randF });
            //else
            Move({ 0.f, 0.f, -.5f });
        }

        if (pInput.IsKeyDown(VK_SPACE))
        {
            Rotate({ 1.f,1.f,1.f });
        }

        if (pInput.IsKeyDown(VK_CONTROL))
        {
            Rotate({ -1.f,-1.f,-1.f });
        }
    }

private:
    void Move( const XMFLOAT3 &offset)
    { m_worldSpecs.position += offset; } 

    void Rotate( const XMFLOAT3 &rotation)
    { m_worldSpecs.orientation += rotation; }

};
