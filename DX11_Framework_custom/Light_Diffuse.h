#include "ILightDirection.h"
class Light_Diffuse :
    public ILightDirection
{
public:
    Light_Diffuse()
    {}

    ~Light_Diffuse()
    {}

    virtual const DirectX::XMFLOAT4 &GetAmbient()const{return m_ambient;}
	virtual void SetAmbient( float R, float G, float B ){ m_ambient = { R,G,B, 1.f }; }

    virtual const DirectX::XMFLOAT4 &GetColor()const { return m_color; }
    virtual void SetColor(float R, float G, float B) { m_color = { R,G,B, 1.f }; }

    virtual float GetIntensity()const { return m_intensity; }
    virtual void SetIntensity(float Value) {m_intensity = Value;}

    virtual const DirectX::XMFLOAT3 &GetDirection()const { return m_direction; }
    virtual void SetDirection(float X, float Y, float Z) { m_direction = { X,Y,Z }; }

private:
    XMFLOAT4 m_ambient = { .0f,.0f,0.1f,1.f };
    XMFLOAT4 m_color = { 1.f,1.f,1.f,1.f };
    XMFLOAT3 m_direction = { -.3f,-1.f,0.f };
    float m_intensity = 1.f;
};
