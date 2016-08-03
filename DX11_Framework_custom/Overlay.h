#pragma once

#include "PrimitiveFactory.h"
#include "Texture.h"
#include "Includes.h"
#include "Utilities.h"
#include "FontLoader.h"
#include "Graphics.h"
#include "Model_Textured.h"
#include "Camera.h"

class Overlay
{
public:
	Overlay();
	~Overlay();

	bool Initialize( const Graphics &Gfx, UINT ScreenWidth, UINT ScreenHeight );
	DirectX::XMMATRIX GetWorldMatrix(const Camera &Cam)const;
	ID3D11ShaderResourceView *GetResourceView();

	void Render(const Graphics &Gfx);
	
private:
	bool fillTexture( BYTE *const Pixels )const;

private:
	D2D1_RECT_F m_SurfaceRect;
	Model_Textured m_Model_Textured;
	Texture m_texture;
	comptr<IDWriteTextFormat> m_Font;
	IWICBitmap *m_pRenderSurface;
};