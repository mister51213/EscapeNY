#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
//////////////
// INCLUDES //
//////////////
#include "Includes.h"
#include "Utilities.h"
#include "ImageLoader.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureClass
////////////////////////////////////////////////////////////////////////////////
class Texture
{
private:
	struct TargaHeader
	{
		unsigned char data1[ 12 ];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	Texture();
	Texture( const Texture& );
	~Texture();

	// This procedure loads a texture from file
	bool Initialize( ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const std::string &Filename );

	// This prcedure creates a blank texture based on dimensions
	bool Initialize( ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UINT TextureWidth, UINT TextureHeight );

	// If needed, would also make a procudure that creates a texture from memory
	// I would consider using an std::vector<BYTE> because vector keeps track of it's size, 
	// so wouldn't need a BYTE pointer and a separate UINT ImageSize parameter.
	bool Initialize( ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const std::vector<BYTE> &PixelData );

	// Returns the shader resource view of the texture
	ID3D11ShaderResourceView* GetTexture();

private:
	bool LoadImageData( const std::string &Filename, UINT &TextureWidth, UINT &TextureHeight );
	bool LoadTarga( const std::string &Filename, UINT &TextureWidth, UINT &TextureHeight );
	bool CreateTextureFromWICImage( IWICBitmap *pBitmap );
private:
	std::unique_ptr<unsigned char[]> m_pImageData;
	comptr<ID3D11Texture2D> m_texture;
	comptr<ID3D11ShaderResourceView> m_textureView;
};

