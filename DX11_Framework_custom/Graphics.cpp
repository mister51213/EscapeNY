#include "Graphics.h"

Graphics::Graphics()
{
	m_pDirect3D = 0;
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

	// Create the Direct3D object.
	m_pDirect3D = new D3DGraphics;
	if (!m_pDirect3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_pDirect3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Graphics::Shutdown()
{
	// Release the Direct3D object.
	if (m_pDirect3D)
	{
		m_pDirect3D->Shutdown();
		delete m_pDirect3D;
		m_pDirect3D = 0;
	}

	return;
}

bool Graphics::Frame()
{
	bool result;

	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::Render()
{
	// Clear the buffers to begin the scene.
    // Parameters represent render background color.
	m_pDirect3D->BeginScene(0.5f, 2.0f, 0.0f, 1.0f);

	// Present the rendered scene to the screen.
	m_pDirect3D->EndScene();

	return true;
}
