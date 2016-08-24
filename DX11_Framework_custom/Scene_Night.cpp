#include "Scene_Night.h"
#include "LightSet_Night.h"


Scene_Night::Scene_Night()
{
}


Scene_Night::~Scene_Night()
{
}

bool Scene_Night::Initialize( Camera * pCam, ID3D11Device * pDevice, ID3D11DeviceContext * pContext )
{
	m_pCam = pCam;
	m_pContext = pContext;

	m_pLightSet.reset( new LightSet_Night );

	ModelSpecs_L lSpecs{};
	lSpecs.center = { 0.f, -1.f, 0.f };
	lSpecs.size = { 1.f, 2.f, 1.f };

	return true;
}
