#pragma once
#include "ILightPoint.h"
#include <vector>
#include <time.h>
#include "Utilities.h"

using namespace std;

class Light_Point :
    public ILightPoint
{
public:

    Light_Point(const unsigned int& lightCount)
    {
        m_numLights = lightCount;
    }

    ~Light_Point()
    {}

    void Initalize()
    {
    srand( static_cast<unsigned int>( time( nullptr ) ) );
    float randF = static_cast<float>(rand() % 100)*.01f; // get random float from 0~1.0

        m_lights.resize(m_numLights);
        for (int i = 0; i < m_numLights; i++)
        {
            // Add random color 0~1.0
           XMStoreFloat4(&(m_lights[i].color), { randF,randF+.01f,randF+.05f,1.f });

            // Add random position 0~30.f
           randF = static_cast<float>(rand() % 3000)*.01f;
           XMStoreFloat4(&(m_lights[i].position), { randF,randF+.01f,randF+.05f,1.f });

           // Add random intensity 0~1.0
           m_lights[i].intensity = static_cast<float>(rand() % 100)*.01f;
        }
    }

const DirectX::XMFLOAT3 &GetPosition()const = 0;

void SetPosition( float X, float Y, float Z ) = 0;

private:
    int m_numLights = 4;
    vector<PtLightBufferType> m_lights;

};

