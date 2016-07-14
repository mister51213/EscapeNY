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

bool Texture::Initialize( ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const std::string &Filename )
{
	bool result = false;
	// Load the targa image data into memory.
	UINT height = 0, width = 0;
	result = LoadImageData( Filename, width, height );
	RETURN_IF_FALSE( result );

	// Setup the description of the texture.
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Height = height;
	textureDesc.Width = width;
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

	pContext->UpdateSubresource( m_texture.Get(), 0, nullptr, m_pImageData.get(), width * 4, 0 );

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

bool Texture::Initialize( ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UINT TextureWidth, UINT TextureHeight )
{
	bool result = false;
	UINT width = TextureWidth, height = TextureHeight;

	ImageLoader loader;
	auto imgResult = loader.CreateBitmap( width, height );
	result = SUCCEEDED( imgResult.first );
	RETURN_IF_FALSE( result );

	result = CreateTextureFromWICImage( imgResult.second.Get() );

	// Setup the description of the texture.
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Height = height;
	textureDesc.Width = width;
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

	pContext->UpdateSubresource( m_texture.Get(), 0, nullptr, m_pImageData.get(), width * 4, 0 );

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

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_textureView.Get();
}

bool Texture::LoadImageData( const std::string &Filename, UINT &TextureWidth, UINT &TextureHeight )
{
	// Not ideal having multiple individual loader functions
	std::string extension( Filename.substr( Filename.size() - 3, 3 ) );
	int cmpResult = extension.compare( "tga" );

	if( cmpResult == 0 )
	{
		bool result = LoadTarga( Filename, TextureWidth, TextureHeight );
		RETURN_IF_FALSE( result );
	}
	else
	{
		ImageLoader loader;
		bool result = loader.Initialize();
		RETURN_MESSAGE_IF_FALSE( result, L"Failed to initialize ImageLoader object." );

		std::wstring wFilename( Filename.begin(), Filename.end() );
		auto imgResult = loader.CreateBitmap( wFilename );
		RETURN_MESSAGE_IF_FALSE( 
			SUCCEEDED( imgResult.first ), 
			L"Not a supported file type, or file not found." 
		);

		imgResult.second->GetSize( &TextureWidth, &TextureHeight );
		CreateTextureFromWICImage( imgResult.second.Get() );
	}

	return true;
}

bool Texture::LoadTarga( const std::string &Filename, UINT &TextureWidth, UINT &TextureHeight )
{	
	// Found out that this procedure doesn't support compressed or palette based images
	// The tga file must be an RGB uncompressed image

	// Open the targa file for reading in binary.
	FILE* filePtr = nullptr;
	int error = fopen_s( &filePtr, Filename.c_str(), "rb" );
	RETURN_IF_FALSE( error == 0 );

	// Read in the file header.
	TargaHeader targaFileHeader{};
	unsigned int count = (unsigned int)fread( &targaFileHeader, sizeof( TargaHeader ), 1, filePtr );
	RETURN_IF_FALSE( count == 1 );

	// Get the important information from the header.
	TextureWidth = (int)targaFileHeader.width;
	TextureHeight = (int)targaFileHeader.height;
	int bpp = (int)targaFileHeader.bpp;

	// Check that it is 32 bit and not 24 bit.
	RETURN_IF_FALSE( bpp == 32 );

	// Calculate the size of the 32 bit image data.	
	int imageSize = TextureWidth * TextureHeight * ( bpp / 8 );

	// Allocate memory for the targa image data.
	m_pImageData.reset(new BYTE[ imageSize ]);
	RETURN_IF_FALSE( m_pImageData != nullptr );

	// Read in the targa image data.
	count = (unsigned int)fread( m_pImageData.get(), 1, imageSize, filePtr );
	RETURN_IF_FALSE( count == imageSize );

	// Close the file.
	error = fclose( filePtr );
	RETURN_IF_FALSE( error == 0 );

	return true;
}

bool Texture::CreateTextureFromWICImage( IWICBitmap * pBitmap )
{
	// Get the size of the image to create a source rectangle that encompasses 
	// the entire image
	UINT width = 0, height = 0;
	HRESULT hr = pBitmap->GetSize( &width, &height );
	RETURN_IF_FAILED( hr );
	WICRect srcRect{0, 0, width, height};

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
	m_pImageData.reset( new BYTE[ imageSize ] );
	RETURN_MESSAGE_IF_FALSE(
		m_pImageData != nullptr,
		L"Failed to allocate memory for m_pImageData"
	);
	
	// Copy the image data from the WIC bitmap to the image data member
	CopyMemory( m_pImageData.get(), pPixels, imageSize );

	return true;
}

