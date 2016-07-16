//////////////////////////////////////////////////////////////////////////
// Filename: graphics.cpp
//////////////////////////////////////////////////////////////////////////
#include "Graphics.h"

Graphics::Graphics()
{
}

Graphics::Graphics(const Graphics& other)
{
}

Graphics::~Graphics()
{
}

bool Graphics::Initialize( int ScreenWidth, int ScreenHeight, HWND WinHandle,
	float Red, float Green, float Blue, float Alpha )
{
	// Set background color
	SetBackgroundColor( Red, Green, Blue, Alpha );

	// Create the Direct3D object.
	m_pDirect3D.reset( new D3DGraphics );
	RETURN_MESSAGE_IF_FALSE( m_pDirect3D != nullptr,
		L"Could not allocate memory for Direct3D object.");

	// Initialize the Direct3D object.	
	RETURN_MESSAGE_IF_FALSE( m_pDirect3D->Initialize( ScreenWidth, ScreenHeight,
		VSYNC_ENABLED, WinHandle, FULL_SCREEN ), L"Could not initialize Direct3D." );

	return true;
}

// Called at the beginning of each frame whenever we are going to draw a new 3D scene.
// Initializes the buffers so they are blank and ready to be drawn to.
void Graphics::BeginScene()
{
	auto pContext = m_pDirect3D->m_deviceContext.Get();
	auto pRenderTargetView = m_pDirect3D->m_renderTargetView.Get();
	auto pDepthStencilView = m_pDirect3D->m_depthStencilView.Get();

	// Clear the back buffer.
	pContext->ClearRenderTargetView( pRenderTargetView, m_backgroundColor );

	// Clear the depth buffer.
	pContext->ClearDepthStencilView( pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

// Tells the swap chain to display our 3D scene once all 
// the drawing has completed at the end of each frame.
void Graphics::EndScene()
{	
	// Present the back buffer to the screen since rendering is complete.
	m_pDirect3D->m_swapChain->Present( m_pDirect3D->m_vsync_enabled ? 1 : 0, 0 );
}

void Graphics::SetBackgroundColor( float Red, float Green, float Blue, float Alpha )
{
	m_backgroundColor[ 0 ] = Red;
	m_backgroundColor[ 1 ] = Green;
	m_backgroundColor[ 2 ] = Blue;
	m_backgroundColor[ 3 ] = Alpha;
}

D3DGraphics * Graphics::GetDirect3D() const
{
	return m_pDirect3D.get();
}
