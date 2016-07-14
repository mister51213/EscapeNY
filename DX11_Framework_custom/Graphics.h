#pragma once
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "D3DGraphics.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

////////////////////////////////////////////////////////////////////////////////
// Class name: Graphics
////////////////////////////////////////////////////////////////////////////////
class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Initialize(int ScreenWidth, int ScreenHeight, HWND WinHandle, 
		float Red, float Green, float Blue, float Alpha = 1.f );

	void BeginScene();
	void EndScene();

	void SetBackgroundColor( float Red, float Green, float Blue, float Alpha = 1.f );
	D3DGraphics *GetDirect3D()const;

private:
   	std::unique_ptr<D3DGraphics> m_pDirect3D;
	float m_backgroundColor[ 4 ];
};

inline void HandleResult( const std::wstring &Message )
{
	MessageBox( nullptr, Message.c_str(), L"Error!", MB_OK );
}