#pragma once

#include "Camera.h"
#include "Shader.h"
#include "ILightSet.h"
#include "Model.h"
#include "Actor.h"
#include "Actor_Player_Alt.h"

class Scene_Night
{
public:
	Scene_Night();
	~Scene_Night();
	bool Initialize(
		Camera *m_pCam,
		ID3D11Device *pDevice, 
		ID3D11DeviceContext *pContext);
private:
	Camera *m_pCam;
	std::unique_ptr<ILightSet> m_pLightSet;
	std::vector<std::unique_ptr<Shader>> m_pShaders;
	std::vector<std::unique_ptr<Model>> m_pModels;	
	std::vector<Actor *> m_pActorDrawList;
	Actor_Player_Alt m_player;
	ID3D11DeviceContext *m_pContext;
};

