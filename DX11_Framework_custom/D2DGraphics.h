#pragma once
/************************************************************************************
*  File name		: D2DGraphics.h													*
*  Creation Date	: 7/10/2016														*
*  Purpose			: Direct2D wrapper for rendering fonts							*
*  Description		: Direct2D will render the fonts to a WIC bitmap, then pass the *
*					  data on to the Graphics class to be rendered to a Direct3D	*
*					  texture and used for overlays									*
*  Requirements		: Undecided														*					  
*************************************************************************************/

#include <d2d1.h>
#include <memory>
#include "ImageLoader.h"


#pragma comment(lib, "d2d1.lib")
// TODO: Decide how this class will interact with the main Graphics class
// Will probably refactor once I get the DirectWrite wrapper class written

class D2DGraphics
{
public:
	D2DGraphics();
	~D2DGraphics();

	bool Initialize(const ImageLoader &ImgLoader, const UINT Width, const UINT Height);
	void RenderFont( const std::wstring &String/*, const FontLoader &FntLoader*/ );
	bool FillTexture(BYTE *const Pixels)const;
private:
	Microsoft::WRL::ComPtr<ID2D1Factory> m_pFactory;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_pRenderTarget;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_pBrush;
	Microsoft::WRL::ComPtr<IWICBitmap> m_pRenderSurface;
};

