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
    bool result = true;

    // Create Direct 3D object
    m_pD3D = new D3DGraphics;

    if (!m_pD3D)
    {
        result = false;
    }
    // Initialize Direct3D object
	if( result )
	{
		result = m_pD3D->Initialize(
			screenWidth, screenHeight,
			VSYNC_ENABLED, hwnd, FULL_SCREEN,
			SCREEN_DEPTH, SCREEN_NEAR );
	}
	else
	{
		result = false;
		MessageBox( hwnd, L"Could not initialize Direct3D", L"error", MB_OK );
	}
	// Initialize ImageLoader
	if( result )
	{
		m_pImgLoader = std::make_unique<ImageLoader>();
		result = m_pImgLoader->Initialize();
	}
	else
	{
		result = false;
		MessageBox( hwnd, L"Could not initialize the ImageLoader", L"error", MB_OK );
	}
	// Initialize Direct2D
	if( result )
	{
		m_pD2D = std::make_unique<D2DGraphics>();
		result = m_pD2D->Initialize( *m_pImgLoader, screenWidth, screenHeight );
	}
	else
	{
		result = false;
		MessageBox( hwnd, L"Could not initialize Direct2D", L"error", MB_OK );
	}
	return result;
}

bool Graphics::Frame()
{
    bool result;

	// Render the graphics scene each frame.
	result = Render();
	
	return result;
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