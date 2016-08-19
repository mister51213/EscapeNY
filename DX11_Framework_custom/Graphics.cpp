//////////////////////////////////////////////////////////////////////////
// Filename: graphics.cpp
//////////////////////////////////////////////////////////////////////////
#include "Graphics.h"
#include "Model.h"

Graphics::Graphics()
{
}

Graphics::Graphics(const Graphics& other)
{
}

Graphics::~Graphics()
{
}

bool Graphics::Initialize( 
    int ScreenWidth, 
    int ScreenHeight, 
    HWND WinHandle,
	float Red, 
    float Green, 
    float Blue, 
    float Alpha )
{
	// Create the Windows Imaging Component object
	m_pWic.reset( new Wic );
	RETURN_MESSAGE_IF_FALSE( m_pWic != nullptr,
		L"Could not allocate memory for Windows Imaging Component object." );

	// Initialize the Windows Imaging Comonent object	
	RETURN_MESSAGE_IF_FALSE( m_pWic->Initialize(),
		L"Could not initialize the Windows Imaging Component ." );

	// Set background color
	SetBackgroundColor( Red, Green, Blue, Alpha );

	// Create the Direct3D object.
	m_pDirect3D.reset( new D3DGraphics );
	RETURN_MESSAGE_IF_FALSE( m_pDirect3D != nullptr,
		L"Could not allocate memory for Direct3D object.");

	// Initialize the Direct3D object.	
	RETURN_MESSAGE_IF_FALSE(
		m_pDirect3D->Initialize(
			g_screenWidth, g_screenHeight,
			g_vsyncEnabled,
			WinHandle, g_fullScreen ),
		L"Could not initialize Direct3D." );

	// Create the Direct3D object.
	m_pDirect2D.reset( new D2DGraphics );
	RETURN_MESSAGE_IF_FALSE( m_pDirect3D != nullptr,
		L"Could not allocate memory for Direct2D object." );

	// Initialize the Direct3D object.	
	RETURN_MESSAGE_IF_FALSE( 
		m_pDirect2D->Initialize( *this, ScreenWidth, ScreenHeight ), 
		L"Could not initialize Direct3D." );

	return true;
}

// Called at the beginning of each frame whenever we are going to draw a new 3D scene.
// Initializes the buffers so they are blank and ready to be drawn to.
void Graphics::BeginScene()
{
	auto pContext = m_pDirect3D->m_pDeviceContext.Get();
	auto pRenderTargetView = m_pDirect3D->m_pRenderTargetView.Get();
	auto pDepthStencilView = m_pDirect3D->m_pDepthStencilView.Get();

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
	m_pDirect3D->m_pSwapChain->Present( m_pDirect3D->m_vsync_enabled ? 1 : 0, 0 );
}

void Graphics::BeginDraw2D()const
{
	m_pDirect2D->BeginDraw();
}

void Graphics::EndDraw2D()const
{
	m_pDirect2D->EndDraw();
}

void Graphics::RenderString( const std::wstring & String, IDWriteTextFormat *pFont, const D2D1_RECT_F &PositionAndSize )const
{
	m_pDirect2D->m_pContext->DrawTextW( String.c_str(), String.size(), pFont, PositionAndSize,
	m_pDirect2D->m_pBrush.Get() );
}

//TODO: why is this getting a null pointer now????
void Graphics::RenderModel( const Model &rModel )const
{
	// Set vertex buffer stride and offset.
	auto indexCount = rModel.GetIndexCount();
	unsigned int stride = rModel.GetStride();
	unsigned int offset = 0;
	auto pVertexBuffer = rModel.GetVertexBuffer();
	auto pIndexBuffer = rModel.GetIndexBuffer();

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	m_pDirect3D->m_pDeviceContext->IASetVertexBuffers( 0, 1, &pVertexBuffer, &stride, &offset );

	// Set the index buffer to active in the input assembler so it can be rendered.
	m_pDirect3D->m_pDeviceContext->IASetIndexBuffer( pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	m_pDirect3D->m_pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	m_pDirect3D->m_pDeviceContext->DrawIndexed( indexCount, 0, 0 );

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

Wic * Graphics::GetWIC() const
{
	return m_pWic.get();
}

D2DGraphics * Graphics::GetDirect2D() const
{
	return m_pDirect2D.get();
}