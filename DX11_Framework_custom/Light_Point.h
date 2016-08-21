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

    Light_Point()
    {}

    ~Light_Point()
    {}

    void InitLights(unsigned int lightCount = 4) override 
    {
    m_numLights = lightCount;

    srand( static_cast<unsigned int>( time( nullptr ) ) );
    float randF = static_cast<float>(rand() % 100)*.01f; // get random float from 0~1.0

    //TEST
    m_colors.diffuseColor[0] = { 1.f,1.f,1.f,1.f };
    m_colors.diffuseColor[1] = { 1.f,1.f,1.f,1.f };
    m_colors.diffuseColor[2] = { 0.f,0.f,1.f,1.f };
    m_colors.diffuseColor[3] = { 0.f,0.f,1.f,1.f };

    m_positions.lightPosition[0] = { -100.f,100.f,-100.f,1.f };
    m_positions.lightPosition[1] = { 100.f,100.f,-100.f,1.f };
    m_positions.lightPosition[2] = { -100.f,100.f,100.f,1.f };
    m_positions.lightPosition[3] = { 100.f,100.f,100.f,1.f };

    //for (int i = 0; i < m_numLights; i++)
        //{
            // Add random color 0~1.0
           //XMStoreFloat4(&(m_colors.diffuseColor[i]), { randF,randF+.01f,randF+.05f,1.f });

            // Add random position 0~30.f
           //randF = static_cast<float>(rand() % 2000)*.01f;
           //XMStoreFloat4(&(m_positions.lightPosition[i]), { randF,randF+.01f,randF+.05f,1.f });

           // Add random intensity 0~1.0
           //m_lights[i].intensity = static_cast<float>(rand() % 100)*.01f;
        //}
    }

    LightColorBufferType GetColors() const override { return m_colors; }
    LightPositionBufferType GetPositions() const override { return m_positions; }

    virtual unsigned int GetNumLights() const override { return m_numLights; }

    //vector<PtLightBufferType> GetLightList() const { return m_lights; }

// NOT USED
    virtual float GetIntensity()const { return 4.f;}
    virtual void SetIntensity(float Value) {}
    virtual const DirectX::XMFLOAT4 &GetColor()const { return XMFLOAT4{}; }
    virtual void SetColor(float R, float G, float B) {}
    virtual const DirectX::XMFLOAT4 &GetAmbient()const { return XMFLOAT4{}; }
    virtual void SetAmbient(float R, float G, float B) {}

private:
    int m_numLights;
    //vector<PtLightBufferType> m_lights;
    LightColorBufferType m_colors;
    LightPositionBufferType m_positions;
};

