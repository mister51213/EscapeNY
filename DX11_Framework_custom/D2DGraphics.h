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

#include "Includes.h"
#include "Utilities.h"
#include "ImageLoader.h"
#include "FontLoader.h"

#pragma comment(lib, "d2d1.lib")
// TODO: Decide how this class will interact with the main Graphics class
// Will probably refactor once I get the DirectWrite wrapper class written

class D2DGraphics
{
public:
   	friend class Graphics;
	D2DGraphics();
	~D2DGraphics();

	void BeginDraw();
	void EndDraw();
	bool Initialize( D3DGraphics &D3D, const ImageLoader &ImgLoader, const UINT Width, const UINT Height);
	
    IWICBitmap *GetRenderSurface()const;
    
private:
	comptr<ID2D1Factory> m_pFactory;
	comptr<ID2D1RenderTarget> m_pRenderTarget;
	comptr<ID2D1SolidColorBrush> m_pBrush;
	comptr<IWICBitmap> m_pRenderSurface;
};