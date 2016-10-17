#pragma once
#include "Actor.h"
#include "Light_Spot.h"
#include "Camera.h"

class Actor_Light :
    public Actor
{
public:
     Actor_Light() = default;
    ~Actor_Light() = default;

    // NOTE: MUST CALL PARENT INITIALIZE FIRST!
	void InitSpotLight(const Camera &Cam);
	void SetPosition(const XMFLOAT3 &Pos);
	void SetOrientation(const XMFLOAT3 &Dir);
	ILight* GetLight();

protected:
    Light_Spot m_spotLight;
};

