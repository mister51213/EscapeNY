#include "Graphics.h"

Graphics::Graphics()
{
    // initialize pointer to null for safety
    	m_pD3D = 0;
}

Graphics::Graphics(const Graphics& other)
{
}


Graphics::~Graphics()
{
}


bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
    bool result;

    // Create Direct 3D object
    m_pD3D = new D3DGraphics;
    if (!m_pD3D)
    {
        false;
    }

    // Initialize Direct3D object
    result = m_pD3D->Initialize(
        screenWidth, screenHeight,
        VSYNC_ENABLED, hwnd, FULL_SCREEN,
        SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize Direct3D", L"error", MB_OK);
        return false;
    }

	return true;
}

bool Graphics::Frame()
{
    bool result;

	// Render the graphics scene each frame.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}

bool Graphics::Render()
{
    // Clear the buffers to begin the scene.
	m_pD3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// Present the rendered scene to the screen.
	m_pD3D->EndScene();

	return true;
}

void Graphics::Shutdown()
{

	return;
}