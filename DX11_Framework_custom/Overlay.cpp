#include "Overlay.h"

Overlay::Overlay()
{}


Overlay::~Overlay()
{}

bool Overlay::Initialize( const Graphics &Gfx )
{
	m_pRenderSurface = Gfx.GetDirect2D()->GetRenderSurface();
	
	bool result = m_texture.Initialize( Gfx, m_pRenderSurface );
	RETURN_IF_FALSE( result );

	PrimitiveMaker primMaker;
	primMaker.CreatePlane( {0.f,300.f,0.f}, {200.f,20.f} );
	result = m_TexturedModel.Initialize( primMaker, Gfx );

	FontLoader fntLoader;
	result = fntLoader.Initialize();
	RETURN_IF_FALSE( result );

	m_Font = fntLoader.MakeFont( L"Arial" );
	RETURN_IF_FALSE( m_Font != nullptr );
	
	return true;
}

ID3D11ShaderResourceView * Overlay::GetResourceView()
{
	return m_texture.GetTextureView();
}

// The Render function puts the model geometry on the video card to prepare it for drawing by the color shader.
void Overlay::Render( const Graphics &Gfx )
{
	// Begin and End draw are done here because the overlay is rendering to a texture
	// and not to the back buffer, so Overlay needs to control when to begin and end the
	// drawing.
	
	// All overlay rendering should be done between BeginDraw and EndDraw
	Gfx.BeginDraw2D();

	// The rect is where to draw the text and when to either wrap the text or cut it off		
	Gfx.RenderString( L"Hello", m_Font.Get(), D2D1::RectF( 0.f, 0.f, 80.f, 60.f ) );
	
	// Signal direct2d that we are done drawing
	Gfx.EndDraw2D();

	// Get the pointer from the ID3D11Texture2D and pass to fillTexture
	D3D11_MAPPED_SUBRESOURCE ms{};
	auto context = Gfx.GetDirect3D()->GetDeviceContext();
	auto pTexture = m_texture.GetTexture();

	// Fill the ID3D11Texture2D with the IWICBitmap image data
	HRESULT hr = context->Map( pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms );
	if( FAILED( hr ) )
	{
		int a = 0;
	}

	fillTexture( reinterpret_cast<BYTE *>( ms.pData ) );
	context->Unmap( pTexture, 0 );

	// Graphics will handle setting the render state for the model
	Gfx.RenderModel( m_TexturedModel );
}

bool Overlay::fillTexture( BYTE *const Pixels )const
{
	// Lock the render surface
	comptr<IWICBitmapLock> lock;
	HRESULT hr = m_pRenderSurface->Lock( nullptr, WICBitmapLockRead, &lock );
	RETURN_IF_FAILED( hr );

	// Get the stride to calculate the size of image buffer and for correctly 
	// aligned copies
	UINT stride = 0;
	hr = lock->GetStride( &stride );
	RETURN_IF_FAILED( hr );

	// Get the width and height of the image
	UINT width = 0, height = 0;
	hr = lock->GetSize( &width, &height );
	RETURN_IF_FAILED( hr );

	// Calculate the the image buffer and the size
	BYTE *data = nullptr;
	UINT bufferSize = stride * height;
	hr = lock->GetDataPointer( &bufferSize, &data );
	RETURN_IF_FAILED( hr );

	/*for( int y = 0; y < height; ++y )
	{
		for( int x = 0; x < width; ++x )
		{
			int idx = x + ( y * width );
			auto pixel = data[ idx ];
			if( pixel != 0 )
			{
				int a = 0;
			}
		}
	}*/
	// Copy by row to correctly align the image
	for( UINT y = 0; y < height; ++y )
	{
		CopyMemory( &Pixels[ y * stride ], &data[ y * stride ], stride );
	}

	return true;
}