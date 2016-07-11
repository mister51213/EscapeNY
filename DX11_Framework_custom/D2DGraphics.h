#pragma once
/************************************************************************************
*  File name		: D2DGraphics.h													*
*  Creation Date	: 7/10/2016														*
*  Purpose			: Direct2D wrapper for rendering fonts							*
*  Description		: Direct2D will render the fonts to a WIC bitmap, then pass the *
*					  data on to the Graphics class to be rendered to a Direct3D	*
*					  texture and used for overlays									*
*  Requirements		: D3DGraphics, ImageLoader, FontLoader							*
*************************************************************************************/

#include <d2d1.h>
#include <memory>
#include "ImageLoader.h"
#include "FontLoader.h"
#include "D3DGraphics.h"


#pragma comment(lib, "d2d1.lib")
// TODO: Decide how this class will interact with the main Graphics class
// Will probably refactor once I get the DirectWrite wrapper class written

class D2DGraphics
{
public:
	D2DGraphics();
	~D2DGraphics();

	void BeginDraw();
	void EndDraw();
	bool Initialize(const D3DGraphics &D3D, const ImageLoader &ImgLoader, const UINT Width, const UINT Height);
	void DrawString( const std::wstring &String, const FontLoader &FntLoader, const D2D1_RECT_F &PositionAndSize );
	void Render( const D3DGraphics &D3D );

private:
	bool fillTexture( BYTE *const Pixels )const;

private:
	Microsoft::WRL::ComPtr<ID2D1Factory> m_pFactory;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_pRenderTarget;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrush;
	Microsoft::WRL::ComPtr<IWICBitmap> m_pRenderSurface;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> overlay;
};

