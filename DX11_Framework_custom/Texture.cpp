////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Texture.h"

Texture::Texture()
{
}

Texture::Texture( const Texture& other )
{}

Texture::~Texture()
{}

Texture & Texture::operator=( const Texture & Tex )
{
	m_texture = Tex.m_texture;
	m_textureView = Tex.m_textureView;
	
	return *this;
}

bool Texture::Initialize( ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const std::wstring &Filename )
{	
	UINT height = 0, width = 0;
	std::unique_ptr<BYTE[]> pImageData;
	bool result = loadImageData( Filename, width, height, pImageData );
	RETURN_IF_FALSE( result );

	result = createTextureAndResourceView( pDevice, pContext, width, height, pImageData );
	RETURN_IF_FALSE( result );

	return true;
}

bool Texture::Initialize( ID3D11Device * pDevice, ID3D11DeviceContext * pContext, 
	UINT TextureWidth, UINT TextureHeight )
{
	bool result = false;
	UINT width = TextureWidth, height = TextureHeight;

	ImageLoader loader;
	auto imgResult = loader.CreateBitmap( width, height );
	result = SUCCEEDED( imgResult.first );
	RETURN_IF_FALSE( result );

	std::unique_ptr<BYTE[]> pImageData;
	result = createTextureFromWICImage( imgResult.second.Get(), pImageData );
	RETURN_IF_FALSE( result );

	result = createTextureAndResourceView( pDevice, pContext, width, height, pImageData );
	RETURN_IF_FALSE( result );

	return true;
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_textureView.Get();
}

bool Texture::createTextureAndResourceView( ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UINT TextureWidth, UINT TextureHeight, std::unique_ptr<BYTE[]> &pImageData )
{
	// Setup the description of the texture.
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = TextureWidth;
	textureDesc.Height = TextureHeight;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	HRESULT hResult = pDevice->CreateTexture2D( &textureDesc, nullptr, m_texture.GetAddressOf() );
	RETURN_IF_FAILED( hResult );

	pContext->UpdateSubresource( m_texture.Get(), 0, nullptr, pImageData.get(), TextureWidth * 4, 0 );

	// Setup the shader resource view description.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
	hResult = pDevice->CreateShaderResourceView( m_texture.Get(), &srvDesc,
		m_textureView.GetAddressOf() );
	RETURN_IF_FAILED( hResult );

	// Generate mipmaps for this texture.
	pContext->GenerateMips( m_textureView.Get() );

	return true;
}

bool Texture::loadImageData( const std::wstring &Filename, UINT &TextureWidth, UINT &TextureHeight, std::unique_ptr<BYTE[]> &pImageData )
{
	ImageLoader loader;
	bool result = loader.Initialize();
	RETURN_MESSAGE_IF_FALSE( result, L"Failed to initialize ImageLoader object." );

	auto imgResult = loader.CreateBitmap( Filename );
	result = SUCCEEDED( imgResult.first );
	RETURN_MESSAGE_IF_FALSE( result, L"Not a supported file type, or file not found." );

	imgResult.second->GetSize( &TextureWidth, &TextureHeight );
	result = createTextureFromWICImage( imgResult.second.Get(), pImageData );
	RETURN_MESSAGE_IF_FALSE( result, L"Failed to create WIC bitmap." );

	return true;
}

bool Texture::createTextureFromWICImage( IWICBitmap * pBitmap, std::unique_ptr<BYTE[]> &pImageData )
{
	// Get the size of the image to create a source rectangle that encompasses 
	// the entire image
	UINT width = 0, height = 0;
	HRESULT hr = pBitmap->GetSize( &width, &height );
	RETURN_IF_FAILED( hr );
	WICRect srcRect{0, 0, static_cast<long>( width ), static_cast<long>( height )};

	// Create a lock to get the data pointer to the pixel array
	comptr<IWICBitmapLock> pLock;
	hr = pBitmap->Lock( &srcRect, WICBitmapLockRead, pLock.GetAddressOf() );
	RETURN_IF_FAILED( hr );

	// Calculate the image size
	UINT stride = 0;
	pLock->GetStride( &stride );	
	UINT imageSize = stride * height;

	// Get pointer to the pixel data
	BYTE *pPixels = nullptr;
	hr = pLock->GetDataPointer( &imageSize, &pPixels );
	RETURN_MESSAGE_IF_FALSE(
		SUCCEEDED( hr ),
		L"Failed to acquire the bitmap pointer."
	);

	// Allocate memory for the image data member
	pImageData.reset( new BYTE[ imageSize ] );
	RETURN_MESSAGE_IF_FALSE(
		pImageData != nullptr,
		L"Failed to allocate memory for m_pImageData"
	);
	
	// Copy the image data from the WIC bitmap to the image data member
	CopyMemory( pImageData.get(), pPixels, imageSize );

	return true;
}

