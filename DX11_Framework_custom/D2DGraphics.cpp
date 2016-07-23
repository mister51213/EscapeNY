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
	m_pRenderTarget->BeginDraw();
   	m_pRenderTarget->Clear();
}

void D2DGraphics::EndDraw()
{
	m_pRenderTarget->EndDraw();
}

bool D2DGraphics::Initialize( const Graphics &Gfx, const UINT ScreenWidth, const UINT ScreenHeight )
{	
	ID2D1Factory *pFactory = nullptr;
	// Initialize the Direct2D factory
	HRESULT hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory );	
	m_pFactory.Attach( pFactory );
	RETURN_IF_FAILED( hr );

	// Use the image loader to create an empty texture buffer
	auto bitmapResult = ImageLoader::CreateBitmap( ScreenWidth, ScreenHeight, *Gfx.GetWIC() );
	hr = bitmapResult.first;
	RETURN_IF_FAILED( hr );

	// Use the texture buffer to create a render target 
	// First, move the newly created empty bitmap to the bitmap member
	m_pRenderSurface = std::move( bitmapResult.second );

	// Create a pixel format for the render target
	auto pixelFormat = D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED
	);

	// Setup the render target properties
	auto renderProperties = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		pixelFormat
	);

	// Create the render target using the bitmap member, pixel format 
	// and render target properties	
	hr = m_pFactory->CreateWicBitmapRenderTarget(
		m_pRenderSurface.Get(),
		renderProperties,
		&m_pRenderTarget
	);
	RETURN_IF_FAILED( hr );

	// Create a solid color brush for painting the fonts
	D2D1::ColorF defaultColor( D2D1::ColorF::White );
	hr = m_pRenderTarget->CreateSolidColorBrush( defaultColor, m_pBrush.GetAddressOf() );
	RETURN_IF_FAILED( hr );

	return true;
}

IWICBitmap * D2DGraphics::GetRenderSurface() const
{
	return m_pRenderSurface.Get();
}