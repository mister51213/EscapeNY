#pragma once
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Wic.h"
#include "D2DGraphics.h"
#include "D3DGraphics.h"

/////////////
// GLOBALS //
/////////////
const bool g_fullScreen = false;
const bool g_vsyncEnabled = true;
const float g_screenDepth = 6000.0f;
const float g_screenNear = 0.1f;
const UINT g_screenWidth = 800;
const UINT g_screenHeight = 600;

////////////////////////////////////////////////////////////////////////////////
// Class name: Graphics
////////////////////////////////////////////////////////////////////////////////

// FWD DECLARATION (to avoid circular dependency; Model includes Graphics.h)
class Model;

class Graphics
{
public:
	bool Initialize(int ScreenWidth, int ScreenHeight, HWND WinHandle, 
		float Red, float Green, float Blue, float Alpha = 1.f );

	void BeginScene();
	void EndScene();
   	void BeginDraw2D()const;
	void EndDraw2D()const;

	void RenderString(
        const std::wstring &String, 
        IDWriteTextFormat *pFont, 
        const D2D1_RECT_F &PositionAndSize )const;
	void RenderModel( const Model &rModel )const;
	void SetBackgroundColor( float Red, float Green, float Blue, float Alpha = 1.f );

	Wic *GetWIC()const;
	D2DGraphics *GetDirect2D()const;
	D3DGraphics *GetDirect3D()const;

private:
	std::unique_ptr<Wic> m_pWic;
	std::unique_ptr<D3DGraphics> m_pDirect3D;
	std::unique_ptr<D2DGraphics> m_pDirect2D;
	float m_backgroundColor[ 4 ];
};