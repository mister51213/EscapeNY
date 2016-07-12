#include "D2DGraphics.h"

using namespace Microsoft::WRL;

D2DGraphics::D2DGraphics()
{
}


D2DGraphics::~D2DGraphics()
{}

bool D2DGraphics::Initialize( const ImageLoader &ImgLoader, const UINT Width, const UINT Height )
{
	// Initialize the Direct2D factory
	HRESULT hr = D2D1CreateFactory( 
		D2D1_FACTORY_TYPE_SINGLE_THREADED, m_pFactory.GetAddressOf() 
	);
	if( FAILED( hr ) )
	{
		return false;
	}

	// Use the image loader to create an empty texture buffer
	auto bitmapResult = ImgLoader.CreateBitmap( Width, Height );
	hr = bitmapResult.first;
	if( FAILED( hr ) )
	{
		return false;
	}

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
		m_pRenderTarget.GetAddressOf()
	);
	if( FAILED( hr ) )
	{
		return false;
	}

	// Create a solid color brush for painting the fonts
	D2D1::ColorF defaultColor( D2D1::ColorF::White );
	hr = m_pRenderTarget->CreateSolidColorBrush( defaultColor, m_pBrush.GetAddressOf() );
	if( FAILED( hr ) )
	{
		return false;
	}

	return true;
}

bool D2DGraphics::FillTexture( BYTE *const Pixels )const
{
	// The purpose of this function is to copy the render target to 
	// a ID3D11Texture2D, this will probably go through some changes
	// so I'll hold off commenting for now.
	BYTE *data = nullptr;
	UINT stride = 0;
	UINT width = 0;
	UINT height = 0;

	ComPtr<IWICBitmapLock> lock;
	HRESULT hr = m_pRenderSurface->Lock(
		nullptr, WICBitmapLockRead, lock.GetAddressOf() 
	);
	if( SUCCEEDED( hr ) )
	{
		hr = lock->GetStride( &stride );
	}
	if( SUCCEEDED( hr ) )
	{
		hr = lock->GetSize( &width, &height );
	}
	if( SUCCEEDED( hr ) )
	{
		UINT bufferSize = stride * height;
		hr = lock->GetDataPointer( &bufferSize, &data );
	}
	
	if( SUCCEEDED( hr ) )
	{
		for( UINT y = 0; y < height; ++y )
		{
			CopyMemory( &Pixels[ y * stride ], &data[ y * stride ], stride );
		}
	}


	return SUCCEEDED( hr );
}
