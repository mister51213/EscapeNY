#pragma once

#include "PrimitiveMaker.h"
#include "Texture.h"
#include "Includes.h"
#include "Utilities.h"
#include "FontLoader.h"
#include "Graphics.h"
#include "TexturedModel.h"

class Overlay
{
public:
	Overlay();
	~Overlay();

	bool Initialize( const Graphics &Gfx );
	ID3D11ShaderResourceView *GetResourceView();

	void Render(const Graphics &Gfx);
	
private:
	bool fillTexture( BYTE *const Pixels )const;

private:
	D2D1_RECT_F m_SurfaceRect;
	TexturedModel m_TexturedModel;
	Texture m_texture;
	comptr<IDWriteTextFormat> m_Font;
	IWICBitmap *m_pRenderSurface;
};