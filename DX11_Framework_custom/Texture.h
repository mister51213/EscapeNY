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
public:
	Texture();
	Texture( const Texture& );
	~Texture();

	Texture &operator=( const Texture &Tex );

	// This procedure loads a texture from file
	bool Initialize( ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const std::wstring &Filename );

	// This prcedure creates a blank texture based on dimensions
	bool Initialize( ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UINT TextureWidth, UINT TextureHeight );

	
	// If needed, would also make a procedure that creates a texture from memory
	// I would consider using an std::vector<BYTE> because vector keeps track of it's size, 
	// so wouldn't need a BYTE pointer and a separate UINT ImageSize parameter.
	//bool Initialize( ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const std::vector<BYTE> &PixelData );

    // ID3D11Texture2D is Microsoft's structure for HOLDING the PIXEL DATA of a texture;
    // the GetTexture() function ACCESSES that texture.

	// Interface used to get the data (Returns the shader resource view of the texture)
	ID3D11ShaderResourceView* GetTexture();

private:
	bool createTextureAndResourceView( ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UINT TextureWidth, UINT TextureHeight, std::unique_ptr<BYTE[]> &pImageData);
	bool loadImageData( const std::wstring &Filename, UINT &TextureWidth, UINT &TextureHeight, std::unique_ptr<BYTE[]> &pImageData );
	bool createTextureFromWICImage( IWICBitmap *pBitmap, std::unique_ptr<BYTE[]> &pImageData );
private:
	comptr<ID3D11Texture2D> m_texture;
	comptr<ID3D11ShaderResourceView> m_textureView;
};

