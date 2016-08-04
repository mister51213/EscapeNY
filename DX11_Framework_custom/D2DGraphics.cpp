#include "D2DGraphics.h"
#include "Graphics.h"

D2DGraphics::D2DGraphics()
{
}


D2DGraphics::~D2DGraphics()
{}

void D2DGraphics::BeginDraw()
{	
    // Anything needed for the overlay should go between the begin and end
	// draw calls
	m_pContext->BeginDraw();
}

void D2DGraphics::EndDraw()
{
	HRESULT hr = m_pContext->EndDraw();
}

bool D2DGraphics::Initialize( const Graphics &Gfx, const UINT ScreenWidth, const UINT ScreenHeight )
{	
	// Get the D3D device and use it to create the D2D device
	auto pDevice3D = Gfx.GetDirect3D()->GetDevice();
	comptr<IDXGIDevice> pDxgiDevice;
	
	// Casts the D3D device pointer to a DXGIDevice pointer
	HRESULT hr = pDevice3D->QueryInterface( pDxgiDevice.GetAddressOf() );
	
	// Creates the D2D1Device
	hr = D2D1CreateDevice( pDxgiDevice.Get(), nullptr, m_pDevice.GetAddressOf() );
	RETURN_IF_FAILED( hr );

	// Use the D2D device to create the D2DDeviceContext
	hr = m_pDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_pContext.GetAddressOf()
	);
	RETURN_IF_FAILED( hr );

	// Get the D3D render target (the back buffer) and convert to 
	// an intermediate DXGI surface
	auto pRenderTarget3D = Gfx.GetDirect3D()->GetRenderTarget();
	IDXGISurface *pSurface = nullptr;
	hr = pRenderTarget3D->QueryInterface( &pSurface );
	RETURN_IF_FAILED( hr );

	// Use the intermediate DXGI surface to create the D2D render target
	hr = m_pContext->CreateBitmapFromDxgiSurface( 
		pSurface,
		nullptr, 
		m_pRenderSurface.GetAddressOf() );
	RETURN_IF_FAILED( hr );

	m_pContext->SetTarget( m_pRenderSurface.Get() );

	// Create a solid color brush for painting the fonts
	D2D1::ColorF defaultColor( D2D1::ColorF::White );
	hr = m_pContext->CreateSolidColorBrush( defaultColor, m_pBrush.GetAddressOf() );
	RETURN_IF_FAILED( hr );

	return true;
}
