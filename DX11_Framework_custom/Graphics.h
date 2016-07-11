#pragma once
#include <windows.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "D2DGraphics.h"
#include "ImageLoader.h"
#include "d3dgraphics.h"
#include <memory>

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
    D3DGraphics* m_pD3D;
	std::unique_ptr<D2DGraphics> m_pD2D;
	std::unique_ptr<ImageLoader> m_pImgLoader;
};