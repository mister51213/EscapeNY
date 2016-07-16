#include "D2DGraphics.h"

using namespace Microsoft::WRL;

D2DGraphics::D2DGraphics()
{
}


D2DGraphics::~D2DGraphics()
{}

void D2DGraphics::BeginDraw()
{	// Anything needed for the overlay should go between the begin and end
	// draw calls
	m_pRenderTarget->BeginDraw();
}

void D2DGraphics::EndDraw()
{
	m_pRenderTarget->EndDraw();

}

bool D2DGraphics::Initialize( D3DGraphics &D3D, const ImageLoader &ImgLoader, 
	const UINT ScreenWidth, const UINT ScreenHeight )
{
	ID2D1Factory *pFactory = nullptr;
	// Initialize the Direct2D factory
	HRESULT hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory );	
	m_pFactory.Attach( pFactory );
	RETURN_IF_FAILED( hr );

	// Use the image loader to create an empty texture buffer
	auto bitmapResult = ImgLoader.CreateBitmap( ScreenWidth, ScreenHeight );
	hr = bitmapResult.first;
	RETURN_IF_FAILED( hr );

	// Use the texture buffer to create a render target 
	m_pRenderSurface = std::move( bitmapResult.second );
	auto pixelFormat = D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED
	);
	auto renderProperties = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		pixelFormat
	);
	hr = m_pFactory->CreateWicBitmapRenderTarget(
		m_pRenderSurface.Get(),
		renderProperties,
		&m_pRenderTarget
	);
	RETURN_IF_FAILED( hr );

	// Create a solid color brush for painting the fonts
	D2D1::ColorF defaultColor( D2D1::ColorF::White );
	hr = m_pRenderTarget->CreateSolidColorBrush( defaultColor, &m_pBrush );
	RETURN_IF_FAILED( hr );

	// Initialize and create the overlay texture
	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Width = ScreenWidth;
	texDesc.Height = ScreenHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DYNAMIC;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texDesc.MiscFlags = 0;

	auto pDevice = D3D.GetDevice();
	hr = pDevice->CreateTexture2D( &texDesc, nullptr, &overlay );
	
	
	if( FAILED( hr ) )
	{
		MessageBox( nullptr, L"Could not initialize overlay texture", L"error", MB_OK );
		return false;
	}

	return true;
}

void D2DGraphics::Render( D3DGraphics &D3D )
{	
	// Get the pointer from the ID3D11Texture2D and pass to fillTexture
	D3D11_MAPPED_SUBRESOURCE ms{};
	auto context = D3D.GetDeviceContext();

	context->Map( overlay.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms );
	fillTexture( reinterpret_cast<BYTE *>( ms.pData ) );
	context->Unmap( overlay.Get(), 0 );

}

void D2DGraphics::DrawString( const std::wstring & String, const FontLoader & FntLoader, const D2D1_RECT_F &PositionAndSize )
{
	m_pRenderTarget->DrawTextW(
		String.c_str(), String.size(), FntLoader.GetTextFormat(),
		PositionAndSize, m_pBrush.Get()
	);
}

bool D2DGraphics::fillTexture( BYTE *const Pixels )const
{
	// The purpose of this function is to copy the render target to 
	// a ID3D11Texture2D, this will probably go through some changes
	// so I'll hold off commenting for now.

	ComPtr<IWICBitmapLock> lock;
	HRESULT hr = m_pRenderSurface->Lock( nullptr, WICBitmapLockRead, &lock );
	RETURN_IF_FAILED( hr );

	UINT stride = 0;
	hr = lock->GetStride( &stride );
	RETURN_IF_FAILED( hr );

	UINT width = 0, height = 0;
	hr = lock->GetSize( &width, &height );
	RETURN_IF_FAILED( hr );

	BYTE *data = nullptr;
	UINT bufferSize = stride * height;
	hr = lock->GetDataPointer( &bufferSize, &data );
	RETURN_IF_FAILED( hr );

	for( UINT y = 0; y < height; ++y )
	{
		CopyMemory( &Pixels[ y * stride ], &data[ y * stride ], stride );
	}

	return true;
}
