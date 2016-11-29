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


#pragma comment(lib, "d2d1.lib")
// TODO: Decide how this class will interact with the main Graphics class
// Will probably refactor once I get the DirectWrite wrapper class written

using Microsoft::WRL::ComPtr;

class D2DGraphics
{
public:
   	friend class Graphics;

	void BeginDraw();
	void EndDraw();
    bool Initialize(const Graphics &Gfx, const UINT Width, const UINT Height);
    
private:
	ComPtr<ID2D1Device> m_pDevice;
	ComPtr<ID2D1DeviceContext> m_pContext;
	ComPtr<ID2D1SolidColorBrush> m_pBrush;
	ComPtr<ID2D1Bitmap1> m_pRenderSurface;
};