#pragma once
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "D3DGraphics.h"
#include "camera.h"
#include "model.h"
#include "colorshader.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

////////////////////////////////////////////////////////////////////////////////
// Class name: Graphics
////////////////////////////////////////////////////////////////////////////////
class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
   	D3DGraphics* m_pDirect3D;    
	Camera* m_Camera;
	Model* m_Model;
	ColorShader* m_ColorShader;
};