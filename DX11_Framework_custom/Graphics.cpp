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
    // Create Direct 3D object
    m_pD3D = new D3DGraphics;
	bool result = ( m_pD3D != nullptr );

	if( !result )
	{
		MessageBox( hwnd, L"Could not allocate memory for D3DGraphics", L"error", MB_OK );
		return false;
    }
    
	// Initialize Direct3D object
	result = m_pD3D->Initialize(
		screenWidth, screenHeight,
		VSYNC_ENABLED, hwnd, FULL_SCREEN,
		SCREEN_DEPTH, SCREEN_NEAR );
	if( !result )
	{
		MessageBox( hwnd, L"Could not initialize Direct3D", L"error", MB_OK );
		return false;
	}
	
	// Initialize ImageLoader
	m_pImgLoader = std::make_unique<ImageLoader>();
	result = m_pImgLoader->Initialize();
	if( !result )
	{
		MessageBox( hwnd, L"Could not initialize the ImageLoader", L"error", MB_OK );
		return false;
	}

	// Initialize Direct2D
	m_pD2D = std::make_unique<D2DGraphics>();
	result = m_pD2D->Initialize( *m_pD3D, *m_pImgLoader, screenWidth, screenHeight );
	if( !result )
	{
		MessageBox( hwnd, L"Could not initialize Direct2D", L"error", MB_OK );
		return false;
	}

	// Initialize FontLoader
	m_pFntLoader = std::make_unique<FontLoader>();
	result = m_pFntLoader->Initialize();
	if( !result )
	{
		MessageBox( hwnd, L"Could not initialize FontLoader", L"error", MB_OK );
		return false;
	}

	return true;
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

	/*********************************************************************/
	// This section should be after the D3D rendering
	m_pD2D->BeginDraw();
	// Renders the string using the default font (Arial)
	auto positionAndSize = D2D1::RectF( 0.f, 0.f, 90.f, 24.f );
	m_pD2D->DrawString( L"This is a sample string", *m_pFntLoader, positionAndSize );
	// Finalize the 2D rendering
	m_pD2D->EndDraw();
	// Render overlay surface
	m_pD2D->Render( *m_pD3D );

	// TODO: Needs the Texture class and TextureShader classes to be finished
	// Will need to set the overlay as the active shader resource before presenting
	/*********************************************************************/
	// Present the rendered scene to the screen.
	m_pD3D->EndScene();

	return true;
}

void Graphics::Shutdown()
{

	return;
}